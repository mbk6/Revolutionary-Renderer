#include "model3d.h"

Model3D::Model3D(std::string obj_path_, ofColor color_, ofVec3f position_) {
	readFromOBJ(obj_path_);
	color = color_;
	position = position_;
	fixVertices();
}

Model3D::Model3D(std::vector<ofVec3f> vertices_, std::vector<int*> edges_, ofColor color_, ofVec3f position_) {
	vertices = vertices_;
	edges = edges_;
	color = color_;
	position = position_;
	fixVertices();
}

void Model3D::readFromOBJ(std::string file_path) {
	//Fills the model's vertex and edge vectors using an obj file
}

void Model3D::fixVertices() {
	//Use the std::accumulate function to compute the "center" of the model by averaging its verticies
	ofVec3f relative_center = std::accumulate(vertices.begin(), vertices.end(), ofVec3f(0, 0, 0)) / vertices.size();

	//Change the basis of the local coordinate system so that each vertex is relative to the center
	for (ofVec3f &vertex : vertices) {
		vertex -= relative_center;
	}
}