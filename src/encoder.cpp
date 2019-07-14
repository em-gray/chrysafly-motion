#include <Arduino.h>
#include "Encoder.h"

#define range 4096 // 12-bit system, also timeout speed 
#define bandgap range * 3/4


// Encoder constructor
Encoder::Encoder(int input) {
    for (int i = 0; i < buffer; i++) {
        position[i].time=millis();
        position[i].numRevolutions=0;
        position[i].positionAngular=0;
    }
}

void Encoder::setMax() {
    return;
}

void Encoder::setMin() {
    return;
}

float Encoder::Max() {
    return 0;
}

float Encoder::Min() {
    return 0;
}

//returns angular velocity in revolutions per second. 
float Encoder::getVelocity() {
    return 0;
}

float Encoder::getPosition() {
    return position[0].numRevolutions + position[0].positionAngular/ (float) range;
}

//Oh I am soo sorry, arduino
void Encoder::shift() {
    for (int i = buffer; i > 1; i++) {
        position[buffer] = position[buffer - 1]; // 4 gets 3, 3 gets 2, 1 gets 0.
    }
}

/* Must be called frequently (considering 13RPM motor I would say minimum once per second)
   This due to the strange nature of the magnetic encoders. Other option 
   would be to run this function through interupts (~240Hz) though there is only one 
   interrupt arduino pin available thanks to motor driver shield(?) */
void Encoder::update() {
    shift();
    position[0].time = millis();

    position[0].positionAngular = pulseIn(pin, HIGH, range);
    if (position[0].positionAngular > 1) {  // corresponding to MAE3 datasheet, this is not technically correct 
        position[0].positionAngular - 2;    // but good enough for our purposes.
    }

    //numRevolutions check
    if (position[0].positionAngular > range/2) { 
        if (position[0].positionAngular - position[2].positionAngular > bandgap) { // the second one is actually the previous one due to the shift
            position[0].numRevolutions--;
        }
    } else {
        if (position[2].positionAngular - position[0].positionAngular > bandgap) {
            position[0].numRevolutions++;
        }
    }
}

