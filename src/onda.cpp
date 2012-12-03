#include "onda.h"

onda::onda(int topx=0, int topy=0, int width=512, int height=256, int npoints=128)
{
    _topx    = topx;
    _topy    = topy;
    _width   = width;
    _height  = height;
    _npoints = npoints;
    style = NULL;
    _debug = false;

    if(_npoints > 8192)
        _npoints = 8192;

    buffer = new float[_npoints];
}

onda::onda()
{
    _topx    = 0;
    _topy    = 0;
    _width   = 512;
    _height  = 256;
    _npoints = 128;
    _debug = false;
    style = NULL;
    buffer = new float[_npoints];
}

void onda::volume_cb(float &vol)
{
        ofFmodSoundPlayer::setVolume(ofClamp(vol, 0, 1.0f));
}

void onda::update()
{
    if (getIsPlaying()){
        FMOD_Channel_GetWaveData(channel, buffer, _npoints, 0);
    }
}


void onda::draw()
{
    int y = _topy;
    int x = _topx;
    int h = _height;

    float dx = (float) _width / (_npoints-1);

    if(_debug){
        ofLine(x,y,x+_width,y+_height);
        ofLine(x,y+_height,x+_width,y);
    }

    if (style){
        ofPushStyle();
        ofSetStyle(*style);
    }

    for (int i = 0;i < _npoints - 1; i++){
        ofLine(x + i*dx, y + h/2 - buffer[i]*h, x + (i+1)*dx, y + h/2 - buffer[i+1]*h);
    }

    if (style){
        ofPopStyle();
    }
}

