#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include "Encoder.h"
#include <Calibration.h>
#include <Wire.h>

// Wing segment reference:
//     _.._        _.._
//   :' A  \     /  A `;  
//   (  B   \(*)/   B  )
//    \------(*)------/
//     ) C  /(*)\  C (
//     \ D /  ^  \ D /
//      ^^^       ^^^

// Notes on calibration protocol:
// - bring wings in to min, set encoder lapCount to 0
// - bring wings out to max, counting encoder rotations. Save num rotations as max.

// Code for Master-duino
#define motor_A 9 // some pwm pin (from motor driver)
#define motor_B 10 // some pwm pin (from motor driver)

#define encoder_A A2 // some analog in pin
#define encoder_B A3 // some analog in pin

#define READ_RATE 10 //miliseconds 

int currentSpeed = 300;                            // speed (Set Point)
int targetSpeed = 0;                              // speed (actual value)
long previousUpdate = millis();

Encoder A(encoder_A);
Encoder B(encoder_B);
Calibration calibration;

// Initialize 24v14 as ouçr shield version
DualG2HighPowerMotorShield24v14 md;

void encoderUpdate() {
  if (millis() - previousUpdate >= READ_RATE ) {
    previousUpdate = millis();  // or += READ_RATE...
    A.update();
    //B.update();
  }
}

void calibrate(){
  // This is where the menu state machine and its navigation is going to go.
  // This will set minSweep and maxSweep for each wing level

}          

int getPid(int targetValue, int currentValue)   {           
  // will return target velocity given current velocity, to be sent to motor driver
  return 0;
}

void setup() {
  //analogReference(EXTERNAL);       
  Serial.begin(9600);                   
  Serial.println("Online");
  pinMode(motor_A, OUTPUT);
  pinMode(motor_B, OUTPUT);
  pinMode(encoder_A, INPUT); 
  pinMode(encoder_B, INPUT);
  md.init();

  // init both encoders

  // Initialize Calibration object
  Calibration calibration = Calibration();

}

void loop() {
  encoderUpdate();

  // Check if calibration switch is on
  // If so, run calibration protocall
  if (calibration.readCalibSwitch()) {
    // Check if open or close buttons are pressed (if both, default it open)
    if (calibration.readOpenButton()) {
        
    } else if (calibration.readCloseButton()) {
      
    }

    // Check if set max or set min buttons are pressed (if both default is set max)
    if (calibration.readSetMaxButton()) {
      calibration.setMax();
    } else if (calibration.readSetMinButton()) {
      calibration.setMin();
    }

    //Exampled of getting motor max/min vals
    calibration.getMax(1); // Max position of bottom motor
    calibration.getMin(0); // Min position of top motor
  } 
}
