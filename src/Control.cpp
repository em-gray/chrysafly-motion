#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <Control.h>
#include <Encoder.h>

// ADJUSTABLE PARAMETERS
#define P_COEFF 150;
#define OFFSET 50;

// CONSTANTS -- DO NOT CHANGE
#define MIN_SPEED -400
#define MAX_SPEED 400
#define TOP_MOTOR 0
#define BOTTOM_MOTOR 1

bool isInit = false;
int cappedSpeed;
int flippedSpeed;

DualG2HighPowerMotorShield24v14 motorLib;

Control::Control() {}

void Control::run(float currPos, float nextPos, int motor) {
    float diff = nextPos - currPos;

    if (!isInit){
        motorLib.init();
        motorLib.enableDrivers();
        isInit = true;
    }
    
    int speed = diff*(float)P_COEFF;

    if (speed > MAX_SPEED) {
        cappedSpeed = MAX_SPEED - 1;
    } 
    else if (speed < MIN_SPEED) {
        cappedSpeed = MIN_SPEED + 1;
    }
    else {
        cappedSpeed = speed;
    }

    flippedSpeed = cappedSpeed*(-1);
    
    if (motor == TOP_MOTOR) {
        motorLib.setM1Speed(flippedSpeed);
    }
    else {
        motorLib.setM2Speed(flippedSpeed);
    }

}
