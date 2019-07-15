#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <Encoder.h>
#include <Arduino.h>


class Calibration{
    private: 
        // Max and min positions of wing in number of rotations
        // Top motor is 0, bottom is 1
        float maxPos[2];
        float minPos[2];

        // 3-bit addresses for command buttons in multiplexer
        bool calibAddr[3] = {0, 0, 0};
        bool arduinoAddr[3] = {0, 0, 1};
        bool motorAddr[3] = {0, 1, 0};
        bool openAddr[3] = {0, 1, 1};
        bool closeAddr[3] = {1, 0, 0};
        bool maxAddr[3] = {1, 0, 1};
        bool minAddr[3] = {1, 1, 0};
 

        // Pin numbers for encoders and multiplexer
        int top_encoder;
        int bot_encoder;
        int sigOut;
        int sigC;
        int sigB;
        int sigA;

        // Encoder objects
        Encoder top;
        Encoder bottom;
    
        // Pin for top/bottom toggle switch
        int toggle_pin;
    
    public:
        Calibration();
        void Init();

        // Methods for reading buttons through multiplexer
        bool readCalibSwitch();
        bool readMotorSwitch();
        bool readArduinoSwitch();
        bool readOpenButton();
        bool readCloseButton();
        bool readSetMaxButton();
        bool readSetMinButton();

        // Methods for setting max and min positions
        void setMax(int motor);
        void setMin(int motor);

        // Methods for getting max and min positions
        float getMax(int motor);
        float getMin(int motor);
};

#endif