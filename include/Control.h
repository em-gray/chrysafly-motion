#define CONTROL_H
// #include <Encoder.h>

class Control {
    // private:
    //     Encoder encoders[2];

    public:
        Control();
        void run(float currPos, float nextPos, int motor);
};