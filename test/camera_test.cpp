#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "test_utils.h"

int test_width = 1920;
int test_height = 1080;
Camera test_camera = Camera(ofVec3f(0, 0, 5), ofVec2f(0, 0), 1.5f, 1.0f, 3.0f, 0.5f, 600.0f, test_width, test_height);

TEST_CASE("Test ofVec2f transform(ofVec3f point3d)") {
	ofVec2f out_of_bounds_point = ofVec2f(-1 * (test_width * 0.25) - 1, -1 * (test_height * 0.25) - 1);
	
	SECTION("Point in front of the camera") {
		REQUIRE(test_camera.transform(ofVec3f(0, 0, 0)) == ofVec2f(test_width / 2, test_height / 2));
	}

	SECTION("Point behind the camera") {
		REQUIRE(test_camera.transform(ofVec3f(0, 0, 100)) == out_of_bounds_point);
	}
}

TEST_CASE("Test void rotateCoords(float& coord0, float& coord1, float& angle)") {
	float coord0 = 1.0f;
	float coord1 = -1.0f;

	SECTION("Rotate two coordinates by 0 degrees") {
		test_camera.rotateCoords(coord0, coord1, 0.0);
		REQUIRE(nearlyEquivalent(coord0, 1.0f));
		REQUIRE(nearlyEquivalent(coord1, -1.0f));
	}

	SECTION("Rotate two coordinates by 360 degrees") {
		test_camera.rotateCoords(coord0, coord1, 2 * PI);
		REQUIRE(nearlyEquivalent(coord0, 1.0f));
		REQUIRE(nearlyEquivalent(coord1, -1.0f));
	}

	SECTION("Rotate two coordinates by 180 degrees") {
		test_camera.rotateCoords(coord0, coord1, PI);
		REQUIRE(nearlyEquivalent(coord0, -1.0f));
		REQUIRE(nearlyEquivalent(coord1, 1.0f));
	}
}

TEST_CASE("Test bool inBounds(ofVec2f point2d)") {

	SECTION("Inside") {
		//Top Left
		REQUIRE(test_camera.inBounds(ofVec2f(test_width / -4, test_height / -4)));
		//Bottom Left
		REQUIRE(test_camera.inBounds(ofVec2f(test_width / -4, test_height * 1.25)));
		//Top Right
		REQUIRE(test_camera.inBounds(ofVec2f(test_width * 1.25, test_height / -4)));
		//Bottom Right
		REQUIRE(test_camera.inBounds(ofVec2f(test_width * 1.25, test_height * 1.25)));
	}

	SECTION("Outside") {
		//Just Outside Top Left
		REQUIRE(!test_camera.inBounds(ofVec2f(test_width / -4 - 1, test_height / -4 - 1)));
		//Just Outside Bottom Left
		REQUIRE(!test_camera.inBounds(ofVec2f(test_width / -4 - 1, test_height * 1.25 + 1)));
		//Just Outside Top Right
		REQUIRE(!test_camera.inBounds(ofVec2f(test_width * 1.25 + 1, test_height / -4 - 1)));
		//Just Outside Bottom Right
		REQUIRE(!test_camera.inBounds(ofVec2f(test_width * 1.25 + 1, test_height * 1.25 + 1)));
	}
}

TEST_CASE("void computeLocalBasis()") {
	test_camera.computeLocalBasis();

	//Forward, right, and up
	REQUIRE(test_camera.local_basis[0] == ofVec3f(0, 0, -1));
	REQUIRE(test_camera.local_basis[1] == ofVec3f(1, 0, 0));
	REQUIRE(test_camera.local_basis[2] == ofVec3f(0, 1, 0));
}

TEST_CASE("Test void update(ofVec3f direction, ofVec3f rotation, bool mouse_turning, float time_interval)") {
	test_camera.update(ofVec3f(1, 0, 0), ofVec2f(1, 1), false, 1);

	//Position and rotation should change accordingly
	REQUIRE(test_camera.position == ofVec3f(3, 0, 5));
	REQUIRE(test_camera.rotation == ofVec3f(1, 1));
}

TEST_CASE("Test void zoom(float zoom_scale)") {
	test_camera.zoom(1.0f);
	REQUIRE(test_camera.field_of_view == 600.0f * 1.1f);
	test_camera.zoom(-1.0f);
	REQUIRE(test_camera.field_of_view == 600.0f * 1.1f * std::powf(1.1f, -1.0f));
	test_camera.zoom(0.0f);
	REQUIRE(test_camera.field_of_view == 600.0f * 1.1f * std::powf(1.1f, -1.0f));
}

//void drawModel(Model3D* model) cannot be tested directly