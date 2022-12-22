#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Spheroid.h"
#include "math.h"
#include "utilities.h"
#include "globals.h"

//Sclices are hardcoded. There isn't really a reason to change this during runtime etc at least for the current program
const int horizontal_slices = 70;
const int vertical_slices = 50;
const float vertical_slice_stride = (2 * M_PI) / vertical_slices;
const float horizontal_slice_stride =  M_PI / horizontal_slices;

//Based in part off of this helpful article for generating a 3D sphere: http://www.songho.ca/opengl/gl_sphere.html
//I did not use my bspline code since generating the correct control points was difficult..
void Spheroid::generateGeometry() {
    float x, y, rsinu, rcosu, y_slice_angle, x_slice_angle;
    std::vector<glm::vec3> verts, norms, colours;
    std::vector<glm::vec2> texCoords;

    for(int i = 0; i <= horizontal_slices; ++i) {
        //iterate over the appropriate x angles of the sphere mesh
        x_slice_angle = M_PI / 2 - i * horizontal_slice_stride;
        // Because we do scaling later there is no need to change the radius here even through the equation is rsinu etc.. I.E. Radius is always 1 when generating our spheres
        rcosu = cos(x_slice_angle);
        rsinu = sin(x_slice_angle);


        for (int j = 0; j <= vertical_slices; ++j) {
            //Iterate through the appropriate y angles of the sphere
            y_slice_angle = j * vertical_slice_stride;

            //generate our verts
            x = rcosu * cos(y_slice_angle);
            y = rcosu * sin(y_slice_angle);
            verts.push_back(glm::vec3(x, y, rsinu));

            //Get our vector norms. Again because we're scaling later the x,y,rsinu components are already normalized
            norms.push_back(glm::vec3(x, y, rsinu));

            texCoords.push_back(glm::vec2(
                    static_cast<float>(j) / vertical_slices,
                    static_cast<float>(i) / horizontal_slices));

            //Colours is unneccesary here but a possible improvement is for debug mode to disable the texture mapping so i want to set some colour.
            //May not implement that however
            colours.push_back(glm::vec3(1.f,0.f,0.f));
        }
    }

    //Once we have our verts, norms, and texCoords, we can run through and create the appropriate triangles to form our sphere mesh
    int vert_by_horz;
    int vert_and_horz;
    for(int i = 0; i < horizontal_slices; ++i) {
        vert_by_horz = i * (vertical_slices + 1);
        vert_and_horz = vert_by_horz + vertical_slices + 1;

        for (int j = 0; j < vertical_slices; ++j, ++vert_by_horz, ++vert_and_horz) {
            if (i != (horizontal_slices - 1)) {
                sphere.verts.push_back(verts.at(vert_by_horz + 1));
                sphere.verts.push_back(verts.at(vert_and_horz));
                sphere.verts.push_back(verts.at(vert_and_horz + 1));

                sphere.cols.push_back(colours.at(vert_by_horz + 1));
                sphere.cols.push_back(colours.at(vert_and_horz));
                sphere.cols.push_back(colours.at(vert_and_horz + 1));

                sphere.normals.push_back(norms.at(vert_by_horz + 1));
                sphere.normals.push_back(norms.at(vert_and_horz));
                sphere.normals.push_back(norms.at(vert_and_horz + 1));

                sphere.texCoords.push_back(texCoords.at(vert_by_horz + 1));
                sphere.texCoords.push_back(texCoords.at(vert_and_horz));
                sphere.texCoords.push_back(texCoords.at(vert_and_horz + 1));
            }

            if (i != 0) {
                sphere.verts.push_back(verts.at(vert_by_horz));
                sphere.verts.push_back(verts.at(vert_and_horz));
                sphere.verts.push_back(verts.at(vert_by_horz + 1));

                sphere.cols.push_back(colours.at(vert_by_horz));
                sphere.cols.push_back(colours.at(vert_and_horz));
                sphere.cols.push_back(colours.at(vert_by_horz + 1));

                sphere.normals.push_back(norms.at(vert_by_horz));
                sphere.normals.push_back(norms.at(vert_and_horz));
                sphere.normals.push_back(norms.at(vert_by_horz + 1));

                sphere.texCoords.push_back(texCoords.at(vert_by_horz));
                sphere.texCoords.push_back(texCoords.at(vert_and_horz));
                sphere.texCoords.push_back(texCoords.at(vert_by_horz + 1));
            }
        }
    }



    m_gpu_geom.bind();
    m_gpu_geom.setVerts(sphere.verts);
    m_gpu_geom.setCols(sphere.cols);
    m_gpu_geom.setNormals(sphere.normals);
    m_gpu_geom.setTexCoords(sphere.texCoords);
    m_size = sphere.verts.size();

}


glm::mat4 Spheroid::localTransforms()
{
    glm::mat4 mat = glm::mat4(1.f);
    mat = glm::scale(mat, scale);

    //counter rotation due to orbit
    mat = glm::rotate(mat, glfwtime  * orbit_speed , -orbital_inc_axis);

    //Orient the axis of the body
    mat = glm::rotate(mat, axial_tilt, glm::vec3(1.0, 0.0, 0.0));

    //rotate the planet around itself
    mat = glm::rotate(mat, glfwtime  * rotation_speed , glm::vec3(0.0f, 0.0f, 1.0f));

    return mat;
}

glm::mat4 Spheroid::globalTransforms()
{
    glm::mat4 mat = glm::mat4(1.f);
    Spheroid* up = parent;
    while(up)
    {
        mat = glm::rotate(mat,glfwtime  * up->orbit_speed ,  -up->orbital_inc_axis);
        up = up->parent;
    }

    mat = glm::rotate(mat,glfwtime  * orbit_speed  ,  orbital_inc_axis);

    mat = glm::translate(mat,  glm::vec3(distance_from_parent, 0, 0) );
    return mat;
}

glm::mat4 Spheroid::combineParentTransformationMatrix(Spheroid* s, glm::mat4 mat)
{
    if(s->parent == nullptr)
    {
        return mat;
    }
    mat = mat * s->parent->globalTransforms();
    return mat;
}


glm::mat4 Spheroid::generateTransformationMatrix()
{
    trans_mat = glm::mat4(1.f);
    trans_mat = trans_mat * combineParentTransformationMatrix(this, glm::mat4(1.f));
    trans_mat *= globalTransforms();
    trans_mat *= localTransforms();
    return trans_mat;
}

void Spheroid::render(ShaderProgram &shader) {
    if(!pause)
    {
        generateTransformationMatrix();
        position = glm::vec3(glm::vec4(1.f) * trans_mat);
    }
    //update position
    for(int i = 0; i < sphere.verts.size(); ++i)
    {
        sphere.verts.at(i) = vec3matCombination(sphere.verts.at(i), trans_mat);
    }
    m_gpu_geom.bind();

    glm::mat4 transform = trans_mat;
    GLint transformationLocation = glGetUniformLocation(shader.programID.value(), "M");
    glUniformMatrix4fv(transformationLocation, 1, GL_FALSE, glm::value_ptr(transform));

    if(!debug_mode)texture.bind();
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(m_size));
    if(!debug_mode)texture.unbind();
}
