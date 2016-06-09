#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "Wings.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

#define COLOR_WIDTH 1920
#define COLOR_HEIGHT 1080


#define CONV  float(COLOR_HEIGHT) / float(DEPTH_HEIGHT)


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

		ofxKFW2::Device kinect;	
		ICoordinateMapper* coordinateMapper;
		ofShader shader;
		ofTexture bodyIndexTex;
		ofPixels bodyIndexPix;
		unsigned char pixels[DEPTH_WIDTH * DEPTH_HEIGHT];

		bool bStitchFaces;
		bool bDrawBodies;

		//ofImage foregroundImg;
		//vector<ofVec2f> colorCoords;
		int numBodiesTracked;
		bool bHaveAllStreams;
		

		wing RightWing, LeftWing;
};
