#include "testApp.h"
#include "ofxXmlSettings.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0,0,10);
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    ofxXmlSettings settings;
    settings.loadFile("configuracion.xml");

    ofAddListener(arduino.EInitialized, this, &testApp::setupArduino);
    bSetupArduino   = false;
    lt = ofGetElapsedTimeMillis();

    //valores predeterminados
    string port = "/dev/ttyUSB0";
    float max_quiet = 0.2;
    float max_trig = 1.0;
    float alfa_luz = 0.8;
    float alfa_pres = 0.95;
    int inv_luz = 0;
    int umb_luz = 600;
    int sust_luz = 600;
    int sust_pres = 600;
    int rel_luz = 100;
    int rel_pres = 100;


    settings.pushTag("defaults");
        port = settings.getValue("port", "/dev/ttyUSB0");
        bola_on = max_trig =  settings.getValue("volumen_activo", 1.0);
        bola_off = max_quiet =  settings.getValue("volumen_normal", 0.2);
        settings.pushTag("luz");
            inv_luz =  settings.getValue("invertir", 0);
            umb_luz =  settings.getValue("umbral", 100);
            sust_luz =  settings.getValue("duracion", 600);
            umb_luz =  settings.getValue("umbral", 100);
            rel_luz =  settings.getValue("duracion_fin", 100);
            alfa_luz =  settings.getValue("alfa", 0.8);
        settings.popTag();
        settings.pushTag("presion");
            //inv_luz =  settings.getValue("invertir", 0);
            sust_pres =  settings.getValue("duracion", 600);
            alfa_pres =  settings.getValue("alfa", 0.95);
            rel_pres =  settings.getValue("duracion_fin", 100);
        settings.popTag();
    settings.popTag();

    arduino.connect(port, 57600);

    int cnt;

    cnt = settings.getNumTags("luz");
    ofLog() << "adsr cnt " << cnt;
    for(int idx=0; idx < cnt ; idx++) {
        settings.pushTag("luz", idx);
            float quiet = 0.2;
            float trig = 1.0;
            float alfa = 0.8;
            int inv = 0;
            int umb = 600;
            int sust = 600;
            int rel = 100;
            inv =  settings.getValue("invertir",inv_luz );
            umb =  settings.getValue("umbral", umb_luz);
            trig =  settings.getValue("umbral", max_trig);
            quiet =  settings.getValue("umbral", max_quiet);
            sust =  settings.getValue("duracion", sust_luz);
            rel =  settings.getValue("duracion_fin", rel_luz);
            alfa = settings.getValue("alfa", alfa_luz);

        adsr *a = new adsr();

        a->luz_hold = true;
        a->sustl = sust;
        a->rell = rel;
        a->alfa_sus = alfa;
        a->umbral = umb;
        a->max_trig = trig;
        a->max_quiet = quiet;

        a->_slope_sign = 1;

        if (inv) {
            //a->umbral = -1*umb;
            a->invert = true;
        } else {

        }
        adsrs.push_back(a);
        settings.popTag();
    }

    cnt = settings.getNumTags("presion");
    ofLog() << "adsr presion cnt " << cnt;
    for(int idx=0; idx < cnt ; idx++) {
        settings.pushTag("presion", idx);
            int sust = 600;
            int rel = 100;
            float alfa = 0.95;
            sust =  settings.getValue("duracion", sust_pres);
            rel =  settings.getValue("duracion_fin", rel_pres);
            alfa = settings.getValue("alfa", alfa_pres);

        adsr *a = new adsr();

        a->sustl = sust;
        a->rell = rel;
        a->presion = true;
        a->alfa_sus = alfa;
        a->max_trig = max_trig;
        a->max_quiet = max_quiet;

        presion.push_back(a);
        settings.popTag();
    }

    cnt = settings.getNumTags("player");
    for(int idx=0; idx < cnt ; idx++) {
        bool has_sens = false;
        settings.pushTag("player", idx);
        string fn = settings.getValue("file", "");
        if (!fn.empty()) {
            printf("file: %s\n", fn.c_str());

            onda *player = new onda();

            if (!adsrs.empty()){
                if (settings.tagExists("luz")) {
                    ofLog() << "adsr connect play idx " << idx;
                    adsr *a = adsrs[ settings.getValue("luz", 0)%adsrs.size() ];
                    ofAddListener(a->while_triggered, player, &onda::volume_cb); 
                    ofAddListener(a->on_trigger, this, &testApp::bola_off_cb);
                    ofAddListener(a->on_release, this, &testApp::bola_on_cb);
                    has_sens = true;
                }
            }

            if (!presion.empty()){
                if (settings.tagExists("presion")) {
                    ofLog() << "adsr connect play idx " << idx;
                    adsr *a = presion[ settings.getValue("presion", 0)%adsrs.size() ];
                    ofAddListener(a->while_triggered, player, &onda::volume_cb); 
                    ofAddListener(a->on_trigger, this, &testApp::bola_off_cb);
                    ofAddListener(a->on_release, this, &testApp::bola_on_cb);
                    has_sens = true;
                }
            }

            player->loadSound(fn);
            player->setVolume(settings.getValue("volumen", max_quiet));
            player->setMultiPlay(false);

            player->play();
            player->setLoop(true);

            if (has_sens) {
                reproductores.push_back(player);
            } else {
                reproductores_bg.push_back(player);
            }
        }
        settings.popTag();
    }

}

void testApp::bola_off_cb(int &vol){
    for (vector<onda *>::iterator it=reproductores.begin() ; it != reproductores.end() ; it++) {
        (*it)->setVolume(bola_off);
    }

}
void testApp::bola_on_cb(int &vol){
    for (vector<onda *>::iterator it=reproductores.begin() ; it != reproductores.end() ; it++) {
        (*it)->setVolume(bola_on);
    }

}

//--------------------------------------------------------------
void testApp::setupArduino(const int & version){

    ofRemoveListener(arduino.EInitialized, this, &testApp::setupArduino);

    bSetupArduino   = true;

    for(int idx=0; idx < 6; idx++) {
        arduino.sendAnalogPinReporting(idx, ARD_ANALOG);
    }

    for(int idx=2; idx < 8; idx++) {
        arduino.sendDigitalPinMode(idx, ARD_INPUT);
    }

}


//--------------------------------------------------------------
void testApp::update(){
    if (arduino.isArduinoReady()){
        if( !bSetupArduino ){
            setupArduino(+0);
            return;
        } else {
            arduino.update();
        }
    } else {
        return;
    }

    int t = ofGetElapsedTimeMillis();
    if (t - lt >= 100){
        lt = t;    
        for(int idx=0; idx < adsrs.size(); idx++) {
            int val = arduino.getAnalog(idx%6);
            if (val == -1){ continue; }
            val = ofClamp(2*val, 0, 1024);
            adsrs[idx]->add_sample(val);
            //ofLog() << "canal " << idx << " val " << arduino.getAnalog(idx);
        }
        for(int idx=0; idx < presion.size(); idx++) {
            int val = arduino.getDigital((idx%6)+2);
            if (val == -1){ continue; }
            int smp = 512*(2-val);
            //ofLog() <<"pres idx = " << idx << "val = " << smp;
            presion[idx]->add_sample(smp);
            //ofLog() << "canal " << idx << " val " << arduino.getAnalog(idx);
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
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

