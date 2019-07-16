#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <Encoder.h>
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
//     \ D / (3) \ D /
//      ^^^       ^^^

// Notes on calibration protocol:
// - bring wings in to min, set encoder lapCount to 0
// - bring wings out to max, counting encoder rotations. Save num rotations as max.

// Code for Master-duino
#define A_MOTOR_PIN 9 // some pwm pin (from motor driver)
#define B_MOTOR_PIN 10 // some pwm pin (from motor driver)

#define A_ENCODER_PIN A2 // some analog in pin
#define B_ENCODER_PIN A3 // some analog in pin

#define TIMER_SYNC A5 // for Arduino 0 to tell Arduino 1 to sync up

//MOTOR IDENTIFIERS
#define MOTOR_A 0; // top motor
#define MOTOR_B 1; // bottom motor

// #define READ_RATE 10 //miliseconds 

// VARIABLE INITIALIZATIONS
int i;
int time;
int timeRef;
int timeAbs;
bool resetArduino1;
float period;
float pos[2];
float nextPos[] = {0,0,0,0};
int calibrateMotor;
boolean calibrating;
boolean wasCalibrating = true;

float maxPosA;
float minPosA;

float maxPosB;
float minPosB;

// TODO: CHANGE THIS BACK TO ZEROES
float allMin[] = {1, 1, 1, 1};
float allMax[] = {15, 15, 15, 15};

Encoder A;
Encoder B;
//Calibration calibration;
LinePath linePath;
Control motorControl;
SigmoidPath sigmoidPath;

// Initialize 24v14 as our shield version
DualG2HighPowerMotorShield24v14 md;

//----------------------------------------------------------------------------------------
int muxRead(int A, int B, int C) {
  digitalWrite(13, C);
  digitalWrite(11, B);
  digitalWrite(5, A);
  //delay(100);
  return digitalRead(3);
}

bool readSafeStart(){
  return muxRead(1,1,1);
}

bool readFreezeSwitch(){
  return muxRead(0,0,0);
}
bool readMotorSwitch(){
  return muxRead(0,1,0);
}
bool readArduinoSwitch(){
  return muxRead(0,0,1);
}
bool readOpenButton(){
  return muxRead(0,1,1);
}
bool readCloseButton(){
  return muxRead(1,0,0);
}
bool readSetMaxButton(){
  return muxRead(1,0,1);
}
bool readSetMinButton(){
  return muxRead(1,1,0);
}
void setMax(int motor){
  if(motor == 0){
    maxPosA = A.getPosition();
  }
  if(motor == 1){
    maxPosB = B.getPosition();
  }
}
void setMin(int motor){
  if(motor == 0){
    minPosA = A.getPosition();
  }
  if(motor == 1){
    minPosB = B.getPosition();
  }
}
float getMin(int motor){
  if(motor == 0){
    return minPosA;
  }
  else { // if motor == 1
    return minPosB;
  }
}
float getMax(int motor){
  if(motor == 0){
    return maxPosA;
  }
  else { // if motor == 1
    return maxPosB;
  }
}
//----------------------------------------------------------------------------------------
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
  // Serial.print("POSITIONS ");
  // Serial.print(pos[0]);
  // Serial.print(" ");
  // Serial.println(pos[1]);

  // Read time each loop
  time = millis();
}

void calibratingMotor(int motor, boolean isClosing) {
  if(isClosing) {
    nextPos[motor] = linePath.getClosePath(pos[motor]);
  }
  else{
    nextPos[motor] = linePath.getOpenPath(pos[motor]);
    if(nextPos[motor] > pos[motor]) {
    }
  }
  motorControl.run(pos[motor], nextPos[motor], motor);
}

void calibrate(){
    wasCalibrating = true;

    calibrateMotor = readMotorSwitch();

    if((ARDUINO_0 + !readArduinoSwitch()) % 2) {
      // Calibrating 
      if(readOpenButton()) {
        calibratingMotor(calibrateMotor, false);
      }
      else if(readCloseButton()) {
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
    if (readSetMaxButton()) {
      setMax(calibrateMotor);
    } else if (readSetMinButton()) {
      setMin(calibrateMotor);
    }

}     

void postCalibrationSetup() {
  timeRef = time;
  wasCalibrating = false;
  
  if(ARDUINO_0) {
    for(i = 0; i < 2; i++) {
      allMin[i] = getMin(i);
      allMax[i] = getMax(i);
    }
  }
  else {
    for(i = 2; i < 4; i++) {
      allMin[i] = getMin(i-2);
      allMax[i] = getMax(i-2);
    }
  }

  sigmoidPath.Init(allMin, allMax);
}

void normalRun() {
  // Serial.print("Normal run loop");
  if(wasCalibrating) {
    postCalibrationSetup();
    Serial.print("Post calibration setup complete");
  }

  if ((time - timeRef)/1000.0 > period) {
    timeRef = time;
  }

  if(ARDUINO_0) {
    for(i = 0; i < 2; i++) {
      nextPos[i] = sigmoidPath.getNextPos(i, (time - timeRef)/1000.0);
      motorControl.run(pos[i], nextPos[i], i) ;
      if (i == 0){
        Serial.print("Target position: ");
        Serial.println(next);
        Serial.print("Time: ");
        Serial.println((time - timeRef)/1000.0);
      }

    }  
  }
  else {
      for(i = 2; i < 4; i++) {
      nextPos[i] = sigmoidPath.getNextPos(i, (time - timeRef)/1000.0);
      motorControl.run(pos[i-2], nextPos[i], i) ;   
    } 
  }
}


void setup() {
  //analogReference(EXTERNAL);       
  Serial.begin(9600);                   
  Serial.println("Online");
  pinMode(A_MOTOR_PIN, OUTPUT);
  pinMode(B_MOTOR_PIN, OUTPUT);
  pinMode(A_ENCODER_PIN, INPUT); 
  pinMode(B_ENCODER_PIN, INPUT);
  md.init();

  if (ARDUINO_0){
    pinMode(TIMER_SYNC, OUTPUT);
  } else { // ARDUINO_1
    pinMode(TIMER_SYNC, INPUT);
  };

  A.init(A_ENCODER_PIN);
  B.init(B_ENCODER_PIN);

  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  sigmoidPath.Init(allMin, allMax);
  period = sigmoidPath.getPeriod();
    // Init reset value
  resetArduino1 = 0;
  
}



void loop() {
  dataInit();
  //encoderUpdate();
  // UNCOMMENT FOR CALIBRATION MENU DEBUG
  // Serial.print(" ");
  // Serial.print(muxRead(0,0,0)); //Calibrate mode on or off 
  // Serial.print(" ");
  // Serial.print(muxRead(0,0,1)); //Arduino Switch
  // Serial.print(" ");
  // Serial.print(muxRead(0,1,0)); //Motor Switch
  // Serial.print(" ");
  // Serial.print(muxRead(0,1,1)); //Forewards
  // Serial.print(" ");
  // Serial.print(muxRead(1,0,0)); //Backwards
  // Serial.print(" ");
  // Serial.print(muxRead(1,0,1)); //Setmax 
  // Serial.print(" ");
  // Serial.print(muxRead(1,1,0)); //Setmin
  // Serial.println("");

  // Routine for periodicity and synchronization
  if (ARDUINO_0){
    if ((time - timeRef)/1000.0 > period) {
      timeRef = time;
      if (resetArduino1){
        resetArduino1 = 0;
      } else {
        resetArduino1 = 1;
      }
      digitalWrite(TIMER_SYNC, resetArduino1);
    }
  } else { // ARDUINO_1
    int readSync = digitalRead(TIMER_SYNC);
    if(readSync != resetArduino1){
      timeRef = time;
      resetArduino1 = readSync;
    }
  };


  //Check if calibration switch is on --> if on, run calibration protocol
  if (!readFreezeSwitch()) {
    // Check if open or close buttons are pressed (if both, default it open)
    normalRun();
  } 
  else {

    calibrate();
  }
};
