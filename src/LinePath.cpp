#include <Arduino.h>
#include <LinePath.h>

// ADJUSTABLE PARAMETER
#define OPEN_INCREMENT 0.01;
#define CLOSE_INCREMENT 1;

LinePath::LinePath() {}

float LinePath::getOpenPath(float currPos) {
    float nextPos = currPos + OPEN_INCREMENT;
    return nextPos;
}

float LinePath::getClosePath(float currPos) {
    float nextPos = currPos - CLOSE_INCREMENT;
    return nextPos;
}