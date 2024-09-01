#ifndef BALL_H
#define BALL_H

#include "glad/glad.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Shader.h"

using namespace std;
using namespace glm;

extern const float PI;

class Ball {
   public:
    vec2 position;
    float radius;
    float mass;

    vec2 velocity;
    float resCoeff;

    vec4 color;

    Ball(vec2 position, float radius, float mass, Shader &shader);

    void render();

   private:
    Shader shader;
    GLuint VAO;
    GLuint segments = 20;

    void initVertexData();
};

#endif