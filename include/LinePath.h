#define LINE_PATH_H

class LinePath {
    public: 
        LinePath();
        float getOpenPath(float currPos, boolean arduino_0);
        float getClosePath(float currPos, boolean arduino_0);
};