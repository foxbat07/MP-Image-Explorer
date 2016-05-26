//
//  imageDataClass.h
//  MPExplorer_004
//
//  Created by Mohit Hingorani on 10/2/15.
//`
//

#ifndef __MPExplorer_004__imageDataClass__
#define __MPExplorer_004__imageDataClass__

#include "ofxExif.h" //once is start using EXIF

#endif /* defined(__MPExplorer_004__imageDataClass__) */




class ImageDataClass
{
    
public:
    
    ofImage fullImage;
    ofImage thumbImage;
    
    string imagePath;
    string thumbPath;
    
    double imageNumber;
    double localNumber;
    
    double dFocalLength;    // in mm
    double dISOSpeed;       //in 100s
    double dShutterSpeed;   //in seconds
    double dAperture;       //F-Stop
    
    vector<double> exifData;
    
    int clusterNumber;
    
    int imageWidth;
    int imageHeight;
    
    Point imagePosition;
    ofVec3f imageInSpacePosition;
    
    vector <string> tagwords;
    string tagString;
    
    bool isImageInRange;
    bool isImageDrawn;
    bool isImageSelected;
    bool isImageHover;
    
    void setThumbImage(string thumbPath);
    void setFullImage(string imagePath);
    void loadFullImage();
    void clearFullImage();
    void loadThumbImage();

    
    void initialize( int i );
    void setExif( vector <string> exifInfo );
    void setTags( vector <string>  tags );
    void setClusterNumber( int clusterNumber);
    void setPosition();
    void setPositionInSpace();
    void setBooleanFlags();
    
    
    
    //void update();
    
    
    

    
    
};
