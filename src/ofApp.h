#pragma once

#include "ofMain.h"
#include "model3d.h"

#include <vector>
#include <sstream>
#include <cmath>

class Renderer : public ofBaseApp {

private:

	/////////////// PROGRAM VARIABLES \\\\\\\\\\\\\\\\

	// Application Parameters
	float frame_time = 0;						/* frametime in seconds, updated with every call of the update() method */
	bool edit_mode = false;						/* indicates whether the program is in edit-mode*/
	std::vector<Model3D> models;				/* Collection of models in the scene */
	Model3D* edit_mode_model = nullptr;			/* the current model being edited in edit-mode */
	float edit_mode_model_dist;					/* the distance from the camera to the chosen model at the time it was chosen */
	int grab_range = 100;						/* the distance the mouse needs to be from the center of an object in order to move it in edit mode */

	// Window parameters
	int win_width;								/* width of the screen */
	int win_height;								/* height of the screen */
	int win_margin[2];							/* The amount of pixels outside width and height of the screen where lines will still be drawn */
	ofVec2f win_center;							/* center of the screen */
	ofVec2f last_mouse_pos = ofVec2f(-1, -1);	/* last recorded screen coordinates of the mouse */

	// Camera parameters
	ofVec3f cam_pos = ofVec3f(0, 0, 5);			/* Word coordinates of the camera */
	ofVec2f cam_rot = ofVec2f(0, 0);			/* The horizontal and vertical rotation of the camera in radians */
	float max_vertical_angle = 1.5;				/* The maximum angle magnitude the user can look up or down */
	float move_speed = 3;						/* Speed of camera movement units per second */
	float turn_speed = 1;						/* Speed of camera rotation in radians per second */
	float mouse_sensitivity = 0.7;				/* Speed of camera roation when controlled by mouse. Proportional to radians per second (I think) */
	float field_of_view = 600;					/* Controls the camera's field of view (unknown units, maybe proportional to 1 / radians?) */
	ofVec3f local_basis[3];						/* Defines unit vectors pointing forward, right, and up respectively. Updated when entering edit mode */
	float translation_speed = 0.002;			/* Speed at which objects can be moved with in edit mode (unsure of units) */

	// Controls - keys that are held down are represented by booleans turned on and off as they are pressed/released
	//						    w      s      a      d    space  shift   up	   down   left   right
	bool pressed_keys[10] = { false, false, false, false, false, false, false, false, false, false };



	//////////////////// RENDERER METHODS \\\\\\\\\\\\\\\\\\\\\
	
	/* Transforms a point in 3d space to a 2d screen coordinate */
	ofVec2f transform(ofVec3f point3d);
	
	/* Rotates two coordinates by a given angle */
	void rotateCoords(float& coord1, float& coord2, float& angle);

	/* Returns whether an ofVec2f is in the bounds of the screen */
	bool inBounds(ofVec2f point2d);

	/* Computes a set of three vectors representing a local basis of the current camera position */
	void computeLocalBasis();

public:
    
	/* Constructor passes in window size */
    Renderer(int width, int height);

	//////////////////// OPENFRAMEWORKS METHODS \\\\\\\\\\\\\\\\\\\\\
    
	void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
	void mouseScrolled(ofMouseEventArgs& mouse);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

};
