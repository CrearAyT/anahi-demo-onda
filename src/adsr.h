#ifndef ADSR_H
#define ADSR_H

#include "ofMain.h"

class diff {
    public:
        int _x; 
        int operator()(int x);

        diff();
        diff(int x);
};

class lpfilt {
    public:
        float _alfa;
        float _x;
        float _y;

        float operator()(float x);

        lpfilt();
        lpfilt(float alfa, float x0, float y0);
};

enum _STATE  {QUIET , ATTACK , SUSTAIN , RELEASE};

struct adsr_internal_state {
        bool triggered;
        int sample;
        int dx;
        float filtered_sample;
};

class adsr {
    public:
        ofEvent<int> on_release;

        //on_trigger(int &sample)
        ofEvent<int> on_trigger;

        //while_triggered(float &filtered_sample)
        ofEvent<float> while_triggered;

        //internal_state_changed(adsr_internal_state &state)
        ofEvent<adsr_internal_state> internal_state_changed;

        _STATE state;
        int umbral;
        bool invert;
        bool presion;
        bool luz_hold;
        int _slope_sign;

        adsr_internal_state info;

        bool triggered;

        int samplec;
        int attackl;
        int sustl;
        int rell;
        float alfa_rel;
        float alfa_sus;
        float alfa_att;

        float max_trig;
        float max_quiet;

        adsr();
        void update();
        void draw();
        float add_sample(int sample);

    private:
        diff dx;
        lpfilt lp1;

        float _quiet(int sample);        
        float _attack(int sample);        
        float _sustain(int sample);        
        float _release(int sample);        
        void update_internal_state(float val, bool triggered, int sample, int d);
};


#endif // ADSR_H
