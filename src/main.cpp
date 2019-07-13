#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <EnableInterrupt.h>
#include <encoder.h>
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

#define encoder_A A3 // some analog in pin
#define encoder_B A5 // some analog in pin

int currentSpeed = 300;                            // speed (Set Point)
int targetSpeed = 0;                              // speed (actual value)

// Initialize 24v14 as our shield version
DualG2HighPowerMotorShield24v14 md;

void calibrate(){
  // This is where the menu state machine and its navigation is going to go.
  // This will set minSweep and maxSweep for each wing level
}          

int getPid(int targetValue, int currentValue)   {           
  // will return target velocity given current velocity, to be sent to motor driver
}

void setup() {

  analogReference(EXTERNAL);       
  Serial.begin(115200);                     // Current external ref is 3.3V
  pinMode(motor_A, OUTPUT);
  pinMode(motor_B, OUTPUT);
  pinMode(encoder_A, INPUT); 
  pinMode(encoder_B, INPUT);
  md.init();

  // init both encoders

  calibrate();  

}

void loop() {

  // put your main code here, to run repeatedly:
  // while (not at max limit - safety factor)
  // start motion of A
  // start motion of B

  // PID for both to keep velocity constant
  
}

