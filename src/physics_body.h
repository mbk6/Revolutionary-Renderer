#pragma once

#include "model3d.h"


class PhysicsBody : public Model3D {
private:
	const float GRAVITATIONAL_CONSTANT = 0.002; 	// 6.67408e-11;
	const float ELASTICITY = 1;
	float mass;

	/* Computes the approximate radius of the body for treating it like a sphere */
	float average_radius();


public:
	PhysicsBody(std::string obj_path_, ofColor color_, float mass_, ofVec3f initial_pos_, ofVec3f initial_vel_, ofVec3f initial_angular_vel_, float size_scale_)
		: Model3D(obj_path_, color_, initial_pos_, size_scale_) {
		radius = average_radius();
		mass = mass_;
		velocity = initial_vel_;
		angular_vel = initial_angular_vel_;
	}

	float radius;
	ofVec3f force;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f angular_vel; 

	/* Updates the PhysicsBody's velocity, postion, and rotation */
	void update(float time_interval);

	/* Computes the gravitational force between two bodies, and adds it to other.force */
	void exertGravity(PhysicsBody &other);

	/* If the PhysicsBody is close enough to another, it will collide elasticly with it, changing the other's velocity */
	void collideWith(PhysicsBody& other);

};