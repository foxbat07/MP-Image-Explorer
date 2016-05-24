//
//  imageDataClass.cpp
//  MPExplorer_004
//
//  Created by Mohit Hingorani on 10/2/15.
//
//

#include "imageDataClass.h"


//pathnames



const int imageThumbWidth = 100;
const int imageThumbHeight = 100;

const int gridSize = 13;

// currently 800

 void ImageDataClass::setExif( vector <string> exifInfo )
{
    this->imageNumber = ofToInt(exifInfo[0]);

    this->dAperture = ofToDouble(exifInfo[1]);
    this->dISOSpeed = ofToDouble(exifInfo[2]);
    this->dFocalLength = ofToDouble(exifInfo[3]);
    this->dShutterSpeed = ofToDouble(exifInfo[4]);
    
    for ( int i = 1 ; i<5 ; i++ )
    {
        this->exifData.push_back( ofToDouble (exifInfo[i]) );
    }
    
    //cout << this->imageNumber <<" "<< this->dAperture <<" "<<this->dFocalLength <<" "<<this->dISOSpeed<<" "<<this->dShutterSpeed<<" "<<endl  ;

    
}

void ImageDataClass::setTags( vector <string>  tags )
{
    this->tagwords = tags;  // assigning tags
    //cout << ofToString( this->tagwords) ;
    

}


void ImageDataClass::setThumbImage(string thumbPath)
{
    this->thumbPath = thumbPath;
    this->thumbImage.loadImage( this->thumbPath);
    
}

void ImageDataClass::setFullImage(string imagePath)
{
    this->imagePath = imagePath;
    //this->fullImage.loadImage( this->imagePath);
    
}




void ImageDataClass::loadFullImage()
{
    this->fullImage.loadImage( this->imagePath);
    
    
    
}

void ImageDataClass::clearFullImage()
{
    this->fullImage.clear();
    
}




void ImageDataClass::setBooleanFlags()
{
    
    this->isImageInRange = true ;
    this->isImageDrawn= true ;
    this->isImageSelected = false ;
    this->isImageHover= false;
    
    //cout <<"SET BOOLEAN: "<< this->imageNumber<<endl  ;
  
}



void ImageDataClass::setClusterNumber(int clusterNumber)
{
    this->clusterNumber = clusterNumber;
    
}




