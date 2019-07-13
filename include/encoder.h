#ifndef ENCODER_H
#define ENCODER_H

class Encoder{
    private:
        int lapCount;
        int maxPos;
        int lastPos;
        int position;
        int pin;
    
    public:
        Encoder(int input);
    
        void update();

};

#endif