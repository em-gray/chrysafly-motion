#ifndef SIGMOIDPATH_H
#define SIGMOIDPATH_H

class SigmoidPath{
    private:
        int cascadeOffset = 5000;   // ms
        int sigmoidLength = 60000;  // ms
        int midpointPause = 1000;   // ms
        int totalDuration = sigmoidLength * 2 + 6 * cascadeOffset + midpointPause;

    public:
        SigmoidPath(float minPos[], float maxPos[]);
        int getPath();
};

#endif