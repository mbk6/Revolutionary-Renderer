#include "ofApp.h"



ofVec2f Renderer::transform(ofVec3f point3d) {
	// 3d transformation equations from https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point
	// and https://www.youtube.com/watch?v=g4E9iq0BixA

	//Recenter points based on camera position, mazing cam_pos the new origin
	float x = point3d.x - cam_pos.x;
	float y = point3d.y - cam_pos.y;
	float z = point3d.z - cam_pos.z;

	//Rotate x and z by cam_rot[0] about the origin, then y and z by cam_rot[1]
	rotateCoords(x, z, cam_rot[0]);
	rotateCoords(y, z, cam_rot[1]);

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
		//Return an out of bounds point so that it is not drawn
		return ofVec2f(-1*win_margin[0] - 1, -1 * win_margin[1] - 1);
	}
}

void Renderer::rotateCoords(float& coord1, float& coord2, float& angle) {
	//Rotation equation from https://www.youtube.com/watch?v=g4E9iq0BixA
	float temp = coord1;
	coord1 = temp * std::cos(angle) + coord2 * std::sin(angle);
	coord2 = coord2 * std::cos(angle) - temp * std::sin(angle);
}

bool Renderer::inBounds(ofVec2f point2d)
{
	//Check if the point lies on within the limits of the screen plus the outer margin
	return (point2d.x >= -1*win_margin[0] && point2d.x <= win_width + win_margin[0]) && (point2d.y >= -1*win_margin[1] && point2d.y <= win_height + win_margin[1]);
}

void Renderer::computeLocalBasis() {
	// Equation derived by me!
	
	// Use horizontal and vertical rotation to form the unit vector pointing in the direction of the camera
	local_basis[0] = ofVec3f(std::sin(cam_rot.x), std::sin(cam_rot.y), -1*std::cos(cam_rot.x)).normalize();

	// Reverse sine and cosine to get the vector pointing exactly to the right of the camera and parallel to the x/z plane
	local_basis[1] = ofVec3f(std::cos(cam_rot.x), 0, std::sin(cam_rot.x));

	// Take the cross product of the two - this will point straight up form the camera
	local_basis[2] = local_basis[1].getCrossed(local_basis[0]).normalize();
}

Renderer::Renderer(int width, int height) {
	win_width = width;
    win_height = height;
	win_center = ofVec2f(win_width / 2, win_height / 2);

	// Set the margins to be a quarter of the size of the width/height
	win_margin[0] = win_width / 4;
	win_margin[1] = win_height / 4;
}



////////////////// OPENFRAMEWORKS METHODS \\\\\\\\\\\\\\\\\\\\\
//--------------------------------------------------------------
void Renderer::setup() {
		ofSetBackgroundColor(ofColor::black);
		
		//Set the initial local basis
		computeLocalBasis();

		// Fill the models vector with models
		std::string teapot_path = "C:\\Users\\happy\\source\\repos\\CS126FA19\\fantastic-finale-mbk6\\models\\teapot.obj";
		std::string cube_path = "C:\\Users\\happy\\source\\repos\\CS126FA19\\fantastic-finale-mbk6\\models\\cube.obj";
		std::string tetrahedron_path = "C:\\Users\\happy\\source\\repos\\CS126FA19\\fantastic-finale-mbk6\\models\\tetrahedron.obj";
		std::string sphere_path = "C:\\Users\\happy\\source\\repos\\CS126FA19\\fantastic-finale-mbk6\\models\\sphere.obj";
		
		//Add objects to the scene using new Model3D constructor
		models.push_back(Model3D(sphere_path, ofColor::green, ofVec3f(0, 0, 0), 0.2));
		models.push_back(Model3D(cube_path, ofColor::red, ofVec3f(1, 0, 0), 1));
		models.push_back(Model3D(cube_path, ofColor::yellow, ofVec3f(0, -1, 0), 1));
		models.push_back(Model3D(tetrahedron_path, ofColor::blue, ofVec3f(0, 1, 0), 0.5));
		

}

//--------------------------------------------------------------
void Renderer::update(){
	
	//Get the last frame time in seconds. This will be used to maintain speeds despite an inconsistent framerate
	frame_time = ofGetLastFrameTime();

	models[0].rotate(ofVec3f(1, 1, 1)* frame_time);


	/*
		Movement System:	Standard wsad movement, relative to current view direction.
		wsad will only adjust the camera's x and z coordinates, while space and shift will move it only in the y direction
	*/

	//Generate unit vector in direction of camera position, with only x and z components. Start with an empty vector
	ofVec3f move_direction = ofVec3f(0, 0, 0);
	//Use the local basis to deterimine which way to move. Update the local basis whenever the camera turns

	// WSAD movement
	if (pressed_keys[0]) {
		//Subtract the nonzero part of local_basis[0], since the camera shouldn't move in the y direction
		move_direction += (local_basis[0] - ofVec3f(0, local_basis[0].y, 0));
	}
	if (pressed_keys[1]) {
		//Subtract the nonzero part of local_basis[0], since the camera shouldn't move in the y direction
		move_direction -= (local_basis[0] - ofVec3f(0, local_basis[0].y, 0));
	}
	if (pressed_keys[2]) {
		move_direction -= local_basis[1];
	}
	if (pressed_keys[3]) {
		move_direction += local_basis[1];
	}
	// Vertical movement does not depend on camera position
	if (pressed_keys[4]) {
		move_direction += ofVec3f(0, 1, 0);
	}
	if (pressed_keys[5]) {
		move_direction += ofVec3f(0, -1, 0);
	}
	// Turning
	if (pressed_keys[6]) {
		cam_rot.y += turn_speed * frame_time;
		if (std::abs(cam_rot[1]) > max_vertical_angle) {
			//Change the magnitude of cam_rot[1] back to max_vertical_angle but keep the sign the same
			cam_rot[1] = std::copysign(max_vertical_angle, cam_rot[1]);
		}
		//Update the local basis
		computeLocalBasis();
	}
	if (pressed_keys[7]) {
		cam_rot.y -= turn_speed * frame_time;
		if (std::abs(cam_rot[1]) > max_vertical_angle) {
			//Change the magnitude of cam_rot[1] back to max_vertical_angle but keep the sign the same
			cam_rot[1] = std::copysign(max_vertical_angle, cam_rot[1]);
		}
		//Update the local basis
		computeLocalBasis();
	}
	if (pressed_keys[8]) {
		cam_rot.x -= turn_speed * frame_time;
		//Update the local basis
		computeLocalBasis();
	}
	if (pressed_keys[9]) {
		cam_rot.x += turn_speed * frame_time;
		//Update the local basis
		computeLocalBasis();
	}

	// Update camera position
	cam_pos += move_direction * move_speed * frame_time;

}

//--------------------------------------------------------------
void Renderer::draw() { 
	
	//Draw all "wires"
	ofVec2f point0;
	ofVec2f point1;
	for (Model3D model : models) {
		ofSetColor(model.color);
		for (ofVec2f edge : model.edges) {
			//Transform the two vertices indicated by the currend edge
			point0 = transform(model.vertices[(int)edge.x] + model.position);
			point1 = transform(model.vertices[(int)edge.y] + model.position);

			//Only draw the edge if both points are in bounds
			if (inBounds(point0) && inBounds(point1)) {
				ofDrawLine(point0, point1);
			}
		}
	}


	// OSD - Display frame rate, frame time, camera position/rotation, field of view, and the local basis vectors
	ofSetColor(ofColor::white);
	std::stringstream string_stream;
	string_stream << "fps: " << ofGetFrameRate();
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 10));
	string_stream.str("");
	string_stream << "frame time (s): " << frame_time;
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 20));
	string_stream.str("");
	string_stream << "cam_pos: (" << cam_pos.x<<", "<<cam_pos.y<<", "<<cam_pos.z<<")";
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 30));
	string_stream.str("");
	string_stream << "cam_rot: (" << cam_rot.x << ", " << cam_rot.y << ")";
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 40));
	string_stream.str("");
	string_stream << "fov: " << field_of_view;
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 50));
	string_stream.str("");
	string_stream << "local_basis: (" << local_basis[0].x << ", " << local_basis[0].y << ", " << local_basis[0].z << "), (" << local_basis[1].x << ", " << local_basis[1].y << ", " << local_basis[1].z << "), (" << local_basis[2].x << ", " << local_basis[2].y << ", " << local_basis[2].z << ")";
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 60));
	string_stream.str("");

}

//--------------------------------------------------------------
void Renderer::keyPressed(int key){

	key = tolower(key);

	//Set the corresponding bools in pressed_keys to true
	if (key == 'w') {
		pressed_keys[0] = true;
	}
	if (key == 's') {
		pressed_keys[1] = true;
	}
	if (key == 'a') {
		pressed_keys[2] = true;
	}
	if (key == 'd') {
		pressed_keys[3] = true;
	}
	if (key == ' ') {
		pressed_keys[4] = true;
	}
	if (key == OF_KEY_SHIFT) {
		pressed_keys[5] = true;
	}
	if (key == OF_KEY_UP) {
		pressed_keys[6] = true;
	}
	if (key == OF_KEY_DOWN) {
		pressed_keys[7] = true;
	}
	if (key == OF_KEY_LEFT) {
		pressed_keys[8] = true;
	}
	if (key == OF_KEY_RIGHT) {
		pressed_keys[9] = true;
	}

	// Keys that don't need to be held
	if (key == '=') {
		field_of_view *= 1.1;
	}
	if (key == '-') {
		field_of_view /= 1.1;
	}
	if (key == OF_KEY_ESC) {
		exit();
	}

}

//--------------------------------------------------------------
void Renderer::keyReleased(int key){
	
	key = tolower(key);

	//Set the corresponding bool pressed_keys to false
	if (key == 'w') {
		pressed_keys[0] = false;
	}
	if (key == 's') {
		pressed_keys[1] = false;
	}
	if (key == 'a') {
		pressed_keys[2] = false;
	}
	if (key == 'd') {
		pressed_keys[3] = false;
	}
	if (key == ' ') {
		pressed_keys[4] = false;
	}
	if (key == OF_KEY_SHIFT) {
		pressed_keys[5] = false;
	}
	if (key == OF_KEY_UP) {
		pressed_keys[6] = false;
	}
	if (key == OF_KEY_DOWN) {
		pressed_keys[7] = false;
	}
	if (key == OF_KEY_LEFT) {
		pressed_keys[8] = false;
	}
	if (key == OF_KEY_RIGHT) {
		pressed_keys[9] = false;
	}
}

//--------------------------------------------------------------
void Renderer::mouseMoved(int x, int y ){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::mouseDragged(int x, int y, int button){
	// If left-click, change the camera rotation
	if (button == 0) {
		ofHideCursor();
		ofVec2f current_mouse_pos = ofVec2f(x, -y);

		//Subtract the old mouse position from the current position, and update the old mouse position
		cam_rot += (current_mouse_pos - last_mouse_pos) * mouse_sensitivity * frame_time;
		
		//Limit the camera's vertical rotation
		if (std::abs(cam_rot[1]) > max_vertical_angle) {
			//Change the magnitude of cam_rot[1] back to max_vertical_angle but keep the sign the same
			cam_rot[1] = std::copysign(max_vertical_angle, cam_rot[1]);
		}
		//Update the local basis
		computeLocalBasis();

		last_mouse_pos = current_mouse_pos;
	}

	// If right-click, enter and execute edit mode
	else if (button == 2) {
		
		if (!edit_mode) {
			//Entering edit-mode:	Find the object whose projected center is closest to the mouse
			float min_mouse_dist = std::numeric_limits<float>::max();
			float mouse_dist;
			for (Model3D& model : models) {
				// Compute the distance between the object's projected center and the mouse location, and
				// Point edit_mode_model to the current closest model
				mouse_dist = (ofVec2f(x, y) - transform(model.position)).length();
				//Divide the grab range by the object's distance, since a smaller looking object should have a smaller grab range
				if (mouse_dist < grab_range / (cam_pos - model.position).length() && edit_mode_model_dist < min_mouse_dist) {
					edit_mode_model = &model;
					min_mouse_dist = edit_mode_model_dist;
				}
			}
			if (edit_mode_model != nullptr) {
				// Enter edit mode, set the last mouse position to the current mouse position,
				// calculate the distance from the camera to the selected model, and update the local basis
				edit_mode = true;
				last_mouse_pos = ofVec2f(x, -y);
				edit_mode_model_dist = (cam_pos - edit_mode_model->position).length();
				computeLocalBasis();
			}
			else {
				//If no model was close enough to be selected, do nothing
				return;
			}
		}
		// Hide the mouse, compute how much it has moved, and move the object by a scale of the local basis vectors
		// How much the object should move should increase with its distance from the camera, to preserve
		// the illusion that it's being dragged across the flat screen
		ofHideCursor();
		ofVec2f current_mouse_pos = ofVec2f(x, -y);
		ofVec2f mouse_difference = current_mouse_pos - last_mouse_pos;
		edit_mode_model->position += (mouse_difference.x * local_basis[1] + mouse_difference.y * local_basis[2]) * edit_mode_model_dist * translation_speed;
		last_mouse_pos = current_mouse_pos;
	}
}

//--------------------------------------------------------------
void Renderer::mousePressed(int x, int y, int button){
	//This method is called before mouseDragged(), so its useful to set the last mouse position to the current mouse position
	last_mouse_pos = ofVec2f(x, -y);
}

//--------------------------------------------------------------
void Renderer::mouseReleased(int x, int y, int button){
	ofShowCursor();

	//End edit mode
	edit_mode = false;
	edit_mode_model = nullptr;
}

//--------------------------------------------------------------
void Renderer::mouseScrolled(ofMouseEventArgs& mouse) {
	// If currently in edit mode and right click is held, use the local basis to move the selected model towards or away from the camera
	if (edit_mode && mouse.button == 2) {
		edit_mode_model->position += mouse.scrollY * local_basis[0] * edit_mode_model_dist* translation_speed * 10;
	}
	// Otherwise, change the FOV
	else {
		if (mouse.scrollY > 0) {
			field_of_view *= 1.1;
		}
		else if (mouse.scrollY < 0) {
			field_of_view /= 1.1;
		}
	}

}

//--------------------------------------------------------------
void Renderer::mouseEntered(int x, int y){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::mouseExited(int x, int y){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::windowResized(int w, int h){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::gotMessage(ofMessage msg){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::dragEvent(ofDragInfo dragInfo){ 
	//Unused...for now
}
