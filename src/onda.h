#ifndef ONDA_H
#define ONDA_H

#include "ofMain.h"


class onda : public ofSoundPlayer
{
    public:
        onda(int topx, int topy, int width, int height, int npoints);
        onda();
        void update();
        void draw();

        int _topx, _topy, _width, _height;
        int _npoints;
        bool _debug;
        float *buffer;
        ofStyle *style;
};

class ondaStyle : public ofStyle
{
    public:
        ondaStyle(){
            strokeWidth = 1.0;
        }

        float strokeWidth;
        ofColor strokeColor;
};

#endif // ONDA_H
