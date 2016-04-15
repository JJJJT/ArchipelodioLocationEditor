#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    hkMap.loadImage("hk_area.png");
    hkMap.resize(800,800*hkMap.getHeight()/(float)hkMap.getWidth());
    loadUsers();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(128);
    ofSetColor(255);
    hkMap.draw(0,0);
    map.drawBlobs(0,0,hkMap.getWidth(),hkMap.getHeight());
    
    
    ofPushMatrix();
        ofTranslate(0,hkMap.getHeight());
        map.draw(0,0);
    ofPopMatrix();
}

void ofApp::loadUsers()
{
    string url = "http://www.mb09.com/ARCHIPELAUDIO/api/users";
    if (!users.open(url))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }
    map.clear();
    inputPts.clear();
    for(int i=0;i<users.size();i++)
    {
        record r;
        r.init(users[i]);
        cout<<"-----"<<endl<<r.display()<<endl;
        map.records.push_back(r);
    }
    map.refresh();
}

void ofApp::updateUser(const record &r)
{
    string url = "http://www.mb09.com/ARCHIPELAUDIO/api/update?uid=";
    url += r.uid;
    url += "&loclat=";
    url += ofToString(r.loclat);
    url += "&loclong=";
    url += ofToString(r.loclong);
    url += "&heading=";
    url += ofToString(r.heading);
    url += "&altitude=";
    url += ofToString(r.altitude);
    url += "&tester=";
    url += ofToString(r.tester);
    cout<<"updating...\n"<<url<<endl;
    users.open(url);
}


//mapping correctly even if outputMin > outputMax
float ofApp::valueMap(float input, float inputMin, float inputMax, float outputMin, float outputMax)
{
    float iMin = inputMin;
    float iMax = inputMax;
    if(iMin>iMax)
    {
        float temp = iMin;
        iMin = iMax;
        iMax = temp;
    }
    float ratio = (input - inputMin) / (float)(inputMax - inputMin);
    return (ratio * outputMin) + (1-ratio)*outputMax;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1')  //reload users
    {
        loadUsers();
    }
    if(key == ' ')  //add testing user
    {
        record r;
        r.uid = "Tester"+ofToString((int)ofRandom(10000));
        r.loclat = valueMap(ofGetMouseY(),0,hkMap.getHeight(),hklat1,hklat2);
        r.loclong = valueMap(ofGetMouseX(),0,hkMap.getWidth(),hklong1,hklong2);
        r.heading = ofRandom(0,1);
        r.altitude = ofRandom(0,100);
        r.tester = 1;

        map.records.push_back(r);
        map.refresh();
        updateUser(r);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    map.drag(ofGetMouseX(),ofGetMouseY(),0,0,hkMap.getWidth(),hkMap.getHeight());
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    map.hitTest(ofGetMouseX(),ofGetMouseY(),0,0,hkMap.getWidth(),hkMap.getHeight());
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    map.refresh();
    if(map._chosen>-1)
    {
        updateUser(map.records[map._chosen]);
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
