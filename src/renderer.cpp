#include "renderer.h"

/////////////// RENDERER METHODS \\\\\\\\\\\\\\\\\

void Renderer::updateCamera() {
	/*
	Movement System:
	Standard WSAD/SHIFT/SPACE movement relative to current view direction.
	wsad will only adjust the camera's x and z coordinates, while space and shift will move it only in the y direction.
	Check the corresponding element of the pressed_keys array to see if a key is currently pressed.
	Use the local basis to deterimine which way to move, and update it whenever the camera turns.
	*/

	//Generate unit vector in direction of camera position, with only x and z components. Start with an empty 3D vector
	ofVec3f move_direction = ofVec3f(0, 0, 0);
	//Generate a vector representing the direction of camera rotation. Start with an empty 2D vector
	ofVec2f rotation_change = ofVec2f(0, 0);

	// W
	if (pressed_keys[0]) {
		//Subtract off the y compoment of camera.local_basis[0], since the camera shouldn't move in the y direction
		move_direction += (camera.local_basis[0] - ofVec3f(0, camera.local_basis[0].y, 0));
	}
	// S
	if (pressed_keys[1]) {
		//Subtract off the y component of camera.local_basis[0], since the camera shouldn't move in the y direction
		move_direction -= (camera.local_basis[0] - ofVec3f(0, camera.local_basis[0].y, 0));
	}
	// A
	if (pressed_keys[2]) {
		move_direction -= camera.local_basis[1];
	}
	// D
	if (pressed_keys[3]) {
		move_direction += camera.local_basis[1];
	}

	// Vertical movement does not depend on camera orientation. Vertical movement besides jumping is not enabled in walk mode
	// SPACE
	if (pressed_keys[4]) {
		if (!walk_mode_toggle) {
			move_direction += ofVec3f(0, 1, 0);
		}
	}
	// SHIFT
	if (pressed_keys[5]) {
		if (!walk_mode_toggle) {
			move_direction += ofVec3f(0, -1, 0);
		}
	}
	// Turning
	// UP
	if (pressed_keys[6]) {
		rotation_change += ofVec2f(0, 1);
	}
	// DOWN
	if (pressed_keys[7]) {
		rotation_change += ofVec2f(0, -1);
	}
	// LEFT
	if (pressed_keys[8]) {
		rotation_change += ofVec2f(-1, 0);
	}
	// RIGHT
	if (pressed_keys[9]) {
		rotation_change += ofVec2f(1, 0);
	}

	// Update camera position
	camera.update(move_direction, rotation_change, false, frame_time);


	// Walk mode
	if (walk_mode_toggle) {
		// Accelerate the camera downwards if the user isn't currently on the "floor"
		if (camera.position.y >= floor_height + player_height) {
			cam_velocity += gravity * frame_time;
			camera.update(cam_velocity, ofVec3f(), false, frame_time);
		}
		// If the camera is on the floor, keep it there
		if (camera.position.y < floor_height + player_height) {
			camera.position.y = floor_height + player_height;
			cam_velocity.y = 0;
		}
	}
}

void Renderer::updatePhysics() {

	//If the frame time is too large, don't update anything. This keeps large chaotic velocities from breaking the program
	if (frame_time <= 0.2) {

		
		if (scene_models.size() > 0) {
			//Exert gravity and handle collision between every two PhysicsBodies in the scene
			for (int i = 0; i < scene_models.size() - 1; i++) {
				if (PhysicsBody* body0 = dynamic_cast<PhysicsBody*>(scene_models[i])) {
					for (int j = i + 1; j < scene_models.size(); j++) {
						if (PhysicsBody* body1 = dynamic_cast<PhysicsBody*>(scene_models[j])) {
							if (current_demo == PLANETS) {
								body0->gravitateWith(body1);
							}
							body0->collideWith(body1);
						}
					}
				}
			}
			//Handle collisions with planes
			for (int i = 0; i < scene_models.size(); i++) {
				if (PhysicsBody* body0 = dynamic_cast<PhysicsBody*>(scene_models[i])) {
					for (int j = 0; j < scene_models.size(); j++) {
						if (Plane* plane = dynamic_cast<Plane*>(scene_models[j])) {
							body0->collideWith(plane);
						}
					}
				}
			}
		}

		//Update bodies and reset all force vectors
		for (Model3D* model : scene_models) {
			if (PhysicsBody* body = dynamic_cast<PhysicsBody*>(model)) {
				//If in edit mode, don't move the object, so that it can still be "grabbed"
				if (edit_mode_model != model) {
					body->update(frame_time);
				}
				body->force = ofVec3f(0, 0, 0);
			}
		}
	}
}

void Renderer::clearScene() {
	// Delete everything in scene_models
	for (int i = 0; i < scene_models.size(); i++) {
		delete scene_models[i];
	}
	scene_models.clear();
}

void Renderer::updateHead() {

	//Only enable tracking if one face is present
	if(face_finder.size() == 1) {
		
		//Use the velocity of a tracked face to move the camera left, right, up, and down
		//Use changes in the area of the rectangle returned by the object finder to move the camera forward and backward
		
		ofRectangle new_face_rect = face_finder.getObjectSmoothed(0);

		//If face_rect is its default value, skip the rest of this method, since it relies on the previously recorded face_rect
		if (face_rect.x < 0) {
			face_rect = new_face_rect;
			return;
		}

		float area_difference = new_face_rect.getArea() - face_rect.getArea();
		ofVec2f face_velocity = (new_face_rect.position - face_rect.position) / frame_time;

		//Only update the camera position if the change in face position is significant enough
		if (face_velocity.length() > 1) {
			face_velocity *= -1;
			//Use the local basis to move the camera
			camera.position += (face_velocity.x * camera.local_basis[1] + face_velocity.y * camera.local_basis[2]) * frame_time * 0.01;
		}
		if (std::abs(area_difference) > 1) {
			//Use the local basis to move the camera
			camera.position += area_difference * camera.local_basis[0] * frame_time * 0.005;
		}

		//Update the face rectangle record
		face_rect = new_face_rect;
	}
}

Renderer::Renderer(int width, int height) {
	win_width = width;
    win_height = height;
}


///////////////// GUI BUTTON PRESSES \\\\\\\\\\\\\\\\\\\\\\\\\\\

void Renderer::initPlanetsDemo() {
	//Clear models and add demo planet set
	current_demo = PLANETS;
	clearScene();
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::white, 100, ofVec3f(10, 0, 0), ofVec3f(0, 5, 0), ofVec3f(0.5, -0.5, 0.5), 0.1)); /* "Planet" */
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::green, 200, ofVec3f(0, 0, 8), ofVec3f(0, -5, 0), ofVec3f(-0.5, -0.5, 0.5), 0.12)); /* "Planet" */
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::blue, 150, ofVec3f(6, 0, 6), ofVec3f(0, -5, 0), ofVec3f(-0.5, -0.5, 0.5), 0.2)); /* "Planet" */
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::red, 100, ofVec3f(0, 0, -10), ofVec3f(4, 0, 0), ofVec3f(-0.5, -0.5, 0.5), 0.1)); /* "Planet" */
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::yellow, 300000, ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0.2)); /* "Sun" */
}

void Renderer::initModelsDemo() {
	//Clear models and add demo models set
	current_demo = MODELS;
	clearScene();
	scene_models.push_back(new Model3D("..\\models\\cow.obj", ofColor::white, ofVec3f(0, 0, -1), 0.2));
	scene_models.push_back(new Model3D("..\\models\\teapot.obj", ofColor::lightBlue, ofVec3f(2, 0, 0), 0.4));
	scene_models.push_back(new Model3D("..\\models\\cube.obj", ofColor::green, ofVec3f(-2, 0, 0), 1));

}

void Renderer::initBoxDemo() {
	//Clear the scene and add the demo box configuration
	current_demo = BOX;
	clearScene();

	//Where to put the walls depends on the size
	int wall_offset = (box_size_slider+1) / 2;

	//Add floor, walls, and ceiling
	scene_models.push_back(new Plane(ofVec3f(0, -wall_offset, 0), ofVec3f(0, 1, 0), ofColor::gray, 2*((box_size_slider+1)/2) + 1));
	scene_models.push_back(new Plane(ofVec3f(0, wall_offset, 0), ofVec3f(0, -1, 0), ofColor::gray, 2*((box_size_slider+1)/2) + 1));
	scene_models.push_back(new Plane(ofVec3f(wall_offset, 0, 0), ofVec3f(-1, 0, 0), ofColor::gray, 2*((box_size_slider+1)/2) + 1));
	scene_models.push_back(new Plane(ofVec3f(-wall_offset, 0, 0), ofVec3f(1, 0, 0), ofColor::gray, 2*((box_size_slider+1)/2) + 1));
	scene_models.push_back(new Plane(ofVec3f(0, 0, wall_offset), ofVec3f(0, 0, -1), ofColor::gray, 2*((box_size_slider+1)/2) + 1));
	scene_models.push_back(new Plane(ofVec3f(0, 0, -wall_offset), ofVec3f(0, 0, 1), ofColor::gray, 2*((box_size_slider+1)/2) + 1));


	//Add random balls with random velocities
	
	float pos_bound = 0.4 * box_size_slider; /* Possible positions depend on the size of the box */
	float vel_bound = 1.5*pos_bound;		 /* Possible velocities depend on the size of the box */

	ofVec3f new_ball_position;
	ofVec3f new_ball_velocity;
	ofVec3f new_ball_angular_vel;
	ofColor new_ball_color;
	float new_ball_mass;
	float new_ball_size;
	
	for (int i = 0; i < num_balls_slider; i++) {

	//Random float generation method from https://stackoverflow.com/questions/686353/random-float-number-generation

		new_ball_position.x =  -pos_bound + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (2*pos_bound)));
		new_ball_position.y = -pos_bound + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (2 * pos_bound)));
		new_ball_position.z = -pos_bound + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (2 * pos_bound)));
		
		new_ball_velocity.x = -vel_bound + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (2*vel_bound)));
		new_ball_velocity.y = -vel_bound + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (2*vel_bound)));
		new_ball_velocity.z = -vel_bound + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (2*vel_bound)));
		
		new_ball_angular_vel.x = -2 + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (4)));
		new_ball_angular_vel.y = -2 + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (4)));
		new_ball_angular_vel.z = -2 + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (4)));
		
		new_ball_color.r = 100 + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (155)));
		new_ball_color.g = 100 + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (155)));
		new_ball_color.b = 100 + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (155)));

		new_ball_mass = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20)));

		//Size of balls depends on size of box
		float size = (0.05 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5)))) * box_size_slider / 20;

		scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", new_ball_color, new_ball_mass, new_ball_position, new_ball_velocity, new_ball_angular_vel, size));
	}

}

void Renderer::createNewPlanet() {
	//Create a new planet if there aren't too many already
	if (scene_models.size() < MAX_MODEL_COUNT) {
		scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", (ofColor)new_planet_color, (float)new_planet_mass, (ofVec3f)new_planet_pos, (ofVec3f)new_planet_vel, ofVec3f(), (float)new_planet_size));
	}
}

void Renderer::deletePlanets() {
	//Clear the scene, then add the "sun" back
	clearScene();
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::yellow, 300000, ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0.2)); /* "Sun" */
}

void Renderer::createNewModel() {
	//Create a new model with a file dialog if there aren't too many already
	if (scene_models.size() < MAX_MODEL_COUNT) {
		//Based on instructions at https://openframeworks.cc/documentation/utils/ofSystemUtils/#show_ofSystemLoadDialog
		ofFileDialogResult read_file = ofSystemLoadDialog("Choose File");
		if (read_file.bSuccess) {
			scene_models.push_back(new Model3D(read_file.getPath(), (ofColor)new_model_color, (ofVec3f)new_model_pos, (float)new_model_size));
		}
	}
}


////////////////// AUTOGENERATED OPENFRAMEWORKS METHODS \\\\\\\\\\\\\\\\\\\\\
//--------------------------------------------------------------
void Renderer::setup() {

	//Set window title
	ofSetWindowTitle("Revolutionary Renderer");

	//Seed random number generator
	std::srand(static_cast <unsigned> (time(0)));
	
	//Initialize the webcam
	webcam.setup(1024, 576);

	//Initialize th camera
	camera = Camera(ofVec3f(0, 0, 5), ofVec2f(0, 0), 1.5f, 1.0f, 3.0f, 0.5f, 600.0f, win_width, win_height);

	//Initialize the background
	ofSetBackgroundColor(ofColor::black);

	//Initialize the face tracker. Face tracking code based on examples provided with the ofxCv addon
	face_finder.setup("haarcascade_frontalface_default.xml");
	face_finder.setPreset(ofxCv::ObjectFinder::Fast);
	face_finder.getTracker().setSmoothingRate(.2);
	

	//////SETUP GUI\\\\\\\\

	//Main Panel
	main_panel.setup();
	main_panel.setPosition(0, 70);
	main_panel.add(walk_mode_toggle.setup("Walk Mode", false));
	main_panel.add(osd_toggle.setup("Show OSD", false));
	main_panel.add(floor_toggle.setup("Show floor", false));
	main_panel.add(head_control_toggle.setup("Head Control", false));
	main_panel.add(demos_label.setup("Demos", ""));
	main_panel.add(models_demo_button.setup("Models"));
	main_panel.add(planets_demo_button.setup("Planets"));
	main_panel.add(box_demo_button.setup("Box"));

	//New Planet Panel
	new_planet_panel.setup();
	new_planet_panel.setName("Create Planet");
	new_planet_panel.setPosition(win_width - new_planet_panel.getWidth() - 5, 0);
	new_planet_panel.add(new_planet_color.setup("Color", ofColor::green, ofColor::black, ofColor::white));
	new_planet_panel.add(new_planet_pos.setup("Position", ofVec3f(), ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10)));
	new_planet_panel.add(new_planet_vel.setup("Velocity", ofVec3f(), ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10)));
	new_planet_panel.add(new_planet_mass.setup("Mass:", 100, 100, 100000));
	new_planet_panel.add(new_planet_size.setup(0.1));
	new_planet_panel.add(create_planet_button.setup("Create Planet"));
	new_planet_panel.add(delete_planets_button.setup("Reset"));

	//New Model Panel
	new_model_panel.setup();
	new_model_panel.setName("Add Model to Scene");
	new_model_panel.setPosition(win_width - new_planet_panel.getWidth() - 5, 0);
	new_model_panel.add(new_model_color.setup("Color", ofColor::white, ofColor::black, ofColor::white));
	new_model_panel.add(new_model_pos.setup("Position", ofVec3f(), ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10)));
	new_model_panel.add(new_model_size.setup("Size Scale", 1, 0, 2));
	new_model_panel.add(create_model_button.setup("Create Model"));
	new_model_panel.add(delete_models_button.setup("Remove All"));

	//Box panel
	box_panel.setup();
	box_panel.setName("Box Parameters");
	box_panel.setPosition(win_width - box_panel.getWidth() - 5, 0);
	box_panel.add(box_size_slider.setup("Box Size", 20, 5, 40));
	box_panel.add(num_balls_slider.setup("Number of Balls", 20, 5, 40));
	box_panel.add(box_run_button.setup("Rerun"));



	//Button Listeners
	planets_demo_button.addListener(this, &Renderer::initPlanetsDemo);
	models_demo_button.addListener(this, &Renderer::initModelsDemo);
	box_demo_button.addListener(this, &Renderer::initBoxDemo);
	create_planet_button.addListener(this, &Renderer::createNewPlanet);
	delete_planets_button.addListener(this, &Renderer::deletePlanets);
	create_model_button.addListener(this, &Renderer::createNewModel);
	delete_models_button.addListener(this, &Renderer::clearScene);
	box_run_button.addListener(this, &Renderer::initBoxDemo);

}

//--------------------------------------------------------------
void Renderer::update(){

	//Get the last frame time in seconds. This will be used to maintain speeds regardless of framerate consistency
	frame_time = ofGetLastFrameTime();

	//Update the camera position and rotation
	updateCamera();

	//Update all physical interactions in the scene
	updatePhysics();

	//Update face tracking if its enabled
	if (head_control_toggle) {
		webcam.update();
		if (webcam.isFrameNew()) {
			face_finder.update(webcam);
		}
		updateHead();
	}
}

//--------------------------------------------------------------
void Renderer::draw() { 
	//Draw the floor if enabled
	if (floor_toggle) {
		camera.drawModel(&floor);
	}

	//Draw all scene models
	for (Model3D* model : scene_models) {
		camera.drawModel(model);
	}

	//Draw the GUI
	main_panel.draw();
	if (current_demo == PLANETS) {
		new_planet_panel.draw();
	}
	else if (current_demo == MODELS) {
		new_model_panel.draw();
	}
	else if (current_demo == BOX) {
		box_panel.draw();
	}

	// Draw the OSD - Display frame rate, frame time, camera position/rotation, field of view, and the local basis vectors
	if (osd_toggle) {
		ofSetColor(ofColor::white);
		ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()) , ofVec2f(10, 10));
		ofDrawBitmapString("frame time (s): " + ofToString(frame_time) , ofVec2f(10, 20));
		ofDrawBitmapString("camera.position: (" + ofToString(camera.position.x) + ", " + ofToString(camera.position.y) + ", " + ofToString(camera.position.z) + ")", ofVec2f(10, 30));
		ofDrawBitmapString("camera.rotation: (" + ofToString(camera.rotation.x) + ", " + ofToString(camera.rotation.y) + ")" , ofVec2f(10, 40));
		ofDrawBitmapString("fov: " + ofToString(camera.field_of_view), ofVec2f(10, 50));
		ofDrawBitmapString("camera.local_basis: (" + ofToString(camera.local_basis[0].x) + ", " + ofToString(camera.local_basis[0].y) + ", " + ofToString(camera.local_basis[0].z)
			+ "), (" + ofToString(camera.local_basis[1].x) + ", " + ofToString(camera.local_basis[1].y)	+ ", " + ofToString(camera.local_basis[1].z)
			+ "), (" + ofToString(camera.local_basis[2].x) + ", " + ofToString(camera.local_basis[2].y) + ", " + ofToString(camera.local_basis[2].z) + ")", ofVec2f(10, 60));
	}

}

//--------------------------------------------------------------
void Renderer::keyPressed(int key){

	key = tolower(key);

	//Set the corresponding bools in pressed_keys to true
	if (key == 'w') {
		pressed_keys[0] = true;
	}
	if (key == 's') {
		pressed_keys[1] = true;
	}
	if (key == 'a') {
		pressed_keys[2] = true;
	}
	if (key == 'd') {
		pressed_keys[3] = true;
	}
	if (key == ' ') {
		pressed_keys[4] = true;
		
		// In walk mode, let the player jump
		if (walk_mode_toggle) {
			cam_velocity.y += jump_speed;
		}
	}
	if (key == OF_KEY_SHIFT) {
		pressed_keys[5] = true;
	}
	if (key == OF_KEY_UP) {
		pressed_keys[6] = true;
	}
	if (key == OF_KEY_DOWN) {
		pressed_keys[7] = true;
	}
	if (key == OF_KEY_LEFT) {
		pressed_keys[8] = true;
	}
	if (key == OF_KEY_RIGHT) {
		pressed_keys[9] = true;
	}

	// Keys that don't need to be held
	if (key == '=') {
		camera.field_of_view *= 1.1;
	}
	if (key == '-') {
		camera.field_of_view /= 1.1;
	}
	if (key == OF_KEY_ESC) {
		exit();
	}
}

//--------------------------------------------------------------
void Renderer::keyReleased(int key){
	
	key = tolower(key);

	//Set the corresponding bool pressed_keys to false
	if (key == 'w') {
		pressed_keys[0] = false;
	}
	if (key == 's') {
		pressed_keys[1] = false;
	}
	if (key == 'a') {
		pressed_keys[2] = false;
	}
	if (key == 'd') {
		pressed_keys[3] = false;
	}
	if (key == ' ') {
		pressed_keys[4] = false;
	}
	if (key == OF_KEY_SHIFT) {
		pressed_keys[5] = false;
	}
	if (key == OF_KEY_UP) {
		pressed_keys[6] = false;
	}
	if (key == OF_KEY_DOWN) {
		pressed_keys[7] = false;
	}
	if (key == OF_KEY_LEFT) {
		pressed_keys[8] = false;
	}
	if (key == OF_KEY_RIGHT) {
		pressed_keys[9] = false;
	}
}

//--------------------------------------------------------------
void Renderer::mouseMoved(int x, int y ){
	//Unused autogenerated method
}

//--------------------------------------------------------------
void Renderer::mouseDragged(int x, int y, int button) {
	// If left-click, change the camera rotation
	if (button == 0) {
		ofVec2f current_mouse_pos = ofVec2f(x, -y);

		//Turn the camera using the mouse movement
		camera.update(ofVec3f(), (current_mouse_pos - last_mouse_pos), true, frame_time);
		
		//Subtract the old mouse position from the current position, and update the old mouse position
		last_mouse_pos = current_mouse_pos;
	}

	// If right-click, enter and execute edit mode
	else if (button == 2) {

		if (!edit_mode) {
			//Entering edit-mode:	Find the object whose projected center is closest to the mouse
			float min_mouse_dist = std::numeric_limits<float>::max();
			float mouse_dist;
			for (Model3D* model : scene_models) {
				// Compute the distance between the object's projected center and the mouse location, and
				// point edit_mode_model to the current closest model
				mouse_dist = (ofVec2f(x, y) - camera.transform(model->position)).length();
				//Divide the grab range by the object's distance, since a smaller looking object should have a smaller grab range
				if (mouse_dist < grab_range / (camera.position - model->position).length() && edit_mode_model_dist < min_mouse_dist) {
					edit_mode_model = model;
					min_mouse_dist = edit_mode_model_dist;
				}
			}
			//Now edit_mode_model points to whatever object, if any, is within grab range and whose projected center is closest to the mouse.
			if (edit_mode_model != nullptr) {
				// Enter edit mode, set the last mouse position to the current mouse position,
				// calculate the distance from the camera to the selected model
				edit_mode = true;
				last_mouse_pos = ofVec2f(x, -y);
				edit_mode_model_dist = (camera.position - edit_mode_model->position).length();
			}
			else {
				//If no model was close enough to be selected, do nothing
				return;
			}
		}
		// Hide the mouse, compute how much it has moved, and move the object by a scale of the local basis vectors determined 
		// using that movement. How much the object moves with the mouse should increase with its distance from the camera, to preserve 
		// the illusion that it's being dragged across the flat screen
		ofHideCursor();
		ofVec2f current_mouse_pos = ofVec2f(x, -y);
		ofVec2f mouse_difference = current_mouse_pos - last_mouse_pos;
		ofVec3f position_change = (mouse_difference.x * camera.local_basis[1] + mouse_difference.y * camera.local_basis[2]) * edit_mode_model_dist * edit_translation_speed;
		edit_mode_model->position += position_change;
		//If the model is a PhysicsBody, give it the velocity determined by the mouse
		if (PhysicsBody* body = dynamic_cast<PhysicsBody*>(edit_mode_model)) {
			//...But only control the body's velocity with the mouse if it's large enough, otherwise set it to zero
			if ((last_mouse_pos - current_mouse_pos).length() > 1) {
				body->velocity = position_change / frame_time;
			}
			else {
				body->velocity = ofVec3f(0, 0, 0);
			}
		}
		last_mouse_pos = current_mouse_pos;
	
	}
	// If middle-click, rotate the selected object
	else if (edit_mode && button == 1) {
		ofVec2f current_mouse_pos = ofVec2f(x, -y);
		ofVec2f mouse_difference = current_mouse_pos - last_mouse_pos;

		//Use multiples of the rightward and upward local basis vectors to rotate the selected object
		ofVec3f rotation_vector = (mouse_difference.x * camera.local_basis[2] - mouse_difference.y * camera.local_basis[1]) * edit_rotation_speed;
		edit_mode_model->rotate(rotation_vector);
		//If the model is a PhysicsBody, give it the angular velocity of the mouse
		if (PhysicsBody* body = dynamic_cast<PhysicsBody*>(edit_mode_model)) {
			body->angular_vel = rotation_vector / frame_time;
		}
		last_mouse_pos = current_mouse_pos;
	}
}

//--------------------------------------------------------------
void Renderer::mousePressed(int x, int y, int button){
	//This method is called before mouseDragged(), so its useful to set the last mouse position to the current mouse position
	last_mouse_pos = ofVec2f(x, -y);
}

//--------------------------------------------------------------
void Renderer::mouseReleased(int x, int y, int button) {
	
	//Stay in edit mode if the middle button was just released. If right-click is released, show the cursor and exit edit mode
	if (button != 1) {
	ofShowCursor();

	//End edit mode
	edit_mode = false;
	edit_mode_model = nullptr;
	}
}

//--------------------------------------------------------------
void Renderer::mouseScrolled(ofMouseEventArgs& mouse) {
	// If currently in edit mode and right click is held, use the local basis to move the selected model towards or away from the camera
	if (edit_mode && mouse.button == 2) {
		edit_mode_model->position += mouse.scrollY * camera.local_basis[0] * edit_mode_model_dist* edit_translation_speed * 10;
	}
	// Otherwise, change the FOV
	else {
		camera.zoom(mouse.scrollY);
	}

}

//--------------------------------------------------------------
void Renderer::mouseEntered(int x, int y){
	//Unused autogenerated method
}

//--------------------------------------------------------------
void Renderer::mouseExited(int x, int y){
	//Unused autogenerated method
}

//--------------------------------------------------------------
void Renderer::windowResized(int w, int h){
	//Unused autogenerated method
}

//--------------------------------------------------------------
void Renderer::gotMessage(ofMessage msg){
	//Unused autogenerated method
}

//--------------------------------------------------------------
void Renderer::dragEvent(ofDragInfo dragInfo){ 
	//Unused autogenerated method
}