#define LINE_PATH_H

class LinePath {
    public: 
        LinePath();
        float getOpenPath(float currPos);
        float getClosePath(float currPos);
};