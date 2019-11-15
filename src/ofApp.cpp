#include "ofApp.h"

Renderer::Renderer(int width, int height) {
	win_width = width;
    win_height = height;
}

//--------------------------------------------------------------
void Renderer::setup(){

}

//--------------------------------------------------------------
void Renderer::update(){
	
}

//--------------------------------------------------------------
void Renderer::draw(){ 
	ofDrawCircle(100, 100, 100);
    ofDrawLine(50, 50, 600, 700);
    ofDrawRectangle(ofVec2f(300, 350), 40, 200);
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
