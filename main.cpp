#include "include/Camera.h"
#include "include/InputHandler.h"
#include "include/Mesh.h"
#include "include/ObjLoader.h"
#include "include/Scene.h"
#include "include/Shader.h"
#include "include/struct.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>

// Timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    Scene *scene = static_cast<Scene *>(glfwGetWindowUserPointer(window));
    if (scene) {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        for (auto &camera : scene->getCameras()) {
            if (camera) {
                camera->setAspectRatio(aspectRatio);
            }
        }
    }
}

static void fps_counter(GLFWwindow *window) {
    static double previous_seconds = glfwGetTime();
    static int    frame_count;
    double        current_seconds = glfwGetTime();
    double        elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        double fps = static_cast<double>(frame_count) / elapsed_seconds;
        double ms_per_frame = 1000.0 / fps;

        std::ostringstream oss;
        oss << " [FPS: " << fps << " Frame time: " << ms_per_frame << "(ms)]";
        glfwSetWindowTitle(window, oss.str().c_str());
        frame_count = 0;
    }
    frame_count++;
}

// Initialize GLFW and create a window
GLFWwindow *initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Scop", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    return window;
}

// Initialize GLAD
bool initGLAD() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

static std::vector<std::shared_ptr<Mesh>> loadMeshesFromObj(const std::string &filePath) {
    ObjLoader objLoader(filePath);
    auto      meshes = objLoader.getMeshes();

    int meshIndex = 0;
    for (const auto &meshPtr : meshes) {
        std::cout << "Mesh #" << meshIndex << std::endl;

        const auto &vertices = meshPtr->getVertices();
        const auto &indices = meshPtr->getIndices();
        const auto &material = meshPtr->getMaterial();

        if (material.name.empty()) {
            std::cout << "  Material: (No material assigned)" << std::endl;
        } else {
            std::cout << "  Material Name: " << material.name << std::endl;
            std::cout << "    Ambient: (" << material.ambient.r << ", " << material.ambient.g
                      << ", " << material.ambient.b << ")" << std::endl;
            std::cout << "    Diffuse: (" << material.diffuse.r << ", " << material.diffuse.g
                      << ", " << material.diffuse.b << ")" << std::endl;
            std::cout << "    Specular: (" << material.specular.r << ", " << material.specular.g
                      << ", " << material.specular.b << ")" << std::endl;
            std::cout << "    Shininess: " << material.shininess << std::endl;
            std::cout << "    Diffuse Map Path: " << material.diffuseMapPath << std::endl;
        }

        std::cout << "  Number of Vertices: " << vertices.size() << std::endl;
        std::cout << "  Number of Indices: " << indices.size() << std::endl;

        size_t maxVerticesToShow = std::min(vertices.size(), static_cast<size_t>(5));
        for (size_t i = 0; i < maxVerticesToShow; ++i) {
            const auto &vertex = vertices[i];
            std::cout << "    Vertex " << i << ": Position(" << vertex.position.x << ", "
                      << vertex.position.y << ", " << vertex.position.z << "), "
                      << "Normal(" << vertex.normal.x << ", " << vertex.normal.y << ", "
                      << vertex.normal.z << "), "
                      << "TexCoords(" << vertex.texCoords.x << ", " << vertex.texCoords.y << ")"
                      << std::endl;
        }
        if (vertices.size() > maxVerticesToShow) {
            std::cout << "    ... (" << vertices.size() - maxVerticesToShow << " more vertices)"
                      << std::endl;
        }

        size_t maxTrianglesToShow = std::min(indices.size() / 3, static_cast<size_t>(5));
        for (size_t i = 0; i < maxTrianglesToShow * 3; i += 3) {
            std::cout << "    Triangle " << i / 3 << ": Indices(" << indices[i] << ", "
                      << indices[i + 1] << ", " << indices[i + 2] << ")" << std::endl;
        }
        if (indices.size() / 3 > maxTrianglesToShow) {
            std::cout << "    ... (" << (indices.size() / 3) - maxTrianglesToShow
                      << " more triangles)" << std::endl;
        }

        ++meshIndex;
    }

    return meshes;
}

int main() {
    // Initialize GLFW
    GLFWwindow *window = initGLFW();
    if (!window)
        return -1;

    // Initialize GLAD
    if (!initGLAD())
        return -1;

    // Set viewport
    glViewport(0, 0, 800, 600);

    {
        // Create shader program
        Scene scene;

        // class Shader shader;
        auto shader = std::make_shared<Shader>();

        shader->addShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
        shader->addShaderFromFile("shaders/fragment.glsl", GL_FRAGMENT_SHADER);

        try {
            shader->link();
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        scene.addShader(shader);

        auto camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        camera->setAspectRatio(800.0f / 600.0f);
        scene.addCamera(camera);
        scene.setActiveCamera(0);

        auto camera2 = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f)); // Camera 2
        camera2->setAspectRatio(800.0f / 600.0f);
        scene.addCamera(camera2);

        glfwSetWindowUserPointer(window, &scene);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        InputHandler::initialize(window, &scene);

        std::string files = "Models/BugattiV2/untitled.obj";
        try {
            auto meshes = loadMeshesFromObj(files);
            for (auto &mesh : meshes) {
                scene.addMesh(mesh);
            }
            std::cout << "Model loaded successfully: " << files << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Failed to load the model: " << e.what() << std::endl;
        }

        // Disable VSync
        glfwSwapInterval(0);

        // Render loop
        while (!glfwWindowShouldClose(window)) {

            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Input processing
            InputHandler::processInput(deltaTime);

            scene.render();

            fps_counter(window);

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
