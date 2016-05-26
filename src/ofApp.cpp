#include "ofApp.h"

#include <algorithm>


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync( true );
    ofSetFrameRate(60);
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableSmoothing();
    ofSetWindowTitle("User Interface");
    //secondWindow.setup("Image Visualizer", 50, 50, 1920, 1200, false);
    cout<< "Welcome to Mohit's user study "<< endl ;
    //setting up labels;
    ExifLabels.push_back("Aperture");
    ExifLabels.push_back("ISO");
    ExifLabels.push_back("Focal Len");
    ExifLabels.push_back("Exposure");
    //////////////////
    

    
    globalMinExifData.assign(5,1000);
    globalMaxExifData.assign(5,0);
    
    fbo.allocate(gridSize * imageThumbWidth, gridSize* imageThumbHeight, GL_RGBA );
    fbo2.allocate(somGridSize * imageThumbWidth, somGridSize* imageThumbHeight, GL_RGBA );
    fbo3.allocate(gridSize * imageThumbWidth, gridSize* imageThumbHeight, GL_RGBA );

    
    
//    superImage.allocate( ofGetWidth(), ofGetHeight() , GL_RGBA);
//    
//    superImage.begin();
//    ofBackgroundGradient( ofColor( 255 ), ofColor( 0 ) );
//    image0.draw(20, 0);
//    superImage.end();
//



    
    
    //loadFilesOldWay();
    
    loadFilesNewWay();
    
    //reshuffle vectors always
    
    std::random_shuffle(imageVector.begin(), imageVector.end());
 
    
    
    //lets cluster the data
    
    for (int i = 0; i < NUMPOINTS; i++) {
        clusterer.addTrainingInstance( imageVector[i].exifData );
    }
    
    clusterer.setNumClusters(NUMCLUSTERS);
    for (int i = 0; i < NUMCLUSTERS; i++) {
        colors[i].setHsb( ( i* 255)/NUMCLUSTERS,255,255 );
    }
    
     findMinMaxVector(); // should have min max by now
    
    
    
    // ofxUI
    
    drawPadding = false;
    gui0 = new ofxUISuperCanvas("Mohit Hingorani");
    gui0->addSpacer();
    gui0->setHeight(500);
    gui0->setWidth(300);
    gui0->setPosition(ofGetWidth() - 300 - xMargin, yMargin + 550 );
    gui0->addLabel("User Study: ");
    gui0->addLabel("Image Selection & Visualization ");

    gui0->addSpacer();
    gui0->addFPSSlider("FPS SLIDER");
//    gui0->addSpacer();
//    gui0->addLabel("Cluster Slider");
//    gui0->addRangeSlider("RANGE", 0.0, NUMCLUSTERS , &min , &max);
    gui0->addSpacer();
    gui0->addLabel("OUTPUT");
    gui0->addTextInput("File Name", outputFileName)->setAutoClear(false);
    gui0->addLabelButton("Create Data File", 1 );
    gui0->addLabelButton("Clear All", 1 );
    
    gui0->addSpacer();
    gui0->addLabelToggle("TOGGLE VIEW", true);
    
    
    
    
    
    
    gui1 = new ofxUISuperCanvas("  Parallel Coordiantes");
    gui1->setTheme(OFX_UI_THEME_HINGOOO);
    gui1->setHeight(500);
    gui1->setWidth(500);
    gui1->toggleVisible();
    gui1->setPosition(ofGetWidth() - 500 -xMargin, yMargin + 600);
    gui1->addSpacer(0, 200);
    gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);

    
    
    
    gui2 = new ofxUISuperCanvas("Next Image Set");
    //gui2->setTheme(OFX_UI_THEME_HINGOOO);
    //gui2->addLabel("NEXT SET", OFX_UI_FONT_SMALL);
    gui2->setHeight(imageThumbHeight * gridSize);
    gui2->setWidth(100);
    //gui2->addButton("Next Image", 1, 92 ,gridSize * imageThumbHeight - 45);
    gui2->addImageButton("Next Image", "arrowImage1.png", 1,92,  gridSize * imageThumbHeight - 45 );
    gui2->setPosition( xMargin + gridSize * imageThumbWidth + 10, yMargin );
    

    
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
    ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);

    
    

    //clusterer.beginTraining(this, &ofApp::callbackTrainingDone);
    
    
    // get SOm stuff ready
    
//    findSOMMinMaxVectorGlobal();  // set up SOM stuff
//    double minInstance[4] = { globalMinExifData[0] , globalMinExifData[1] , globalMinExifData[2] , globalMinExifData[3] };
//    double maxInstance[4] = { globalMaxExifData[0] , globalMaxExifData[1] , globalMaxExifData[2] , globalMaxExifData[3] };
//    
//    som.setFeaturesRange(4, minInstance, maxInstance);
//    som.setInitialLearningRate(0.07);
//    som.setNumIterations(900);
//    som.setMapSize(somGridSize, somGridSize );
//    som.setup();
    
    
    // setup SOM ends here
    
    
    updateGridFbo();
    
    

//    superImageShader.load( "shaderVert.c", "shaderFrag.c" );
//   
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
        //update SOM
//    int frameNum = ofGetFrameNum();
//    // double instance[4] = { somExifData[frameNum][0] ,  somExifData[frameNum][1] ,  somExifData[frameNum][2],  somExifData[frameNum][3] };
//    if(frameNum%120 == 0)
//        updateGridFbo();    //update every two seconds
//    
//    if( frameNum < 10 &&  somExifData[frameNum].size() == 4  )
//    {
//        
//    double instance[4] = { somExifData[frameNum][0] ,  somExifData[frameNum][1] ,  somExifData[frameNum][2],  somExifData[frameNum][3] };
//
//    
//    
//    //  dMinAperture, dMinShutterSpeed, dMinFocalLength, dMinAperture
//    som.updateMap(instance);
//    
//    cout<<"som called: "<<frameNum << endl;
//    
//    // end of SOM
//    
//    }
//    
    //updateGridFbo();

}

//--------------------------------------------------------------


void ofApp::draw(){
    
    //ofPushMatrix();
    // main view
    if (toggleview ==true)
        {
        ofBackground(255);
        ofSetColor(ofColor::blueSteel);
            
        ofDrawBitmapString( " RL0: " +  ofToString(reduceExifLimits0)+" RL1: " +ofToString(reduceExifLimits1) + " mouse grid: " + ofToString(mouseInsideGrid) + "  sn: "+ ofToString(selectedImageNumber)+ " in: "+ ofToString(actualNumber) +" is: "+ofToString(imageSet) + " " +ofToString(selectedImages) , xMargin, 20);
        ofSetColor(ofColor::white);
        fbo.draw(xMargin,yMargin );
        
       
            
            
        //choose hover image
        imageVector.at(selectedImageNumber).loadFullImage();
        imageVector.at(selectedImageNumber).fullImage.draw(ofGetWidth() -xMargin - imageVector.at(selectedImageNumber).fullImage.width , yMargin , imageVector.at(selectedImageNumber).fullImage.width , imageVector.at(selectedImageNumber).fullImage.height );
        imageVector.at(selectedImageNumber).clearFullImage();
            
        
        gui0->setPosition( ofGetWidth() -xMargin - 300 , yMargin + 550 );
        gui2->setVisible(true);
            
        drawParallelCoordiantes = false;
        if(drawParallelCoordiantes)
        {
            gui1->setVisible(true);
            drawParallelCoordinates();
        }
        else
            gui1->setVisible(false);
        }
    
    // selection view
     if (toggleview == false)
        {
        gui2->toggleVisible();
        drawSeletedImages();
        fbo3.draw(xMargin, yMargin);
        drawParallelCoordiantes = true;
        
        if(drawParallelCoordiantes)
        {
            gui1->setVisible(true);
            drawParallelCoordinates();
        }
        else
        {
            gui1->setVisible(false);
        }
            gui0->setPosition( ofGetWidth() -xMargin - 350  , yMargin );
         
        ofSetColor(ofColor::blue);
        ofDrawBitmapString( " RL0: " +  ofToString(reduceExifLimits0)+" RL1: " +ofToString(reduceExifLimits1) + " mouse grid: " + ofToString(mouseInsideGrid) + "  sn: "+ ofToString(selectedImageNumber)+ " in: "+ ofToString(actualNumber) +" is: "+ofToString(imageSet) + " " +ofToString(selectedImages.size()) , xMargin, 20);
            ofSetColor(ofColor::white);
            
            // draw exif + big image
            
            
            
        if (selectedImageNumber < selectedImageVector.size()  )
                {
                //choose hover image
                selectedImageVector.at(selectedImageNumber).loadFullImage();
                selectedImageVector.at(selectedImageNumber).fullImage.draw(ofGetWidth() -xMargin - selectedImageVector.at(selectedImageNumber).fullImage.width - 500 , yMargin , selectedImageVector.at(selectedImageNumber).fullImage.width , selectedImageVector.at(selectedImageNumber).fullImage.height );
                selectedImageVector.at(selectedImageNumber).clearFullImage();
                    
                    for ( int i = 0 ; i<  4 ; i ++ )
                    {
                        ofSetColor(ofColor::red);
                        ofDrawBitmapString( ExifLabels[i], ofGetWidth() - xMargin - 500 - 140 , 640 + yMargin + i * 20 );
                        ofDrawBitmapString(ofToString(selectedImageVector[selectedImageNumber].exifData[i]), ofGetWidth() - xMargin - 500 -40, 640 + yMargin + i * 20 );
                        ofSetColor(ofColor::white);
                        
                        ofSetColor(ofColor::red);
                        //ofDrawBitmapString( ExifLabels[i], ofGetWidth() - xMargin -400  + i * 80, ofGetHeight() -  yMargin  );
                        ofDrawBitmapString(ofToString(selectedImageVector[selectedImageNumber].exifData[i]), ofGetWidth() - xMargin -430  + i * 110, ofGetHeight() -  yMargin  );
                        ofSetColor(ofColor::white);
                        

                    }
                
                
                }
            
        }
        
        
        
        //drawSuperImage();
        

    
    
// end of draw
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
        case '/':
        {
            gui0->toggleVisible();
            break;
            
        }
    
        case '.':
        {
            gui1->toggleVisible();
            drawParallelCoordiantes = !drawParallelCoordiantes;
            
            break;
            
        }
        case '[':
        {
            reduceExifLimits0 = !reduceExifLimits0;
            findMinMaxVector();
            break;
            
        }
            

        case ']':
        {
            reduceExifLimits1 = !reduceExifLimits1;
            findMinMaxVector();
            
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
    
    if ( toggleview ==true )            // only for image selection
    {
        if ( x > xMargin && y > yMargin &&  x <  ( xMargin + gridSize * imageThumbWidth ) &&  y < ( yMargin + gridSize * imageThumbHeight ) )
        {
            mouseInsideGrid = true;
            int xLevel = ( x - xMargin ) / imageThumbWidth;
            int yLevel = ( y - yMargin ) / imageThumbHeight;
            selectedImageNumber  = yLevel  + xLevel * gridSize + imageSet * gridSize * gridSize;  /// change algo here
            actualNumber = imageVector[selectedImageNumber].imageNumber;
            
            
            
            //cout<<sin<<" "<<imageVector[sin].imageNumber<<endl ;
        }
        else
        {
            mouseInsideGrid = false;
        }
        
    }
    else{
        
        // selection view
        if ( x > xMargin && y > yMargin &&  x <  ( xMargin + gridSize * imageThumbWidth ) &&  y < ( yMargin + gridSize * imageThumbHeight ) )
        {
            mouseInsideGrid = true;
            int xLevel = ( x - xMargin ) / imageThumbWidth;
            int yLevel = ( y - yMargin ) / imageThumbHeight;
            selectedImageNumber = 0;
            selectedImageNumber  = yLevel  + xLevel * gridSize ;  /// change algo here
            
            if ( selectedImageNumber < selectedImageVector.size() )
            {
                actualNumber = selectedImageVector[selectedImageNumber].imageNumber;
            }
            else
            {
                //actualNumber = selectedImageVector[0].imageNumber;
                selectedImageNumber = 0;
            }
            

   
        }
    
    }
    
    
//    imageVector[selectedImageNumber].isImageHover = true;
    //updateGridFbo();
    
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    
    
    if (toggleview && mouseInsideGrid)
    {
    updateSelections(actualNumber, selectedImageNumber);
    selectedImageVector.push_back(imageVector.at(selectedImageNumber));
    updateGridFbo();
    }
    

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
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if(name == "Create Data File")
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        cout << "value: " << button->getValue() << endl;
        writeSelectedImages(outputFileName);
        
    }
    else if(name == "Next Image")
    {
        ofxUIImageButton *button = (ofxUIImageButton *) e.widget;
        cout << "value: " << button->getValue() << endl;
        if(button->getValue()){
            updateImages();

            }
    }
    
    else if(name == "File Name")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
        {
            cout << "ON ENTER: ";
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
        {
            cout << "ON FOCUS: ";
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cout << "ON BLUR: ";
        }
        outputFileName = textinput->getTextString();
        cout << outputFileName << endl;
    }

    else if(name == "TOGGLE VIEW")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value: " << toggle->getValue() << endl;
        toggleview = !toggleview;
    }
    
    else if(name == "Clear All")
    {
        clearAll();
        
    }
    


    
    
}
//--------------------------------------------------------------
void ofApp::exit()
{
    delete gui0;
}

void ofApp::draw3Dgrid()
{
    ofSetColor(0,0,0,128);
    
    
    for ( float  i = 0 ; i < 10 ; i++)
    {
        
        ofLine( i * 100 , 0, 0, i * 100 , 1000 , 0);
        ofLine(  0 , i * 100 , 0,  1000,  i * 100 , 0);
        ofLine(  0 , 0 , i * 100 , 0, 1000,  i * 100 );
    }

    for ( int  k = 0 ; k <= 10 ; k++ )
    {
        ofPushMatrix();
        
        ofTranslate(0, 0 , k *100) ;
        
        for ( int  i = 0 ; i <= 10 ; i++ )
        {
            for ( int j = 0 ; j <= 10 ; j++ )
            {
                
                         ofLine(  i * 100 , 0 , 0, i * 100 , 1000 , 0 );
                         ofLine(  0 , j * 100 , 0,  1000,  j * 100 , 0);
              
            }
        }
        
        ofPopMatrix();
        
        
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
    
    
    
    //reduced limits
    if (reduceExifLimits0)
    {
        maxExifData[0] = 32;
        maxExifData[1] = 3200;
        
        maxExifData[2] = 200;
        
        maxExifData[3] = 1.0;
        
    }
    
    if (reduceExifLimits1)
    {
        maxExifData[0] = 32;
        maxExifData[1] = 3200;
        
        maxExifData[2] = 200;
        
        maxExifData[3] = 0.2;
        
    }
    
    minExifRange.clear();
    maxExifRange.clear();
    
    for ( int  i = 0 ; i< minExifData.size() ; i++)
    {
        
        minExifRange.push_back( ofToFloat( ofToString( minExifData[i])  )  );
        maxExifRange.push_back( ofToFloat( ofToString( maxExifData[i])  )  );
        
    }
    
    
    
    
    
    
    
    
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


void ofApp::drawSecondWindow(void)
{
    
    // drawing stuff now
    
    secondWindow.begin();
    ofBackground(0);
    cam.begin();
    //ofRotateY(ofGetFrameNum());
    
    ofEnableDepthTest();
    for (int i = 0; i < testImagesDrawn ; i++)
    {
       
            if( imageVector[i].exifData[1] >= minExifRange[1] &&  imageVector[i].exifData[1] <= maxExifRange[1] && imageVector[i].exifData[0] >= minExifRange[0] &&  imageVector[i].exifData[0] <= maxExifRange[0]    )
            {
                if( imageVector[i].exifData[1] >= minExifRange[2] &&  imageVector[i].exifData[1] <= maxExifRange[2] && imageVector[i].exifData[3] >= minExifRange[3] &&  imageVector[i].exifData[3] <= maxExifRange[3] )    // add cluster as well
                {
                    
                    ofPushMatrix();
                    ofEnableDepthTest();
                    
                    ofTranslate(imageVector[i].exifData[0] * 50 -900 , imageVector[i].exifData[2] * 10 - 1500 ,imageVector[i].exifData[3] * 400 );
                    //ofTranslate(100* ofNoise(ofGetFrameNum()),100* ofNoise(ofGetFrameNum()),100*  ofNoise(ofGetFrameNum()) );
                    
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
    
    
    
    
    //draw3Dgrid();
    //ofDisableDepthTest();
    //ofPopMatrix();
    cam.end();
    
    
    ofPushMatrix();
    
    fbo2.draw(secondWindow.getWidth()  - fbo2.getWidth()- xMargin, yMargin);
    ofPopMatrix();
    
    secondWindow.end();
    
}


//fix

void ofApp::updateSelections( double actualImageNumber  , double selectedImageNumber )
{
    
    
    if ( imageVector[selectedImageNumber].isImageSelected == false )
    {
        imageVector[selectedImageNumber].isImageSelected = true;
        // add it to the picked images vector
        selectedImages.push_back(actualImageNumber);
        cout<<"number of selected images: " <<selectedImages.size() <<endl;
        
        cout<< "actual number selected:  "<< actualImageNumber << "  selected number: "<< selectedImageNumber  <<endl;
        
    }
    else if ( imageVector[selectedImageNumber].isImageSelected == true)
    {
        
        imageVector[selectedImageNumber].isImageSelected = false;
        
        for ( int i = 0 ; i < selectedImages.size() ; i ++ )
        {
            if ( actualImageNumber == selectedImages[i] )
            {
                selectedImages.erase(selectedImages.begin() + i );
                
            }
        }
        
    }
    
    
}



//
void ofApp::updateGridFbo()
{
    //cout<<endl<<"updating FBO"<<endl;
    
    fbo.begin();
    ofClear(255,255,255, 0 );
    int imagesDisplayed = gridSize * gridSize;
    int startNumber = imageSet * imagesDisplayed;
    
    //cout<<"images displayed: "<<imagesDisplayed<<"start Number: "<<startNumber;
    
    
    // create more white
    
    //for (int i = startNumber ; i < startNumber + imagesDisplayed  ; i++)
    for (int i = 0 ; i < imagesDisplayed  ; i++)

    {
        //imageThumbs[i].draw( i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight );
        int iReal = i + startNumber;
        imageVector[iReal].loadThumbImage();
        imageVector[iReal].thumbImage.draw( i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight , imageThumbWidth, imageThumbHeight );
    
        if( imageVector[iReal].isImageSelected == true  )
        {
            ofSetColor( ofColor::green,192);
            ofNoFill();
            ofSetLineWidth(4);
            ofRect( i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight , imageThumbWidth, imageThumbHeight );
            ofSetColor( ofColor::white );
            
        }
        
    }
    fbo.end();
    
    
}


//wrong image being dislayed due to different numbers

void ofApp::drawFullImage(double selectedNumber)
{
    
    //create selected image
    ofPushMatrix();
    string selectedFullImagePath = ofToString(folderName) + "/" + ofToString(imageBaseName) + ofToString( selectedNumber + 1 ) + ofToString(imageExtension);        // fix original image issue
    //selectedFullImagePath = imageVector[selectedImageNumber].imagePath;
    
    selectedFullImage.loadImage(selectedFullImagePath);
    selectedFullImage.draw(ofGetWidth() -xMargin - selectedFullImage.width , yMargin , selectedFullImage.width , selectedFullImage.height );
    ofPopMatrix();

    
    
}


void ofApp::updateImages(void)
{
    
    
        if ( imageSet <  ( NUMPOINTS / ( gridSize*gridSize ) )  - 1 )
            imageSet+=1;

        else
            imageSet=0;
    
        cout<<imageSet <<"imageSet updated"<<endl;
        
        updateGridFbo();
        //imageVector.clear();

}



void ofApp::writeSelectedImages(string clientName)
{
    ofFile file( clientName + ".txt", ofFile::WriteOnly);
    
    for( int i = 0 ; i <  selectedImages.size() ; i++)
    {
       // file << selectedImages[i] <<endl;
        file<< ofToString(selectedImageVector[i].imageNumber,0,11,' ')<< ","<< selectedImageVector[i].dAperture<<","<< selectedImageVector[i].dISOSpeed <<","<< selectedImageVector[i].dFocalLength<<","<< selectedImageVector[i].dShutterSpeed << endl;
        
    }
    
    cout<< "file created"<<endl ;
    
    //        file << s->x << " " << s->y << " ";
    //        file << s->scale << " " << s->scalex << " " << s->scaley << " ";
    //        file << s->rotate << " ";
    //        file << s->r << " " << s->g << " " << s->b << " " << s->brightness;
    
    createSuperImage();
    
}


void ofApp::createSuperImage()
{
    // create super image by layering data
    //FBO: super Image
    for ( int i = 0 ; i < selectedImages.size(); i++)
    {
            
        
        
    }
    
    
}


void ofApp::drawParallelCoordinates()
{
    //glEnable(GL_DEPTH_TEST);
    
    
    // creating parallel coordiantes
    ofPushMatrix();
    ofTranslate(ofGetWidth() -500 -xMargin, yMargin + 600);
    int xOffset = 56;
    int yOffset = 22;
    
    // replace this i with selection vectors actual image Numbers?
    
    for ( int i = 0 ; i< selectedImages.size() ; i ++ )
    {
        
        for ( int j = 0 ; j < 4 ; j++)
        {
            for ( int k = 0 ; k < 11 ; k++ )
            {
                ofSetLineWidth(2);
                ofSetColor( ofColor::gray , 150 );
                int setX  = xOffset + j * 112 ;
                int setY  = 400+ yOffset - k * 40 ;
                ofLine(setX, setY, setX+60, setY);

            }
            
        }
        
            if( selectedImageVector[i].exifData[1] >= minExifRange[1] &&  selectedImageVector[i].exifData[1] <= maxExifRange[1] && selectedImageVector[i].exifData[0] >= minExifRange[0] &&  selectedImageVector[i].exifData[0] <= maxExifRange[0] && selectedImageVector[i].exifData[2] >= minExifRange[2] &&  selectedImageVector[i].exifData[2] <= maxExifRange[2] && selectedImageVector[i].exifData[3] >= minExifRange[3] &&  selectedImageVector[i].exifData[3] <= maxExifRange[3]    )
                {
                
                    ofSetLineWidth(1);
                    selectedImageVector[i].isImageDrawn =true;
                    // begin of line draw
                    for ( int j = 0 ; j < 4 ; j ++ )
                    {
                        ofSetColor( ofColor::blue , 255 );
                        int setX  = xOffset + j * 112 ;
                        int setY  = 400+ yOffset - ( selectedImageVector[i].exifData[j] - minExifData[j]) / ( maxExifData[j] - minExifData[j] ) * 400 ;
                        ofLine(setX, setY, setX+60, setY);
                        
                    }
                    
                    for ( int j = 0 ; j < 3 ; j ++ )
                    {
                        ofSetColor( ofColor::blue , 255 );
                        int setX1  = xOffset + j * 112 + 60;
                        int setY1  = 400 + yOffset - ( selectedImageVector[i].exifData[j] - minExifData[j]) / ( maxExifData[j] - minExifData[j] ) * 400 ;
                        
                        int setX2  = xOffset + (j+1) * 112 ;
                        int setY2  = 400 + yOffset - ( selectedImageVector[i].exifData[j+1] - minExifData[j+1]) / ( maxExifData[j+1] - minExifData[j+1] ) * 400 ;
                        
                        ofLine(setX1, setY1, setX2, setY2);
                        
                    }
                    
                    // end of inner line draw
                    ofSetColor(ofColor::white);
                    
                    //////////////////draw selected Line highlight ///////////////////
                     if (selectedImageNumber < selectedImageVector.size() && selectedImageVector[i].isImageDrawn == true )
                     {
                            ofSetLineWidth(3);
                            selectedImageVector[i].isImageDrawn =true;
                            // begin of line draw
                            for ( int j = 0 ; j < 4 ; j ++ )
                            {
                                ofSetColor( ofColor::red , 255 );
                                int setX  = xOffset + j * 112 ;
                                int setY  = 400+ yOffset - ( selectedImageVector[selectedImageNumber].exifData[j] - minExifData[j]) / ( maxExifData[j] - minExifData[j] ) * 400 ;
                                ofLine(setX, setY, setX+60, setY);
                                
                            }
                            
                            for ( int j = 0 ; j < 3 ; j ++ )
                            {
                                ofSetColor( ofColor::red , 255 );
                                int setX1  = xOffset + j * 112 + 60;
                                int setY1  = 400 + yOffset - ( selectedImageVector[selectedImageNumber].exifData[j] - minExifData[j]) / ( maxExifData[j] - minExifData[j] ) * 400 ;
                                
                                int setX2  = xOffset + (j+1) * 112 ;
                                int setY2  = 400 + yOffset - ( selectedImageVector[selectedImageNumber].exifData[j+1] - minExifData[j+1]) / ( maxExifData[j+1] - minExifData[j+1] ) * 400 ;
                                
                                ofLine(setX1, setY1, setX2, setY2);
                                
                            }
                         
                     }
                    

                    ////////////////// ////////////////// ////////////////// //////////////////
                    
            }
        
        else
            selectedImageVector[i].isImageDrawn = false;
        
    }// end of drawing lines
    
    
    
    ofSetColor(ofColor::white);
    ofPopMatrix();
    
    //end of parallel coordiantes
    
    
    
}



void ofApp::drawSOM()
{
    //update FBO
    
//        fbo2.begin();
//    
//        //ofClear(255,255,255);
//        ofBackground(192,40 );
//        for (int i = 0; i < somGridSize; i++) {
//            for (int j = 0; j < somGridSize; j++) {
//    
//                double * exifData = som.getMapAt(i,j);
//                ofSetColor(ofColor::blue);
//                ofDrawBitmapString( ofToString( exifData[0] ), i * imageThumbWidth , j * imageThumbHeight + textDiff );
//                ofSetColor(ofColor::yellowGreen);
//                ofDrawBitmapString(ofToString( exifData[1] )  , i * imageThumbWidth , j * imageThumbHeight + 2* textDiff );
//                ofSetColor(ofColor::green);
//                ofDrawBitmapString(ofToString( exifData[2] ) , i * imageThumbWidth , j * imageThumbHeight + 3* textDiff );
//                ofSetColor(ofColor::red);
//                ofDrawBitmapString(ofToString( exifData[3] ) , i * imageThumbWidth , j * imageThumbHeight + 4* textDiff );
//                ofSetColor(ofColor::white );
//    
//                //imageThumbs[i * gridSize + j ].draw( i * imageThumbWidth , j * imageThumbHeight );
//            }
//            
//        }
//        fbo2.end();
//
    
}


void ofApp::trainClusters()
{
//    //ofBackgroundGradient(ofColor::black, ofColor::grey);
//    //ofBackground(red, green, blue);
//    
//    if (!clusterer.isTrained()) {
//        ofSetColor(0);
//        ofDrawBitmapString("Please wait... Training clusterer in its own thread! ("+ofToString(ofGetFrameNum())+")", 50, 50);
//        return;
//    }
//    clusters = clusterer.getClusters();
//    
//    if ( clusterer.isTrained() && isClusterAssigned == false )
//    {
//        isClusterAssigned = true;
//        for ( int i = 0 ; i <NUMPOINTS ; i ++ )
//        {
//            imageVector[i].setClusterNumber(clusters[i] );
//            cout<<i <<"  "<< clusters[i] <<endl ;
//        }
//        
//        
//    }

    
}


void ofApp::drawSuperImage()
{
    
    //show created fbo with the new super image
    
    
    
    
//    float time = ofGetElapsedTimef();
//    
//    //superImageShader.setUniform1f( "iterator", iterator );
//    
//    superImageShader.begin(); //Enable the shader
//    
//    //cout<<"shader called"<<endl;
//    superImageShader.setUniform1f("time", time );
//    superImageShader.setUniform2f("resolution", ofGetWindowWidth(), ofGetWindowHeight() );
//    superImageShader.setUniform2f("mouse", ofGetMouseX(),ofGetMouseY() );
//
//    superImageShader.setUniformTexture("texture0", image0.getTextureReference(), 1);
//    superImageShader.setUniformTexture("texture1", image1.getTextureReference(), 2);
//    
//    //superImage.draw(0,0);
//    
//    //Draw fbo image
//    superImageShader.end(); //Disable the shader
 
    
}



void ofApp::drawSeletedImages()
{
    
    //cout<<"image vector size " <<imageVector.size()<< endl;
    //cout<<"selected Image Size:"<<selectedImageVector.size()<< endl;
    
    fbo3.begin();
    
    ofClear(255,255,255, 0 );
    if(selectedImageVector.size()!=0)
        {
        for (int i = 0 ; i < selectedImageVector.size()    ; i++)
            {
                //imageThumbs[i].draw( i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight );
        //        try {
        //            //cout<<" selectedImages[i]: "<< selectedImages[i] <<" imageVector[selectedImages[i]]:  "<< imageVector[selectedImages[i]].imageNumber<<endl;
        //            //cout<<" selectedImages[i]: "<< selectedImages[i] << endl;
                
        //
            if ( selectedImageVector[i].isImageDrawn == true )
                 selectedImageVector[i].thumbImage.draw( i/gridSize * imageThumbWidth , i % gridSize * imageThumbHeight, imageThumbWidth, imageThumbHeight );
            }
    
        }
        
    fbo3.end();
    
    
   // fbo3.draw(xMargin, yMargin);

}


void ofApp::clearAll()
{
    std::random_shuffle(imageVector.begin(), imageVector.end());
    
    
    for ( int i = 0 ; i< imageVector.size() ; i++)
    {
        imageVector[selectedImageNumber].isImageSelected = false;
    }
    
    
    selectedImageVector.clear();
    
 
}



void ofApp::loadFilesOldWay()
{
    
    
    
    
    string metaFilepath = ofToString(metadataFolder) + "/" + ofToString(metadataName) + ofToString(numberOfImages) + ofToString(metadataExtention);
    
    string tagPath = ofToString(metadataFolder) + "/" + ofToString(tagName) + ofToString(numberOfImages) + ofToString(metadataExtention);  //just the tag name is different
    
    
    //metadata
    string metaFilepath10000 = ofToString(metadataFolder) + "/" + ofToString(metadataName) + ofToString("10000") + ofToString(metadataExtention);       //for SOM
    
    ofFile metadataFile10000;
    metadataFile10000.open ( metaFilepath10000 ,ofFile::ReadWrite, false ); //for SOM
    //cout<<metaFilepath<<endl;
    //cout<<metaFilepath10000<<endl;
    
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
            
            string thumbPath = ofToString(thumbnailFolder) + "/" + ofToString(thumbnailName) + ofToString(i+1) + ofToString(thumbnailExtention);  //just the tag name is different
            string imagePath = ofToString(folderName) + "/" + ofToString(imageBaseName) + ofToString(i+1) + ofToString(imageExtension);  //just the tag name is different
            // +1 because the first line contains the fields
            
            
            
            vector <string> tagWords;
            tagWords = ofSplitString(tagLine, " ");
            tagWords.erase(tagWords.begin()); // delete the first number
            
            vector <string> metaExif;
            metaExif = ofSplitString(metaLine, " ");
            
            ImageDataClass tempDataClass;
            tempDataClass.setExif(metaExif);
            tempDataClass.setTags(tagWords);
            tempDataClass.setThumbImage(thumbPath);
            tempDataClass.setFullImage(imagePath);
            tempDataClass.setBooleanFlags();
            
            imageVector.push_back(tempDataClass);
            
        }// end of data loading
        
        
        
    }//end of if file

    
    //lets load data for 10000 file now
    if ( metadataFile10000.isFile() )
    {
        ofBuffer metaBuffer100000 = metadataFile10000.readToBuffer();
        //cout << metaBuffer.getText() << endl;
        
        
        for ( int i = 0 ; i< noImages; i ++ )
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

    
    
}




void ofApp::loadFilesNewWay()
{
    
//    
//    string newFlickrFolder = "newFlickr/";
//    string newTableDataFolder = "tabledata/";
//    string newTableData = "tabledata-2016-";
//    
//    
//    string newImages = "Images/";
//    string imageSizeT = "Large Square";
//    string imageSizeM = "Medium";
//    string imageSizeL = "Large";
//
    
      string DataPath = ofToString(newFlickrFolder) +  ofToString(newTableDataFolder) +ofToString(newTableData) +  "01-01" + ".csv";  //new csv names for all
      string thumbNailPath = ofToString(newFlickrFolder) + ofToString(newImages) + ofToString(imageSizeT) + "01/" ;
      string imagePath =  ofToString(newFlickrFolder) + ofToString(newImages) + ofToString(imageSizeM) + "01/" ;
    
    int dateArray [] = {31,28,31,30,22};
    int dateArray1 [] = {5,5,5,5,5};
    int counter = 0 ;
    
    for( int j  = 1 ;  j <= 3 ; j++ )
        {
            for ( int i = 1 ; i < dateArray1[j]; i ++ )
            {
                
                // fix ofToString percision
                
                
                // find data
                //string DataPath = ofToString(newFlickrFolder) +  ofToString(newTableDataFolder) +ofToString(newTableData) +  "01-01" + ".csv";  //new csv names for all
                string DataPath = ofToString(newFlickrFolder) +  ofToString(newTableDataFolder) +ofToString(newTableData) +  ofToString(j,2,'0') +"-"+ ofToString(i,2,'0') + ".csv";  //new csv names for
                csv.loadFile(ofToDataPath(DataPath));
                cout<<DataPath<< endl;
                cout<<csv.numRows<< endl;
                
                
                for ( int k = 1 ; k < csv.numRows ; k++)
                {
                
                 //if( csv.data[k][1] != NULL && csv.data[k][2] != NULL  && csv.data[k][3] != NULL && csv.data[k][4] != NULL && csv.data[k][5] != NULL )
                    {
                        string thumbNailPath = ofToString(newFlickrFolder) + ofToString(newImages) + ofToString(imageSizeT) + ofToString(j,2,'0') +"/"+ ofToString(csv.data[k][0])+ ".jpg" ;
                        string imageMediumPath =  ofToString(newFlickrFolder) + ofToString(newImages) + ofToString(imageSizeM) + ofToString(j,2,'0') +"/" + ofToString(csv.data[k][0])+ ".jpg";
                
                        cout<< thumbNailPath<< endl;
                        
                        
                        // exists(); to see if paths are avaialble
                        ofFile thumbFile;
                        bool thumbExists = thumbFile.doesFileExist(thumbNailPath);
                        cout<< thumbExists << endl;
                        
                        ofFile mediumFile;
                        bool mediumExists = mediumFile.doesFileExist(imageMediumPath);
                        cout<<mediumExists << endl;
                        
                       // cout<< csv.get
                        
                        if ( thumbExists == true && mediumExists == true )
                            {
                                //now go through each line put exif in vector
                                
                                vector <string> metaExif;
                                for ( int l = 0 ; l < 4 ; l++)          // not shutter speed
                                {
                                    //cout<< ofToString(metaExif) << endl;
                                    
                                    string temp = csv.getString(k,l) ;
                                    

                                    metaExif.push_back(temp);
                                }
                                
                                
                                
                                string ss =  csv.getString(k, 4);
                                
                                if ( ss != " " )
                                {
                                vector <string> sss;
                                sss = ofSplitString(ss , "/");
                                //cout<<ofToString(sss)<<endl ;
                                
                                if (sss.size() != 0)
                                    {
                                        
                                    if (sss.size() == 1)
                                    {
                                        
                                        metaExif.push_back( ofToString(sss[0]) ) ;
                                    }
                                    
                                    if (sss.size() == 2)
                                    {
                                        double num = ofToDouble( sss[0] );
                                        double den = ofToDouble( sss[1] );
                                        
                                        double s = num/den;
                                        cout<< s;
                                        metaExif.push_back( ofToString(s) ) ;
                                        
                                    }
                                    
                                    cout<< ofToString( metaExif )<< endl;
                                        
                                    
                                    //final check
                                    
                                    if ( ofToDouble(metaExif[0]) != 0 && ofToDouble(metaExif[1]) != 0 && ofToDouble(metaExif[2]) != 0 &&ofToDouble(metaExif[3]) != 0 && ofToDouble(metaExif[4]) != 0  )
                                        {
                                            
                                       
                                                ImageDataClass tempDataClass;
                                                tempDataClass.setExif(metaExif);
                                                tempDataClass.setThumbImage(thumbNailPath);
                                                tempDataClass.setFullImage(imageMediumPath);
                                                tempDataClass.setBooleanFlags();
                                                
                                                imageVector.push_back(tempDataClass);
                                                counter++;
                                                cout<<"counter: " << counter<< endl;
                                                
                                                // if data fine then add paths
                                        }
                                        
                                    
                                    }
                                }
                                
                            }
                            

                    
                    
                        
                    }
                    
                    
                    
                }
                
                
                
                
            }
            
        }
    
    
    cout << "data entered "<< counter << endl;
    //cout << csv.data[0][1] << endl;

    
    //ofFile metadataFile;
    //metadataFile.open ( metaFilepath ,ofFile::ReadWrite, false );
  
   // string thumbPath = ofToString(thumbnailFolder) + "/" + ofToString(thumbnailName) + ofToString(i+1) + ofToString(thumbnailExtention);  //just the tag name is different
   // string imagePath = ofToString(folderName) + "/" + ofToString(imageBaseName) + ofToString(i+1) + ofToString(imageExtension);  //just the tag name is different

    
    
    
    

    
    
}




