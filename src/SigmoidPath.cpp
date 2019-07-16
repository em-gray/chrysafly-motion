#include <Arduino.h>
#include "SigmoidPath.h"

float cascade_offset = 7.0;
float sigmoid_length = 55.0;
float midpoint_pause = 5.0;
float total_duration = (sigmoid_length * 2) + (6 * cascade_offset) + midpoint_pause;

float clamp(float x, float startPoint, float endPoint){
    if (x < startPoint){
        return startPoint;
    } else if (x > endPoint){
        return endPoint;
    } else {
        return x;
    }
};

float getPath(Motor motor, float time, float offset, bool closing){
    if (closing)
    {
        float pos = clamp((time - (offset * cascade_offset)) / sigmoid_length, 0, 1);
        // Evaluate Hermite interpolation poluynomial, scale according to max
        // Serial.print("Position: ");
        // Serial.println(pos);
        return motor.maxPos - ((motor.maxPos - motor.minPos) * (pos * pos * ((3.0 - (2.0 * pos)))));
        
        }
    else 
    {
        float pos = clamp((-1.0 * ((time - (total_duration / 2.0) - sigmoid_length - (offset * cascade_offset)))) / sigmoid_length, 0, 1);
        return motor.maxPos - ((motor.maxPos - motor.minPos) * (pos * pos * ((3.0 - (2.0 * pos)))));
    }
};
    
float SigmoidPath::getNextPos(int motor, float time){
    float pos;
    if(time < total_duration / 2.0){
        // Generate opening paths based on motor params
        switch(motor){
            case 0:
                pos = getPath(m0, time, 3.0, true);
                break;
            case 1:
                pos = getPath(m1, time, 2.0, true);
                break;
            case 2:
                pos = getPath(m2, time, 1.0, true);
                break;
            case 3:
                pos = getPath(m3, time, 0.0, true);
                break;
            }
    } 
    else 
    {
        // Generate closing paths based on motor params
        switch(motor){
            case 0:
                pos = getPath(m0, time, 0.0, false);
                break;
            case 1:
                pos = getPath(m1, time, 1.0, false);
                break;
            case 2:
                pos = getPath(m2, time, 2.0, false);
                break;
            case 3:
                pos = getPath(m3, time, 3.0, false);
                break;
        }
    }
    return pos;
};

SigmoidPath::SigmoidPath(){
};

void SigmoidPath::Init(float minPos[], float maxPos[]){
    m0.maxPos = maxPos[0];
    m0.minPos = minPos[0];

    m1.maxPos = maxPos[1];
    m1.minPos = minPos[1];

    m2.maxPos = maxPos[2];
    m2.minPos = minPos[2];
    
    m3.maxPos = maxPos[3];
    m3.minPos = minPos[3];

    cascadeOffset = cascade_offset;
    sigmoidLength = sigmoid_length;
    midpointPause = midpoint_pause;
    totalDuration = (sigmoid_length * 2.0) + (6.0 * cascade_offset) + (midpoint_pause);
};

float SigmoidPath::getPeriod(){
    return sigmoid_length * 2 + 6 * cascade_offset + midpoint_pause;
};
