//#include <GL/glew.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>
#include <imgui/imgui.h>

#include "../include/Geometry.h"
#include "../include/GLDebug.h"
#include "../include/Log.h"
#include "../include/ShaderProgram.h"
#include "../include/Shader.h"
#include "../include/Texture.h"
#include "../include/Window.h"
#include "../include/Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../include/UnitCube.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../include/UnitCube.h"
#include "Spheroid.h"
#include "ImGuiDisplay.h"
#include "globals.h"
// EXAMPLE CALLBACKS

float lastF = 0.f;

class GLFWCallbacks : public CallbackInterface {

public:
	GLFWCallbacks()
		: camera(glm::radians(45.f), glm::radians(45.f), 3.0)
		, aspect(1.0f)
		, rightMouseDown(false)
		, mouseOldX(0.0)
		, mouseOldY(0.0)
	{}

	virtual void keyCallback(int key, int scancode, int action, int mods) {}
	virtual void mouseButtonCallback(int button, int action, int mods) {
        if(!ImGui::IsAnyWindowHovered()) {
            if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                if (action == GLFW_PRESS) rightMouseDown = true;
                else if (action == GLFW_RELEASE) rightMouseDown = false;
            }
        }
	}
	virtual void cursorPosCallback(double xpos, double ypos) {
        if(!ImGui::IsAnyWindowHovered()) {
            if (rightMouseDown) {
                camera.incrementTheta(ypos - mouseOldY);
                camera.incrementPhi(xpos - mouseOldX);
            }
            mouseOldX = xpos;
            mouseOldY = ypos;
        }
	}
	virtual void scrollCallback(double xoffset, double yoffset) {
        if(!ImGui::IsAnyWindowHovered()) {
            camera.incrementR(yoffset);
        }
	}
	virtual void windowSizeCallback(int width, int height) {
        if(!ImGui::IsAnyWindowHovered()) {
            // The CallbackInterface::windowSizeCallback will call glViewport for us
            CallbackInterface::windowSizeCallback(width,  height);
            aspect = float(width)/float(height);
        }
	}

	void viewPipeline(ShaderProgram &sp, glm::vec3 cam_pos) {
		glm::mat4 M = glm::mat4(1.0);
		glm::mat4 V = camera.getView();
		glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);

        GLint location = glGetUniformLocation(sp, "lPosition");
        glUniform3fv(location, 1, glm::value_ptr(glm::vec3(0.f,0.f,0.f)));
        location = glGetUniformLocation(sp, "vPos");
        glUniform3fv(location, 1, glm::value_ptr(camera.getPos()));
        //glUniform3fv(location, 1, glm::value_ptr(cam_pos));

        GLint uniMat = glGetUniformLocation(sp, "M");
		uniMat = glGetUniformLocation(sp, "V");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(V));
		uniMat = glGetUniformLocation(sp, "P");
		glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(P));

	}
	Camera camera;
private:
	bool rightMouseDown;
	float aspect;
	double mouseOldX;
	double mouseOldY;
};

void handleDeltaTime();

int main() {
    Log::debug("Starting main");
    // WINDOW
    glfwInit();
    Window window(800, 800, "Orrery");

    GLDebug::enable();

    // CALLBACKS
    auto a4 = std::make_shared<GLFWCallbacks>();
    window.setCallbacks(a4);


    //Imgui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.window.get(), false);
    ImGui_ImplOpenGL3_Init("#version 150");

    ShaderProgram shader("shaders/test.vert", "shaders/test.frag");
    ShaderProgram noLightShader("shaders/lamp.vert", "shaders/lamp.frag");


    Spheroid sun("../textures/2k_sun.jpg", 0.f, 20.f, glm::vec3(1.f), 0.1f,glm::vec3(0.0f, 0.f, 1.f), 0.f);

    Spheroid mercury("../textures/2k_mercury.jpg",3.f, 90.f, glm::vec3(0.15f), 6.f, glm::vec3(0.0f, 0.8f, 0.2f), 1.5f);
    Spheroid venus("../textures/2k_venus_atmosphere.jpg",5.f, 90.f, glm::vec3(0.34f), 2.f, glm::vec3(0.0f, 0.8f, 0.2f), 1.3f);
    Spheroid earth("../textures/2k_earth_daymap_flipped.jpg",7.f, 90.f, glm::vec3(0.5f), 2.f, glm::vec3(0.0f, 0.8f, 0.2f), 1.f);
    //Spheroid earth_clouds("../textures/2k_earth_clouds.jpg",7.f, 90.f, glm::vec3(0.505f), 2.f, glm::vec3(0.0f, 0.8f, 0.2f), 1.f); //glm::vec3(0.0f, 0.8f, 0.2f)
    Spheroid moon("../textures/2k_moon.jpg", 2.f, 80.f, glm::vec3(0.2f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 3.f);
    Spheroid mars("../textures/2k_mars.jpg", 10.f, 80.f, glm::vec3(0.4f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 0.9f);

    Spheroid jupiter("../textures/2k_jupiter.jpg", 16.f, 80.f, glm::vec3(1.2f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 0.7f);
    Spheroid saturn("../textures/2k_saturn.jpg", 20.f, 80.f, glm::vec3(1.0f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 0.6f);
    Spheroid saturn_rings("../textures/2k_saturn.jpg", 20.f, 80.f, glm::vec3(1.9f, 0.f, 1.9f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 0.6f);
    Spheroid uranus("../textures/2k_uranus.jpg", 23.f, 80.f, glm::vec3(0.9f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 0.5f);
    Spheroid neptune("../textures/2k_neptune.jpg", 26.f, 80.f, glm::vec3(0.82f), 4.f, glm::vec3(0.0f, 1.3f, -0.3f), 0.4f);

    Spheroid skybox("../textures/8k_stars_milky_way.jpg", 0.f, 90.f, glm::vec3(100.f), 0.f, glm::vec3(0.0f, 0.0f, 1.0f), 0.f);

    std::vector<Spheroid*> planitoids = {&mercury, &venus, &earth, &saturn_rings, &moon, &mars, &jupiter, &saturn, &uranus, &neptune};
    sun.children.insert(sun.children.begin(), planitoids.begin(), planitoids.end());

    mercury.parent = &sun;
    venus.parent = &sun;
    earth.parent = &sun;
    saturn_rings.parent = &sun;
    //earth_clouds.parent = &sun;
    moon.parent = &earth;
    mars.parent = &sun;
    jupiter.parent = &sun;
    saturn.parent = &sun;
    uranus.parent = &sun;
    neptune.parent = &sun;

    earth.children.push_back(&moon);

    // RENDER LOOP
    while (!window.shouldClose()) {
        handleDeltaTime();
        glfwPollEvents();
        buildImGui();


        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shader.use();
        if(!pause)
            glfwtime = glfwtime + dTime * animationSpeed;

        a4->viewPipeline(shader, earth.position);
        for(const auto p : planitoids)
            p->render(shader);

        noLightShader.use();
        a4->viewPipeline(noLightShader, venus.position);
        sun.render(noLightShader);
        skybox.render(noLightShader);


        glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}


void handleDeltaTime()
{
    float currentFrameTime = glfwGetTime();
    dTime = currentFrameTime - lastF;
    lastF = currentFrameTime;
}
