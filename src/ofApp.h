#pragma once

#include "ofMain.h"

#include <vector>
#include <sstream>
#include <cmath>

class Renderer : public ofBaseApp {

private:

	/////////////// VARIABLES \\\\\\\\\\\\\\\\

	// Window parameters
	int win_width;
	int win_height;
	ofVec2f win_center;
	ofVec2f last_mouse_pos = ofVec2f(-1, -1);

	// Camera parameters
	ofVec3f cam_pos = ofVec3f(0, 0, 5);
	ofVec2f cam_rot = ofVec2f(0, 0);	//cam_rot.x = left/right view angle, cam_rot.y = up/down view angle
	double move_speed = 0.06;
	double turn_speed = 0.02;
	int field_of_view = 600;

	// Controls
	//						    w      s      a      d    space  shift   up	   down   left   right
	bool pressed_keys[10] = { false, false, false, false, false, false, false, false, false, false };

	//Verticies of a 2x2 cube centered at (0,0,0)
	ofVec3f cube_verts[8] = {
		ofVec3f(-1, -1, -1), ofVec3f(-1, -1, 1), ofVec3f(-1, 1, -1), ofVec3f(-1, 1, 1),
		ofVec3f(1, -1, -1), ofVec3f(1, -1, 1), ofVec3f(1, 1, -1), ofVec3f(1, 1, 1)
	};


	//////////////////// METHODS \\\\\\\\\\\\\\\\\\\\\
	
	/**
		Transforms a point in 3d space to a 2d screen coordinate
	*/
	ofVec2f transform(ofVec3f point3d);
	
	/**
		Rotates two corrdinates by a given angle
	*/
	void rotateCoords(float& coord1, float& coord2, float& angle);

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
