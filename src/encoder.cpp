#include <Arduino.h>

class Encoder{
    private:
        int lapCount;
        int maxPos;
        int lastPos;
        int position;
        int pin;
    
    public:
        Encoder(int input)
        {
        pin = input;
        lapCount = 0;
        position = analogRead(pin);
        lastPos = 0;
        }
    
    void update(){
        int pwmValue = pulseIn(pin, HIGH);

        if ((0.75*(maxPos) <= lastPos < maxPos) && (0 <= pwmValue <= 0.25*(maxPos)))
        {
            lapCount++;
        } 
        else if ((0.75*(maxPos) <= pwmValue < maxPos) && (0 <= lastPos <= 0.25*(maxPos)))
        {
            lapCount--;
        }
        lastPos = pwmValue;
    }

};

