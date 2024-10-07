#include "include/Texture.h"
#include "include/Camera.h"
#include "include/InputHandler.h"
#include "include/Mesh.h"
#include "include/Scene.h"
#include "include/Shader.h"
#include "include/includes.h"
#include <glm/gtc/matrix_transform.hpp>

// Timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

int main() {
    // Initialize GLFW
    std::vector<Vertex> vertices = {
        // positions           // texture coords
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {}, {0.0f, 0.0f}}, // bottom left
        {{0.5f, -0.5f, 0.5f}, {}, {1.0f, 0.0f}},  // bottom right
        {{0.5f, 0.5f, 0.5f}, {}, {1.0f, 1.0f}},   // top right
        {{-0.5f, 0.5f, 0.5f}, {}, {0.0f, 1.0f}},  // top left

        // Back face
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f}}, // bottom left
        {{0.5f, -0.5f, -0.5f}, {}, {1.0f, 0.0f}},  // bottom right
        {{0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},   // top right
        {{-0.5f, 0.5f, -0.5f}, {}, {0.0f, 1.0f}},  // top left

        // Left face
        {{-0.5f, 0.5f, 0.5f}, {}, {1.0f, 0.0f}},   // top right
        {{-0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},  // top left
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}}, // bottom left
        {{-0.5f, -0.5f, 0.5f}, {}, {0.0f, 0.0f}},  // bottom right

        // Right face
        {{0.5f, 0.5f, 0.5f}, {}, {1.0f, 0.0f}},   // top left
        {{0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},  // top right
        {{0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}}, // bottom right
        {{0.5f, -0.5f, 0.5f}, {}, {0.0f, 0.0f}},  // bottom left

        // Top face
        {{-0.5f, 0.5f, -0.5f}, {}, {0.0f, 1.0f}}, // top left
        {{0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},  // top right
        {{0.5f, 0.5f, 0.5f}, {}, {1.0f, 0.0f}},   // bottom right
        {{-0.5f, 0.5f, 0.5f}, {}, {0.0f, 0.0f}},  // bottom left

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}}, // top right
        {{0.5f, -0.5f, -0.5f}, {}, {1.0f, 1.0f}},  // top left
        {{0.5f, -0.5f, 0.5f}, {}, {1.0f, 0.0f}},   // bottom left
        {{-0.5f, -0.5f, 0.5f}, {}, {0.0f, 0.0f}}   // bottom right
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,  2,  3,  0,  // front face
        4,  5,  6,  6,  7,  4,  // back face
        8,  9,  10, 10, 11, 8,  // left face
        12, 13, 14, 14, 15, 12, // right face
        16, 17, 18, 18, 19, 16, // top face
        20, 21, 22, 22, 23, 20  // bottom face
    };

    GLFWwindow *window = initGLFW();
    if (!window)
        return -1;

    // Initialize GLAD
    if (!initGLAD())
        return -1;

    // Set viewport and callback
    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    {
        // Create shader program
        Scene scene;

        auto shader = std::make_shared<Shader>();

        // class Shader shader;

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

        glfwSetWindowUserPointer(window, camera.get());

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        InputHandler::initialize(window, camera.get());

        auto cubeMesh = std::make_shared<Mesh>(vertices, indices);
        scene.addMesh(cubeMesh);

        cubeMesh->setModelMatrix(glm::mat4(1.0f)); // Identity matrix

        auto cubeMesh2 = std::make_shared<Mesh>(vertices, indices);
        scene.addMesh(cubeMesh2);

        glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
        cubeMesh2->setModelMatrix(modelMatrix2);

        auto cubeMesh3 = std::make_shared<Mesh>(vertices, indices);
        scene.addMesh(cubeMesh3);

        glm::mat4 modelMatrix3 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
        modelMatrix3 = glm::rotate(modelMatrix3, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cubeMesh3->setModelMatrix(modelMatrix3);

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

            glm::mat4 updatedModelMatrix2 =
                glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
            float angle = currentFrame * glm::radians(50.0f); // Rotate at 50 degrees per second
            updatedModelMatrix2 =
                glm::rotate(updatedModelMatrix2, angle, glm::vec3(0.0f, 1.0f, 0.0f));
            cubeMesh2->setModelMatrix(updatedModelMatrix2);

            scene.render();

            fps_counter(window);
            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}
