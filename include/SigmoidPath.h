#ifndef SIGMOIDPATH_H
#define SIGMOIDPATH_H

struct Motor {
    float minPos;
    float maxPos;
    float span = maxPos - minPos;
};

class SigmoidPath{
    private:
        float cascadeOffset = 7.0;
        float sigmoidLength = 55.0;
        float midpointPause = 5.0;
        float totalDuration = 157.0;
        Motor m0;
        Motor m1;
        Motor m2;
        Motor m3;

    public:
        SigmoidPath();
        void Init(float minPos[], float maxPos[]);
        // returns desired position givne time
        float getNextPos(int motor, float time);
        float getPeriod();
};

#endif