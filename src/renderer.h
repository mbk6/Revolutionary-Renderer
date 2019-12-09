// RENDERER - Defines the core of the 3d wireframe renderer and its demonstration modes

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"			/* ofxCv and ofxOpenCv external libraries used for the head-controlled camera feature */

#include "physics_body.h"	/* Also includes model3d.h */
#include "plane.h"
#include "camera.h"

#include <vector>
#include <sstream>
#include <cmath>

class Renderer : public ofBaseApp {

private:

	/////////////// PROGRAM VARIABLES \\\\\\\\\\\\\\\\

	////// GUI \\\\\\\
	
	//Main Panel - Contains toggles for various features, and lets the user choose a demo
	ofxPanel main_panel;
	ofxToggle walk_mode_toggle;
	ofxToggle osd_toggle;
	ofxToggle floor_toggle;
	ofxToggle head_control_toggle;
	ofxLabel demos_label;
	ofxButton planets_demo_button;
	ofxButton models_demo_button;
	ofxButton box_demo_button;

	//New planet panel - Provides interface for creating and removing planets in the PLANETS demo
	ofxPanel new_planet_panel;
	ofxColorSlider new_planet_color;
	ofxVec3Slider new_planet_pos;
	ofxVec3Slider new_planet_vel;
	ofxFloatSlider new_planet_mass;
	ofxFloatSlider new_planet_size;
	ofxButton create_planet_button;
	ofxButton delete_planets_button;

	//New model panel - Provides interface for creating and removing models in the MODELS demo
	ofxPanel new_model_panel;
	ofxColorSlider new_model_color;
	ofxVec3Slider new_model_pos;
	ofxFloatSlider new_model_size;
	ofxButton create_model_button;
	ofxButton delete_models_button;

	//Box panel - Provides interface for modifying parameters of the bod in the BOX demo
	ofxPanel box_panel;
	ofxIntSlider box_size_slider;
	ofxIntSlider num_balls_slider;
	ofxButton box_run_button;
	
	//Enumerator representing the current demo mode
	enum DemoMode {
		NONE, PLANETS, MODELS, BOX
	};


	// Head-control parameters		
	ofVideoGrabber webcam;									/* Videograbber object used for webcam input */
	ofxCv::ObjectFinder face_finder;						/* ObjectFinder used to find and track faces */
	ofRectangle face_rect = ofRectangle(-10, -10, 0, 0);	/* The current record of the rectangle surrounding the user's face */

	// Application parameters
	float frame_time = 0;						/* frametime in seconds, updated with every call of the update() method */
	bool edit_mode = false;						/* indicates whether the user is currently manipulating objects in the scene */
	std::vector<Model3D*> scene_models;			/* Collection of all models in the scene */
	const int MAX_MODEL_COUNT = 10;				/* The maximum number of models allowed in the scene */
	DemoMode current_demo = NONE;				/* The current demo mode */

	// Edit Mode parameters
	Model3D* edit_mode_model = nullptr;			/* the current model being manipulated in edit-mode */
	float edit_mode_model_dist = 0;				/* the distance from the camera to the chosen model at the time it was chosen */
	int grab_range = 200;						/* the distance the mouse needs to be from the center of an object in order to move it in edit mode */
	float edit_translation_speed = 0.002;		/* Speed at which objects can be moved with in edit mode (unsure of units) */
	float edit_rotation_speed = 0.01;			/* Speed at which objects can be rotated with in edit mode (unsure of units) */

	// Walk mode parameters
	float floor_height = -1;					/* y coordinate of the floor */
	float player_height = 1;					/* height from the camera to the floor when standing on the floor */
	float jump_speed = 5;						/* speed the user can jump */
	ofVec3f cam_velocity = ofVec3f(0, 0, 0);	/* current velocity of the camera (units/sec) */
	ofVec3f gravity = ofVec3f(0, -10, 0);		/* gravity vector (units/sec/sec) */

	//Standard Scene Objects
	Plane floor = Plane(ofVec3f(0, floor_height, 0), ofVec3f(0, 1, 0), ofColor::gray, 30);	/* The plane shown when "Show Floor" is enabled */

	// Window parameters
	int win_width;								/* width of the screen */
	int win_height;								/* height of the screen */
	Camera camera;								/* Camera object used to view the scene and draw to the screen */
	ofVec2f last_mouse_pos = ofVec2f(-1, -1);	/* last recorded screen coordinates of the mouse */


	// Movement Controls - keys that are held down are represented by booleans turned on and off as they are pressed/released
	//						    w      s      a      d    space  shift   up	   down   left   right
	bool pressed_keys[10] = { false, false, false, false, false, false, false, false, false, false };


public:
    
	//////////////////// RENDERER METHODS \\\\\\\\\\\\\\\\\\\\\

	/* Renderer Constructor */
    Renderer(int width, int height);

	/* Updates the camera position and rotation based on user controls */
	void updateCamera();

	/* Updates all physical interactions between objects in the scene */
	void updatePhysics();

	/* Clears the scene_models vector and deletes all objects in it */
	void clearScene();

	/* Updates the position of the camera for head control */
	void updateHead();

	//////////////////// GUI BUTTON PRESSES \\\\\\\\\\\\\\\\\

	// Demo initializers
	void initPlanetsDemo(); /* Puts a "sun" at the origin with high mass, and a demo set of planets orbiting it */
	void initModelsDemo();  /* Generates three models rendered at once to show off the power of the program */
	void initBoxDemo();		/* Generates a medium-sized box for the box demo */
	// Demo modifiers
	void createNewPlanet(); /* Creates a new planet using parameters from the planet creator panel */
	void deletePlanets();	/* Removes all planets except for the "sun" */
	void createNewModel();	/* Creates a new model using parameters from the model creator panel */


	//////////////////// AUTOGENERATED OPENFRAMEWORKS METHODS \\\\\\\\\\\\\\\\\\\\\
    
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
