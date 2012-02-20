#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
//    string audios[] = { "/mnt/sda5/pardo/sonidos_anahi/2004_1-2.aif.ogg", 
//                        "/mnt/sda5/pardo/sonidos_anahi/2007final_1-2.aif.ogg", 
//                        "/mnt/sda5/pardo/sonidos_anahi/2009-final.aif.ogg"
//                    };
    
    ofBackground(0,0,10);

    vector<string> audios;
    audios.push_back("/home/pardo/musica/Sara Vaughan - Peter Gunn.ogg");
    audios.push_back("/home/pardo/musica/Fever (Peggy Lee).ogg");
    audios.push_back("/home/pardo/musica/Amadou & Mariam - Dimanche à Bamako - 10 - La Paix.ogg");

    int cnt = audios.size();

    int w = ofGetWidth() * .9;
    int h = ofGetHeight() * .9;
    int dy = h / cnt;

    int topx = (ofGetScreenWidth() - w)/2;
    int topy = (ofGetScreenHeight() - dy*cnt)/2;

    printf("getwidth: %i , w: %i , topx: %i\n",ofGetScreenWidth(), w, topx);
    
    ondaStyle *estilo = new ondaStyle();

    estilo->lineWidth = 3;
    estilo->color.r = 127;
    estilo->color.g = 127;

    for (int idx=0; idx < cnt ; idx++) {
        onda *player = new onda(topx, topy + dy*idx, w, dy*.9, 128);
        player->loadSound(audios[idx]);
        reproductores.push_back(player);
        player->play();
        player->setLoop(true);
        player->setMultiPlay(false);
        player->style = estilo;
        player->_debug = true;
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

