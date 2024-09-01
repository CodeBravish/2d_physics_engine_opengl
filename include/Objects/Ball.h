#ifndef BALL_H
#define BALL_H

#include "glad/glad.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    Ball();
    Ball(vec2 position, float radius, float mass, GLuint shader_ID);

    void render();

   private:
    GLuint shader_ID;
    GLuint VAO;
    GLuint segments = 20;

    void initVertexData();
};

#endif