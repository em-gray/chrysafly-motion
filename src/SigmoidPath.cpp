#include <Arduino.h>
#include "SigmoidPath.h"

float cascadeOffset = 7.0;
float sigmoidLength = 55.0;
float midpointPause = 5.0;
float totalDuration = 157.0;

float clamp(float x, float startPoint, float endPoint){
    float output;
    if (x < startPoint){
        output = startPoint;
    }; 
    if (x > endPoint){
         output = endPoint;
    };
    return output;
};

float getPath(Motor motor, float time, float offset, bool closing){
    if (closing)
    {
        float pos = clamp((time - (offset * cascadeOffset)) / sigmoidLength, 0, 1);
        // Evaluate Hermite interpolation poluynomial, scale according to max
        Serial.print("TIME IN:");
        int input = (time - (offset * cascadeOffset)) / sigmoidLength;
        Serial.println(input);
        Serial.print("Position: ");
        Serial.println(pos);
        return motor.maxPos - ((motor.maxPos - motor.minPos) * (pos * pos * ((3.0 - 2.0) * pos)));
        }
    else 
    {
        float pos = clamp((-1.0 * ((time - (totalDuration / 2.0) - sigmoidLength - (offset * cascadeOffset)))) / sigmoidLength, 0, 1);
        return motor.maxPos - ((motor.maxPos - motor.minPos) * (pos * pos * ((3.0 - 2.0) * pos)));
    }
};
    
float SigmoidPath::getNextPos(int motor, float time){
    float pos;
    if(time < totalDuration / 2.0){
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

    totalDuration = sigmoidLength * 2.0 + 6.0 * cascadeOffset + midpointPause;
};

float SigmoidPath::getPeriod(){
    return totalDuration;
};
