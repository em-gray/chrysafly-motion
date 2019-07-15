#ifndef SIGMOIDPATH_H
#define SIGMOIDPATH_H

struct Motor {
    float minPos;
    float maxPos;
    float span = maxPos - minPos;
};

class SigmoidPath{
    private:
        float cascadeOffset;
        float sigmoidLength;
        float midpointPause;
        float totalDuration;
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