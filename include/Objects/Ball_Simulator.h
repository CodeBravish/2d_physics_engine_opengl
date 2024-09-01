#ifndef BALL_SIMULATOR_H
#define BALL_SIMULATOR_H

#include "Ball.h"

void simulateBalls(Ball* balls, size_t size, float delta_time);
void renderBalls(Ball* balls, size_t size);
void handleBallCollisions(Ball* balls, size_t size, unsigned int x_boundary,
                          unsigned int y_boundary);

#endif