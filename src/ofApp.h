#pragma once

#include "ofMain.h"

#include "metaballMap.h"
#include "record.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
		
    
    float valueMap(float input, float inputMin, float inputMax, float outputMin, float outputMax);
    metaballMap map;
    vector<ofVec2f> inputPts;
    vector<ofRectangle> outputRects;

    ofImage hkMap;
    
    
    ofxJSONElement users;
    void loadUsers();
    void updateUser(const record &r);
    
};
