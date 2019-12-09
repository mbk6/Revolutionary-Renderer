// MODEL3D - Defines the Model3D class - for storing state and behavior for all 3D wireframe objects

#pragma once
#include <fstream>

#include "ofMain.h"

class Model3D {

protected:
	/* Fills the model's vertex and edge vectors using an OBJ file at the given file path */
	void readFromOBJ(std::string file_path);

	/* Adds an edge to the edge vector only if it or its reverse are not already in the edge vector */
	void addEdge(int vert0, int vert1);

	/* Modifies the model's vertex data to be relative to the object's relative center. Also scales the object by the given size scale */
	void fixVertices(float size_scale);

	/* Rotates a given 3D vector about a given axis by an angle given by the magnitude of that axis */
	void rotateVector(ofVec3f& vector, ofVec3f rotation_vector);

public:

	//Default Model3D constructor
	Model3D();

	//Model3D constructor
	Model3D(std::string obj_path_, ofColor color_, ofVec3f position_, float size_scale_);

	//Model3D destructor (needed a virtual method in order for other classes to inherit properly from this one)
	virtual ~Model3D();

	ofVec3f position;			/* Position of the model in world coordinates */
	ofColor color;				/* Color of the model */
	std::vector<ofVec3f> vertices;		/* Set of verticies defining the shape of the object */
	std::vector<ofVec2f> edges;		/* Set of integer pairs representing the indices of vertices that are connected by an edge */

	/* Rotates the entire model about a given axis by an angle given by the magnitude of that axis */
	void rotate(ofVec3f rotation_vector);
};