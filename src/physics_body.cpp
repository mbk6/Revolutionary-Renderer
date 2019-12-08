#include "physics_body.h"

void PhysicsBody::update(float time_interval) {
	acceleration = force / mass;
	velocity += acceleration * time_interval;
	position += velocity * time_interval;
	rotate(angular_vel * time_interval);

}

void PhysicsBody::exertGravity(PhysicsBody &other) {
	ofVec3f displacement = other.position - position;
	//Only gravitate if objects are not colliding
	if (displacement.length() < radius + other.radius) {
		return;
	}
	ofVec3f grav_force = GRAVITATIONAL_CONSTANT * mass * other.mass / displacement.lengthSquared() * displacement.getNormalized();
	force += grav_force;
	other.force -= grav_force;
}

void PhysicsBody::collideWith(PhysicsBody& other) {
	
	ofVec3f displacement = (position - other.position);

	if (displacement.length() > radius + other.radius) {
		return;
	}

	//Method below derived from https://studiofreya.com/3d-math-and-physics/simple-sphere-sphere-collision-detection-and-collision-response/
	
	//Find Unit vector tangental to the object's displacement
	ofVec3f tan_unit = displacement.getNormalized();
	
	//Find tangental velocities
	ofVec3f tan_vel0 = velocity.dot(tan_unit) * tan_unit;
	ofVec3f tan_vel1 = other.velocity.dot(tan_unit) * tan_unit;

	//Find perpendicular velocities
	ofVec3f perp_vel0 = velocity - tan_vel0;
	ofVec3f perp_vel1 = other.velocity - tan_vel1;

	velocity = ELASTICITY*(tan_vel0 * (mass - other.mass) / (mass + other.mass) + tan_vel1 * (2 * other.mass) / (mass + other.mass) + perp_vel0);
	other.velocity = ELASTICITY*(tan_vel0 * (2 * mass) / (mass + other.mass) + tan_vel1 * (other.mass - mass) / (mass + other.mass) + perp_vel1);
}

float PhysicsBody::average_radius() {
	float sum;
	for (ofVec3f vert : vertices) {
		sum += vert.length();
	}
	return sum / vertices.size();
}
