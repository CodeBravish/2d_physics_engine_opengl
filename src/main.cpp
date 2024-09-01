#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Renderer/Shader.h"
#include "Objects/Ball.h"
#include "Objects/Ball_Simulator.h"

using namespace std;
using namespace glm;

unsigned int screen_width = 800;
unsigned int screen_height = 800;
unsigned int width = 5;
unsigned int height = 5;
const float PI = pi<float>();

float scaleFactor = 10.0f;
float resCoeff = 0.9f;
float radius = 0.01;
vec2 velocity = vec2(5.0f, 5.0f);
vec2 position = vec2(2.5f, 0.0f);

GLFWwindow *glSetup();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void handleCollisions();
vec4 rgba(uint red, uint green, uint blue, uint alpha);

int main() {
    // Setup GLFW
    GLFWwindow *window = glSetup();

    if (window == nullptr) return -1;
    //

    // Shader Constructor
    Shader BallShader("../assets/shaders/default.vert", "../assets/shaders/default.frag");

    // Vertex Data
    vector<float> vertices;
    int segments = 20;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= segments; i++) {
        float angle = 2 * PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }
    radius = radius * scaleFactor;
    //

    // Vertex Buffers
    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //

    // Projection
    // mat4 projection = ortho(0.0f, static_cast<float>(screen_width),
    //                         static_cast<float>(screen_height), 0.0f, -1.0f, 1.0f);
    mat4 projection = ortho(0.0f, static_cast<float>(width), static_cast<float>(height),
                            0.0f, -1.0f, 1.0f);

    float prev_time = static_cast<float>(glfwGetTime());
    float accumulator = 0.0f;
    float fixed_time_step = 1.0f / 60.0f;

    Ball balls[2];

    for (size_t i = 0; i < 2; i++) {
        balls[i] = Ball(vec2(5.0f / 3 * (i + 1), 2.5), 0.5, 1.0, BallShader.ID);
    }

    while (!glfwWindowShouldClose(window)) {
        float curr_time = static_cast<float>(glfwGetTime());
        float delta_time = curr_time - prev_time;
        prev_time = curr_time;

        accumulator += delta_time;

        while (accumulator >= fixed_time_step) {
            simulateBalls(balls, 2, fixed_time_step);
            handleBallCollisions(balls, 2, width, height);
            accumulator -= fixed_time_step;
        }

        // Render
        processInput(window);

        // Clear Screen
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate Shader
        BallShader.use();

        mat4 view = mat4(1.0f);

        unsigned int viewLoc = glGetUniformLocation(BallShader.ID, "view");
        unsigned int projectionLoc = glGetUniformLocation(BallShader.ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

        renderBalls(balls, 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void handleCollisions() {
    if (abs(position.y) + radius >= height) {
        position.y = height - radius;
        velocity.y *= -resCoeff;
    }
    if (abs(position.y) - radius <= 0.0f) {
        position.y = radius;
        velocity.y *= -resCoeff;
    }

    if (abs(position.x) + radius >= width) {
        position.x = width - radius;
        velocity.x *= -resCoeff;
    }
    if (abs(position.x) - radius <= 0.0f) {
        position.x = radius;
        velocity.x *= -resCoeff;
    }
}

GLFWwindow *glSetup() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window =
        glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, screen_width, screen_height, screen_width,
                            screen_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return nullptr;
    }

    return window;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    screen_width = width;
    screen_height = width;

    glViewport(0, 0, screen_width, screen_height);
}

vec4 rgba(uint red, uint green, uint blue, uint alpha) {
    return vec4(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}