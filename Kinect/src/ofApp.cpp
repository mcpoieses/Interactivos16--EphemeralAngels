#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

#define COLOR_WIDTH 1920
#define COLOR_HEIGHT 1080


#define CONV  float(COLOR_HEIGHT) / float(DEPTH_HEIGHT)

//--------------------------------------------------------------
void ofApp::setup(){

	RightWing.setup(15, 0, "pluma01.png", "pluma02.png", "pluma03.png", "pluma04.png");
	LeftWing.setup(15, 1, "pluma01.png", "pluma02.png", "pluma03.png", "pluma04.png");
	ofSetBackgroundColor(0);

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	if (kinect.getSensor()->get_CoordinateMapper(&coordinateMapper) < 0) {
		ofLogError() << "Could not acquire CoordinateMapper!";
	}

	bStitchFaces = false;
	bDrawBodies = true;
	
	if (ofIsGLProgrammableRenderer()) {
		shader.load("shadersGL3/bodyIndex");
	}
	else {
		shader.load("shadersGL3/bodyIndex");
	}
	
	numBodiesTracked = 0;
	bHaveAllStreams = false;

	//foregroundImg.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, OF_IMAGE_COLOR);

	//colorCoords.resize(DEPTH_WIDTH * DEPTH_HEIGHT);

	//ofSetWindowShape(ofGetWidth(), ofGetHeight());

	bodyIndexTex.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, GL_RED);
	bodyIndexPix.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, GL_RED);
}

//--------------------------------------------------------------
void ofApp::update(){

	ofVec2f p;
	float blend = 0.3;

	kinect.update();

	//kinect example things
	/*
	// Get pixel data
	auto& depthPix = kinect.getDepthSource()->getPixels();
	auto& bodyIndexPix = kinect.getBodyIndexSource()->getPixels();
	auto& colorPix = kinect.getColorSource()->getPixels();

	// Make sure there's some data here, otherwise the cam probably isn't ready yet
	if (!depthPix.size() || !bodyIndexPix.size() || !colorPix.size()) {
		bHaveAllStreams = false;
		return;
	}
	else {
		bHaveAllStreams = true;
		}

	// Count number of tracked bodies
	numBodiesTracked = 0;
	auto bodies = kinect.getBodySource()->getBodies();
	
	for (auto& body : bodies) {
		if (body.tracked) {
			numBodiesTracked++;
		}
	}

	// Do the depth space -> color space mapping
	// More info here:
	// https://msdn.microsoft.com/en-us/library/windowspreview.kinect.coordinatemapper.mapdepthframetocolorspace.aspx
	// https://msdn.microsoft.com/en-us/library/dn785530.aspx
	coordinateMapper->MapDepthFrameToColorSpace(DEPTH_SIZE, (UINT16*)depthPix.getPixels(), DEPTH_SIZE, (ColorSpacePoint*)colorCoords.data());

	// Loop through the depth image
	for (int y = 0; y < DEPTH_HEIGHT; y++) {
		for (int x = 0; x < DEPTH_WIDTH; x++) {
			int index = (y * DEPTH_WIDTH) + x;
			foregroundImg.setColor(x, y, ofColor::black);

			// This is the check to see if a given pixel is inside a tracked  body or part of the background.
			// If it's part of a body, the value will be that body's id (0-5), or will > 5 if it's
			// part of the background
			// More info here:
			// https://msdn.microsoft.com/en-us/library/windowspreview.kinect.bodyindexframe.aspx
			float val = bodyIndexPix[index];
			if (val >= bodies.size()) {
				continue;
			}

			// For a given (x,y) in the depth image, lets look up where that point would be
			// in the color image
			ofVec2f mappedCoord = colorCoords[index];

			// Mapped x/y coordinates in the color can come out as floats since it's not a 1:1 mapping
			// between depth <-> color spaces i.e. a pixel at (100, 100) in the depth image could map
			// to (405.84637, 238.13828) in color space
			// So round the x/y values down to ints so that we can look up the nearest pixel
			mappedCoord.x = floor(mappedCoord.x);
			mappedCoord.y = floor(mappedCoord.y);

			// Make sure it's within some sane bounds, and skip it otherwise
			if (mappedCoord.x < 0 || mappedCoord.y < 0 || mappedCoord.x >= COLOR_WIDTH || mappedCoord.y >= COLOR_HEIGHT) {
				continue;
			}

			// Finally, pull the color from the color image based on its coords in
			// the depth image
			foregroundImg.setColor(x, y, colorPix.getColor(mappedCoord.x, mappedCoord.y));
		}
	}

	// Update the images since we manipulated the pixels manually. This uploads to the
	// pixel data to the texture on the GPU so it can get drawn to screen
	foregroundImg.update();
	*/

	//--
	//Getting joint positions (skeleton tracking)
	//--
	//
	bodyIndexPix = kinect.getBodyIndexSource()->getPixels();
	auto bodies = kinect.getBodySource()->getBodies();
		for (auto body : bodies) {

			if (!body.tracked && body.bodyId == RightWing.wingId && body.bodyId == LeftWing.wingId) {
				RightWing.reset(); LeftWing.reset();
			}

			if (body.tracked) {

				//cout << unsigned(body.bodyId) << " ";
				if (RightWing.wingId == 254 && LeftWing.wingId == 254) {
					RightWing.alive = true; LeftWing.alive = true;
					RightWing.wingId = body.bodyId;
					LeftWing.wingId = body.bodyId;
					//cout << unsigned(RightWing.wingId) << endl;
				}
				else if (RightWing.wingId == body.bodyId && LeftWing.wingId == body.bodyId) {

				//rightwing
					//old formulation for color space projection
						/*
						p = RightWing.oldbones.getVertex(0)*blend + body.joints[JointType_ShoulderRight].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						RightWing.bones.setVertex(0, p);
						p = RightWing.oldbones.getVertex(1)*blend + body.joints[JointType_ElbowRight].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						RightWing.bones.setVertex(1, p);
						p = RightWing.oldbones.getVertex(2)*blend + body.joints[JointType_WristRight].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						RightWing.bones.setVertex(2, p);
						p = RightWing.oldbones.getVertex(3)*blend + body.joints[JointType_HandRight].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						RightWing.bones.setVertex(3, p);
						RightWing.oldbones = RightWing.bones;

						//leftwing
						p = LeftWing.oldbones.getVertex(0)*blend + body.joints[JointType_ShoulderLeft].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						LeftWing.bones.setVertex(0, p);
						p = LeftWing.oldbones.getVertex(1)*blend + body.joints[JointType_ElbowLeft].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						LeftWing.bones.setVertex(1, p);
						p = LeftWing.oldbones.getVertex(2)*blend + body.joints[JointType_WristLeft].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						LeftWing.bones.setVertex(2, p);
						p = LeftWing.oldbones.getVertex(3)*blend + body.joints[JointType_HandLeft].getProjected(kinect.getBodySource()->getCoordinateMapper())*(1 - blend);
						//cout << p.x << " " << p.y << endl;
						LeftWing.bones.setVertex(3, p);
						LeftWing.oldbones = LeftWing.bones;
						*/

					p = RightWing.oldbones.getVertex(0)*blend + body.joints[JointType_ShoulderRight].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					RightWing.bones.setVertex(0, p);
					p = RightWing.oldbones.getVertex(1)*blend + body.joints[JointType_ElbowRight].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					RightWing.bones.setVertex(1, p);
					p = RightWing.oldbones.getVertex(2)*blend + body.joints[JointType_WristRight].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					RightWing.bones.setVertex(2, p);
					p = RightWing.oldbones.getVertex(3)*blend + body.joints[JointType_HandRight].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					RightWing.bones.setVertex(3, p);
					RightWing.oldbones = RightWing.bones;

					//leftwing
					p = LeftWing.oldbones.getVertex(0)*blend + body.joints[JointType_ShoulderLeft].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					LeftWing.bones.setVertex(0, p);
					p = LeftWing.oldbones.getVertex(1)*blend + body.joints[JointType_ElbowLeft].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					LeftWing.bones.setVertex(1, p);
					p = LeftWing.oldbones.getVertex(2)*blend + body.joints[JointType_WristLeft].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					LeftWing.bones.setVertex(2, p);
					p = LeftWing.oldbones.getVertex(3)*blend + body.joints[JointType_HandLeft].getPositionInDepthMap()*(1 - blend);
					//cout << p.x << " " << p.y << endl;
					LeftWing.bones.setVertex(3, p);
					LeftWing.oldbones = LeftWing.bones;

					p = body.joints[JointType_SpineShoulder].getPositionInDepthMap() - body.joints[JointType_SpineBase].getPositionInDepthMap();
					LeftWing.featherl = RightWing.featherl = p.length()*0.75;
				}
			}
			//escape_if:
		}	
	//
	//--

	RightWing.update();
	LeftWing.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	//cout << bodyIndexPix.getNumChannels() << " " << bodyIndexPix.getPixelFormat()	<< endl;

	unsigned char* pixels = new unsigned char[DEPTH_HEIGHT * DEPTH_WIDTH];
	if (LeftWing.alive && RightWing.alive) {
		for (int y = 0; y < DEPTH_HEIGHT; y++) {
			for (int x = 0; x < DEPTH_WIDTH; x++) {
				int index = y*DEPTH_WIDTH + x;
				int val = (int)bodyIndexPix[index];
				pixels[index] = val;
				//if (val <= 5)
					//cout << pixels[index] << " " << val << " " << float(RightWing.wingId) <<  endl;
			}
		}
		bodyIndexTex.loadData(pixels, DEPTH_WIDTH, DEPTH_HEIGHT, GL_RED);
		//bodyIndexTex.draw(0, 0);
		//other kinect sources
			/*
			kinect.getDepthSource()->draw(0, 0, previewWidth, previewHeight);  // note that the depth texture is RAW so may appear dark
																		   // Color is at 1920x1080 instead of 512x424 so we should fix aspect ratio
			float colorHeight = previewWidth * (kinect.getColorSource()->getHeight() / kinect.getColorSource()->getWidth());
			float colorTop = (previewHeight - colorHeight) / 2.0;

			kinect.getColorSource()->draw(previewWidth, 0 + colorTop, previewWidth, colorHeight);
			kinect.getBodySource()->drawProjected(previewWidth, 0 + colorTop, previewWidth, colorHeight);

			kinect.getInfraredSource()->draw(0, previewHeight, previewWidth, previewHeight);
			*/


			//shaders
				/*
				//foregroundImg.draw(0, 0, COLOR_WIDTH, COLOR_HEIGHT);
				cam.begin();
				ofPushMatrix();
				ofScale(100, 100, 100);

				shader.begin();
				shader.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
				if (ofIsGLProgrammableRenderer()) {
					shader.setUniformTexture("uBodyIndexTex", kinect.getBodyIndexSource()->getTexture(), 1);
					shader.setUniformTexture("uColorTex", kinect.getColorSource()->getTexture(), 2);
				}
				else {
					// TEMP: Until OF master fixes texture binding for old pipeline.
					shader.setUniform1i("uBodyIndexTex", 1);
					kinect.getBodyIndexSource()->getTexture().bind(1);
					shader.setUniform1i("uColorTex", 2);
					kinect.getColorSource()->getTexture().bind(2);
				}
				ofSetColor(255);
				ofMesh mesh = kinect.getDepthSource()->getMesh(bStitchFaces, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
				mesh.draw();
				if (!ofIsGLProgrammableRenderer()) {
					// TEMP: Until OF master fixes texture binding for old pipeline.
					kinect.getColorSource()->getTexture().unbind(2);
					kinect.getBodyIndexSource()->getTexture().unbind(1);
				}
				shader.end();

				ofPopMatrix();
				cam.end();
				*/

		ofPushMatrix();
		ofTranslate(0.5*(float(COLOR_WIDTH) - float(DEPTH_WIDTH) * CONV), 0);
		ofScale(CONV, CONV);
	
		shader.begin();
		shader.setUniform1i("wingID", int(RightWing.wingId));
		if (ofIsGLProgrammableRenderer()) {
			shader.setUniformTexture("uBodyIndexTex", bodyIndexTex, 1);
		}
		else {
			// TEMP: Until OF master fixes texture binding for old pipeline.
			shader.setUniform1i("uBodyIndexTex", 1);
			bodyIndexTex.bind(1);
		}
		kinect.getInfraredSource()->draw(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
		if (!ofIsGLProgrammableRenderer()) {
			// TEMP: Until OF master fixes texture binding for old pipeline.
			bodyIndexTex.unbind(1);
		}
		shader.end();
		RightWing.draw();
		LeftWing.draw();
		
	ofPopMatrix();
	}
	
	//kinect.getBodyIndexSource()->draw(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
	//kinect.getInfraredSource()->draw(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
	//kinect.getBodySource()->drawProjected(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT, ofxKFW2::ProjectionCoordinates::DepthCamera);

	
	/*
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;
	ss << "Tracked bodies: " << numBodiesTracked << endl;
	ss << LeftWing.angle01 << " " << LeftWing.angle12 << " " << LeftWing.angle23 << endl;
	ofDrawBitmapStringHighlight(ss.str(), 20, 20);
	*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//manipulaate wing with mouse
	/*float dist = INT_MAX, d_temp;
	int vert_ID, num_vert;
	num_vert = RightWing.bones.getNumVertices();
	ofVec2f vect;
	ofPoint p(mouseX, mouseY);
	for (int i = 0; i < num_vert; i++) {
		vect = RightWing.bones.getVertex(i);
		d_temp = (vect.x - p.x)*(vect.x - p.x) + (vect.y - p.y)*(vect.y - p.y);
		if (d_temp < dist) {
			dist = d_temp;
			vert_ID = i;
		}
	}
	RightWing.bones.setVertex(vert_ID, p);
	*/
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

