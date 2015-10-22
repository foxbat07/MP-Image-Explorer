#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    
    red = 0;
    green = 255;
    blue = 128;
    //ofSetVerticalSync( true );
    ofSetFrameRate(60);
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //ofEnableSmoothing();
    ofSetWindowTitle("User Interface");
    secondWindow.setup("Image Visualizer", 50, 50, 1440, 900, false);
    
    //setting up labels;
    ExifLabels.push_back("Apertue");
    ExifLabels.push_back("ISO");
    ExifLabels.push_back("Focal Len");
    ExifLabels.push_back("Exposure");
    //////////////////
    
    globalMinExifData.assign(5,1000);
    globalMaxExifData.assign(5,0);
    
    fbo.allocate(gridSize * imageThumbWidth, gridSize* imageThumbHeight, GL_RGBA );
    fbo2.allocate(somGridSize * imageThumbWidth, somGridSize* imageThumbHeight, GL_RGBA );

    
    //open up two files for meta and tags
    
    string metaFilepath = ofToString(metadataFolder) + "/" + ofToString(metadataName) + ofToString(numberOfImages) + ofToString(metadataExtention);
    
    string tagPath = ofToString(metadataFolder) + "/" + ofToString(tagName) + ofToString(numberOfImages) + ofToString(metadataExtention);  //just the tag name is different
    
    
    //metadata
    string metaFilepath10000 = ofToString(metadataFolder) + "/" + ofToString(metadataName) + ofToString("10000") + ofToString(metadataExtention);       //for SOM

    ofFile metadataFile10000;
    metadataFile10000.open ( metaFilepath10000 ,ofFile::ReadWrite, false ); //for SOM
    cout<<metaFilepath<<endl;
    cout<<metaFilepath10000<<endl;
    
    ////
    
    
    ofFile tagFile;
    tagFile.open(tagPath, ofFile::ReadWrite, false);
    
    ofFile metadataFile;
    metadataFile.open ( metaFilepath ,ofFile::ReadWrite, false );
    
    //got two files yo
    
    
    
    if ( tagFile.isFile() && metadataFile.isFile() )
    {
        ofBuffer tagBuffer = tagFile.readToBuffer();
        //cout << tagBuffer.getText() << endl;
        
        ofBuffer metaBuffer = metadataFile.readToBuffer();
        //cout << metaBuffer.getText() << endl;
        
        
        for ( int i = 0 ; i< noImages ; i ++ )
        {
            
            string tagLine = tagBuffer.getNextLine();
            string metaLine = metaBuffer.getNextLine();
            
            string imagePath = ofToString(thumbnailFolder) + "/" + ofToString(thumbnailName) + ofToString(i+1) + ofToString(thumbnailExtention);  //just the tag name is different
            
            
            
            vector <string> tagWords;
            tagWords = ofSplitString(tagLine, " ");
            tagWords.erase(tagWords.begin()); // delete the first number
            
            vector <string> metaExif;
            metaExif = ofSplitString(metaLine, " ");
            
            ImageDataClass tempDataClass;
            tempDataClass.setExif(metaExif);
            tempDataClass.setTags(tagWords);
            tempDataClass.setThumbImage(imagePath);
            tempDataClass.setBooleanFlags();
            
            imageVector.push_back(tempDataClass);

        }// end of data loading
        

    }//end of if file

 
    
    //lets load data for 10000 file now
    if ( metadataFile10000.isFile() )
    {
        ofBuffer metaBuffer100000 = metadataFile10000.readToBuffer();
        //cout << metaBuffer.getText() << endl;
        
        
        for ( int i = 0 ; i< 10000 ; i ++ )
        {
            
            string metaLine = metaBuffer100000.getNextLine();
            //cout<<metaLine << endl ;
            vector <string> metaExif;
            vector <double> dMetaExif;
            
            metaExif = ofSplitString(metaLine, " ");
            
            for ( int i = 1 ; i<metaExif.size() ; i++ )
            {
                dMetaExif.push_back( ofToDouble (metaExif[i]) ) ;
                cout<< metaExif[i] <<endl;
                
            }
            
            somExifData.push_back(dMetaExif);       //pushed back all data

        }// end of data loading
    }//end of meta file
    
    cout<< somExifData.size()<<endl ;
    

    
    //lets cluster the data
    
    for (int i = 0; i < NUMPOINTS; i++) {
        clusterer.addTrainingInstance( imageVector[i].exifData );
    }
    
    clusterer.setNumClusters(NUMCLUSTERS);
    for (int i = 0; i < NUMCLUSTERS; i++) {
        colors[i].setHsb( ( i* 255)/NUMCLUSTERS , 255, 255 );
    }
    
     findMinMaxVector(); // should have min max by now
    
    
    
    // ofxUI
    drawPadding = false;
    gui0 = new ofxUISuperCanvas("paramaters");
    gui0->setHeight(500);
    gui0->setWidth(300);
    gui0->setPosition(0 ,500);

    gui0->addSpacer();
    gui0->addFPSSlider("FPS SLIDER");
    gui0->addSpacer();
    gui0->addLabel("Cluster Slider");
    gui0->addRangeSlider("RANGE", 0.0, NUMCLUSTERS , &min , &max);
    gui0->addSpacer();
    
    
    gui1 = new ofxUISuperCanvas("Parallel Coordiantes");
    gui1->setTheme(OFX_UI_THEME_HINGOOO);
    
    //gui1->addLabel(" Parallel Coordinates");
    gui1->setPosition(0, 0);
    gui1->setHeight(500);
    gui1->setWidth(500);
    //gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
   
    gui1->addSpacer(0, 200);
    
    
    gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    //gui1->addVerticalRangeSlider( ExifLabels[0] , ofToFloat(ofToString(minExifData[0])) ,  ofToFloat(ofToString(maxExifData[0])) , &minExifRange[0]  , &maxExifRange[0] );

    green = 200;
    
    //gui0->addVerticalRangeSlider("x", 0, 255, &red, &green , 50 ,200);

    
    for ( int  i = 0 ; i < 4 ; i ++ )
    {
        //gui1->addLabel(ExifLabels[i]);
        gui1->addSpacer(40 , 400);
        gui1->addRangeSlider( ExifLabels[i] , ofToFloat(ofToString(minExifData[i])) ,  ofToFloat(ofToString(maxExifData[i])) , &minExifRange[i]  , &maxExifRange[i] , 60, 400 );
        
        //gui1->addVerticalRangeSlider( ExifLabels[i] , ofToFloat(ofToString(minExifData[i])) ,  ofToFloat(ofToString(maxExifData[i])) , minExifRange[i]  , maxExifRange[i], 60 ,400  );
    
    }
    
    //gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);  // for vertical sliders
    //gui0->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    //gui0->addSpacer();
    
    ofAddListener(gui0->newGUIEvent,this,&ofApp::guiEvent);
    ofAddListener(gui1->newGUIEvent,this,&ofApp::guiEvent);

    clusterer.beginTraining(this, &ofApp::callbackTrainingDone);
    
    
    // get SOm stuff ready
    
    findSOMMinMaxVectorGlobal();  // set up SOM stuff
    double minInstance[4] = { globalMinExifData[0] , globalMinExifData[1] , globalMinExifData[2] , globalMinExifData[3] };
    double maxInstance[4] = { globalMaxExifData[0] , globalMaxExifData[1] , globalMaxExifData[2] , globalMaxExifData[3]  };
    
    som.setFeaturesRange(4, minInstance, maxInstance);
    som.setInitialLearningRate(0.2);
    som.setNumIterations(1000);
    som.setMapSize(somGridSize, somGridSize );
    som.setup();
    
    
    // setup SOM ends here
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    //update SOM
    int frameNum = ofGetFrameNum();
    // double instance[4] = { somExifData[frameNum][0] ,  somExifData[frameNum][1] ,  somExifData[frameNum][2],  somExifData[frameNum][3] };
    
    if( frameNum < 1000 &&  somExifData[frameNum].size() == 4  )
    {
        
    double instance[4] = { somExifData[frameNum][0] ,  somExifData[frameNum][1] ,  somExifData[frameNum][2],  somExifData[frameNum][3] };

    
    
    //  dMinAperture, dMinShutterSpeed, dMinFocalLength, dMinAperture
    som.updateMap(instance);
    
    cout<<"som called: "<<frameNum << endl;
        
    // end of SOM
    
    }
    
    
    
    
    
    //update FBO
    
    fbo2.begin();
    ofClear(255,255,255);
    for (int i = 0; i < somGridSize; i++) {
        for (int j = 0; j < somGridSize; j++) {
            
            double * exifData = som.getMapAt(i,j);
            ofSetColor(ofColor::blue);
            ofDrawBitmapString( ofToString( exifData[0] ), i * imageThumbWidth , j * imageThumbHeight + textDiff );
            ofSetColor(ofColor::grey);
            ofDrawBitmapString(ofToString( exifData[1] )  , i * imageThumbWidth , j * imageThumbHeight + 2* textDiff );
            ofSetColor(ofColor::green);
            ofDrawBitmapString(ofToString( exifData[2] ) , i * imageThumbWidth , j * imageThumbHeight + 3* textDiff );
            ofSetColor(ofColor::red);
            ofDrawBitmapString(ofToString( exifData[3] ) , i * imageThumbWidth , j * imageThumbHeight + 4* textDiff );
            
            
            ofSetColor(ofColor::white );
            
            //imageThumbs[i * gridSize + j ].draw( i * imageThumbWidth , j * imageThumbHeight );
        }
        
    }
    fbo2.end();
    
    
    fbo.begin();
    ofClear(255,255,255);
    //ofTranslate(xMargin, yMargin);
    for (int i = 0 ; i < NUMPOINTS; i++)
    {
        //imageThumbs[i].draw( i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight );
        imageVector[i].thumbImage.draw(i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight );
        
    }
    fbo.end();

    
    
    

    

}

//--------------------------------------------------------------


void ofApp::draw(){
    
    //ofPushMatrix();
    
    
    ofBackground(0);
    
    fbo.draw(500,0);
    
    //ofBackgroundGradient(ofColor::black, ofColor::grey);
    //ofBackground(red, green, blue);
    
    if (!clusterer.isTrained()) {
        ofSetColor(0);
        ofDrawBitmapString("Please wait... Training clusterer in its own thread! ("+ofToString(ofGetFrameNum())+")", 50, 50);
        return;
    }
    
    clusters = clusterer.getClusters();
    
    
    if ( clusterer.isTrained() && isClusterAssigned == false )
    {
        isClusterAssigned = true;
        for ( int i = 0 ; i <NUMPOINTS ; i ++ )
        {
            imageVector[i].setClusterNumber(clusters[i] );
            cout<<i <<"  "<< clusters[i] <<endl ;
        }
        
        
    }
    
    
    // drawing stuff now   
    
    secondWindow.begin();
    ofBackground(0);
    cam.begin();
    ofRotateY(ofGetFrameNum());
    
    ofEnableDepthTest();
    for (int i = 0; i < testImagesDrawn ; i++)
    {
        if ( imageVector[i].clusterNumber >= min && imageVector[i].clusterNumber <=max )
        {
          if( imageVector[i].exifData[1] >= minExifRange[1] &&  imageVector[i].exifData[1] <= maxExifRange[1] && imageVector[i].exifData[0] >= minExifRange[0] &&  imageVector[i].exifData[0] <= maxExifRange[0]    )
            {
                if( imageVector[i].exifData[1] >= minExifRange[2] &&  imageVector[i].exifData[1] <= maxExifRange[2] && imageVector[i].exifData[3] >= minExifRange[3] &&  imageVector[i].exifData[3] <= maxExifRange[3] )    // add cluster as well
                    {
                        
                    ofPushMatrix();
                    ofEnableDepthTest();
                        
                    ofTranslate(imageVector[i].exifData[0] * 50 -900 , imageVector[i].exifData[2] * 10 - 1500 ,imageVector[i].exifData[3] * 400 );
                        
                    //ofTranslate(imageVector[i].exifData[0] * 50 -900 , imageVector[i].exifData[1] * 3 - 1500 ,imageVector[i].exifData[3] * 400 );

                        
                    
                        
                    ofSetColor( colors[clusters[i]] , 128 );
                    ofNoFill();
                    ofSetLineWidth(5);
                    ofRect(0, 0, 100, 100);
                        
                    ofSetColor(255);
                    imageVector[i].thumbImage.draw(0, 0, 0);
                        
                    //rectangles are faster ok?
                    //ofSetColor(sin(i)* 200, cos(i)* 255, 0);
                    //ofRect(0, 0, 100, 100);
                    ofDisableDepthTest();
                    ofPopMatrix();
                    }
            }
            
        }
        
    }

    
//    for ( int  i = 0 ; i < 20000 ; i ++ )
//    {
//        ofPushMatrix();
//        ofSetColor(sin(i)* 200, cos(i)* 255, 0);
//        ofRotate(ofGetFrameNum());
//        ofTranslate( sin(i *5) * 1000, cos( i * 5) * 100, sin(i *5) * 300 );
//        ofRect(0, 0, 100, 100);
//        ofPopMatrix();
//        
//    }
    
    //draw3Dgrid();
    //ofDisableDepthTest();
    //ofPopMatrix();
    cam.end();
    
    
    ofPushMatrix();
    fbo2.draw(100, 100);
    ofPopMatrix();
    
    secondWindow.end();
    
    
    //glEnable(GL_DEPTH_TEST);
    
    // creating parallel coordiantes
    
    int xOffset = 56;
    int yOffset = 22;
    ofSetLineWidth(1);
    for ( int i = 0 ; i< NUMPOINTS ; i ++ )
        {
        if ( imageVector[i].clusterNumber >= min && imageVector[i].clusterNumber <=max )
        {
        
            if( imageVector[i].exifData[1] >= minExifRange[1] &&  imageVector[i].exifData[1] <= maxExifRange[1] && imageVector[i].exifData[0] >= minExifRange[0] &&  imageVector[i].exifData[0] <= maxExifRange[0]    )
            {
                if( imageVector[i].exifData[1] >= minExifRange[2] &&  imageVector[i].exifData[1] <= maxExifRange[2] && imageVector[i].exifData[3] >= minExifRange[3] &&  imageVector[i].exifData[3] <= maxExifRange[3] )
                {
            
                    
                // begin of line draw
                for ( int j = 0 ; j < 4 ; j ++ )
                {
                    ofSetColor( colors[clusters[i]] , 128 );
                    int setX  = xOffset + j * 112 ;
                    int setY  = 400+ yOffset - ( imageVector[i].exifData[j] - minExifData[j]) / ( maxExifData[j] - minExifData[j] ) * 400 ;
                    ofLine(setX, setY, setX+60, setY);
                    
                }
                
                for ( int j = 0 ; j < 3 ; j ++ )
                {
                    ofSetColor( colors[clusters[i]] , 128 );
                    int setX1  = xOffset + j * 112 + 60;
                    int setY1  = 400 + yOffset - ( imageVector[i].exifData[j] - minExifData[j]) / ( maxExifData[j] - minExifData[j] ) * 400 ;
                    
                    int setX2  = xOffset + (j+1) * 112 ;
                    int setY2  = 400 + yOffset - ( imageVector[i].exifData[j+1] - minExifData[j+1]) / ( maxExifData[j+1] - minExifData[j+1] ) * 400 ;
                    
                    ofLine(setX1, setY1, setX2, setY2);
                    
                }
                // edn of inner line draw
            
            
                }
            }
        }
            
            
        }// end of drawing lines
    
    
// end of draw
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
        case 'p':
        {
            drawPadding = !drawPadding;
            gui0->setDrawWidgetPadding(drawPadding);
        }
            break;
            
        case 'g':
        {
            gui0->toggleVisible();
        }
            break;
            
            
        case 'o':
        {
            gui1->setTheme(t++);
            cout<<t<<endl;
            break;
        }

        case 'i':
        {
            gui1->setTheme(t--);
             cout<<t<<endl;
            break;
            
        }
            
        default:
            break;


    }

    
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    
    if ( x < 1000 & y< 500 )
    {
        cam.disableMouseInput();
        
    }
    else
        cam.enableMouseInput();

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if ( x < 1000 & y< 500 )
    {
        cam.disableMouseInput();
        
    }
    else
        cam.enableMouseInput();
    

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    
}
//--------------------------------------------------------------
void ofApp::exit()
{
    delete gui0;
}







void ofApp::draw3Dgrid()
{
    ofSetColor(0,0,0,128);
//    
//    for ( float  i = 0 ; i < 10 ; i++)
//    {
//        
//        ofLine( i * 100 , 0, 0, i * 100 , 1000 , 0);
//        ofLine(  0 , i * 100 , 0,  1000,  i * 100 , 0);
//        ofLine(  0 , 0 , i * 100 , 0, 1000,  i * 100 );
//    }

//    for ( int  k = 0 ; k <= 10 ; k++ )
//    {
//        ofPushMatrix();
//        
//        ofTranslate(0, 0 , k *100) ;
//        
//        for ( int  i = 0 ; i <= 10 ; i++ )
//        {
//            for ( int j = 0 ; j <= 10 ; j++ )
//            {
//                
//                         ofLine(  i * 100 , 0 , 0, i * 100 , 1000 , 0 );
//                         ofLine(  0 , j * 100 , 0,  1000,  j * 100 , 0);
//              
//            }
//        }
//        
//        ofPopMatrix();
//        
//        
//    }
//    
//
    
    
    for ( int i = 0 ; i< 100 ; i ++ )
    {
        
    }
    
    
}




void ofApp::findMinMaxVector(void)
{
    minExifData = imageVector[0].exifData;
    maxExifData = imageVector[0].exifData;
    
    for ( int  i = 0 ; i < NUMPOINTS ; i++)
    {
        for ( int j = 0 ; j < 4 ; j++ )
        {
            if (minExifData[j] > imageVector[i].exifData[j])
                minExifData[j] = imageVector[i].exifData[j];
            
            if (maxExifData[j] < imageVector[i].exifData[j])
                maxExifData[j] = imageVector[i].exifData[j];
            
        }
    }
    
    //we should have clean min max in double;
    //now convert them all to float lol

    //minExifRange.assign ( minExifData.begin() , minExifData.end() ) ;
    //maxExifRange.assign ( maxExifData.begin() , maxExifData.end() ) ;
    
    for ( int  i = 0 ; i< minExifData.size() ; i++)
    {
        
        minExifRange.push_back( ofToFloat( ofToString( minExifData[i])  )  );
        maxExifRange.push_back( ofToFloat( ofToString( maxExifData[i])  )  );
        
    }
    
    maxExifData[3] = 5.0;
    
    
    
    
    
}



void ofApp::findSOMMinMaxVectorGlobal(void)
    {
        for ( int  i = 0 ; i < somExifData.size() ; i++)
        {
            for ( int j = 0 ; j < somExifData[i].size() ; j++ )
            {
            
                double dataCell = somExifData.at(i).at(j);
                
                
                if (globalMinExifData[j] > dataCell)
                    globalMinExifData[j] = dataCell;
                
                if (globalMaxExifData[j] < dataCell)
                    globalMaxExifData[j] = dataCell;
                
                
            }
        }


    }



