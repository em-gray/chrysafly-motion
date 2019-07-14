#include <Arduino.h>
#include <Encoder.h>
#include <Calibration.h>

// Encoder constructor
Calibration::Calibration() {
    // 3-bit addresses for multiplexer
    const bool maxPin[] = {0, 0, 0}; // TO BE CHANGED
    const bool minPin[] = {0, 0, 0};
    const bool openPin[] = {0, 0, 0};
    const bool closePin[] = {0, 0, 0};

    // Initialize pin for top/bottom toggle switch
    int toggle_pin = 0; // TO BE CHANGED
    pinMode(toggle_pin, INPUT);

    // Pin numbers for encoders
    int top_encoder = 0; // TO BE CHANGED
    int bot_encoder = 1;

    // Initialize encoder objects
    Encoder top(top_encoder);
    Encoder bottom(bot_encoder);

    // Stores max and min positions of motors in number of rotations
    // Top motor is index 0, bottom is 1
    // Sets max and min to 0 at the start
    float maxPos[2] = {0.0, 0.0};
    float minPos[2] = {0.0, 0.0};
}

// Sets current position as max position of motor
void Calibration::setMax(){
    if (digitalRead(toggle_pin)){
    // Toggle switch is 1, set for bottom motor
        maxPos[1] = bottom.getPosition();
    } else {
    // Toggle switch is 0, set for top motor
        maxPos[0] = top.getPosition();
    }
}

// Sets current positoin as min positon of wings
void Calibration::setMin(){
    if (digitalRead(toggle_pin)){
    // Toggle switch is 1, set for bottom motor
        minPos[1] = bottom.getPosition();
    } else {
    // Toggle switch is 0, set for top motor
        minPos[0] = top.getPosition();
    }
}

// Returns max positon of given motor (0 for top, 1 for bottom)
float Calibration::getMax(int motor) {
    return maxPos[motor];
}

// Returns min position of given motor (0 for top, 1 for bottom)
float Calibration::getMin(int motor) {
    return minPos[motor];
}