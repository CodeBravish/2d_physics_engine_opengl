#ifndef BALL_H
#define BALL_H

#include "glad/glad.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Renderer/Shader.h"

using namespace std;
using namespace glm;

extern const float PI;
extern Shader BallShader;

class Ball {
   public:
    vec2 position;
    float radius;
    float mass;

    vec2 velocity = vec2(0.0f);

    vec4 color = vec4(1.0f);

    Ball(vec2 position, float radius, float mass)
        : position(position), radius(radius), mass(mass) {}

   private:
    GLuint VAO;
    GLuint segments = 20;

    void initVertexData() {
        vector<float> vertices;
        GLuint VBO;

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);


        for (int i = 0; i <= segments; i++) {
            GLfloat angle = 2 * PI * i / segments;
            GLfloat x = 0.5f * cos(angle);
            GLfloat y = 0.5f * sin(angle);
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

    void render() {
        BallShader.use();

        mat4 model = mat4(1.0f);

        model = translate(model, vec3(this->position, 1.0f));
        model = scale(model, vec3(vec2(this->radius), 1.0f));

        unsigned int modelLoc = glGetUniformLocation(BallShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, this->segments + 2);
    }
};

#endif