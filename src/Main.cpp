#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Stem.h"
#include "EntityManager.h"
#include "FourierSeries.h"
#include "LeafBuilder.h"
#include "Renderer.h"
#include "Scene.h"
#include "Controller.h"
#include "WorldTime.h"
#include "Tree.h"
#include "TextureManager.h"
#include "TextureBuilder.h"
#include "Leaf.h"
#include <glm/glm.hpp>
#include <vector>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "First OpenGL program on Linux", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW Window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD\n";
        return -1;
    }

    glm::vec3 cameraEye(0.0f, -1.7f, 0.6f);
    glm::vec3 cameraFacing(0.0f, 1.15f, 0.6f);
    glm::vec3 cameraDir = cameraEye - cameraFacing;

    glm::vec3 cameraLeft = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), cameraDir);
    glm::vec3 cameraUp = glm::cross(cameraDir, cameraLeft);

    Camera camera(cameraEye, cameraFacing, cameraUp);
    Scene scene(camera);

    TextureManager textureManager;
    Renderer renderer(textureManager);
    EntityManager entityManager(renderer.bufferManager, scene);
    Controller controller(entityManager, renderer, scene);
    WorldTime worldTime;

    textureManager.addTexture(TextureBuilder::generateStemTexture(4, 4), "StemTexture");
    FourierTerm ft1(0.0f, 0.5f, 0.5f, 2.0f);
    FourierTerm ft2(0.0f, 0.3f, 0.5f * 6, 2.0f);
    FourierTerm ft3(0.0f, 0.2f, 0.5f * 8, 2.0f);

    FourierSeries fs(0.0f, {ft1, ft2, ft3});
    LeafSurface ls(fs);
    textureManager.addTexture(TextureBuilder::generateLeafTexture(256, 256, ls), "LeafTexture");

    // Random seed for LCG using system clock
    std::linear_congruential_engine<unsigned int, 16807, 0, 2147483647> lcg;
    lcg.seed(std::chrono::system_clock::now().time_since_epoch().count());

    int treeId = entityManager.addEntity(std::make_unique<Tree>(entityManager, lcg));
    entityManager.addEntityToScene(treeId);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        controller.updateStates(worldTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}