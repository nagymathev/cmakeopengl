#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "Shader.h"
#include "stb_image.h"
#include "asset_locations.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "VertexArrayObject.h"

#include "Cube.hpp"

#pragma region global variables

// GLOBAL VARIABLES
int windowHeight = 600;
int windowWidth = 800;
glm::mat4 projection;
glm::mat4 model;

glm::vec3 cameraPos(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

float lastX = windowWidth / 2, lastY = windowHeight / 2;
bool firstMouse = true;
float mouseHeldTime = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

float delta_time = 0.0f; // The time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

#pragma endregion

float clamp(float x, float lowerlimit = 0.0f, float upperlimit = 1.0f) {
    if (x < lowerlimit) return lowerlimit;
    if (x > upperlimit) return upperlimit;
    return x;
}
float smoothstep(float x, float edge0 = 0.0f, float edge1 = 1.0f) {
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0));

    return x * x * (3.0f - 2.0f * x);
}

unsigned int CreateTexture(const char* filename, GLenum format = GL_RGB) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((TEXTURES_ROOT_DIR + std::string(filename)).c_str(),
                                    &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    windowHeight = height;
    windowWidth = width;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        mouseHeldTime += delta_time;
        camera.Zoom -= smoothstep(mouseHeldTime);
    }
    else if (mouseHeldTime > 0.1f)
    {
        mouseHeldTime -= delta_time;
        camera.Zoom += smoothstep(mouseHeldTime);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, windowWidth, windowHeight);
    glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    unsigned int containerTexture = CreateTexture("container2.png", GL_RGBA);
    unsigned int containerTextureSpecularMap = CreateTexture("container2_specular.png", GL_RGBA);
    unsigned int emissionMap = CreateTexture("matrix.jpg");

    glm::vec3 lightCoords(2.0f, 1.0f, -5.0f);
    Shader lightShader("lighting.vert", "light.frag");

    Shader lightingShader("lighting.vert", "lighting.frag");
    lightingShader.use();
    lightingShader.setVec3("lightPos", lightCoords);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, containerTextureSpecularMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionMap);
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setVec3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setVec3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
    lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("light.constant",  1.0f);
    lightingShader.setFloat("light.linear",    0.09f);
    lightingShader.setFloat("light.quadratic", 0.032f);

    std::vector<glm::vec3> cube_positions = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    Cube directionalLight{ &lightShader };
    directionalLight.scale = glm::vec3(0.2f);

    std::vector<Cube*> cubes;
    for (auto pos : cube_positions)
    {
        auto* cube = new Cube{ &lightingShader };
        cube->position = pos;
        cubes.push_back(cube);
    }

    /*
     * ------------------------------------------
     *                  THE LOOP
     * ------------------------------------------
     */

	while (!glfwWindowShouldClose(window))
	{
        auto currentFrame = static_cast<float>(glfwGetTime());
        delta_time = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.Zoom = clamp(camera.Zoom, 10.0f, 75.0f);
        mouseHeldTime = clamp(mouseHeldTime, 0.0f, 1.0f);

        processInput(window);

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(camera.Zoom),
                                      (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();


        /*
         * ---------------------------
         *      The Light Cube
         * ---------------------------
         */
        lightShader.use();
        glm::vec3 lightColors {
            sin(currentFrame) + 1,
            sin(currentFrame * 2) + 1,
            sin(currentFrame * 0.5) + 1
        };
        lightShader.setVec3("lightColor", lightColors);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

//        lightCoords.x = sin(currentFrame * 2);
//        lightCoords.y = cos(currentFrame * 5);
//        lightCoords.z = sin(currentFrame) * 4 - 4;

        directionalLight.position = lightCoords;
        directionalLight.update(delta_time);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*
         * ------------------------------------
         *          Rendering Cubes
         * ------------------------------------
         */

        lightingShader.use();
        lightingShader.setVec3("light.diffuse", lightColors *= 0.5);
        lightingShader.setVec3("light.specular", lightColors);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("lightPos", directionalLight.position);

        for (int i = 0; i < cubes.size(); i++)
        {
            cubes[i]->update(delta_time);
            cubes[i]->angle += i * delta_time;
            cubes[i]->rotation_angle = glm::vec3(currentFrame * (i + 1),
                                                 delta_time * currentFrame + (i * 5),
                                                 sin(currentFrame) * (i + 1) * 50);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

		glBindVertexArray(0);
        glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
