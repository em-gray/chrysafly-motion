#ifndef SIGMOIDPATH_H
#define SIGMOIDPATH_H

struct Motor {
    float minPos;
    float maxPos;
};

class SigmoidPath{
    private:
        int cascadeOffset;  // ms
        int sigmoidLength;  // ms
        int midpointPause;  // ms
        int totalDuration;
        Motor m0;
        Motor m1;
        
        float clamp(float x, float startPoint, int endPoint);

    public:
        SigmoidPath(float minPos[], float maxPos[]);
        // returns desired position givne time
        float getNextPos(int motor, int time, bool isClosing);
};

#endif