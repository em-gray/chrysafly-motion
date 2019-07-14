#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <encoder.h>
#include <Motor.h>
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

bool isClosing;
int timerStart;

// Position variables
int cascadeOffset = 5000; // ms
int sigmoidLength = 60000; // ms
int midpointPause = 1000; // ms

int totalDuration = sigmoidLength * 2 + 6 * cascadeOffset + midpointPause;




// Initialize 24v14 as our motor driver
DualG2HighPowerMotorShield24v14 md;

void calibrate(){
  // This is where the menu state machine and its navigation is going to go.
  // This will set minSweep and maxSweep for each wing level
}          

// assume min-max will take a sigmoidLength: 6000ms
// assume minimum position is zero
// Motor A is 0, Motor B is 1
float getSigmoid(int motor, float maxPos, float x, bool isClosing){
  // basically the smoothstep function
  // scale, bias and saturate x to 0..
  x = clamp((x / maxPos), 0, sigmoidLength);
  // evaluate hermite interpolation polynomial
  return x * x* (3 - 2 * x);
}


float clamp(float x, float startPoint, int endPoint){
  if (x < startPoint)
    int x = startPoint;
  if (x > endPoint)
    x = endPoint;
  return x;
}

void setup() {
  Serial.begin(115200);                     
  pinMode(motor_A, OUTPUT);
  pinMode(motor_B, OUTPUT);
  pinMode(encoder_A, INPUT); 
  pinMode(encoder_B, INPUT);

  // Initialize motor driver
  md.init();
  md.calibrateCurrentOffsets();
  md.enableDrivers();
  delay(1); // The drivers require a maximum of 1ms to elapse when brought out of sleep mode.

  // init both encoders

  calibrate();  

  timerStart = millis();
}

// At the start of the loop, assuming proper calibration, the wings should be
// at their max desired open position
void loop() {
  


  }


  // put your main code here, to run repeatedly:
  // while (not at max limit - safety factor)
  // start motion of A
  // start motion of B

  // PID for A to keep velocity constant

  
}

