#include <Arduino.h>
#include "Encoder.h"

#define range 4096 // 12-bit system, also timeout speed 
#define bandgap 2048

static const int buffer = 2;
// should really be using a circular buffere here instead of an array 
// but we don't got time for that

Encoder::Encoder(){};

// Encoder constructor
void Encoder::init(int input) {
    pin = input;
    for (int i = 0; i < buffer; i++) {
        position[i].time=micros();
        position[i].numRevolutions=0;
        position[i].positionAngular=0;
    }
}

//returns angular velocity in revolutions per second. 
float Encoder::getVelocity() {
    return 0;
}

float Encoder::getPosition() {
    return position[0].numRevolutions + position[0].positionAngular/ (float) range;
}

/* Must be called frequently (considering 13RPM motor I would say minimum once per second)
   This due to the strange nature of the magnetic encoders. Other option 
   would be to run this function through interupts (~240Hz) though there is only one 
   interrupt arduino pin available thanks to motor driver shield(?) */
void Encoder::update() {
    position[1] = position[0];

    position[0].time = micros();
    position[0].positionAngular = pulseIn(pin, HIGH);
    //Serial.println(position[0].positionAngular);

    // if (position[0].positionAngular > 1) {  // corresponding to MAE3 datasheet, this is not technically correct 
    //     position[0].positionAngular -= 1;    // but good enough for our purposes.
    // }

    //calculate number of revolutions
    if (position[0].positionAngular < range/4) { 
        if (position[1].positionAngular - position[0].positionAngular > bandgap) { 
            position[0].numRevolutions++;
            // Serial.print("increase: ");
            // Serial.print(position[1].positionAngular);
            // Serial.print("  ");
            // Serial.println(position[0].positionAngular);
        }
    } else if (position[0].positionAngular > 3*range/4) {
        if (position[0].positionAngular - position[1].positionAngular > bandgap) {
            position[0].numRevolutions--;
            // Serial.print("decrease: ");
            // Serial.print(position[1].positionAngular);
            // Serial.print("  ");
            // Serial.println(position[0].positionAngular);
        }
    }

    // *** Logging Info:
    //Serial.print("Revolutions:");
    //Serial.println( position[0].numRevolutions+(position[0].positionAngular/ (float)range));
}

