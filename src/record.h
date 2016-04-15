//
//  record.h
//  metaball
//
//  Created by mb09 on 13/4/16.
//
//

#ifndef record_h
#define record_h

#include "ofxJSON.h"

class record{
public:
    string uid;
    float loclat;
    float loclong;
    float heading;
    float altitude;
    int tester;
    
    record()
    {
    }
    
    void init(Json::Value value)
    {
        uid = value["uid"].asString();;
        sscanf(value["loclat"].asString().c_str(),"%f",&loclat);
        sscanf(value["loclong"].asString().c_str(),"%f",&loclong);
        sscanf(value["heading"].asString().c_str(),"%f",&heading);
        sscanf(value["altitude"].asString().c_str(),"%f",&altitude);
        sscanf(value["tester"].asString().c_str(),"%d",&tester);
    }
    
    string display()
    {
        string output = uid;
        output += "\nloclat  : "+ofToString(loclat);
        output += "\nloclong : "+ofToString(loclong);
        output += "\nheading : "+ofToString(heading);
        output += "\naltitude: "+ofToString(altitude);
        output += "\ntester  : "+ofToString(tester);
        return output;
    }
};


#endif /* record_h */
