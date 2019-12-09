// PLANE - Defines the Plane class - a specific 3D model representing a flat square plane

#pragma once

#include "model3d.h"

class Plane : public Model3D {

private:
	/* Given a size, generates the set of size*size vertices and edges connecting them to define a square-shaped plane */
	void generatePlane(int size);

	/* Rotates the plane so that the given vector is its normal vector */
	void rotateToNormal(ofVec3f normal);

public:

	int size;			/* Number of vertices along the edge of the plane */
	ofVec3f normal;		/* Vector normal to the plane */

	//Plane constructor
	Plane(ofVec3f position_, ofVec3f normal_, ofColor color_, int size_);

};