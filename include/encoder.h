#ifndef ENCODER_H
#define ENCODER_H

static const int buffer = 10;
// should really be using a circular buffere here instead of an array 
// but we don't got time for that

struct Position {
    int positionAngular; // from 0 to 4095 for 12 bit encoder
    int numRevolutions;
    int time;
};

class Encoder{
    private:
        Position position[buffer]; 
        Position maxPos;
        Position minPos;
        int pin;

        void shift();
    
    public:
        Encoder(int input);

        void update();
        void setMax();
        void setMin();

        float getVelocity();
        float getPosition();

        float Max();
        float Min();
};

#endif