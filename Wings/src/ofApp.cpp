#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	RightWing.setup(10, 1, "pluma01.png", "pluma02.png", "pluma03.png", "pluma04.png");
	/*
	//skeleton
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	mesh.enableColors();

	ofVec3f A(100.0, 100.0, 0.0);
	ofVec3f B(300.0, 100.0, 0.0);
	ofVec3f C(500, 100.0, 0.0);
	ofVec3f D(641.4, 241.4, 0.0);

	mesh.addVertex(A);
	mesh.addColor(ofFloatColor(1.0, 0.0, 0.0)); // Red

	mesh.addVertex(B);
	mesh.addColor(ofFloatColor(0.0, 1.0, 0.0)); // Green

	mesh.addVertex(C);
	mesh.addColor(ofFloatColor(1.0, 1.0, 0.0)); // Blue

	mesh.addVertex(D);
	mesh.addColor(ofFloatColor(1.0, 1.0, 0.0)); // Blue

	//feathers
	feathers.setMode(OF_PRIMITIVE_LINES);
	mesh.enableColors();
	int NF = 30;
	ofVec2f v_min, v_max, dr, da;
	float angle;

	v_min = mesh.getVertex(0);
	v_max = mesh.getVertex(1);
	dr = (v_max - v_min)/float(NF);
	for (int i = 0; i < NF; i++) {
		da = dr;
		da.rotate(90, v_min + i*dr);
		da.normalize();
		da.scale(100.0);
		feathers.addVertex(v_min + i*dr);
		feathers.addVertex(v_min + i*dr + da);
	}
	v_min = mesh.getVertex(1);
	v_max = mesh.getVertex(2);
	dr = (v_max - v_min) / float(NF);
	for (int i = 0; i < NF; i++) {
		da = dr;
		angle = (1 - float(i) / float(NF)) * 90;
		da.rotate(angle, v_min + i*dr);
		da.normalize();
		da.scale(100.0);
		feathers.addVertex(v_min + i*dr);
		feathers.addVertex(v_min + i*dr + da);
	}
	v_min = mesh.getVertex(2);
	v_max = mesh.getVertex(3);
	dr = (v_max - v_min) / float(NF);
	for (int i = 0; i <= NF; i++) {
		da = dr;
		angle = (1 - float(i) / float(NF)) * 90;
		da.rotate(angle, v_min + i*dr);
		da.normalize();
		da.scale(100.0);
		feathers.addVertex(v_min + i*dr);
		feathers.addVertex(v_min + i*dr + da);
	}
	*/
}

//--------------------------------------------------------------
void ofApp::update(){
	RightWing.update();
	/*
	int NF = 30;
	ofVec2f v_min, v_max, dr, da;
	float angle, angle01, angle12, angle23;
	float scale = 200, length;
	
	v_min = mesh.getVertex(0);
	v_max = mesh.getVertex(1);
	dr = (v_max - v_min) / float(NF);
	dr.normalize();
	angle01 = acos(dr.dot(ofVec2f(1.0, 0.0)));
	v_min = mesh.getVertex(1);
	v_max = mesh.getVertex(2);
	angle12 = atan2(dr.x * ofVec2f(v_max - v_min).normalize().y - dr.y * ofVec2f(v_max-v_min).normalize().x, dr.dot(ofVec2f(v_max - v_min).normalize()));
	dr = (v_max - v_min) / float(NF);
	dr.normalize();
	v_min = mesh.getVertex(2);
	v_max = mesh.getVertex(3);
	angle23= atan2(dr.x * ofVec2f(v_max - v_min).normalize().y - dr.y * ofVec2f(v_max - v_min).normalize().x, dr.dot(ofVec2f(v_max - v_min).normalize()));
	
	//cout << ofRadToDeg(angle01) << " " << ofRadToDeg(angle12) << " " << ofRadToDeg(angle23);

	angle01 = ofRadToDeg(angle01);
	angle12 = ofRadToDeg(angle12);
	angle23 = ofRadToDeg(angle23);

	v_min = mesh.getVertex(0);
	v_max = mesh.getVertex(1);
	dr = (v_max - v_min) / float(NF);
	for (int i = 0; i < NF; i++) {
		length = feather_l(float(i) / float(3*NF), scale);
		da = dr;
		angle = 90*(1 - float(i)/float(NF)) + (90 + 0.5*angle12)*((float(i) / float(NF)));
		//angle *= pow((1 - angle01 / 90),3.0);
		//if (i == 0)
			//cout << " " << angle << endl;
		da.rotate(angle);
		da.scale(length);
		feathers.setVertex(2*i, v_min + i*dr);
		feathers.setVertex(2*i+1, v_min + i*dr + da);
	}

	v_min = mesh.getVertex(1);
	v_max = mesh.getVertex(2);
	dr = (v_max - v_min) / float(NF);
	for (int i = 0; i < NF; i++) {
		length = feather_l(float(i+NF) / float(3*NF), scale);
		da = dr;
		angle = 0.5*(-angle12 + 180)*(1 - float(i) / float(NF)) + (90 + 0.5*angle23)*((float(i) / float(NF)));
		//angle *= pow((1 - angle01 / 90), 3.0);
		//if (i == 0)
			//cout << " " << angle << endl;
		da.rotate(angle);
		da.scale(length);
		feathers.setVertex(2 * (NF + i), v_min + i*dr);
		feathers.setVertex(2 * (NF + i)+1, v_min + i*dr + da);
	}

	v_min = mesh.getVertex(2);
	v_max = mesh.getVertex(3);
	dr = (v_max - v_min) / float(NF);
	for (int i = 0; i <= NF; i++) {
		da = dr;
		length = feather_l(float(i+2*NF) / float(3*NF), scale);
		angle = 0.5*(-angle23 + 180)*(1 - (float(i) / float(NF) * 100/90));
		//angle *= pow((1 - angle01 / 90), 3.0);
		//if (i == 0)
			//cout << " " << angle << endl;
		da.rotate(angle);
		da.scale(length);
		feathers.setVertex(2*(2*NF+i), v_min + i*dr);
		feathers.setVertex(2*(2*NF+i)+1, v_min + i*dr + da);
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	RightWing.draw();
	
	//mesh.draw();
	//feathers.draw();
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
	float dist = INT_MAX, d_temp;
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
