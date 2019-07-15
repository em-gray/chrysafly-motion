#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <Encoder.h>

class Calibration{
    private: 
        // Max and min positions of wing in number of rotations
        // Top motor is 0, bottom is 1
        float maxPos[];
        float minPos[];

        // 3-bit addresses for multiplexer
        bool calibAddr[];
        bool arduinoAddr[];
        bool motorAddr[];
        bool openAddr[];
        bool closeAddr[];
        bool maxAddr[];
        bool minAddr[];
 

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