#ifndef ENCODER_H
#define ENCODER_H

struct Position {
    int positionAngular; // from 0 to 4095 for 12 bit encoder
    int numRevolutions;
    int time;
};

class Encoder{
    private:
        Position position[2]; 
        Position maxPos;
        Position minPos;
        int pin;

        void shift();
    
    public:
        Encoder(int input);
        Encoder();

        void update();
        void setMax();
        void setMin();

        float getVelocity();
        float getPosition();
};
#endif
