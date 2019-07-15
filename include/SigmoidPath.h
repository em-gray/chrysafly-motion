#ifndef SIGMOIDPATH_H
#define SIGMOIDPATH_H

struct Motor {
    float minPos;
    float maxPos;
    float span = maxPos - minPos;
};

class SigmoidPath{
    private:
        int totalDuration;
        int cascadeOffset;
        int sigmoidLength;
        int midpointPause;
    
        Motor m0;
        Motor m1;
        Motor m2;
        Motor m3;

        float clamp(float x, float startPoint, int endPoint);

    public:
        SigmoidPath();
        void Init(float minPos[], float maxPos[]);
        // returns desired position givne time
        float getNextPos(int motor, int time);
        int getPeriod();
        
};

#endif