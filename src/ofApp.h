#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

#include "physics_body.h"
#include "camera.h"

#include <vector>
#include <sstream>
#include <cmath>

class Renderer : public ofBaseApp {

private:

	/////////////// PROGRAM VARIABLES \\\\\\\\\\\\\\\\

	////// GUI \\\\\\\
	
	//Main
	ofxPanel main_panel;
	ofxToggle walk_mode_toggle;
	ofxToggle osd_toggle;
	ofxToggle floor_toggle;
	ofxToggle head_control_toggle;
	ofxLabel demos_label;
	ofxButton planets_demo_button;
	ofxButton models_demo_button;

	//New planet panel
	ofxPanel new_planet_panel;
	ofxColorSlider new_planet_color;
	ofxVec3Slider new_planet_pos;
	ofxVec3Slider new_planet_vel;
	ofxFloatField new_planet_mass;
	ofxFloatSlider new_planet_size;
	ofxButton create_planet_button;
	ofxButton delete_planets_button;

	//New model panel
	ofxPanel new_model_panel;
	ofxColorSlider new_model_color;
	ofxVec3Slider new_model_pos;
	ofxFloatSlider new_model_size;
	ofxButton create_model_button;
	ofxButton delete_models_button;
	

	enum DemoMode {
		NONE, PLANETS, MODELS
	};




	ofVideoGrabber webcam;						/* Videograbber object used for webcam input */
	ofxCv::ObjectFinder face_finder;			/* Object used to find and track faces */
	float last_area = -1;						/* The area of the tracking rectangle used to calculate changes in face distance */
	ofRectangle face_rect = ofRectangle(-10, -10, 0, 0);

	Camera camera;

	// Application parameters
	float frame_time = 0;						/* frametime in seconds, updated with every call of the update() method */
	bool edit_mode = false;						/* indicates whether the program is in edit-mode*/
	std::vector<Model3D*> scene_models;			/* Collection of models in the scene */
	const int MAX_MODEL_COUNT = 10;				/* The maximum number of models allowed in the scene */
	DemoMode current_demo = NONE;				/* The current demo mode */

	// Edit Mode parameters
	Model3D* edit_mode_model = nullptr;			/* the current model being edited in edit-mode */
	float edit_mode_model_dist = 0;				/* the distance from the camera to the chosen model at the time it was chosen */
	int grab_range = 200;						/* the distance the mouse needs to be from the center of an object in order to move it in edit mode */
	float edit_translation_speed = 0.002;		/* Speed at which objects can be moved with in edit mode (unsure of units) */
	float edit_rotation_speed = 0.01;			/* Speed at which objects can be rotated with in edit mode (unsure of units) */

	// Walk mode parameters
	float floor_height = -1;					/* y coordinate of the floor */
	float player_height = 1;					/* height from the camera to the floor when standing on the floor */
	float jump_speed = 5;						/* speed the user can jump */
	ofVec3f cam_velocity = ofVec3f(0, 0, 0);	/* current velocity of the camera */
	ofVec3f gravity = ofVec3f(0, -10, 0);		/* gravity vector (units/sec) */

	//Standard Scene Objects
	Model3D floor = Model3D("..\\models\\plane_50x50.obj", ofColor::gray, ofVec3f(0, floor_height, 0), 1.0f);

	// Window parameters
	int win_width;								/* width of the screen */
	int win_height;								/* height of the screen */
	ofVec2f last_mouse_pos = ofVec2f(-1, -1);	/* last recorded screen coordinates of the mouse */





	// Controls - keys that are held down are represented by booleans turned on and off as they are pressed/released
	//						    w      s      a      d    space  shift   up	   down   left   right
	bool pressed_keys[10] = { false, false, false, false, false, false, false, false, false, false };



public:
    
	/* Constructor */
    Renderer(int width, int height);

	//////////////////// RENDERER METHODS \\\\\\\\\\\\\\\\\\\\\



	/* Updates the camera position and rotation based on user controls */
	void updateCamera();

	/* Updates all physical interactions between objects in the scene */
	void updatePhysics();

	/* Clears the scene_models vector and deletes all objects in it */
	void clearScene();

	/* Updates the position of the tracked head for head control */
	void updateHead();

	//////////////////// GUI BUTTON PRESSES \\\\\\\\\\\\\\\\\

	void initPlanetsDemo();
	void initModelsDemo();
	void createNewPlanet();
	void deletePlanets();
	void createNewModel();


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
