#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofVec2f.h"
#include "onda.h"

#include <queue>

enum State { CLICK, RELEASE, RAY };

struct PlayItem {
    ofSoundPlayer *player;
    float start;
    float end;
};

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

        State state;
        ofVec2f start, end;
        float tstart;
        float raylen;
        float raymin;
        ofStyle estilo_rayo;

        vector<onda *> reproductores;
        vector<ofSoundPlayer *> reproductores_rayo;

        queue<PlayItem *> playlist;

    private:

        void buildPlaylist();
        void executePlaylist();
};

#endif

