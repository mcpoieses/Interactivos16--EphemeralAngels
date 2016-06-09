#pragma once

#include "ofMain.h"

class wing{
	public:
		ofMesh bones, oldbones;
		ofMesh feathers;
		ofImage feather1, feather2, feather3, feather4;

		float angle01, angle12, angle23;

		int NF; // number of feathers per bone
		int side; //0 left, 1 right

		void setup(int NF, int RoL, string img1_name, string img2_name, string img3_name, string img4_name);
		void update();
		void draw();
		//void angle_func(int sec, float k, float b, float dt);
};

float feather_l(float l_frac, float scale);





