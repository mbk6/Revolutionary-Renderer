#include "physics_body.h"

void PhysicsBody::update(float time_interval) {
	acceleration = force / mass;
	velocity += acceleration * time_interval;
	position += velocity * time_interval;
	rotate(angular_vel * time_interval);

}

void PhysicsBody::exertGravity(PhysicsBody* other) {
	ofVec3f displacement = other->position - position;
	//Only gravitate if objects are not colliding
	if (displacement.length() < radius + other->radius) {
		return;
	}
	ofVec3f grav_force = GRAVITATIONAL_CONSTANT * mass * other->mass / displacement.lengthSquared() * displacement.getNormalized();
	force += grav_force;
	other->force -= grav_force;
}

void PhysicsBody::collideWith(Model3D* other) {
	
	//Handle collision with other PhysicsBody
	if (PhysicsBody* other_body = dynamic_cast<PhysicsBody*>(other)) {
		ofVec3f displacement = (position - other_body->position);
		ofVec3f norm_ba = displacement.getNormalized();
		ofVec3f norm_ab = -norm_ba;

		//Don't collide if the two bodies aren't close enough or aren't moving towards each other
		if (displacement.length() > radius + other_body->radius || (velocity.dot(norm_ab) < 0 && other_body->velocity.dot(norm_ba) < 0)) {
			return;
		}

		//Method below derived from https://studiofreya.com/3d-math-and-physics/simple-sphere-sphere-collision-detection-and-collision-response/

		//Find Unit vector tangental to the object's displacement
		ofVec3f tan_unit = displacement.getNormalized();

		//Find tangental velocities
		ofVec3f tan_vel0 = velocity.dot(tan_unit) * tan_unit;
		ofVec3f tan_vel1 = other_body->velocity.dot(tan_unit) * tan_unit;

		//Find perpendicular velocities
		ofVec3f perp_vel0 = velocity - tan_vel0;
		ofVec3f perp_vel1 = other_body->velocity - tan_vel1;

		velocity = ELASTICITY * (tan_vel0 * (mass - other_body->mass) / (mass + other_body->mass) + tan_vel1 * (2 * other_body->mass) / (mass + other_body->mass) + perp_vel0);
		other_body->velocity = ELASTICITY * (tan_vel0 * (2 * mass) / (mass + other_body->mass) + tan_vel1 * (other_body->mass - mass) / (mass + other_body->mass) + perp_vel1);

		ofVec3f center = (mass / other_body->mass) * position + (other_body->mass / mass) * other->position;
	}

	//Handle Collision with a Plane
	else if (Plane* other_plane = dynamic_cast<Plane*>(other)) {
		//Derived by me!

		ofVec3f normal = other_plane->normal.getNormalized();
		ofVec3f displacement = position - other_plane->position;
		float normal_displacement_angle = std::acosf(displacement.dot(normal) / displacement.length());

		
		float distance_to_plane = displacement.length() * std::cosf(normal_displacement_angle);
		ofVec3f projected_center = position - distance_to_plane * normal;
		ofVec3f displacement_from_projection = projected_center - other_plane->position;

		ofVec3f local_basis0 = ((other_plane->vertices[0] + other_plane->vertices[other_plane->size - 1]) / 2 - other_plane->position).getNormalized();
		ofVec3f local_basis1 = (local_basis0.getCrossed(normal)).getNormalized();
	
		float x = std::abs(displacement_from_projection.dot(local_basis0));
		float y = std::abs(displacement_from_projection.dot(local_basis1));


		if (distance_to_plane > radius || x > other_plane->size / 2.0f || y > other_plane->size / 2.0f) {
			return;
		}

		//Decompose velocity into tangental and normal components

		ofVec3f normal_vel = velocity.dot(normal) * normal;
		ofVec3f tangental_vel = velocity - normal_vel;

		velocity = -1*normal_vel + tangental_vel;

		position = projected_center + radius * normal;
	}
}

float PhysicsBody::average_radius() {
	
	float sum = 0.0f;

	for (ofVec3f vert : vertices) {
		sum += vert.length();
	}
	return sum / vertices.size();
}
