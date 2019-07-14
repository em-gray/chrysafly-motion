#define CONTROL_H
#include <Encoder.h>

class Control {
    private:
        int getMotorOutput(float diff);
        Encoder encoders[2];

    public:
        Control();

        void run(int time, float nextPos, int motor, boolean closing);
};