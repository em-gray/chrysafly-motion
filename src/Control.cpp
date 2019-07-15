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

DualG2HighPowerMotorShield24v14 motorLib;

Control::Control() {
    // // Pin numbers for encoders
    // /// TODO: set encoder pin IO!
    // int topEncoder = 0; 
    // int bottomEncoder = 1;

    // // Initialize encoder objects
    // Encoder encoders[2] = {Encoder(topEncoder), Encoder(bottomEncoder)};
}

void Control::run(float currPos, float nextPos, int motor) {
    // float currentPos = encoders[motor].getPosition();
    float diff = nextPos - currPos;
    // Serial.print("Next pos: ");
    // Serial.println(nextPos);

    if (!isInit){
        motorLib.init();
        motorLib.enableDrivers();
        isInit = true;
    }
    

    int speed = diff*(float)P_COEFF;
    // if (closing)
    //     int speed = speed - OFFSET;
    // }

    //Serial.print("Speed: ");
    
    if (motor == TOP_MOTOR) {
        //Serial.println(speed);
        
        motorLib.setM1Speed(speed);
    }
    else {
        //Serial.println(speed);
        motorLib.setM2Speed(speed);
    }
}

// OBSOLETE: decrease motor output if motor driver detects fault
// void driveMotor(int rawSpeed) {
    // if (!motorLib.getM1Fault && !motorLib.getM2Fault) {
    //     int speed = rawSpeed;
    // }
    // else  {
    //     int speed = rawSpeed/2;
    // }
// }
