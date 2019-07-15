#include <Arduino.h>
#include <Encoder.h>
#include <Calibration.h>    

// Calibration constructor
Calibration::Calibration() {};

void Calibration::Init() {
     // Pin numbers for encoders and multiplexer
     // These could be arguments for Init
    top_encoder = 3; // TO BE CHANGED
    bot_encoder = 5;
    sigOut = 0;
    sigC = 1;
    sigB = A2;
    sigA = A3;

    // Initialize multiplexer pins
    pinMode(sigOut, OUTPUT);
    pinMode(sigC, INPUT); // These pins control the address connected to output
    pinMode(sigB, INPUT);
    pinMode(sigA, INPUT);

    // Initialize encoder objects
    Encoder top(top_encoder);
    Encoder bottom(bot_encoder);

     // Stores max and min positions of motors in number of rotations
    // Top motor is index 0, bottom is 1
    // To start, store as 0.0
    maxPos[0] = 0.0;
    maxPos[1] = 0.0;

    minPos[0] = 0.0;
    minPos[1] = 0.0;
    
    // const int multiplexor[9][3] = {
    //     {0, 0, 0},
    //     {0, 0, 1},
    //     {0, 1, 0},
    //     {0, 1, 1},
    //     {1, 0, 0},
    //     {1, 0, 1},
    //     {1, 1, 0}
    // };
    // /*
    // 1 - calibration mode toggle
    // 2 - arduino (top vs. bottom) toggle
    // 3 - motor (top-0 vs. bottom-1) toggle
    // 4 - open motor button
    // 5 - close motor button
    // 6 - set max button
    // 7 - set min button
    // */
  
};

// Reads multiplexer address connected to calibration mode switch
// Returns true if in calibration mode, otherwise false
bool Calibration::readCalibSwitch() {
    // Write proper address to multiplexer
    digitalWrite(sigC, calibAddr[0]);
    digitalWrite(sigB, calibAddr[1]);
    digitalWrite(sigA, calibAddr[2]);

    // Read multiplexer output
    return (bool) digitalRead(sigOut);
}

// Reads multiplexer address connected to motor select switch
// Returns true (1) for bottom motor, if top, false (0)
bool Calibration::readMotorSwitch() {
    // Write proper address to multiplexer
    digitalWrite(sigC, motorAddr[0]);
    digitalWrite(sigB, motorAddr[1]);
    digitalWrite(sigA, motorAddr[2]);

    // Read multiplexer output
    return (bool) digitalRead(sigOut);
}

bool Calibration::readArduinoSwitch() {
    digitalWrite(sigC, arduinoAddr[0]);
    digitalWrite(sigB, arduinoAddr[1]);
    digitalWrite(sigA, arduinoAddr[2]);

    return (bool) digitalRead(sigOut);
}

// Reads multiplexer address connected to open command button
// Returns true if button pressed, otherwise false
bool Calibration::readOpenButton() {
    // Write proper address to multiplexer
    digitalWrite(sigC, openAddr[0]);
    digitalWrite(sigB, openAddr[1]);
    digitalWrite(sigA, openAddr[2]);

    // Read multiplexer output
    return (bool) digitalRead(sigOut);
}

// Reads multiplexer address connected to close command button
// Returns true if button pressed, otherwise false
bool Calibration::readCloseButton() {
    // Write proper address to multiplexer
    digitalWrite(sigC, closeAddr[0]);
    digitalWrite(sigB, closeAddr[1]);
    digitalWrite(sigA, closeAddr[2]);

    // Read multiplexer output
    return (bool) digitalRead(sigOut);
}

// Reads multiplexer address connected to set max command button
// Returns true if button pressed, otherwise false
bool Calibration::readSetMaxButton() {
    // Write proper address to multiplexer
    digitalWrite(sigC, maxAddr[0]);
    digitalWrite(sigB, maxAddr[1]);
    digitalWrite(sigA, maxAddr[2]);

    // Read multiplexer output
    return (bool) digitalRead(sigOut);
}

// Reads multiplexer address connected to set min command button
// Returns true if button pressed, otherwise false
bool Calibration::readSetMinButton() {
    // Write proper address to multiplexer
    digitalWrite(sigC, minAddr[0]);
    digitalWrite(sigB, minAddr[1]);
    digitalWrite(sigA, minAddr[2]);

    // Read multiplexer output
    return (bool) digitalRead(sigOut);
}

// Sets current position as max position of motor
void Calibration::setMax(int motor){
    // Write proper address to multiplexer
    digitalWrite(sigC, motorAddr[0]);
    digitalWrite(sigB, motorAddr[1]);
    digitalWrite(sigA, motorAddr[2]);

    // Read multiplexer output
    if (digitalRead(motor)){
    // Toggle switch is 1, set for bottom motor
        maxPos[1] = bottom.getPosition();
    } else {
    // Toggle switch is 0, set for top motor
        maxPos[0] = top.getPosition();
    }
}

// Sets current positoin as min positon of wings
void Calibration::setMin(int motor){
    // Write proper address to multiplexer
    digitalWrite(sigC, motorAddr[0]);
    digitalWrite(sigB, motorAddr[1]);
    digitalWrite(sigA, motorAddr[2]);

    // Read multiplexer output
    if (motor){
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