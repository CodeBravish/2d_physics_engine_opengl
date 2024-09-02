#include "Objects/Ball.h"

#include <vector>

Ball::Ball() : position(vec2(0)), radius(1.0f), mass(1.0f), shader_ID(0) {}

Ball::Ball(vec2 position, float radius, float mass, GLuint shader_ID)
    : position(position), radius(radius), mass(mass), shader_ID(shader_ID) {
    this->velocity = vec2(0.0f);
    this->resCoeff = 1.0f;
    this->color = vec4(1.0f);

    this->segments = 20;

    initVertexData();
}

void Ball::render() {
    // glUseProgram(this->shader_ID);

    mat4 model = mat4(1.0f);

    model = translate(model, vec3(this->position, 1.0f));
    model = scale(model, vec3(vec2(this->radius), 1.0f));

    unsigned int modelLoc = glGetUniformLocation(this->shader_ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, this->segments + 2);
}

void Ball::initVertexData() {
    vector<float> vertices;
    GLuint VBO;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= this->segments; i++) {
        GLfloat angle = 2 * PI * i / this->segments;
        GLfloat x = 1.0f * cos(angle);
        GLfloat y = 1.0f * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}