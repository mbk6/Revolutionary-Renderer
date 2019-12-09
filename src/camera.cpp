#include "camera.h"


Camera::Camera() {
	//Default constructor does nothing. It's not used by the Renderer program but is required for setting up the camera after initializing it in renderer.cpp
}

Camera::Camera(ofVec3f position_, ofVec2f rotation_, float max_vertical_angle_, float turn_speed_,
	float move_speed_, float mouse_sensitivity_, float field_of_view_, int win_width_, int win_height_) {

	position = position_;
	rotation = rotation_;
	max_vertical_angle = max_vertical_angle_;
	turn_speed = turn_speed_;
	move_speed = move_speed_;
	mouse_sensitivity = mouse_sensitivity_;
	field_of_view = field_of_view_;

	//Initialize the local basis
	computeLocalBasis();

	//Setup window parameters
	win_width = win_width_;
	win_height = win_height_;
	win_center = ofVec2f(win_width / 2, win_height / 2);
	win_margin[0] = win_width / 4;
	win_margin[1] = win_height / 4;
	out_of_bounds_point = ofVec2f(-1 * win_margin[0] - 1, -1 * win_margin[1] - 1);
}

void Camera::update(ofVec3f move_direction, ofVec3f rotation_change, bool mouse_turning, float time_interval) {
	if (move_direction != ofVec3f()) {
		//Update the position
		position += move_direction * move_speed * time_interval;
	}
	if (rotation_change != ofVec2f()) {
		//Update the rotation
		if (mouse_turning) {
			rotation += rotation_change * mouse_sensitivity * time_interval;
		}
		else {
			rotation += rotation_change * turn_speed * time_interval;
		}
		
		// Check for boundaries
		if (std::abs(rotation.y) > max_vertical_angle) {
			//Change the magnitude of cam_rot[1] back to max_vertical_angle but keep the sign the same
			rotation.y = std::copysign(max_vertical_angle, rotation.y);
		}
		//Update the local basis
		computeLocalBasis();
	}


}

void Camera::zoom(float zoom_scale) {
	//Change the field of view exponentially
	field_of_view *= std::powf(zoom_speed, zoom_scale);
}

ofVec2f Camera::transform(ofVec3f point3d) {
	// 3d transformation equations from https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point
	// and https://www.youtube.com/watch?v=g4E9iq0BixA

	//Recenter points based on camera position, mazing camera.position the new origin
	float x = point3d.x - position.x;
	float y = point3d.y - position.y;
	float z = point3d.z - position.z;

	//Rotate x and z by camera.rotation[0] about the origin, then y and z by camera.rotation[1]
	rotateCoords(x, z, rotation.x);
	rotateCoords(y, z, rotation.y);

	//Check if the coordinate is in front of the camera
	if (z < 0) {

		//Scale x and y depending on how far the point is from the camera
		z = field_of_view / z;
		x *= z;
		y *= z;

		//Adjust point with respect to the screen center. Subtract from x-center and add to y-center to maintain proper coordinate system
		return ofVec2f(win_center.x - x, win_center.y + y);
	}
	else {
		//Return an out of bounds point to indicate that it should not be drawn
		return out_of_bounds_point;
	}
}

void Camera::rotateCoords(float& coord0, float& coord1, float angle) {
	//Rotation equation from https://www.youtube.com/watch?v=g4E9iq0BixA
	float temp = coord0;
	coord0 = temp * std::cos(angle) + coord1 * std::sin(angle);
	coord1 = coord1 * std::cos(angle) - temp * std::sin(angle);
}

bool Camera::inBounds(ofVec2f point2d) {
	//Check if the point lies on within the limits of the screen plus the outer margin
	return (point2d.x >= -1 * win_margin[0] && point2d.x <= win_width + win_margin[0]) && (point2d.y >= -1 * win_margin[1] && point2d.y <= win_height + win_margin[1]);
}

void Camera::drawModel(Model3D* model) {
	ofSetColor(model->color);
	//Draw all edges of the given model
	ofVec2f point0;
	ofVec2f point1;
	for (ofVec2f edge : model->edges) {
		//Transform the two vertices indicated by the current edge
		point0 = transform(model->vertices[(int)edge.x] + model->position);
		point1 = transform(model->vertices[(int)edge.y] + model->position);

		//Only draw the edge if both points are in bounds
		if (inBounds(point0) && inBounds(point1)) {
			ofDrawLine(point0, point1);
		}
	}
}

void Camera::computeLocalBasis() {
	// Equations derived by me :)

	// Use horizontal and vertical rotation to form the unit vector pointing in the direction of the camera
	local_basis[0] = ofVec3f(std::sin(rotation.x), std::sin(rotation.y), -1 * std::cos(rotation.x)).normalize();

	// Reverse sine and cosine to get the vector pointing exactly to the right of the camera and parallel to the x/z plane
	local_basis[1] = ofVec3f(std::cos(rotation.x), 0, std::sin(rotation.x));

	// Take the cross product of the two  and normalize it - this will point straight up form the camera
	local_basis[2] = local_basis[1].getCrossed(local_basis[0]).normalize();
}


