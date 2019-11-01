#include <Arduino.h>
#include <LinePath.h>

// ADJUSTABLE PARAMETER
#define OPEN_INCREMENT 0.15;
#define CLOSE_INCREMENT 1;

LinePath::LinePath() {}

float LinePath::getOpenPath(float currPos, boolean arduino_0) {

    if (arduino_0) {
        return currPos + OPEN_INCREMENT;
    }
    else {
        return currPos + CLOSE_INCREMENT;
    }
    
}

float LinePath::getClosePath(float currPos, boolean arduino_0) {

    if (arduino_0) {
        return currPos - CLOSE_INCREMENT;
    }
    else {
        return currPos - OPEN_INCREMENT;
    }

}