// PHYSICS BODY - Defines the PhysicsBody class - a specific type of 3D model that can interact physically with other objects

#pragma once

#include "plane.h" /* Already includes model3d.h */

class PhysicsBody : public Model3D {
private:
	const float GRAVITATIONAL_CONSTANT = 0.002; 	/* How much things gravitate with each other (6.67408e-11 in real life) */
	const float ELASTICITY = 1;						/* How much speed objects retain after a collision */
	float mass;										/* The PhysicsBody's mass */

	/* Computes the approximate radius of the body for treating it like a sphere in collisions */
	float average_radius();

public:

	//PhysicsBody constructor
	PhysicsBody(std::string obj_path_, ofColor color_, float mass_, ofVec3f initial_pos_, ofVec3f initial_vel_, ofVec3f initial_angular_vel_, float size_scale_)
		: Model3D(obj_path_, color_, initial_pos_, size_scale_) {
		radius = average_radius();
		mass = mass_;
		velocity = initial_vel_;
		angular_vel = initial_angular_vel_;
	}

	float radius;			/* Approximate radius of the object used for collision detection. Any non-spherical body is still treated like a sphere */
	ofVec3f force;			/* Current force exerted on this body (not quite newtons but somethihg like that) */
	ofVec3f velocity;		/* Current velocity of this body (units/sec) */
	ofVec3f acceleration;	/* Current acceleration of this body (units/sec/sec) */
	ofVec3f angular_vel;	/* Current angular acceleration of this body (radians/sec) */

	/* Updates the PhysicsBody's velocity, postion, and rotation over a given time interval */
	void update(float time_interval);

	/* Computes the gravitational force between two bodies, and adds it to the force vector of each  */
	void exertGravity(PhysicsBody* other);

	/* Handles collisions between this PhysicsBody, and another PhysicsBody or Plane */
	void collideWith(Model3D* other);

};