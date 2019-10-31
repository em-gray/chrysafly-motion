#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <Encoder.h>
#include <Control.h>
#include <SigmoidPath.h>
#include <LinePath.h>
#include <Wire.h>

#define ARDUINO_0 true //front/top arduino
//#define ARDUINO_1 true

// Wing segment reference:
// Numbers are motors, letters refer to symmetric wing segents.

// ***NOTE: As of July 18th, 2019: Only segment C is physically controllable
// due to motor mounting issues. For BiTF, this means only controlling segment
// C, and only doing so manually while in calibration mode. More to come.

//     _.._        _.._
//   :' A  \  ,, /  A `;  
//   (  B   \(0)/   B  )
//    \------(1)------/
//     ) C  /(2)\  C (
//     \ D / (3) \ D /
//      ^^^       ^^^

// Notes on calibration protocol for auto mode:
// - bring wings in to min, set encoder lapCount to 0
// - bring wings out to max, click out of calibration mode

// Code for Master-duino
#define A_MOTOR_PIN 9 // a pwm pin (from motor driver)
#define B_MOTOR_PIN 10 // a pwm pin (from motor driver)

#define A_ENCODER_PIN A2 // an analog in pin
#define B_ENCODER_PIN A3 // an analog in pin

#define TIMER_SYNC A5 // for Arduino 0 to tell Arduino 1 to sync up

//MOTOR IDENTIFIERS
#define MOTOR_A 0; // top motor
#define MOTOR_B 1; // bottom motor

// VARIABLE INITIALIZATIONS
int i;
unsigned long time;
unsigned long timeRef;
int timeAbs;
boolean resetArduino1;
float period;
float pos[2];
float prev1[] = {0, 0};
float prev2[] = {0, 0};
float nextPos[] = {0, 0, 0, 0};
// TODO: UNCOMMMENT THIS
// float nextPos[] = {0,0,0,0};
int calibrateMotor;
boolean calibrating;
boolean wasCalibrating = true;
boolean wasRunning = false;
boolean safe = false;

float maxPosA;
float minPosA;

float maxPosB;
float minPosB;

// TODO: CHANGE THIS BACK TO ZEROES --> COMPLETED
float allMin[] = {0.0, 0.0, 0.0, 0.0};
float allMax[] = {0.0, 0.0, 0.0, 0.0};

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
  return muxRead(1,0,0);
}
bool readCloseButton(){
  return muxRead(0,1,1);
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
    Serial.print("Max A set as: ");
    Serial.println(maxPosA);
  }
  if(motor == 1){
    maxPosB = B.getPosition();
    Serial.print("Max B set as: ");
    Serial.println(maxPosB);
  }
}
void setMin(int motor){
  if(motor == 0){
    minPosA = A.getPosition();
    Serial.print("Min A set as: ");
    Serial.println(minPosA);
  }
  if(motor == 1){
    minPosB = B.getPosition();
    Serial.print("Min B set as: ");
    Serial.println(minPosB);
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
  prev2[0]=prev1[0];
  prev2[1]=prev1[1];
  prev1[0]=pos[0];
  prev1[1]=pos[1];
  pos[0] = A.getPosition();
  pos[1]= B.getPosition();

  // DEBUGGING
  Serial.print("POS: ");
  Serial.print(pos[0]);
  Serial.print(" ");
  Serial.println(pos[1]);
  Serial.print("PREV1: ");
  Serial.print(prev1[0]);
  Serial.print(" ");
  Serial.println(prev1[1]);
  Serial.print("PREV2: ");
  Serial.print(prev2[0]);
  Serial.print(" ");
  Serial.println(prev2[1]);

  // Read time each loop
  time = millis();
  // if ((time - timeRef)/1000.0 > period){
  //   timeRef = time;
  // }
}

void calibratingMotor(int motor, boolean isClosing) {
  if(isClosing) {
    nextPos[motor] = linePath.getClosePath(pos[motor]);
  }
  else{
    nextPos[motor] = linePath.getOpenPath(pos[motor]);
    // if(nextPos[motor] > pos[motor]) {
    // }
  }
  motorControl.run(pos[motor], nextPos[motor], motor);
}

void calibrate(){
    Serial.println("NOW CALIBRATING");
    wasCalibrating = true;

    calibrateMotor = readMotorSwitch();

    if(readArduinoSwitch()) {
      Serial.println("In this Arduino");
      
      // if(true){
      //   calibratingMotor(calibrateMotor, false);
      //   delay(3);
      //   calibratingMotor(calibrateMotor, true);
      //   delay(3);
      // }
      // Calibrating 
      // IMPORTANT TODO: UNCOMMENT THIS!
      if(readOpenButton()) {
        calibratingMotor(calibrateMotor, false);
        wasRunning = true;
        // Serial.println("Open button read");
      }
      else if(readCloseButton()) {
        calibratingMotor(calibrateMotor, true);
        wasRunning = true;
        // Serial.println("Close button read");
      }
      else {
        if(wasRunning){
          motorControl.run(pos[calibrateMotor], pos[calibrateMotor], calibrateMotor);
          Serial.print("NextPos: ");
          Serial.println(pos[calibrateMotor]);
        }
        else {
          motorControl.run(pos[calibrateMotor], prev1[calibrateMotor], calibrateMotor);
          Serial.print("NextPos: ");
          Serial.println(prev1[calibrateMotor]);
        }
      }

      if (readSetMaxButton()) {
        // Serial.println("Max button read");
        setMax(calibrateMotor);
      } 
      else if (readSetMinButton()) {
        // Serial.println("Min button read");
        setMin(calibrateMotor);
      }
    }

    else {
      if(wasRunning){
        motorControl.run(pos[calibrateMotor], pos[calibrateMotor], calibrateMotor);
        Serial.print("NextPos: ");
        Serial.println(pos[calibrateMotor]);
      }
      else {
        motorControl.run(pos[calibrateMotor], prev1[calibrateMotor], calibrateMotor);
        Serial.print("NextPos: ");
        Serial.println(prev1[calibrateMotor]);
      }
    }
    if(wasRunning){
      motorControl.run(pos[!calibrateMotor], pos[!calibrateMotor], !calibrateMotor);
      Serial.print("NextPos: ");
      Serial.println(pos[!calibrateMotor]);
      wasRunning = false;
    }
    else {
      motorControl.run(pos[!calibrateMotor], prev1[!calibrateMotor], !calibrateMotor);
      Serial.print("NextPos: ");
      Serial.println(prev1[!calibrateMotor]);
    }

    // Check if set max or set min buttons are pressed (if both default is set max)
    safe = false;

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
  Serial.println("NOW RUNNING");

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
      motorControl.run(pos[i], nextPos[i], i);
    }  
  }
  else {
      for(i = 2; i < 4; i++) {
      nextPos[i] = sigmoidPath.getNextPos(i, (time - timeRef)/1000.0);
      Serial.println(nextPos[i]);
      motorControl.run(pos[i-2], nextPos[i], i-2) ;   
    } 
  }

  wasRunning = true;
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

  if (!ARDUINO_0){
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
  time = millis();

  encoderUpdate();
  pos[0] = A.getPosition();
  pos[1]= B.getPosition();
  prev1[0]=pos[0];
  prev1[1]=pos[1];
  prev2[0]=prev1[0];
  prev2[1]=prev1[1];
  delay(1000);
}



void loop() {

  dataInit();
  Serial.print("Pos A: ");
  Serial.println(A.getPosition());
  Serial.print("Pos B: ");
  Serial.println(B.getPosition());
  time = millis();
  //encoderUpdate();
  // UNCOMMENT FOR CALIBRATION MENU DEBUG
  // Serial.print("Kill        ");
  // Serial.println(muxRead(0,0,0)); //Calibrate mode on or off 
  // Serial.print("Arduino   ");
  // Serial.println(muxRead(0,0,1)); //Arduino Switch
  // Serial.print("motor   ");
  // Serial.println(muxRead(0,1,0)); //Motor Switch
  // Serial.print("forward   ");
  // Serial.println(readOpenButton()); //Forewards
  // Serial.print("backward    ");
  // Serial.println(readCloseButton()); //Backwards
  // Serial.print("max   ");
  // Serial.println(muxRead(1,0,1)); //Setmax 
  // Serial.print("min   ");
  // Serial.println(muxRead(1,1,0)); //Setmin
  // Serial.print("safe    ");
  // Serial.println(muxRead(1,1,1));

  // Routine for periodicity and synchronization between Arduinos
  if (!ARDUINO_0){
    // Serial.print("time: ");
    // Serial.println(time);
    // Serial.print("timeRef: ");
    // Serial.println(timeRef);
    if ((time - timeRef)/1000.0 > period) {
      //Serial.println(resetArduino1);
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
    //Serial.println(readSync);
    if(readSync != resetArduino1){
      //Serial.println("SENT");
      // TODO: double check communication
      timeRef = time;
      resetArduino1 = readSync;
    }
  };

  // UNCOMMENT TO DEBUG CASCADE MOTION
  // Serial.print("m0: ");
  // Serial.print(sigmoidPath.getNextPos(0,(time - timeRef)/1000.0));

  // Serial.print(" m1: ");
  // Serial.print(sigmoidPath.getNextPos(1,(time - timeRef)/1000.0));

  // Serial.print(" m2: ");
  // Serial.print(sigmoidPath.getNextPos(2,(time - timeRef)/1000.0));

  // Serial.print(" m3: ");
  // Serial.println(sigmoidPath.getNextPos(3,(time - timeRef)/1000.0));

  //Check if calibration switch is on --> if on, run calibration protocol
  //Serial.print("Freeze switch: ");
  //Serial.println(readFreezeSwitch());

  // TODO: UNCOMMENT THIS!!!
  if (!readFreezeSwitch()) {
    // Serial.print("Previously calibrating: ");
    //Serial.println(wasCalibrating);
    if(wasCalibrating) {   
      if(readSafeStart()) {
        normalRun();
      }
      else{
        calibrate();
      }
    }
    else {
      normalRun();
    }
    // Check if open or close buttons are pressed (if both, default it open)
  } 
  else {
    calibrate();
  }
};
