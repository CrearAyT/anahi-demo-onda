#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofArduino.h"
#include "onda.h"
#include "adsr.h"

#include <queue>


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

        void setupArduino(const int & version);
        bool        bSetupArduino;          // flag variable for setting up arduino once

        int lt;
        vector<onda *> reproductores;
        vector<adsr *> adsrs;
        vector<adsr *> presion;

        ofArduino arduino;

};

#endif

