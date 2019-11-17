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

	//Compute perspective scaling value and scale x and y
	z = field_of_view / z;
	x *= z;
	y *= z;

	//Adjust point with respect to the screen center. subtract from x-center and add to y-center to maintain proper coordinate system
	return ofVec2f(win_center.x - x, win_center.y + y);
}

void Renderer::rotateCoords(float& coord1, float& coord2, float& angle) {
	float temp = coord1;
	coord1 = temp * std::cos(angle) + coord2 * std::sin(angle);
	coord2 = coord2 * std::cos(angle) - temp * std::sin(angle);
}



Renderer::Renderer(int width, int height) {
	win_width = width;
    win_height = height;
	win_center = ofVec2f(win_width / 2, win_height / 2);
}

//--------------------------------------------------------------
void Renderer::setup(){
		ofSetBackgroundColor(ofColor::black);
}

//--------------------------------------------------------------
void Renderer::update(){
	
	/*
		Movement System:	Standard wsad movement, relative to current view direction. wsad will only adjust the player's x and z coordinates
	*/

	//Generate unit vector in direction of camera position, with only x and z components
	ofVec3f move_direction;

	//Update camera position based on held keys
	if (pressed_keys[0]) {
		move_direction = ofVec3f(std::sin(cam_rot.x), 0, -1 * std::cos(cam_rot.x));
	}
	if (pressed_keys[1]) {
		move_direction = -1 * ofVec3f(std::sin(cam_rot.x), 0, -1 * std::cos(cam_rot.x));
	}
	if (pressed_keys[2]) {
		move_direction = -1 * ofVec3f(std::cos(cam_rot.x), 0, std::sin(cam_rot.x));
	}
	if (pressed_keys[3]) {
		move_direction = ofVec3f(std::cos(cam_rot.x), 0, std::sin(cam_rot.x));
	}
	//Vertical movement does not depend on camera position
	if (pressed_keys[4]) {
		move_direction = ofVec3f(0, 1, 0);
	}
	if (pressed_keys[5]) {
		move_direction = ofVec3f(0, -1, 0);
	}
	if (pressed_keys[6]) {
		cam_rot.y += turn_speed;
	}
	if (pressed_keys[7]) {
		cam_rot.y -= turn_speed;
	}
	if (pressed_keys[8]) {
		cam_rot.x -= turn_speed;
	}
	if (pressed_keys[9]) {
		cam_rot.x += turn_speed;
	}

	cam_pos += move_direction * move_speed;

}

//--------------------------------------------------------------
void Renderer::draw(){ 
	for (ofVec3f vertex : cube_verts) {
		ofDrawCircle(transform(vertex), 5);
	}

	std::stringstream string_stream;
	string_stream << "cam_pos: (" << cam_pos.x<<", "<<cam_pos.y<<", "<<cam_pos.z<<")";
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 10));
	string_stream.str("");
	string_stream << "cam_rot: (" << cam_rot.x << ", " << cam_rot.y << ")";
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 20));
	string_stream.str("");
}

//--------------------------------------------------------------
void Renderer::keyPressed(int key){

	key = tolower(key);

	//Set the corresponding bool pressed_keys to true
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

}

//--------------------------------------------------------------
void Renderer::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void Renderer::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void Renderer::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void Renderer::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void Renderer::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void Renderer::windowResized(int w, int h){

}

//--------------------------------------------------------------
void Renderer::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void Renderer::dragEvent(ofDragInfo dragInfo){ 

}
