#include <Arduino.h>
#include <encoder.h>
#include <Arduino.h>

// Encoder constructor
Encoder::Encoder(int input)
{
    pin = input;
    lapCount = 0;
    position = analogRead(pin);
    lastPos = 0;
}

// Encoder member functions

// Update encoder's lap count depending on whether it's crossed 
// between the top two quadrants, and in which direction
void update(){
    int pwmValue = pulseIn(pin, HIGH);

    if ((0.75*(maxPos) <= lastPos < maxPos) && (0 <= pwmValue <= 0.25*(maxPos)))
    {
        lapCount++;
    } 
    else if ((0.75*(maxPos) <= pwmValue < maxPos) && (0 <= lastPos <= 0.25*(maxPos)))
    {
        lapCount--;
    }
        lastPos = pwmValue;
    }
