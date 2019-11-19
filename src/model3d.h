#pragma once

#include "ofMain.h"

class Model3D {

private:

	void readFromOBJ(std::string file_path);


public:
	Model3D();
	Model3D(std::string obj_path_, ofColor color_, ofVec3f position_);
	Model3D(std::vector<ofVec3f> vertices_, std::vector<int*> edges_, ofColor color_, ofVec3f position_);

	ofVec3f position;
	ofColor color;
	std::vector<ofVec3f> vertices;
	std::vector<int*> edges;



};