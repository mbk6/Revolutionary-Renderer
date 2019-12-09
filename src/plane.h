#pragma once

#include "model3d.h"

class Plane : public Model3D {

private:
	void generatePlane(int size);
	void rotateToNormal(ofVec3f normal);

public:
	int size;
	ofVec3f normal;

	Plane(ofVec3f position_, ofVec3f normal_, ofColor color_, int size_) {
		position = position_;
		normal = normal_;
		color = color_;
		size = size_;
		generatePlane(size);
		fixVertices(1.0f);
		rotateToNormal(normal);
	}

};