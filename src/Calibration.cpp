#include <Arduino.h>
#include <Encoder.h>
#include <Calibration.h>

// Encoder constructor
Calibration::Calibration() {

    // 3-bit addresses for command buttons in multiplexer
    const bool maxAddr[] = {1, 0, 0};
    const bool minAddr[] = {1, 0, 1};
    const bool openAddr[] = {0, 1, 0};
    const bool closeAddr[] = {0, 1, 1};

    // Initialize pin for top/bottom toggle switch
    int toggle_pin = 0; // TO BE CHANGED
    pinMode(toggle_pin, INPUT);

    // Pin numbers for encoders and multiplexer
    int top_encoder = 0; // TO BE CHANGED
    int bot_encoder = 1;
    int muxOut = 13;
    int muxS0 = 3;
    int muxS1 = 5;
    int muxS2 = 11;

    // Initialize multiplexer pins
    pinMode(muxOut, OUTPUT);
    pinMode(muxS0, INPUT); // These pins control the address connected to output
    pinMode(muxS1, INPUT);
    pinMode(muxS2, INPUT);

    // Initialize encoder objects
    Encoder top(top_encoder);
    Encoder bottom(bot_encoder);

    // Stores max and min positions of motors in number of rotations
    // Top motor is index 0, bottom is 1
    // Sets max and min to 0 at the start
    float maxPos[2] = {0.0, 0.0};
    float minPos[2] = {0.0, 0.0};
}

// Reads multiplex address connected to open command button
// Returns true if button pressed, otherwise false
bool Calibration::readOpenButton() {
    // Write proper address to multiplexer
    digitalWrite(muxS0, openAddr[0]);
    digitalWrite(muxS1, openAddr[1]);
    digitalWrite(muxS2, openAddr[2]);

    // Read multiplex output
    return digitalRead(muxOut);
}

// Reads multiplex address connected to close command button
// Returns true if button pressed, otherwise false
bool Calibration::readCloseButton() {
    // Write proper address to multiplexer
    digitalWrite(muxS0, closeAddr[0]);
    digitalWrite(muxS1, closeAddr[1]);
    digitalWrite(muxS2, closeAddr[2]);

    // Read multiplex output
    digitalRead(muxOut);
}

// Reads multiplex address connected to set max command button
// Returns true if button pressed, otherwise false
bool Calibration::readSetMaxButton() {
    // Write proper address to multiplexer
    digitalWrite(muxS0, maxAddr[0]);
    digitalWrite(muxS1, maxAddr[1]);
    digitalWrite(muxS2, maxAddr[2]);

    // Read multiplex output
    digitalRead(muxOut);
}

// Reads multiplex address connected to set min command button
// Returns true if button pressed, otherwise false
bool Calibration::readSetMinButton() {
    // Write proper address to multiplexer
    digitalWrite(muxS0, minAddr[0]);
    digitalWrite(muxS1, minAddr[1]);
    digitalWrite(muxS2, minAddr[2]);

    // Read multiplex output
    digitalRead(muxOut);
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