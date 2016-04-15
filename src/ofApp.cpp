#include "ofApp.h"

using namespace cv;
using namespace std;
// static void help()
// {
//     cout << "\nThis program demonstrates kmeans clustering.\n"
//             "It generates an image with random points, then assigns a random number of cluster\n"
//             "centers and uses kmeans to move those cluster centers to their representitive location\n"
//             "Call\n"
//             "./kmeans\n" << endl;
// }
int kmeanFunc(  )
{
    const int MAX_CLUSTERS = 5;
    Scalar colorTab[] =
    {
        Scalar(0, 0, 255),
        Scalar(0,255,0),
        Scalar(255,100,100),
        Scalar(255,0,255),
        Scalar(0,255,255)
    };
    Mat img(500, 500, CV_8UC3);
    RNG rng(12345);
//    for(;;)
    {
        int k, clusterCount = rng.uniform(2, MAX_CLUSTERS+1);
        int i, sampleCount = rng.uniform(1, 1001);
        Mat points(sampleCount, 1, CV_32FC2), labels;
        clusterCount = MIN(clusterCount, sampleCount);
        Mat centers;
        /* generate random sample from multigaussian distribution */
        for( k = 0; k < clusterCount; k++ )
        {
            ofPoint center;
            center.x = rng.uniform(0, img.cols);
            center.y = rng.uniform(0, img.rows);
            Mat pointChunk = points.rowRange(k*sampleCount/clusterCount,
                                             k == clusterCount - 1 ? sampleCount :
                                             (k+1)*sampleCount/clusterCount);
            rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
        }
        randShuffle(points, 1, &rng);
        kmeans(points, clusterCount, labels,
               TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
               3, KMEANS_PP_CENTERS, centers);
        img = Scalar::all(0);
        for( i = 0; i < sampleCount; i++ )
        {
            int clusterIdx = labels.at<int>(i);
            ofPoint ipt = points.at<ofPoint>(i);
            cout << ipt.x << " | " << ipt.y << " | " << clusterIdx << endl;
            
        }

        
    }
    return 0;
}

//--------------------------------------------------------------
void ofApp::setup(){
    kmeanFunc();
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

void ofApp::removeTester(const record &r)
{
    string url = "http://mb09.com/ARCHIPELAUDIO/api/remove?uid=";
    url += r.uid;
    cout<<"deleteing...\n"<<url<<endl;
    users.open(url);
    
    map._chosen = -1;
    loadUsers();
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
    if(key == OF_KEY_BACKSPACE)//remove user (only works on testers)
    {
        if(map._chosen>-1 && map.records[map._chosen].tester == 1)
        {
            removeTester(map.records[map._chosen]);
        }
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
    map.drag(ofGetMouseY(),ofGetMouseX(),0,0,hkMap.getWidth(),hkMap.getHeight());
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
