#include <Arduino.h>
#include <SigmoidPath.h>

SigmoidPath::SigmoidPath(float minPos[], float maxPos[]){
    cascadeOffset = 5000;
    sigmoidLength = 60000;
    midpointPause = 1000;

    totalDuration = sigmoidLength * 2 + 6 * cascadeOffset + midpointPause;
    
    m0.maxPos = maxPos[0];
    m0.minPos = minPos[0];

    m1.maxPos = maxPos[1];
    m1.minPos = minPos[1];
};

float clamp(float x, float startPoint, int endPoint){
    if (x < startPoint) x = startPoint;
    if (x > endPoint) x = endPoint;
    return x;
};


// Motor A is 0, Motor B is 1. 
// Assume Motor A opens before Motor B, and Motor B closes before Motor A.
// Assume time param is relative to start of full-motion loop timer
float SigmoidPath::getNextPos(int motor, int time, bool isClosing){
    int pos;
    if(!isClosing){
        // Generate opening paths based on motor params
        switch(motor){
            case 0:
                pos = clamp((time - cascadeOffset) / sigmoidLength, 0, 1);
                // Evaluate Hermite interpolation poluynomial
                return (m0.maxPos - m0.minPos) * (pos * pos * (3 - 2 * pos)) + m0.minPos;
            case 1:
                pos = clamp(time / sigmoidLength, m1.minPos, m1.maxPos);
                // Evaluate Hermite interpolation poluynomial
                return (m1.maxPos - m1.minPos) * (pos * pos * (3 - 2 * pos)) + m1.maxPos;
        }
    } else {
        // Generate closing paths based on motor params
        switch(motor){
            case 0:
                pos = clamp((-1 * (time - sigmoidLength)) / sigmoidLength, m0.minPos, m0.maxPos);
                // Evaluate Hermite interpolation poluynomial
                return (m0.maxPos - m0.minPos) * (pos * pos * (3 - 2 * pos)) + m0.minPos;
            case 1:
                pos = clamp((-1 * (time - sigmoidLength - cascadeOffset)) / sigmoidLength, m1.minPos, m1.maxPos);
                // Evaluate Hermite interpolation poluynomial
                return (m1.maxPos - m1.minPos) * (pos * pos * (3 - 2 * pos)) + m0.minPos;
        }
    }
};