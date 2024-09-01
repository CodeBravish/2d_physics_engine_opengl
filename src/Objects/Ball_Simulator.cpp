#include "Objects/Ball_Simulator.h"
#include <cstddef>
#include <iostream>

void simulateBalls(Ball* balls, size_t size, float delta_time) {
    for (size_t i = 0; i < size; i++) {
        Ball* ball = &balls[i];

        std::cout << "hello" << std::endl;

        ball->velocity.y += 9.8 * delta_time;
        ball->position += ball->velocity * delta_time;
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

        for (int j = 0; j < size; j++) {
        }
    }
}