#include <Arduino.h>
#include <DualG2HighPowerMotorShield.h>
#include <EnableInterrupt.h>

#define PWM_PIN 3

int pwm_value;

// Initialize 24v14 as our shield version
//DualG2HighPowerMotorShield24v14 md;


void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_PIN, INPUT);
  Serial.begin(115200);
  // initialize motor drivers...
  // initialize serial

}

int currentState = 0;

void loop() {
  // put your main code here, to run repeatedly:
  pwm_value = pulseIn(PWM_PIN, HIGH);
  Serial.println(pwm_value);
  // check state
  // implement whatever reflects current state

  // current state loop is uninterrupted till state change
  //      default state:
  //        while not fully open:
  //          check open
  //          set motors to open direction (initial cascade on)
  //        
              


}