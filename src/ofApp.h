#pragma once

#include "ofMain.h"

#include <vector>

class Renderer : public ofBaseApp {

private:
	// Window parameters
	int win_width;
	int win_height;
	ofVec2f win_center;

	// Camera parameters
	ofVec3f cam_pos = ofVec3f(0, 0, 5);
	int field_of_view = 600;


	//Verticies of a 2x2 cube centered at (0,0,0)
	ofVec3f cube_verts[8] = {
		ofVec3f(-1, -1, -1), ofVec3f(-1, -1, 1), ofVec3f(-1, 1, -1), ofVec3f(-1, 1, 1),
		ofVec3f(1, -1, -1), ofVec3f(1, -1, 1), ofVec3f(1, 1, -1), ofVec3f(1, 1, 1)
	};

	/**
		Transforms a point in 3d space to a 2d screen coordinate
	*/
	ofVec2f transform(ofVec3f point3d);

public:
    // Constructor passes in window size
    Renderer(int width, int height);

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

};
