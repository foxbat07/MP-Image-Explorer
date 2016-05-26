#pragma once

#include "ofMain.h"
#include "ImageDataClass.h"
#include "ofxLearn.h"
#include "ofxSecondWindow.h"
#include "ofxUI.h"
#include "ofxSelfOrganizingMap.h"
#include "ofxCsv.h"


#define NUMPOINTS 10000
#define NUMCLUSTERS 10
#define gridSide 115;

const int imageThumbWidth = 115;
const int imageThumbHeight = 115;

const int xMargin  = 100;
const int yMargin = 50;

const int somGridSize = 9;
const int gridSize = 9;
const int textDiff = 18;
const int testImagesDrawn = 200;

using namespace wng;


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
        void drawSecondWindow(void);
        void updateSelections(double actualNumber , double selectedImageNumber );
        void updateGridFbo();
        void drawFullImage(double selectedImageNumber);
        void updateImages();
        void writeSelectedImages(string clientName);
        void createSuperImage();
        void drawParallelCoordinates();
        void drawSOM();
        void trainClusters();
        void drawSuperImage();
    
        void drawSeletedImages();
    
        void clearAll();
    
        void loadFilesOldWay();
    
        void loadFilesNewWay();
    
    
        // new files
    
        string newFlickrFolder = "newFlickr/";
        string newTableDataFolder = "tabledata/";
        string newTableData = "tabledata-2016-";

    
        string newImages = "Images/";
        string imageSizeT = "Large Square";
        string imageSizeM = "Medium";
        string imageSizeL = "Large";
    
        ofxCsv csv;
        ofxCsv csvRecorder;

        ////
    
    
    
    
    
        string folderName = "mirflickr";
        string imageBaseName = "im";
        string imageExtension = ".jpg";
        
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
        vector <ImageDataClass> selectedImageVector;
    
        int numClusters;
        int numPoints;
        vector <int> selectedImages;
    
        ofxLearnKMeansThreaded clusterer;
        vector<double> instances[NUMPOINTS];
        vector<int> clusters;
    
    
        
        ofColor colors[NUMCLUSTERS];
        ofEasyCam cam;
        ofxSecondWindow secondWindow;
        //ofxSelfOrganizingMap som ;
    
        // check for mouse
        bool mouseInsideGrid = false;
        double selectedImageNumber = 0;
        ofImage selectedFullImage;
        double imageSet = 0 ;
        string outputFileName = "defaultName";
        string sessionName = "0";
        bool drawParallelCoordiantes = false;
        bool toggleview = true;
        //int fileLimit = 400;
    
        double actualNumber = 0;
    
        bool reduceExifLimits0 = false;
        bool reduceExifLimits1 = false;
    
    
    
    
    
    // optional: callback function for when training is done
    void callbackTrainingDone() {
        ofLog(OF_LOG_NOTICE, "Training done!!!");
    }
    
    
    // ofxUI
    ofxUISuperCanvas *gui0;
    ofxUISuperCanvas *gui1;
    
    ofxUISuperCanvas *gui2;
    
    
    
   
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
    
    
    
    
    //FBO for image grid
    ofFbo fbo;
    ofFbo fbo2;
    ofFbo fbo3;
    
    
    //fbo for superimage
    ofFbo superImage;
    
    
    ofImage image0;
    ofImage image1;
    //ofImage image2;
    ofShader superImageShader; //Shader
    
    
    
    
    
};
