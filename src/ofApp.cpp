#include "ofApp.h"



ofVec2f Renderer::transform(ofVec3f point3d) {
	// 3d transformation equations from https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point
	// and https://www.youtube.com/watch?v=g4E9iq0BixA

	double x = point3d.x;
	double y = point3d.y;
	double z = point3d.z;

	//Offset z value to make point visible
	z -= 5;

	//Compute perspective scaling value and scale x and y
	z = 500 / z;
	x *= z;
	y *= z;

	return ofVec2f(win_center.x + x, win_center.y + y);
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
	
}

//--------------------------------------------------------------
void Renderer::draw(){ 
	for (ofVec3f vertex : cube_verts) {
		ofDrawCircle(transform(vertex), 5);
	}
}

//--------------------------------------------------------------
void Renderer::keyPressed(int key){

}

//--------------------------------------------------------------
void Renderer::keyReleased(int key){

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
