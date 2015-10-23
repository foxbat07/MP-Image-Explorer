#pragma once

#include "ofMain.h"
#include "ImageDataClass.h"
#include "ofxLearn.h"
#include "ofxSecondWindow.h"
#include "ofxUI.h"
#include "ofxSelfOrganizingMap.h"


#define NUMPOINTS 1000
#define NUMCLUSTERS 20
#define gridSide 100;

const int imageThumbWidth = 80;
const int imageThumbHeight = 80;

const int xMargin  = 100;
const int yMargin = 50;




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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
        void guiEvent(ofxUIEventArgs &e);
    
        void findMinMaxVector(void);
        void findSOMMinMaxVectorGlobal(void);
    
    
        void draw3Dgrid();
    
    
        //string folderName = "mirflickr";
        //string imageBaseName = "im";
        //string imageExtension = ".jpg";
        
        string metadataFolder = "metadata";
        string tagName = "tagwords";
        string metadataName = "metadata";
        string metadataExtention = ".txt";
        
        string thumbnailFolder = "thumb";
        string thumbnailName = "im";
        string thumbnailExtention = ".png";
        
        string numberOfImages = ofToString(NUMPOINTS);
        int noImages = NUMPOINTS;        // change both beacuse i am a dummy
        bool isClusterAssigned = false;

    
        vector <ImageDataClass> imageVector;
        int numClusters;
        int numPoints;
        
        ofxLearnKMeansThreaded clusterer;
        vector<double> instances[NUMPOINTS];
        vector<int> clusters;
    
    
        
        ofColor colors[NUMCLUSTERS];
        ofEasyCam cam;
        ofxSecondWindow secondWindow;
    
    
        ofxSelfOrganizingMap som ;
    
    
    
    
    
    // optional: callback function for when training is done
    void callbackTrainingDone() {
        ofLog(OF_LOG_NOTICE, "Training done!!!");
    }
    
    
    // ofxUI
    ofxUISuperCanvas *gui0;
    ofxUISuperCanvas *gui1;
    
    
   
    bool drawPadding;
    
    float min = 0 ;
    float max = NUMCLUSTERS-1;
    
    
    
    vector <double> minExifData;
    vector <double> maxExifData;
    
    vector <float> fMinExifData;
    vector <float> fMaxExifData;

    
    vector <float> minExifRange;
    vector <float> maxExifRange;
    
    vector <string> ExifLabels ;
    
    //vector<float> ISOList { 50,100,200,400,500,640,800,1600,3200,6400,12800,25600} ;
    
    //vector<float> ShutterList = { 0.000125, 0.00025 , 0.0005 , 0.001 , 0.002 , 0.004 , 0.005 , 0.1 , 1 , 2, 3, 50 } ;// limit it
    
    //vector< float > ApertureList  { 0.95, 1,1.2,1.4, 1.6 , 1.8 , 2.0, 2.2 , 2.5 , 2.8 , 3.5 , 4.2, 4.8, 5.6, 6.3,7.1 , 8.0 , 9.0 ,11.0 , 13.0 , 16.0 } ;
    
    
    
    vector < vector<double> > somExifData;
    
    float red,green, blue;
    int t = 0 ;
    
    vector <double> globalMinExifData;
    vector <double> globalMaxExifData;
    
    int somGridSize = 10;
    int gridSize = 10;
    int textDiff = 18;
    int testImagesDrawn = 200;
    
    
    
    //FBO for image grid
    ofFbo fbo;
    ofFbo fbo2;
    
    


    
    
    
    
    

    
    
    
		
};
