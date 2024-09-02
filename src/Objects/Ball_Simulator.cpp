#include "Objects/Ball_Simulator.h"
#include <cstddef>
#include <glm/geometric.hpp>

void simulateBalls(Ball* balls, size_t size, float delta_time) {
    for (size_t i = 0; i < size; i++) {
        Ball* ball = &balls[i];


        ball->position += ball->velocity * delta_time;
        // ball->velocity.y += 9.8 * delta_time; //Gravity
    }
}

void renderBalls(Ball* balls, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Ball* ball = &balls[i];
        ball->render();
    }
}

void handleBallCollisions(Ball* balls, size_t size, unsigned int x_boundary,
                          unsigned int y_boundary) {
    for (size_t i = 0; i < size; i++) {
        Ball* ball = &balls[i];

        // >>
        // Boundary Collisions

        if (abs(ball->position.x) + ball->radius >= x_boundary) {
            ball->position.x = x_boundary - ball->radius;
            ball->velocity.x *= -ball->resCoeff;
        } else if (abs(ball->position.x) - ball->radius <= 0.0f) {
            ball->position.x = ball->radius;
            ball->velocity.x *= -ball->resCoeff;
        }

        if (abs(ball->position.y) + ball->radius >= y_boundary) {
            ball->position.y = y_boundary - ball->radius;
            ball->velocity.y *= -ball->resCoeff;
        } else if (abs(ball->position.y) - ball->radius <= 0.0f) {
            ball->position.y = ball->radius;
            ball->velocity.y *= -ball->resCoeff;
        }

        // Ball Collisions
        for (int j = i; j < size; j++) {
            Ball* ball2 = &balls[j];
            if (ball == ball2) continue;

            const float ball_boundary = ball->radius + ball2->radius;
            const vec2 normal_vector = ball->position - ball2->position;
            const float ball_distance = static_cast<float>(length(normal_vector));

            if (ball_distance < ball_boundary) {
                const vec2 normal = normalize(normal_vector);
                const vec2 tangent = vec2(normal.y, -normal.x);

                const vec2 relative_boundary_origin = normal * ball_boundary;

                ball->position = ball2->position + relative_boundary_origin;

                const float vn1 = static_cast<float>(dot(ball->velocity, normal));
                const float vt1 = static_cast<float>(dot(ball->velocity, tangent));
                const float vn2 = static_cast<float>(dot(ball2->velocity, normal));
                const float vt2 = static_cast<float>(dot(ball2->velocity, tangent));

                const float vn1Prime =
                    (ball->resCoeff * vn1 * (ball->mass - ball2->mass) +
                     2.0 * ball2->mass * vn2) /
                    (ball->mass + ball2->mass);

                const float vn2Prime =
                    (ball2->resCoeff * vn2 * (ball2->mass - ball->mass) +
                     2.0 * ball->mass * vn1) /
                    (ball2->mass + ball->mass);

                const float vt1Prime = vt1;
                const float vt2Prime = vt2;

                ball->velocity = (normal * vn1Prime) + (tangent * vt1Prime);
                ball2->velocity = (normal * vn2Prime) + (tangent * vt2Prime);
            }
        }
    }
}