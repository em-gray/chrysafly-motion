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
        bool maxPin[];
        bool minPin[];
        bool openPin[];
        bool closePin[];

        // Pin numbers for encoders
        int top_encoder;
        int bot_encoder;

        // Encoder objects
        Encoder top, bottom;

        // Pin for top/bottom toggle switch
        int toggle_pin;
    
    public:
        Calibration();

        void setMax();
        void setMin();

        float getMax(int motor);
        float getMin(int motor);
};

#endif