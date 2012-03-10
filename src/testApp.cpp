#include "testApp.h"
#include "ofxXmlSettings.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0,0,10);

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

    settings.pushTag("defaults");
    lw = settings.getValue("linewidth", 10);
    lr = settings.getValue("line_red",250);
    lg = settings.getValue("line_green",250);
    lb = settings.getValue("line_blue",29);
    vol =  settings.getValue("volume", 0.2);
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
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

