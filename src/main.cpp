#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include "Encoder.h"
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

  calibrate();  

}

void loop() {
  encoderUpdate();
}

