#include <Arduino.h>
#include <encoder.h>

// Encoder constructor
Encoder::Encoder(int input)
{
    pin = input;
    for (int i = 0; i < buffer; i++) {
        position[i].time=millis();
    }
}

//returns angular velocity of 
int getVelocity(){
    return 0;
}

int getPosition(){
    return 0;
}
// Update encoder's lap count depending on whether it's crossed 
// between the top two quadrants, and in which direction
void update(){
}