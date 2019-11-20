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

void Model3D::rotateVector(ofVec3f &vector, ofVec3f axis, float angle) {

	//Make sure the axis is a normal vector
	axis.normalize();

	//Rotation matrix from https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	float cos = std::cosf(angle);
	float sin = std::sinf(angle);

	ofMatrix3x3 rotation_matrix = ofMatrix3x3(
		cos + x*x*(1.0f - cos),     x*y*(1.0f - cos) - z*sin,   x*z*(1.0f - cos) + y*sin,
		y*x*(1.0f - cos) + z*sin,   cos + y*y*(1.0f - cos),      y*z*(1.0f - cos) - x*sin,
		z*x*(1.0f - cos) - y*sin,   z*y*(1.0f - cos) + x*sin,   cos + z*z*(1.0f - cos)
	);

	//Multiply vector by rotation matrix
	x = rotation_matrix[0] * vector.x + rotation_matrix[1] * vector.y + rotation_matrix[2] * vector.z;
	y = rotation_matrix[3] * vector.x + rotation_matrix[4] * vector.y + rotation_matrix[5] * vector.z;
	z = rotation_matrix[6] * vector.x + rotation_matrix[7] * vector.y + rotation_matrix[8] * vector.z;

	vector.x = x;
	vector.y = y;
	vector.z = z;

}

void Model3D::rotate(ofVec3f axis, float angle) {
	for (ofVec3f& vertex : vertices) {
		rotateVector(vertex, axis, angle);
	}
}