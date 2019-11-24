#include "physics_body.h"

void PhysicsBody::update(float time_interval) {
	acceleration = force / mass;
	velocity += acceleration * time_interval;
	position += velocity * time_interval;
	rotate(angular_vel * time_interval);

}

void PhysicsBody::exertGravity(PhysicsBody &other) {
	ofVec3f radius = other.position - position;
	ofVec3f grav_force = GRAVITATIONAL_CONSTANT * mass * other.mass / radius.lengthSquared() * radius.getNormalized();
	other.force -= grav_force;
}
