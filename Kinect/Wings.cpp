#include "Wings.h"

void wing::setup(int N, int RoL, string img1_name, string img2_name, string img3_name, string img4_name) {

	NF = N;

	feather1.loadImage(img1_name);
	feather1.setAnchorPercent(0.5, 0.0);
	feather2.loadImage(img2_name);
	feather2.setAnchorPercent(0.5, 0.0);
	feather3.loadImage(img3_name);
	feather3.setAnchorPercent(0.5, 0.0);
	feather4.loadImage(img4_name);
	feather4.setAnchorPercent(0.5, 0.0);

	side = pow(-1, RoL);
	featherl = 0;

	alive = false;
	wingId = 254;
	//skeleton
	bones.setMode(OF_PRIMITIVE_LINE_STRIP);

	ofVec3f A(0.5*ofGetScreenWidth(), 0.5*ofGetScreenHeight());
	ofVec3f B(0.5*ofGetScreenWidth() + 1 * side, 0.5*ofGetScreenHeight());
	ofVec3f C(0.5*ofGetScreenWidth() + 2 * side, 0.5*ofGetScreenHeight());
	ofVec3f D(0.5*ofGetScreenWidth() + 3 * side, 0.5*ofGetScreenHeight());

	bones.addVertex(A);
	bones.addVertex(B);
	bones.addVertex(C);
	bones.addVertex(D);

	oldbones = bones;

	feathers.setMode(OF_PRIMITIVE_LINES);
	ofVec2f v_min, v_max, dr, da;

	for (int i = 0; i < bones.getNumVertices() - 1; i++) {
		v_min = bones.getVertex(i);
		v_max = bones.getVertex(i + 1);
		dr = (v_max - v_min) / float(NF);

		for (int j = 0; j < NF + (i == bones.getNumVertices() - 2); j++) {
			da = dr;
			da.rotate(90);
			da.normalize();
			feathers.addVertex(v_min + j*dr);
			feathers.addVertex(v_min + j*dr + da);
		}
	}
}

void wing::reset() {
	alive = false;
	wingId = 254;
	//skeleton

	for(int i=0; i < 4; i++)
	bones.setVertex(i, ofVec3f(0.5*ofGetScreenWidth() + i*side, 0.5*ofGetScreenHeight()));

	oldbones = bones;

	feathers.setMode(OF_PRIMITIVE_LINES);
	ofVec2f v_min, v_max, dr, da;

	/*
	for (int i = 0; i < bones.getNumVertices() - 1; i++) {
		v_min = bones.getVertex(i);
		v_max = bones.getVertex(i + 1);
		dr = (v_max - v_min) / float(NF);

		for (int j = 0; j < NF + (i == bones.getNumVertices() - 2); j++) {
			da = dr;
			da.rotate(90);
			da.normalize();
			feathers.setVertex(j + i*NF, v_min + j*dr);
			feathers.setVertex(j+1 v_min + j*dr + da);
		}
	}
	*/

}

void wing::update() {
	ofVec2f v_min, v_max, dr, da;
	float angle;
	float length;

	if (alive) {
		//**** calculate angles between bones ****
		v_min = bones.getVertex(0);
		v_max = bones.getVertex(1);
		dr = (v_max - v_min) / float(NF);
		dr.normalize();
		angle01 = atan2(-dr.y * 1.0, dr.dot(ofVec2f(side*1.0, 0.0)));
		v_min = bones.getVertex(1);
		v_max = bones.getVertex(2);
		angle12 = atan2(dr.x * ofVec2f(v_max - v_min).normalize().y - dr.y * ofVec2f(v_max - v_min).normalize().x, dr.dot(ofVec2f(v_max - v_min).normalize()));
		dr = (v_max - v_min) / float(NF);
		dr.normalize();
		v_min = bones.getVertex(2);
		v_max = bones.getVertex(3);
		angle23 = atan2(dr.x * ofVec2f(v_max - v_min).normalize().y - dr.y * ofVec2f(v_max - v_min).normalize().x, dr.dot(ofVec2f(v_max - v_min).normalize()));

		angle01 = side*ofRadToDeg(angle01);
		angle12 = side*ofRadToDeg(angle12);
		angle23 = side*ofRadToDeg(angle23);

		//^^^^ calculate angles between bones ^^^^

		//**** calculate feather positions from bone angles ****

		//feathers for bone 1
		v_min = bones.getVertex(0);
		v_max = bones.getVertex(1);
		dr = (v_max - v_min) / float(NF);
		for (int i = 0; i < NF; i++) {
			length = feather_l(float(i) / float(3 * NF), featherl);	//calculate feather length according to position along wing
			da = dr;
			angle = (angle01 + side * 90) * (1 - float(i) / float(NF)) + side*(90 + 0.5*angle12)*((float(i) / float(NF)));	//calculte feather angle with bone, according to angles with incident bones
			da.rotate(angle);
			da.scale(length);
			feathers.setVertex(2 * i, v_min + i*dr);
			feathers.setVertex(2 * i + 1, v_min + i*dr + da);
		}

		//feathers for bone 2
		v_min = bones.getVertex(1);
		v_max = bones.getVertex(2);
		dr = (v_max - v_min) / float(NF);
		for (int i = 0; i < NF; i++) {
			length = feather_l(float(i + NF) / float(3 * NF), featherl);
			da = dr;
			angle = (0.5*(-angle12 + 180)*(1 - float(i) / float(NF)) + (90 + 0.5*angle23)*((float(i) / float(NF))));
			angle *= side;
			da.rotate(angle);
			da.scale(length);
			feathers.setVertex(2 * (NF + i), v_min + i*dr);
			feathers.setVertex(2 * (NF + i) + 1, v_min + i*dr + da);
		}

		//feathers for bone 3
		v_min = bones.getVertex(2);
		v_max = bones.getVertex(3);
		dr = (v_max - v_min) / float(NF);
		for (int i = 0; i <= NF; i++) {
			da = dr;
			length = feather_l(float(i + 2 * NF) / float(3 * NF), featherl);
			angle = 0.5*(-angle23 + 180)*(1 - (float(i) / float(NF) * 100 / 90));
			angle *= side;
			da.rotate(angle);
			da.scale(length);
			feathers.setVertex(2 * (2 * NF + i), v_min + i*dr);
			feathers.setVertex(2 * (2 * NF + i) + 1, v_min + i*dr + da);
		}
	}
}

void wing::draw() {
	
	if (alive) {
		float angle, mag, scl, b;
		int bone_ID;
		ofVec2f r, v;
		ofImage *feather_img;
		for (int i = 0; i < feathers.getNumVertices() - 1; i += 2) {
			r.set(feathers.getVertex(i + 1) - feathers.getVertex(i));
			r.normalize();
			angle = r.angle(ofVec2f(0.0, 1.0));
			mag = feathers.getVertex(i + 1).distance(feathers.getVertex(i));
			//cout << feather1.getHeight() << " ";

			bone_ID = floor(float(i) / float(2 * NF + 1));
			//cout << bone_ID << " " << i << endl;
			if (bone_ID < 2)
				feather_img = &feather2;
			else
				feather_img = &feather1;

			ofPushMatrix();
			ofTranslate(feathers.getVertex(i));
			v.set((bones.getVertex(bone_ID).x - oldbones.getVertex(bone_ID).x + bones.getVertex(bone_ID + 1).x - oldbones.getVertex(bone_ID + 1).x)*0.5, (bones.getVertex(bone_ID).y - oldbones.getVertex(bone_ID).y + bones.getVertex(bone_ID + 1).y - oldbones.getVertex(bone_ID + 1).y)*0.5);
			//b = 2 * abs(sin(r.angle(v)));
			b = 0;
			ofRotate(-angle + b*ofNoise(feathers.getVertex(i + 1).x, feathers.getVertex(i + 1).x));
			scl = mag / feather_img->getHeight();
			ofScale(side*scl, scl);
			//cout << v.length() << endl;
			feather_img->draw(0.0, 0.0);
			ofScale(1 / scl, 1 / scl);
			scl = mag / feather3.getHeight();
			ofScale(0.66*scl, 0.66 * scl);
			//ofRotate(b*ofNoise(feathers.getVertex(i + 1).x, feathers.getVertex(i + 1).x));
			feather3.draw(0.0, 0.0);
			ofScale(1 / scl / 0.66, 1 / scl / 0.66);
			scl = mag / feather4.getHeight();
			ofScale(0.5 * scl, 0.5 * scl);
			ofRotate(b*ofNoise(feathers.getVertex(i + 1).x, feathers.getVertex(i + 1).x));
			feather4.draw(0.0, 0.0);

			ofPopMatrix();
		}
	}
	//feathers.draw();
}
/*
void wing::angle_func(int sec, float k, float b, float dt) {

	float B, F, angle_0;
	float *angle, *w;
	ofVec2f v, r;

	if (sec == 0) {
		angle = &angleOld01;
		angle_0 = angle01;
		w = &w01;
		r.set(bones.getVertex(0));
		v.set((bones.getVertex(0).x - oldbones.getVertex(0).x + bones.getVertex(1).x - oldbones.getVertex(1).x)*0.5 / dt, (bones.getVertex(0).y - oldbones.getVertex(0).y + bones.getVertex(1).y - oldbones.getVertex(1).y)*0.5 / dt);
	}
	if (sec == 1) {
		angle = &angleOld12;
		angle_0 = angle12;
		w = &w12;
		r.set(bones.getVertex(0));
		v.set((bones.getVertex(1).x - oldbones.getVertex(1).x + bones.getVertex(2).x - oldbones.getVertex(2).x)*0.5 / dt, (bones.getVertex(1).y - oldbones.getVertex(1).y + bones.getVertex(2).y - oldbones.getVertex(2).y)*0.5 / dt);
	}
	if (sec == 2) {
		angle = &angleOld23;
		angle_0 = angle23;
		w = &w23;
		r.set(bones.getVertex(0));
		v.set((bones.getVertex(2).x - oldbones.getVertex(2).x + bones.getVertex(3).x - oldbones.getVertex(3).x)*0.5 / dt, (bones.getVertex(2).y - oldbones.getVertex(2).y + bones.getVertex(3).y - oldbones.getVertex(3).y)*0.5 / dt);
	}
	
	float w_t = *w, a_t = *angle;
	//calculate B according to bone orientationw wrt movement direction
	B = b*pow(cos(r.angle(v)), 2.0);

	//calculate force from spring and movement
	F = -k*(angle_0 - a_t) + B*v.length();
	//calculate new angle and new angular velocity
	*angle = a_t + w_t *dt;
	*w = w_t + F*dt;
	cout << *w << " " << w_t << endl;
}
*/


float feather_l(float l_frac, float scale) {
	float A = -1.075, B = 3.5541, C = -2.441;
	return (1 + A*l_frac + B*pow(l_frac, 2.0) + C*pow(l_frac, 3.0))*scale;
}
