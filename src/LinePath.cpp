#include <Arduino.h>
#include <LinePath.h>

// ADJUSTABLE PARAMETER
#define INCREMENT 0.2;

LinePath::LinePath() {}

float LinePath::getOpenPath(float currPos) {
    float nextPos = currPos + INCREMENT;
    return nextPos;
}

float LinePath::getClosePath(float currPos) {
    float nextPos = currPos - INCREMENT;
    return nextPos;
}