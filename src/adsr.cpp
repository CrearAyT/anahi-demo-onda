#include "adsr.h"


diff::diff()
{
    _x = 0;
}

diff::diff(int x)
{
    _x = x;
}

int diff::operator() (int x)
{
    int ret = x - _x;
    _x = x;
    return ret;
}

lpfilt::lpfilt()
{
    _alfa=0.8;
    _x=0;
    _y=0;
}

lpfilt::lpfilt(float alfa, float x0, float y0)
{
    _alfa = alfa;
    _x   = x0;
    _y   = y0;
}

float lpfilt::operator() (float x)
{
        float y = _alfa * _y + (1-_alfa)*x;
        _y = y;
        return y;
}

adsr::adsr()
{
    umbral = 100;
    _slope_sign = 1;

    triggered = False;
    samplec = 0;
    attackl = 15;
    sustl = 100;
    rell = 15;
    alfa_rel = 0.85;
    alfa_sus = 0.85;
    alfa_att = 0.3;
    invert = false;
    presion = false;
    luz_hold = false;

    info.triggered = false;
    info.sample = 0;
    info.filtered_sample = 0;
    info.dx = 0;

}

float  adsr::_quiet(int sample)
{
    int d = dx(sample);
    bool ok = (abs(d)>=umbral);
    if (ok and (d*_slope_sign)>0){
        triggered = true;
        samplec = attackl;
        state = ATTACK;
        ofNotifyEvent(on_trigger, sample, this);
        ofLog() << " s. ATT pres= " << presion;
        return _attack(sample);
    }
    update_internal_state(0, triggered, sample, d);
    return 0;
}

float  adsr::_attack(int sample)
{
    if (samplec){
        lp1._alfa = alfa_att;
        int d = dx(sample);
        samplec-- ;
        float val = lp1(sample);
        val = val*max_trig / 1024.0f;
        ofNotifyEvent(while_triggered, val, this);
        update_internal_state(val, triggered, sample, d);
        return val;
    } else {
        state = SUSTAIN;
        samplec = sustl;
        return _sustain(sample);
    }
}

float  adsr::_sustain(int sample)
{
   if (samplec){
        if (presion && sample > 768){
            if (samplec < 5 ){
                samplec = 5;
            }
        }
        int d = dx(sample);
        sample = 2.5*sample;
        lp1._alfa = alfa_sus;
        if (luz_hold){
            bool ok = !(abs(d)>=umbral);
            int dslp = -1*d*_slope_sign;
            if ((dslp >0) and ok){
                samplec+=1;
            }
            if (!ok){
                samplec = 1;
            }

        }
        samplec-- ;
        float val = lp1(sample);
        val = val*max_trig / 1024.0f;
        ofNotifyEvent(while_triggered, val, this);
        update_internal_state(val, triggered, sample, d);
        return val;
    } else {
        state = RELEASE;
        ofLog() << " s. REL pres= " << presion;
        samplec = rell;
        return _release(sample);
    }
}

float  adsr::_release(int sample)
{
    if (samplec){
        lp1._alfa = alfa_rel;
        int d = dx(sample);
        samplec = samplec - 1;
        sample = 1.5 * sample;
        float val = lp1(sample*(1.*samplec/rell));
        val = val*max_quiet / 1024.0f;
        update_internal_state(val, triggered, sample, d);
        ofNotifyEvent(while_triggered, val, this);
        return val;
    } else {
        state = QUIET;
        triggered = false;
        lp1._y = 0;
        ofNotifyEvent(while_triggered, max_quiet, this);
        ofNotifyEvent(on_release, sample, this);
        update_internal_state(0, triggered, sample, 0);
        ofLog() << " s. QUI pres= " << presion;
        return 0;
    }
}

void adsr::update()
{
    info.triggered = triggered;
    info.sample = dx._x;
    info.filtered_sample = lp1._y;
}

void adsr::update_internal_state(float val, bool triggered, int sample, int d)
{
    info.filtered_sample = val;
    info.triggered = triggered;
    info.sample = sample;
    info.dx = d;
    ofNotifyEvent(internal_state_changed, info, this);
}

float adsr::add_sample(int sample)
{
    int _sample = sample;
    if (invert) {
        _sample = 1023 - sample;
    }

    switch (state) {
        case ATTACK:
            return _attack(_sample);
            break;
        case SUSTAIN:
            return _sustain(_sample);
            break;
        case RELEASE:
            return _release(_sample);
            break;
        case QUIET:
        default:
            return _quiet(_sample);
            break;
    }
}
