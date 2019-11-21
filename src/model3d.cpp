#include "model3d.h"

Model3D::Model3D(std::string obj_path_, ofColor color_, ofVec3f position_, float size_scale_) {
	readFromOBJ(obj_path_);
	color = color_;
	position = position_;
	fixVertices(size_scale_);
}

void Model3D::readFromOBJ(std::string file_path) {
	
	//Open the file with an fstream
	std::fstream obj_reader;

	obj_reader.open(file_path);

	//Check for invalid opening
	if (!obj_reader) {
		std::cout << "Unable to open " << file_path;
		exit(0);
	}

	std::string file_line;
	std::string int_string;
	ofVec3f new_vector;
	int triangle_verts[3];

	//For each line of the file, check for the 'v ' or 'f ' prefix identifying sets of vertices and edges 
	while (obj_reader >> file_line) {
		if (file_line.size() == 1) {
			
			//Vertices
			if (file_line[0] == 'v') {
				//Load the next three floats into a vector and push it into the vertices vector
				obj_reader >> new_vector.x;
				obj_reader >> new_vector.y;
				obj_reader >> new_vector.z;
				vertices.push_back(new_vector);
			}

			//Faces
			else if (file_line[0] == 'f') {

				//For each of the three tokens following 'f ', build int_string using the first set of digits from the string,
				// then convert int_string to an int and subtract 1, since all OBJ face indicies are one too large by convention
				for (int i = 0; i < 3; i++) {
					obj_reader >> file_line;
					int j = 0;
					while (isdigit(file_line[j])) {
						int_string += file_line[j];
						j++;
					}
					triangle_verts[i] = stoi(int_string) - 1;
					int_string = "";
				}

				//Call the addEdge method to keep the edge from being double-counted
				addEdge(triangle_verts[0], triangle_verts[1]);
				addEdge(triangle_verts[1], triangle_verts[2]);
				addEdge(triangle_verts[2], triangle_verts[0]);
			}
		}
	}
}

void Model3D::addEdge(int vert0, int vert1) {
	// Iterate through the edge set checking whether (vert0, vert1) or (vert1, vert0) is already in the set
	for (ofVec2f edge : edges) {
		if ((vert0 == edge.x && vert1 == edge.y) || (vert0 == edge.y && vert1 == edge.x)) {
			return;
		}
	}
	edges.push_back(ofVec2f(vert0, vert1));
}

void Model3D::fixVertices(float size_scale) {
	//Use the std::accumulate function to compute the "center" of the model by averaging its verticies
	ofVec3f relative_center = std::accumulate(vertices.begin(), vertices.end(), ofVec3f(0, 0, 0)) / vertices.size();

	//Change the basis of the local coordinate system so that each vertex is relative to the center, then multiply it by the size scale
	for (ofVec3f &vertex : vertices) {
		vertex -= relative_center;
		vertex *= size_scale;
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
	// Apply the rotateVector() method to each vertex in the model's vertex set
	for (ofVec3f& vertex : vertices) {
		rotateVector(vertex, axis, angle);
	}
}