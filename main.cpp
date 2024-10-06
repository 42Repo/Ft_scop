#include "include/Camera.h"
#include "include/Mesh.h"
#include "include/Shader.h"
#include "include/includes.h"
#include <glm/gtc/matrix_transform.hpp>

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool  firstMouse = true;

// Timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

// Texture toggle
// static bool textureEnabled = false;
// static bool transitioning = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
    Camera &camera = *static_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void processInput(GLFWwindow *window) {
    // Close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Camera &camera = *static_cast<Camera *>(glfwGetWindowUserPointer(window));
    // Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(DOWNWARD, deltaTime);

    // // Toggle texture with a key (e.g., T key)
    // static bool textureKeyPressed = false;
    // if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !textureKeyPressed) {
    //     textureEnabled = !textureEnabled;
    //     transitioning = true;
    //     textureKeyPressed = true;
    // }
    // if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
    //     textureKeyPressed = false;
    // }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Camera &camera = *static_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    (void)xoffset;

    Camera &camera = *static_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera.processMouseScroll(static_cast<float>(yoffset));
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

// // Create VAO, VBO and EBO for the triangle vertices
// void createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {

//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
//                           reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
//     glEnableVertexAttribArray(0);

//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
//                           reinterpret_cast<void *>(static_cast<uintptr_t>(3 * sizeof(float))));
//     glEnableVertexAttribArray(1);
// }

// Main render loop
void renderLoop(GLFWwindow *window, class Shader &shader, Camera &camera, Mesh &cubeMesh) {
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input processing
        processInput(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Use shader program and draw
        shader.use();

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);

        // glBindVertexArray(VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        std::vector<glm::vec3> cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

        for (unsigned int i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle;
            if (i < 3)
                angle = 20.0f * static_cast<float>(i + 1) * static_cast<float>(glfwGetTime());
            else
                angle = 20.0f * static_cast<float>(i + 1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,
            //                reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
            cubeMesh.draw();
        }

        fps_counter(window);
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    {
        // Create shader program
        class Shader shader;
        Camera       camera(glm::vec3(0.0f, 0.0f, 3.0f));

        glfwSetWindowUserPointer(window, &camera);

        shader.addShaderFromFile("shaders/vertex.glsl", GL_VERTEX_SHADER);
        shader.addShaderFromFile("shaders/fragment.glsl", GL_FRAGMENT_SHADER);

        try {
            shader.link();
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
        // Create and bind buffers (VAO, VBO, EBO)
        Mesh cubeMesh(vertices, indices);

        // unsigned int VAO, VBO, EBO;
        // createBuffers(VAO, VBO, EBO);

        // Texture wrapping and filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Load and create a texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load("images/template.png", &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load texture1" << std::endl;
            return -1;
        }
        int            width2, height2, nrChannels2;
        unsigned char *data2 =
            stbi_load("images/template(1).png", &width2, &height2, &nrChannels2, 0);
        if (!data2) {
            std::cout << "Failed to load texture" << std::endl;
            return -1;
        }

        unsigned int texture1, texture2;
        glGenTextures(1, &texture1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        glGenTextures(1, &texture2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data2);

        // Render loop
        renderLoop(window, shader, camera, cubeMesh);
    }
    glfwTerminate();
    return 0;
}
