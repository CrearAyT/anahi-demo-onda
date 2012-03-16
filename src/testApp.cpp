#include "testApp.h"
#include "ofxXmlSettings.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0,0,10);
    ofEnableAlphaBlending();

    vector<string> audios;

    ofxXmlSettings settings;
    settings.loadFile("configuracion.xml");

    int cnt = settings.getNumTags("player");

    int w = ofGetWidth() * .9;
    int h = ofGetHeight() * .9;
    int dy = h / cnt;

    int topx = (ofGetScreenWidth() - w)/2;
    int topy = (ofGetScreenHeight() - dy*cnt)/2;

    printf("getwidth: %i , w: %i , topx: %i\n",ofGetScreenWidth(), w, topx);

    //valores predeterminados
    int lw= 10, lr = 250, lg = 250, lb = 29;
    float vol = 0.2;

    state = RELEASE;

    settings.pushTag("defaults");
    lw = settings.getValue("linewidth", 10);
    lr = settings.getValue("line_red",250);
    lg = settings.getValue("line_green",250);
    lb = settings.getValue("line_blue",29);
    vol =  settings.getValue("volume", 0.2);
    settings.popTag();

    settings.pushTag("rayo");
    estilo_rayo.lineWidth= settings.getValue("linewidth", 10);
    estilo_rayo.color.r = settings.getValue("rayo_red",50);
    estilo_rayo.color.g = settings.getValue("rayo_green",250);
    estilo_rayo.color.b = settings.getValue("rayo_blue",89);
    raylen = settings.getValue("duracion", 2.0);
    raymin = settings.getValue("largo_minimo", 100);
    settings.popTag();

    for(int idx=0; idx < cnt ; idx++) {
        settings.pushTag("player", idx);
        string fn = settings.getValue("file", "");
        if (fn.size()) {
            audios.push_back(fn);
            printf("file: %s\n", fn.c_str());

            ondaStyle *estilo = new ondaStyle();

            estilo->lineWidth = settings.getValue("linewidth",lw);
            estilo->color.r = settings.getValue("line_red",lr);
            estilo->color.g = settings.getValue("line_green",lg);
            estilo->color.b = settings.getValue("line_blue",lb);

            onda *player = new onda(topx, topy + dy*idx, w, dy*.9, 128);

            player->loadSound(fn);
            reproductores.push_back(player);
            player->setVolume(settings.getValue("volume", vol));
            player->play();
            player->setLoop(true);
            player->setMultiPlay(false);
            player->style = estilo;
            player->_debug = false;

            ofSoundPlayer *rayplay = new ofSoundPlayer();
            rayplay->loadSound(fn);
            rayplay->setLoop(false);
            rayplay->setMultiPlay(false);
            rayplay->setVolume(vol);
            reproductores_rayo.push_back(rayplay);
        }
        settings.popTag();
    }

}


//--------------------------------------------------------------
void testApp::update(){
    for (vector<onda *>::iterator it=reproductores.begin() ; it != reproductores.end() ; it++) {
        (*it)->update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    for (vector<onda *>::iterator it=reproductores.begin() ; it != reproductores.end() ; it++) {
        (*it)->draw();
    }
    switch (state){
        case RAY:
            if ((ofGetElapsedTimef() - tstart) > 2 ){
                state = RELEASE;
            }
        case CLICK:
            ofPushStyle();
            ofSetStyle(estilo_rayo);
            ofLine(start.x, start.y, end.x, end.y);
            ofPopStyle();
            break;
        case RELEASE:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    for (vector<onda *>::iterator it=reproductores.begin() ; it != reproductores.end() ; it++) {
        (*it)->play();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    end.set(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    state = CLICK;
    start.set(x,y);
    end.set(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    state = RELEASE;
    if ( (end - start).length() > raymin ){
        state = RAY;
        tstart = ofGetElapsedTimef();

        int w = ofGetScreenWidth();
        int h = ofGetScreenHeight();

        if ( end.x == start.x ){
            end.y = 0;
            start.y = h;
            return;
        }

        float m = (end.y - start.y) / (end.x - start.x);
        float x0 = start.x;
        float y0 = start.y;
        float b = y0 - m*x0;

        float x;

        x = -b / m;
        x = CLAMP(x,0,w);
        start.x = x;
        start.y = m*x + b;

        x = (h-b) / m;
        x = CLAMP(x,0,w);
        end.x = x;
        end.y = m*x + b;

    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

