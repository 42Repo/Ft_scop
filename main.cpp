#include "include/includes.hpp"
#include <fstream>
#include <sstream>

void         framebuffer_size_callback(GLFWwindow *window, int width, int height);
void         processInput(GLFWwindow *window);
std::string  loadShaderSource(const std::string &filename);
GLFWwindow  *initGLFW();
bool         initGLAD();
unsigned int compileShader(unsigned int shaderType, const std::string &filePath);
unsigned int linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
void         createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
void renderLoop(GLFWwindow *window, unsigned int shaderProgram, unsigned int VAO, unsigned int EBO);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::string loadShaderSource(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
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

// Compile a shader from file
unsigned int compileShader(unsigned int shaderType, const std::string &filePath) {
    unsigned int shader = glCreateShader(shaderType);
    std::string  shaderSource = loadShaderSource(filePath);
    const char  *shaderSourceCStr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceCStr, NULL);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

// Link shaders to create a program
unsigned int linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

// Create VAO, VBO and EBO for the triangle vertices
void createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
    // float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    float vertices[] = {
        -1.f,  0.f,  0.0f, // 1st triangle : left
        -0.5f, 1.f,  0.0f, // 1st triangle : top
        0.f,   -0.f, 0.0f, // 1st triangle : middle
        1.f,   0.f,  0.0f, // 2st triangle : right
        0.5f,  1.f,  0.0f, // 2st triangle : top

    };
    unsigned int indices[] = {
        0, 1, 2, // 1st triangle
        3, 4, 2, // 2st triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
    glEnableVertexAttribArray(0);

    // Wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Main render loop
void renderLoop(GLFWwindow *window, unsigned int shaderProgram, unsigned int VAO,
                unsigned int EBO) {
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program and draw
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 2, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                       reinterpret_cast<void *>(static_cast<uintptr_t>(0)));

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    // Initialize GLFW
    GLFWwindow *window = initGLFW();
    if (!window)
        return -1;

    // Initialize GLAD
    if (!initGLAD())
        return -1;

    // Set viewport and callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, "shaders/vertex.glsl");
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");

    // Link shaders into a program
    unsigned int shaderProgram = linkShaderProgram(vertexShader, fragmentShader);

    // Delete the shaders as they're no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create and bind buffers (VAO, VBO, EBO)
    unsigned int VAO, VBO, EBO;
    createBuffers(VAO, VBO, EBO);

    // Render loop
    renderLoop(window, shaderProgram, VAO, EBO);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}
