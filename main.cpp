#include "include/includes.h"
#include "include/shaders.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

// Create VAO, VBO and EBO for the triangle vertices
void createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
    float vertices[] = {
        // positions          // texture coords
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top right
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top left

        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top left

        // Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top right
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom right

        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top right
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom right
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom left

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // bottom left

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top right
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f   // bottom right
    };

    unsigned int indices[] = {
        0,  1,  2,  2,  3,  0,  // front face
        4,  5,  6,  6,  7,  4,  // back face
        8,  9,  10, 10, 11, 8,  // left face
        12, 13, 14, 14, 15, 12, // right face
        16, 17, 18, 18, 19, 16, // top face
        20, 21, 22, 22, 23, 20  // bottom face
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void *>(static_cast<uintptr_t>(3 * sizeof(float))));
    glEnableVertexAttribArray(1);


    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Main render loop
void renderLoop(GLFWwindow *window, class Shader &shader, unsigned int VAO, unsigned int EBO,
                unsigned int texture1, unsigned int texture2) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        processInput(window);

        (void)texture1;
        (void)texture2;
        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use shader program and draw
        shader.use();
        // glm::mat4 trans(1.0f);
        // trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.f));
        // trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f,
        // 0.0f,1.0f)); trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

        // glm::mat4 model(1.0f);
        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(50.0f),
        //                     glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // shader.setMat4("model", model);

        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBindTexture(GL_TEXTURE_2D, texture1);

        std::vector<glm::vec3> cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

        for (unsigned int i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * static_cast<float>(i);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,
                           reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
            // glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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

    {
        // Create shader program
        class Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

        // Create and bind buffers (VAO, VBO, EBO)
        unsigned int VAO, VBO, EBO;
        createBuffers(VAO, VBO, EBO);

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
        renderLoop(window, shader, VAO, EBO, texture1, texture2);

        // Cleanup
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    glfwTerminate();
    return 0;
}
