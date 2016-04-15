//
//  metaballMap.h
//  metaball
//
//  Created by mb09 on 9/4/16.
//
//

#ifndef metaballMap_h
#define metaballMap_h

#include "ofxOpenCV.h"
#include "record.h"


const float hklat1 = 22.12;
const float hklong1 = 114.42;
const float hklat2 = 22.60;
const float hklong2 = 113.82;


class metaballMap{
public:
    
    float ballRadius;
    float blurRadius;
    float thresholdLevel;   //0-1 (black = 0, white = 1)
    
    int canvasResolution;
    
    ofFbo canvas;
    ofxCvContourFinder 	contourFinder;
    ofxCvGrayscaleImage cvCanvas;
    
    
    vector<ofRectangle> outputRects;
    vector<record> records;
    
    int _chosen;            //highlight chosen pt
    
    
    metaballMap()
    {
        init();
    }
    
    void init()
    {
        canvasResolution = 256;
        ballRadius = 5;
        blurRadius = 5;
        thresholdLevel = 0.5;
        
        _chosen = -1;
        
        canvas.allocate(canvasResolution,canvasResolution);
        cvCanvas.allocate(canvasResolution,canvasResolution);
        
        canvas.begin();
        ofClear(ofColor(0,0,0));  //clear fbo;
        canvas.end();
        
        updateCvCanvas();
    }
    
    void clear()
    {
        canvas.begin();
        ofClear(ofColor(0,0,0));  //clear fbo;
        canvas.end();

        outputRects.clear();
        records.clear();
    }
    
    void updateCvCanvas()
    {
        canvas.begin();
        ofClear(ofColor(0,0,0));  //clear fbo;
        ofSetColor(255);
        ofFill();
        for(int i=0;i<records.size();i++)
        {
            ofVec2f temp = ofVec2f(records[i].loclat,records[i].loclong);               //map to canvasSize
            temp.x = valueMap(records[i].loclong, hklong1,hklong2,0,canvasResolution);
            temp.y = valueMap(records[i].loclat, hklat1,hklat2,0,canvasResolution);
            
            ofCircle(temp,ballRadius);
        }
        canvas.end();
        ofPixels canvasPx;
        canvas.readToPixels(canvasPx);
        canvasPx.setNumChannels(1);
        cvCanvas.setFromPixels(canvasPx);
        cvCanvas.blur(blurRadius);
    }
    
    float valueMap(float input, float inputMin, float inputMax, float outputMin, float outputMax)
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

    
    void doTracking()
    {
        contourFinder.findContours(cvCanvas,ballRadius*ballRadius,canvasResolution*canvasResolution*0.2,100,false);
    }
    
    vector<ofRectangle> output()
    {
        outputRects.clear();
        for(int i=0;i<contourFinder.nBlobs;i++)
        {
            outputRects.push_back(contourFinder.blobs[i].boundingRect);
        }
        return outputRects;
    }
    
    vector<ofRectangle> refresh()
    {
        updateCvCanvas();
        doTracking();
        return output();
    }
    
    void draw(float _x = 0, float _y =0, float _w = -1, float _h = -1)
    {
        if(_w == -1) _w = canvasResolution;
        if(_h == -1) _h = canvasResolution;
        ofSetColor(255);
        canvas.draw(_x,_y,_w,_h);
        cvCanvas.draw(_x+_w,_y,_w,_h);
        contourFinder.draw(_x+_w,_y);
        drawBlobs(_x+_w,_y);
    }
    
    void drawBlobs(float _x = 0, float _y =0, float _w = -1, float _h = -1)
    {
        if(_w == -1) _w = canvasResolution;
        if(_h == -1) _h = canvasResolution;
        
        //draw records
        for(int i=0;i<records.size();i++)
        {
            ofVec2f temp;
            temp.x = valueMap(records[i].loclong, hklong1,hklong2,0,_w);
            temp.y = valueMap(records[i].loclat , hklat1,hklat2,0,_h);
            
            ofFill();
            if(i == _chosen) ofSetColor(255,0,0);
            else ofSetColor(0,255,0);
            ofCircle(_x + temp.x, _y + temp.y, 5);
            ofSetColor(0);
            ofDrawBitmapString(records[i].uid,_x + temp.x, _y + temp.y);
        }
        
        //draw groupped boxes
        for(int i=0;i<outputRects.size();i++)
        {
            ofNoFill();
            ofSetColor(255,0,0);
            
            float scaleX = _w / (float) canvasResolution;
            float scaleY = _h / (float) canvasResolution;
            
            ofRect(_x+outputRects[i].getLeft()*scaleX,_y+outputRects[i].getTop()*scaleY,outputRects[i].getWidth()*scaleX,outputRects[i].getHeight()*scaleY);
        }
        
    }
    
    void hitTest(float mouseX, float mouseY, float _x = 0, float _y =0, float _w = -1, float _h = -1)
    {
        _chosen = -1;
        for(int i=0;i<records.size();i++)
        {
            ofVec2f temp;
            temp.x = valueMap(records[i].loclong, hklong1,hklong2,0,_w);
            temp.y =valueMap(records[i].loclat , hklat1,hklat2,0,_h);
            
            if(ofDist(_x + temp.x, _y + temp.y,mouseX,mouseY)<5)
            {
                _chosen = i;
                break;
            }
        }
    }
    
    void drag(float mouseX, float mouseY, float _x = 0, float _y =0, float _w = -1, float _h = -1)
    {
        if(_chosen != -1)
        {
            ofVec2f temp;
            temp.x = valueMap(mouseY, _x,_w+_x, hklong1,hklong2);
            temp.y = valueMap(mouseX, _y,_h+_y, hklat1,hklat2);
            
            records[_chosen].loclat = temp.y;
            records[_chosen].loclong = temp.x;
        }
    }
    
};


#endif /* metaballMap_h */
