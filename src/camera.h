// CAMERA - Defines the Camera class - for containing all state and behavior for viewing a 3D scene

#pragma once

#include "ofMain.h"
#include "model3d.h"

class Camera {

public:

	// Camera parameters
	ofVec3f position;		/* World coordinates of the camera */
	ofVec2f rotation;		/* The horizontal and vertical rotation of the camera in radians */
	float max_vertical_angle;	/* The maximum angle magnitude the user can look up or down (allowing to look straight up or down could cause gimbal lock) */
	float move_speed;		/* Speed of camera movement units per second */
	float turn_speed;		/* Speed of camera rotation in radians per second */
	float mouse_sensitivity;	/* Speed of camera roation when controlled by mouse. Proportional to radians per second (I think) */
	float field_of_view;		/* Controls the camera's field of view (unknown units, maybe proportional to 1 / radians?) */
	float zoom_speed = 1.1;		/* Speed of the camera's zoom feature */
	ofVec3f local_basis[3];		/* Defines unit vectors pointing forward, right, and up respectively. Updated every time the camera turns */

	// Window parameters
	int win_width;			/* Width of the screen */
	int win_height;			/* Height of the screen */
	int win_margin[2];		/* The amount of pixels outside width and height of the screen where lines can still be drawn */
	ofVec2f win_center;		/* Center of the screen */
	ofVec2f out_of_bounds_point;	/* Point to return if intended not to be drawn on the screen */

	//Default Camera constructor
	Camera();

	//Camera constructor
	Camera(ofVec3f position_, ofVec2f rotation_, float max_vertical_angle_, float turn_speed_,
		float move_speed_, float mouse_sensitivity_, float field_of_view_, int win_width_, int win_height_);

	/* Updates the position and rotation of the camera */
	void update(ofVec3f move_direction, ofVec2f rotation_change, bool mouse_turning, float time_interval);

	/* Zooms the camera in or out */
	void zoom(float zoom_scale);

	/* Transforms a point in 3d space to a 2d screen coordinate. This method is the heart of all 3D rendering done in this program */
	ofVec2f transform(ofVec3f point3d);

	/* Rotates two coordinates by a given angle */
	void rotateCoords(float& coord0, float& coord1, float angle);

	/* Returns whether a point is in the bounds of the screen */
	bool inBounds(ofVec2f point2d);

	/* Draws 3D Model on the screen */
	void drawModel(Model3D* model);

	/* Computes a set of three vectors representing a local basis of the current camera position */
	void computeLocalBasis();
};