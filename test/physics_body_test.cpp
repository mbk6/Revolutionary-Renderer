#include "catch.hpp"
#include "test_utils.h"

PhysicsBody test_body = PhysicsBody("..\\models\\sphere.obj", ofColor::white, 100.0f, ofVec3f(1, 0, 0), ofVec3f(1, 0, 0), ofVec3f(PI, 0, 0), 1.0f);
PhysicsBody other_test_body = PhysicsBody("..\\models\\sphere.obj", ofColor::white, 100.0f, ofVec3f(-5, 0, 0), ofVec3f(-1, 0, 0), ofVec3f(PI, 0, 0), 1.0f);
Plane test_plane = Plane(ofVec3f(2, 0, 0), ofVec3f(-1, 0, 0), ofColor::white, 10);

TEST_CASE("Test proper construction of test_body") {

	//Test all instance variables
	REQUIRE(nearlyEquivalent(test_body.radius, 2.54558f));
	REQUIRE(test_body.mass == 100.0f);
	REQUIRE(test_body.velocity == ofVec3f(1, 0, 0));
	REQUIRE(test_body.acceleration == ofVec3f(0, 0, 0));
	REQUIRE(test_body.angular_vel == ofVec3f(PI, 0, 0));
}

TEST_CASE("Test void update(float time_interval)") {

	ofVec3f rotated_vert = ofVec3f(test_body.vertices[0].x, -test_body.vertices[0].y, -test_body.vertices[0].z);

	//Test proper update after a second of time
	test_body.update(1.0f);

	REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(1, 0, 0)));
	REQUIRE(nearlyEquivalent(test_body.position, ofVec3f(2, 0, 0)));
	REQUIRE(nearlyEquivalent(test_body.vertices[0], rotated_vert));
}

TEST_CASE("Test gravitateWith(PhysicsBody* other)") {

	SECTION("Bodies are not colliding") {
		//Gravitate and check force vectors
		test_body.gravitateWith(&other_test_body);
		REQUIRE(nearlyEquivalent(test_body.force, ofVec3f(-0.408163, 0, 0)));
		REQUIRE(nearlyEquivalent(other_test_body.force, ofVec3f(0.408163, 0, 0)));
	}
	
	SECTION("Bodies are colliding") {
		//Move other_test_bdoy to be colliding with test_body
		other_test_body.position = ofVec3f(1, 0, 0);
		
		//Gravitate and check force vectors
		test_body.gravitateWith(&other_test_body);
		REQUIRE(nearlyEquivalent(test_body.force, ofVec3f(-0.408163, 0, 0)));
		REQUIRE(nearlyEquivalent(other_test_body.force, ofVec3f(0.408163, 0, 0)));
	}
}

TEST_CASE("Test collideWith(Model3D* other)") {

	SECTION("Bodies should collide") {
		other_test_body.velocity = ofVec3f(4, 0, 0);
		//Already colliding, but have opposing velocities
		test_body.collideWith(&other_test_body);

		//Velocities should change accordingly
		REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(4, 0, 0)));
		REQUIRE(nearlyEquivalent(other_test_body.velocity, ofVec3f(1, 0, 0)));
	}

	SECTION("Bodies are already moving away from each other") {
		test_body.velocity = ofVec3f(1, 0, 0);
		other_test_body.velocity = ofVec3f(-1, 0, 0);
		
		//Already colliding, but have opposing velocities
		test_body.collideWith(&other_test_body);
		
		//Velocities should be unchanged
		REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(1, 0, 0)));
		REQUIRE(nearlyEquivalent(other_test_body.velocity, ofVec3f(-1, 0, 0)));
	}

	SECTION("Bodies are not intersecting") {
		//Move other_test_body to be far away from test_body
		other_test_body.position - ofVec3f(100, 0, 0);

		test_body.collideWith(&other_test_body);

		//Velocities should be unchanged
		REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(1, 0, 0)));
		REQUIRE(nearlyEquivalent(other_test_body.velocity, ofVec3f(-1, 0, 0)));
	}


	SECTION("Body should collide with plane") {
		test_body.velocity = ofVec3f(1, 0, 0);
		test_body.collideWith(&test_plane);
		//Normal component of velocity should flip
		REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(-1, 0, 0)));
	}

	SECTION("Body is too far from plane") {
		test_body.velocity = ofVec3f(1, 0, 0);
		test_body.position = ofVec3f(-100, 0, 0);
		test_body.collideWith(&test_plane);
		//Velocity should remain unchanged
		REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(1, 0, 0)));
	}

	SECTION("Body is not above the square of the plane") {
		test_body.position = ofVec3f(2, 100, 100);
		test_body.collideWith(&test_plane);
		//Velocity should remain unchanged
		REQUIRE(nearlyEquivalent(test_body.velocity, ofVec3f(1, 0, 0)));
	}
}

//float average_radius() cannot be tested directly and is tested through the
//"Proper Construction" test case