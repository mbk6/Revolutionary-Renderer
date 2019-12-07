#include "ofApp.h"



void Renderer::updateCamera() {
	/*
	Movement System:
	Standard WSAD/SHIFT/SPACE movement relative to current view direction.
	wsad will only adjust the camera's x and z coordinates, while space and shift will move it only in the y direction.
	Check the corresponding element of pressed_keys to see if a key is currently pressed.
	Use the local basis to deterimine which way to move, and update it whenever the camera turns.
	*/

	//Generate unit vector in direction of camera position, with only x and z components. Start with an empty vector
	ofVec3f move_direction = ofVec3f(0, 0, 0);
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
	// Vertical movement does not depend on camera position. Vertical movement besides jumping is not enabled in walk mode
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
		if (camera.position.y < floor_height + player_height) {
			camera.position.y = floor_height + player_height;
			cam_velocity.y = 0;
		}
	}
}

void Renderer::updatePhysics() {

	//If the frame time is too large, don't update anything
	if (frame_time <= 1) {
		//Exert gravity between every two PhysicsBodies in the scene
		for (Model3D* model0 : scene_models) {
			//Dynamic cast method from https://stackoverflow.com/questions/27595076/instanceof-equivalent-in-c?rq=1
			if (PhysicsBody* body0 = dynamic_cast<PhysicsBody*>(model0)) {
				for (Model3D* model1 : scene_models) {
					if (PhysicsBody* body1 = dynamic_cast<PhysicsBody*>(model1)) {
						if (body0 != body1) {
							body0->exertGravity(*body1);
						}
					}
				}
			}

		}

		//Update and reset all force vectors
		for (Model3D* model : scene_models) {
			//If in edit mode, don't move the object, so that it can still be "grabbed"
			if (PhysicsBody* body = dynamic_cast<PhysicsBody*>(model)) {
				if (edit_mode_model != model) {
					body->update(frame_time);
				}
				body->force = ofVec3f(0, 0, 0);
			}
		}
	}
}

void Renderer::clearScene() {
	for (int i = 0; i < scene_models.size(); i++) {
		delete scene_models[i];
	}
	scene_models.clear();
}

void Renderer::updateHead() {
	for (int i = 0; i < face_finder.size(); i++) {
		
		//Use the velocity of a tracked face to move the head reflection left, right, up, and down
		//Use changes in the area of the rectangle returned by the object finder to move the reflection forward and backward
		
	

		ofRectangle new_face_rect = face_finder.getObjectSmoothed(i);
		ofSetColor(ofColor::white);
		ofDrawRectangle(face_rect);

		if (face_rect.x < 0) {
			face_rect = new_face_rect;
			return;
		}


		float areaDiff = new_face_rect.getArea() - face_rect.getArea();

		ofVec2f face_vel = (new_face_rect.position - face_rect.position) / frame_time;

		if (face_vel.length() > 0) {
			face_vel.x *= -1;
			face_vel.y *= -1;

			camera.position += (face_vel.x * camera.local_basis[1] + face_vel.y * camera.local_basis[2]) * frame_time * 0.01;

		}

		if (std::abs(areaDiff) > 0) {
			camera.position += areaDiff * camera.local_basis[0] * frame_time * 0.005;
		}

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
	scene_models.push_back(new Model3D("..\\models\\teapot.obj", ofColor::white, ofVec3f(1, 0, 0), 0.4));
	scene_models.push_back(new Model3D("..\\models\\cube.obj", ofColor::green, ofVec3f(-1, 0, 0), 1));

}

void Renderer::createNewPlanet() {
	if (scene_models.size() < MAX_MODEL_COUNT) {
		scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", (ofColor)new_planet_color, (float)new_planet_mass, (ofVec3f)new_planet_pos, (ofVec3f)new_planet_vel, ofVec3f(), (float)new_planet_size));
	}
	else {
		//Somehow warn the user that they're at the limit
	}
}

void Renderer::deletePlanets() {
	//Clear the scene, then add the sun back
	clearScene();
	scene_models.push_back(new PhysicsBody("..\\models\\sphere.obj", ofColor::yellow, 300000, ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0.2)); /* "Sun" */
}

void Renderer::createNewModel() {
	//Based on instructions at https://openframeworks.cc/documentation/utils/ofSystemUtils/#show_ofSystemLoadDialog
	ofFileDialogResult read_file = ofSystemLoadDialog("Choose File");
	if (read_file.bSuccess) {
		scene_models.push_back(new Model3D(read_file.getPath(), (ofColor)new_model_color, (ofVec3f)new_model_pos, (float)new_model_size));
	}

}


////////////////// OPENFRAMEWORKS METHODS \\\\\\\\\\\\\\\\\\\\\
//--------------------------------------------------------------
void Renderer::setup() {
		
	webcam.setup(1024, 576);

	camera = Camera(ofVec3f(0, 0, 5), ofVec2f(0, 0), 1.5f, 1.0f, 3.0f, 0.5f, 600.0f, win_width, win_height);

	//Face tracking code based on examples provided with ofxCv
	face_finder.setup("haarcascade_frontalface_default.xml");
	face_finder.setPreset(ofxCv::ObjectFinder::Fast);
	face_finder.getTracker().setSmoothingRate(.2);
	

	ofSetBackgroundColor(ofColor::black);

	//////SETUP GUI\\\\\\\\

	//Main Panel
	main_panel.setup();
	main_panel.setPosition(0, 70);
	main_panel.add(walk_mode_toggle.setup("Walk Mode", false));
	main_panel.add(osd_toggle.setup("Show OSD", false));
	main_panel.add(floor_toggle.setup("Show floor", false));
	main_panel.add(head_control_toggle.setup("Head Control", false));
	main_panel.add(demos_label.setup("Demos", ""));
	main_panel.add(planets_demo_button.setup("Planets"));
	main_panel.add(models_demo_button.setup("Models"));

	//New Planet Panel
	new_planet_panel.setup();
	new_planet_panel.setName("Create Planet");
	new_planet_panel.setPosition(win_width - new_planet_panel.getWidth() - 5, 0);
	new_planet_panel.add(new_planet_color.setup("Color", ofColor::green, ofColor::black, ofColor::white));
	new_planet_panel.add(new_planet_pos.setup("Position", ofVec3f(), ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10)));
	new_planet_panel.add(new_planet_vel.setup("Velocity", ofVec3f(), ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10)));
	new_planet_panel.add(new_planet_mass.setup("Mass", 100));
	new_planet_panel.add(new_planet_size.setup(0.1));
	new_planet_panel.add(create_planet_button.setup("Create Planet"));
	new_planet_panel.add(delete_planets_button.setup("Remove All"));

	//New Model Panel
	new_model_panel.setup();
	new_model_panel.setName("Add Model to Scene");
	new_model_panel.setPosition(win_width - new_planet_panel.getWidth() - 5, 0);
	new_model_panel.add(new_model_color.setup("Color", ofColor::white, ofColor::black, ofColor::white));
	new_model_panel.add(new_model_pos.setup("Position", ofVec3f(), ofVec3f(-10, -10, -10), ofVec3f(10, 10, 10)));
	new_model_panel.add(new_model_size.setup("Size Scale", 1, 0, 2));
	new_model_panel.add(create_model_button.setup("Create Model"));
	new_model_panel.add(delete_models_button.setup("Remove All"));

	//Button Listeners
	planets_demo_button.addListener(this, &Renderer::initPlanetsDemo);
	models_demo_button.addListener(this, &Renderer::initModelsDemo);
	create_planet_button.addListener(this, &Renderer::createNewPlanet);
	delete_planets_button.addListener(this, &Renderer::deletePlanets);
	create_model_button.addListener(this, &Renderer::createNewModel);
	delete_models_button.addListener(this, &Renderer::clearScene);

}

//--------------------------------------------------------------
void Renderer::update(){

	//Get the last frame time in seconds. This will be used to maintain speeds regardless of framerate consistency
	frame_time = ofGetLastFrameTime();

	//Update the camera position and rotation
	updateCamera();

	//Update all physical interactions in the scene
	updatePhysics();

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
	//Draw the floor
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

	// Draw the OSD - Display frame rate, frame time, camera position/rotation, field of view, and the local basis vectors
	if (osd_toggle) {
		ofSetColor(ofColor::white);
		std::stringstream string_stream;
		string_stream << "fps: " << ofGetFrameRate();
		ofDrawBitmapString(string_stream.str(), ofVec2f(10, 10));
		string_stream.str("");
		string_stream << "frame time (s): " << frame_time;
		ofDrawBitmapString(string_stream.str(), ofVec2f(10, 20));
		string_stream.str("");
		string_stream << "camera.position: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")";
		ofDrawBitmapString(string_stream.str(), ofVec2f(10, 30));
		string_stream.str("");
		string_stream << "camera.rotation: (" << camera.rotation.x << ", " << camera.rotation.y << ")";
		ofDrawBitmapString(string_stream.str(), ofVec2f(10, 40));
		string_stream.str("");
		string_stream << "fov: " << camera.field_of_view;
		ofDrawBitmapString(string_stream.str(), ofVec2f(10, 50));
		string_stream.str("");
		string_stream << "camera.local_basis: (" << camera.local_basis[0].x << ", " << camera.local_basis[0].y << ", " << camera.local_basis[0].z << "), (" << camera.local_basis[1].x << ", " << camera.local_basis[1].y << ", " << camera.local_basis[1].z << "), (" << camera.local_basis[2].x << ", " << camera.local_basis[2].y << ", " << camera.local_basis[2].z << ")";
		ofDrawBitmapString(string_stream.str(), ofVec2f(10, 60));
		string_stream.str("");
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
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::mouseDragged(int x, int y, int button) {
	// If left-click, change the camera rotation
	if (button == 0) {
		//ofHideCursor();
		ofVec2f current_mouse_pos = ofVec2f(x, -y);

		//Subtract the old mouse position from the current position, and update the old mouse position
		camera.update(ofVec3f(), (current_mouse_pos - last_mouse_pos), true, frame_time);
	
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
				// Point edit_mode_model to the current closest model
				mouse_dist = (ofVec2f(x, y) - camera.transform(model->position)).length();
				//Divide the grab range by the object's distance, since a smaller looking object should have a smaller grab range
				if (mouse_dist < grab_range / (camera.position - model->position).length() && edit_mode_model_dist < min_mouse_dist) {
					edit_mode_model = model;
					min_mouse_dist = edit_mode_model_dist;
				}
			}
			if (edit_mode_model != nullptr) {
				// Enter edit mode, set the last mouse position to the current mouse position,
				// calculate the distance from the camera to the selected model, and update the local basis
				edit_mode = true;
				last_mouse_pos = ofVec2f(x, -y);
				edit_mode_model_dist = (camera.position - edit_mode_model->position).length();
			}
			else {
				//If no model was close enough to be selected, do nothing
				return;
			}
		}
		// Hide the mouse, compute how much it has moved, and move the object by a scale of the local basis vectors
		// How much the object should move should increase with its distance from the camera, to preserve
		// the illusion that it's being dragged across the flat screen
		ofHideCursor();
		ofVec2f current_mouse_pos = ofVec2f(x, -y);
		ofVec2f mouse_difference = current_mouse_pos - last_mouse_pos;
		edit_mode_model->position += (mouse_difference.x * camera.local_basis[1] + mouse_difference.y * camera.local_basis[2]) * edit_mode_model_dist * edit_translation_speed;
		last_mouse_pos = current_mouse_pos;
	
	}
	// If middle-click, rotate the selected object
	else if (edit_mode && button == 1) {
		ofVec2f current_mouse_pos = ofVec2f(x, -y);
		ofVec2f mouse_difference = current_mouse_pos - last_mouse_pos;

		//Use multiples of the rightward and upward local basis vectors to rotate the selected object
		ofVec3f rotation_vector = (mouse_difference.x * camera.local_basis[2] - mouse_difference.y * camera.local_basis[1]) * edit_rotation_speed;
		edit_mode_model->rotate(rotation_vector);
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
	
	//Stay in edit mode if the middle button was just released. Otherwise, show th cursor and exit edit mode
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
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::mouseExited(int x, int y){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::windowResized(int w, int h){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::gotMessage(ofMessage msg){
	//Unused...for now
}

//--------------------------------------------------------------
void Renderer::dragEvent(ofDragInfo dragInfo){ 
	//Unused...for now
}