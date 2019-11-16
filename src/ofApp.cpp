#include "ofApp.h"



ofVec2f Renderer::transform(ofVec3f point3d) {
	// 3d transformation equations from https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point
	// and https://www.youtube.com/watch?v=g4E9iq0BixA

	//Recenter points based on camera position
	float x = point3d.x - cam_pos.x;
	float y = point3d.y - cam_pos.y;
	float z = point3d.z - cam_pos.z;

	//Compute perspective scaling value and scale x and y
	z = field_of_view / z;
	x *= z;
	y *= z;

	//Adjust point with respect to the screen center. subtract from x-center and add to y-center to maintain proper coordinate system
	return ofVec2f(win_center.x - x, win_center.y + y);
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
	
	//Update camera position based on held keys
	if (pressed_keys[0]) {
		cam_pos.z -= cam_speed;
	}
	if (pressed_keys[1]) {
		cam_pos.z += cam_speed;
	}
	if (pressed_keys[2]) {
		cam_pos.x -= cam_speed;
	}
	if (pressed_keys[3]) {
		cam_pos.x += cam_speed;
	}
	if (pressed_keys[4]) {
		cam_pos.y += cam_speed;
	}
	if (pressed_keys[5]) {
		cam_pos.y -= cam_speed;
	}



}

//--------------------------------------------------------------
void Renderer::draw(){ 
	for (ofVec3f vertex : cube_verts) {
		ofDrawCircle(transform(vertex), 5);
	}

	std::stringstream string_stream;
	string_stream << "cam_pos: (" << cam_pos.x<<", "<<cam_pos.y<<", "<<cam_pos.z<<")";
	ofDrawBitmapString(string_stream.str(), ofVec2f(10, 10));
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
