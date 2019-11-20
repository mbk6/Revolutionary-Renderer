#pragma once
#include <fstream>

#include "ofMain.h"

class Model3D {

private:
	/**
		Fills the model's vertex and edge vectors using an OBJ file at the given file path
	*/
	void readFromOBJ(std::string file_path);

	/**
		Adds an edge to the edge vector only if it or its reverse are not already in the edge vector
	*/
	void addEdge(int vert0, int vert1);

	/**
		Modifies the model's vertex data to be relative to the object's relative center
	*/
	void fixVertices();

	/**
		Rotates a given 3D vector about a given axis by a given angle
	*/
	void rotateVector(ofVec3f &vector, ofVec3f axis, float angle);

public:
	Model3D(std::string obj_path_, ofColor color_, ofVec3f position_);

	ofVec3f position;
	ofColor color;
	std::vector<ofVec3f> vertices;
	std::vector<ofVec2f> edges;

	void rotate(ofVec3f axis, float angle);

};