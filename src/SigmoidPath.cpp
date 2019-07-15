#include <Arduino.h>
#include <SigmoidPath.h>

int cascadeOffset = 5000;
int sigmoidLength = 60000;
int midpointPause = 5000;
int totalDuration = sigmoidLength * 2 + 6 * cascadeOffset + midpointPause;

float clamp(float x, float startPoint, int endPoint){
    if (x < startPoint) x = startPoint;
    if (x > endPoint) x = endPoint;
    return x;
};

float getPath(Motor motor, int time, int offset, bool closing){
    if (closing){
        float pos = clamp((time - offset * cascadeOffset) / sigmoidLength, 0, 1);
        // Evaluate Hermite interpolation poluynomial, scale according to max
        return motor.maxPos - (motor.span) * (pos * pos * (3 - 2 * pos));
    }
    else {
        float pos = clamp((-1 * (time - (totalDuration / 2) - sigmoidLength - offset * cascadeOffset)) / sigmoidLength, 0, 1);
        return motor.maxPos - (motor.span) * (pos * pos * (3 - 2 * pos));
    }
};

int SigmoidPath::getPeriod(){
    return totalDuration;
}

SigmoidPath::SigmoidPath(float minPos[], float maxPos[]){
    m0.maxPos = maxPos[0];
    m0.minPos = minPos[0];

    m1.maxPos = maxPos[1];
    m1.minPos = minPos[1];

    m2.maxPos = maxPos[2];
    m2.minPos = minPos[2];

    m3.maxPos = maxPos[3];
    m3.minPos = minPos[3];
};

float SigmoidPath::getNextPos(int motor, int time){
    bool closing = 1;
    if(time < totalDuration / 2){
        // Generate opening paths based on motor params
        switch(motor){
            case 0:
                return getPath(m0, time, 3, closing);
            case 1:
                return getPath(m1, time, 2, closing);
            case 2:
                return getPath(m2, time, 1, closing);
            case 3:
                return getPath(m3, time, 0, closing);
        }
    } else {
        // Generate closing paths based on motor params
        switch(motor){
            case 0:
                return getPath(m0, time, 0, !closing);
            case 1:
                return getPath(m1, time, 1, !closing);
            case 2:
                return getPath(m2, time, 2, !closing);
            case 3:
                return getPath(m3, time, 3, !closing);
        }
    }
    // safe return in case something is missed
    return 10;
};