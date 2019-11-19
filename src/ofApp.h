#pragma once

#include "ofMain.h"
#include "model3d.h"

#include <vector>
#include <sstream>
#include <cmath>

class Renderer : public ofBaseApp {

private:

	/////////////// VARIABLES \\\\\\\\\\\\\\\\

	// Window parameters
	int win_width;
	int win_height;
	int win_margin[2]; /* The amount of pixels outside width and height of the screen where lines will still be drawn */
	ofVec2f win_center;
	ofVec2f last_mouse_pos = ofVec2f(-1, -1);

	// Camera parameters
	ofVec3f cam_pos = ofVec3f(0, 0, 5);
	ofVec2f cam_rot = ofVec2f(0, 0);	//cam_rot.x = left/right view angle, cam_rot.y = up/down view angle
	float max_vertical_angle = 1.5;		/* Not quite PI/2: can't allow the user to look exactly upward or downward to prevent gimbal lock */
	double move_speed = 0.06;
	double turn_speed = 0.015;
	int field_of_view = 600;

	// Controls
	//						    w      s      a      d    space  shift   up	   down   left   right
	bool pressed_keys[10] = { false, false, false, false, false, false, false, false, false, false };

	//Vector of 3D models to render in the scene
	std::vector<Model3D> models;

	//Verticies of a 2x2 cube centered at (0,0,0)
	ofVec3f cube_verts[8] = {
		ofVec3f(-1, -1, -1), ofVec3f(-1, -1, 1), ofVec3f(-1, 1, -1), ofVec3f(-1, 1, 1),
		ofVec3f(1, -1, -1), ofVec3f(1, -1, 1), ofVec3f(1, 1, -1), ofVec3f(1, 1, 1)
	};

	//Pairs of verticies for the edges of the cube (indecies of cube_verts that are connected)
	int cube_edges[12][2] = {
		{0, 1}, {1, 5}, {5, 4}, {4, 0}, /*Bottom*/
		{2, 3}, {3, 7}, {7, 6}, {6, 2}, /*Top*/
		{0, 2}, {1, 3}, {5, 7}, {4, 6}  /*Sides*/
	};


	//////////////////// METHODS \\\\\\\\\\\\\\\\\\\\\
	
	/**
		Transforms a point in 3d space to a 2d screen coordinate
	*/
	ofVec2f transform(ofVec3f point3d);
	
	/**
		Rotates two coordinates by a given angle
	*/
	void rotateCoords(float& coord1, float& coord2, float& angle);

	/**
		Returns whether an ofVec2f is in the bounds of the screen
	*/
	bool inBounds(ofVec2f point2d);

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
