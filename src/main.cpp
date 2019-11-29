#include "ofMain.h"
#include "ofApp.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;
    //========================================================================
int main( ){
	ofSetupOpenGL(WIDTH,HEIGHT,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new Renderer(WIDTH, HEIGHT));

}
