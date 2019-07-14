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
        bool maxAddr[];
        bool minAddr[];
        bool openAddr[];
        bool closeAddr[];

        // Pin numbers for encoders and multiplexer
        int top_encoder;
        int bot_encoder;
        int muxOut;
        int muxS0;
        int muxS1;
        int muxS2;


        // Encoder objects
        Encoder top = Encoder::Encoder(top_encoder);
        Encoder bottom = Encoder::Encoder(bot_encoder);

        // Pin for top/bottom toggle switch
        int toggle_pin;
    
    public:
        Calibration();

        // Methods for reading buttons through multiplexer
        bool readOpenButton();
        bool readCloseButton();
        bool readSetMaxButton();
        bool readSetMinButton();

        // Methods for setting max and min positions
        void setMax();
        void setMin();

        // Methods for getting max and min positions
        float getMax(int motor);
        float getMin(int motor);
};

#endif