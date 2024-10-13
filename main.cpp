#include "include/Camera.h"
#include "include/InputHandler.h"
#include "include/Mesh.h"
#include "include/ObjLoader.h"
#include "include/Scene.h"
#include "include/Shader.h"
#include "include/Texture.h"
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
    ObjLoader   objLoader(filePath);              // Créer un loader et charger le fichier
    const auto &objects = objLoader.getObjects(); // Récupérer tous les objets

    std::vector<std::shared_ptr<Mesh>> meshes;

    // for (const auto &obj : objects) {
    //     std::cout << "Object: " << obj.name << std::endl;
    //     std::cout << "Vertices: " << std::endl;
    //     for (const auto &vertex : obj.vertices) {
    //         std::cout << "  Position: " << vertex.position.x << ", " << vertex.position.y << ", "
    //                   << vertex.position.z << std::endl;
    //         std::cout << "  Normal: " << vertex.normal.x << ", " << vertex.normal.y << ", "
    //                   << vertex.normal.z << std::endl;
    //         std::cout << "  TexCoords: " << vertex.texCoords.x << ", " << vertex.texCoords.y
    //                   << std::endl;
    //     }
    //     // std::cout << "Indices: " << std::endl;
    //     // for (const auto &index : obj.indices) {
    //     //     std::cout << "  " << index << std::endl;
    //     // }
    //     std::cout << std::endl;
    // }

    for (const auto &obj : objects) {
        auto mesh = std::make_shared<Mesh>(obj.vertices, obj.indices);
        meshes.push_back(mesh);
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

        // Load textures
        auto texture1 = std::make_shared<Texture>("images/Untitled.png");
        auto texture2 = std::make_shared<Texture>("images/Untitled2.jpg");
        scene.addTexture(texture1);
        scene.addTexture(texture2);

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
