#include "model3d.h"

Model3D::Model3D() {

}

Model3D::Model3D(std::string obj_path_, ofColor color_, ofVec3f position_) {
	readFromOBJ(obj_path_);
	color = color_;
	position = position_;
}

Model3D::Model3D(std::vector<ofVec3f> vertices_, std::vector<int*> edges_, ofColor color_, ofVec3f position_) {
	vertices = vertices_;
	edges = edges_;
	color = color_;
	position = position_;
}

void Model3D::readFromOBJ(std::string file_path) {
	//Fills the model's vertex and edge vectors using an obj file
}