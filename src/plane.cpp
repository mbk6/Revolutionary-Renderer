#include "plane.h"


Plane::Plane(ofVec3f position_, ofVec3f normal_, ofColor color_, int size_) {
	position = position_;
	normal = normal_;
	color = color_;
	size = size_;
	generatePlane(size);
	fixVertices(1.0f);
	rotateToNormal(normal);
}

void Plane::generatePlane(int size) {

	//Generate vertices
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			vertices.push_back(ofVec3f(row, 0, col));
		}
	}

	//Generate vertical edges
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			int top = i + size * j;
			int bottom = i + size * (j+1);
			edges.push_back(ofVec2f(top, bottom));
		}
	}

	//Generate horizontal edges
	for (int i = 0; i < size * size; i+=size) {
		for (int j = 0; j < size - 1; j++) {
			int left = i + j;
			int right = i + j + 1;
			edges.push_back(ofVec2f(left, right));
		}
	}

}

void Plane::rotateToNormal(ofVec3f normal) {
	normal.normalize();
	//Find the angle from the current normal (straight up) to the given normal. Rotate by this much
	float angle = std::acosf(normal.dot(ofVec3f(0, 1, 0)));
	//Axis of rotation is the cross product of the default normal and the new normal
	ofVec3f axis = normal.getCrossed(ofVec3f(0, 1, 0)).normalize();

	rotate(axis * angle);
}