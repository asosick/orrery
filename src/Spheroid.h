#ifndef INC_SPHEROID_H
#define INC_SPHEROID_H
#include <vector>
#include <glm/gtx/transform.hpp>
#include "../include/Geometry.h"
#include "../include/Texture.h"
#include "../include/ShaderProgram.h"

class Spheroid
{
private:

public:
    GPU_Geometry m_gpu_geom;
    CPU_Geometry sphere;

    glm::mat4 trans_mat;
    GLsizei m_size;
    void generateGeometry();

    Texture texture;
    glm::vec3 scale;
    float axial_tilt;

    glm::vec3 position;
    float rotation_speed;
    float distance_from_parent;
    glm::vec3 orbital_inc_axis;
    float orbit_speed;

    Spheroid* parent;
    std::vector<Spheroid*> children;

    Spheroid(const char *texture_path, float distance_from_parent, float inclination, glm::vec3 scale,
             float rotation_speed, glm::vec3 orbital_inc_axis, float orbit_speed) :
            texture(Texture(texture_path, GL_NEAREST)), distance_from_parent(distance_from_parent), axial_tilt(inclination), scale(scale),
            rotation_speed(rotation_speed), orbit_speed(orbit_speed), orbital_inc_axis(orbital_inc_axis)
            {
        parent = nullptr;
        trans_mat = glm::mat4(1.f);
        trans_mat = glm::translate(trans_mat, position);
        trans_mat = glm::rotate(trans_mat, glm::radians(-90.f), glm::vec3(1.0, 0.0, 0.0));
        trans_mat = glm::rotate(trans_mat, glm::radians(inclination), glm::vec3(1.0, 0.0, 0.0));
        trans_mat = glm::scale(trans_mat, glm::vec3(0.5f, 0.5, 0.5));

        generateGeometry();
    };

    std::vector<glm::vec3> generateTriangles(std::vector<glm::vec3> vector1);
    void render(ShaderProgram &shader);

    glm::mat4 generateTransformationMatrix();

    glm::mat4 generateLocalTransformation(glm::mat4 &mat);

    glm::mat4 generateGlobalTransformations(glm::mat4 &mat);

    glm::mat4 combineParentTransformationMatrix(Spheroid *s, glm::mat4 mat);

    glm::mat4 localTransforms();

    glm::mat4 globalTransforms();

    glm::vec3 getLightPosition();

};



#endif //INC_SPHEROID_H
