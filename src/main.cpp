#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <Encoder.h>
#include <Calibration.h>
#include <Control.h>
#include <SigmoidPath.h>
#include <LinePath.h>
#include <Wire.h>

#define ARDUINO_0 true
//#define ARDUINO_1 true

// Wing segment reference:
// Numbers are motors, letters refer to symmetric wing segents.
//     _.._        _.._
//   :' A  \  ,, /  A `;  
//   (  B   \(0)/   B  )
//    \------(1)------/
//     ) C  /(2)\  C (
//     \ D / (3)  \ D /
//      ^^^       ^^^

// Notes on calibration protocol:
// - bring wings in to min, set encoder lapCount to 0
// - bring wings out to max, counting encoder rotations. Save num rotations as max.

// Code for Master-duino
#define A_MOTOR_PIN 9 // some pwm pin (from motor driver)
#define B_MOTOR_PIN 10 // some pwm pin (from motor driver)

#define A_ENCODER_PIN A2 // some analog in pin
#define B_ENCODER_PIN A3 // some analog in pin

//MOTOR IDENTIFIERS
#define MOTOR_A 0; // top motor
#define MOTOR_B 1; // bottom motor

// #define READ_RATE 10 //miliseconds 

// VARIABLE INITIALIZATIONS
int i;

long time;
long timeRef;
float pos[2];
float nextPos[] = {0,0,0,0};
boolean calibrating;
boolean wasCalibrating;
int calibrateMotor;

Encoder A(A_ENCODER_PIN);
Encoder B(B_ENCODER_PIN);
Calibration calibration;
SigmoidPath sigmoidPath;
LinePath linePath;
Control motorControl;

// Initialize 24v14 as our shield version
DualG2HighPowerMotorShield24v14 md;

//-----------------------------------------------------------------------------------

void encoderUpdate() {

  A.update();
  B.update();

  // if (millis() - previousUpdate >= READ_RATE ) {
  //   previousUpdate = millis();  // or += READ_RATE...
  //   A.update();
  //   B.update();
  // }
}

void dataInit() {
  // Read position each loop
  encoderUpdate();
  pos[0] = A.getPosition();
  pos[1]= B.getPosition();

  // Read time each loop
  time = millis();
}

void calibratingMotor(int motor, boolean isClosing) {
  if(isClosing) {
    nextPos[motor] = linePath.getClosePath(pos[motor]);
  }
  else{
    nextPos[motor] = linePath.getOpenPath(pos[motor]);
  }
  motorControl.run(pos[motor], nextPos[motor], motor);
}

void calibrate(){
    wasCalibrating = true;

    calibrateMotor = calibration.readMotorSwitch();

    if((ARDUINO_0 + !calibration.readArduinoSwitch()) % 2) {
      // Calibrating 
      if(calibration.readOpenButton()) {
        calibratingMotor(calibrateMotor, false);
      }
      else if(calibration.readCloseButton()) {
        calibratingMotor(calibrateMotor, true);
      }
      else {
        motorControl.run(pos[calibrateMotor], pos[calibrateMotor], calibrateMotor);
      }
    }
    else {
      motorControl.run(pos[calibrateMotor], pos[calibrateMotor], calibrateMotor);
    }
    motorControl.run(pos[!calibrateMotor], pos[!calibrateMotor], !calibrateMotor);
  
    // Check if set max or set min buttons are pressed (if both default is set max)
    if (calibration.readSetMaxButton()) {
      calibration.setMax(calibrateMotor);
    } else if (calibration.readSetMinButton()) {
      calibration.setMin(calibrateMotor);
    }
}     

void normalRun() {
  if(wasCalibrating) {
    timeRef = time;
    wasCalibrating = false;
  }

  // TODO: add check here for whether timeFlag been reached --> overwrite "timeRef"
  if(ARDUINO_0) {
    for(i = 0; i < 2; i++) {
      nextPos[i] = sigmoidPath.getNextPos(i, time);
      motorControl.run(pos[i], nextPos[i], i) ;   
    }  
  }
  else {
      for(i = 2; i < 4; i++) {
      nextPos[i] = sigmoidPath.getNextPos(i, time);
      motorControl.run(pos[i-2], nextPos[i], i) ;   
    } 
  }
}

//----------------------------------------------------------------------------------------

void setup(float pos[2], long time) {
  //analogReference(EXTERNAL);       
  Serial.begin(9600);                   
  Serial.println("Online");
  pinMode(A_MOTOR_PIN, OUTPUT);
  pinMode(B_MOTOR_PIN, OUTPUT);
  pinMode(A_ENCODER_PIN, INPUT); 
  pinMode(B_ENCODER_PIN, INPUT);
  md.init();

  // Initialize Calibration object
  Calibration calibration = Calibration();
}

void loop() {
  dataInit();

  // Check if calibration switch is on --> if on, run calibration protocol
  if (!calibration.readCalibSwitch()) {
    // Check if open or close buttons are pressed (if both, default it open)
    normalRun();
  } 
  else {
    calibrate();
  }
}
