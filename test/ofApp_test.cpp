#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ofApp.h"

int test_width = 1920;
int test_height = 1080;
Renderer test_renderer(test_width, test_height);

/* Bool for determining near-equivalence */
//Based on https://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
bool nearlyEquivalent(float num0, float num1) {
	float difference = std::abs(num0 - num1);
	return (difference <= 0.000001);
}


TEST_CASE("Test ofVec2f transform(ofVec3f point3d)") {
	ofVec2f out_of_bounds_point = ofVec2f(-1 * (test_width * 0.25) - 1, -1 * (test_height * 0.25) - 1);
	
	SECTION("Point in front of the camera") {
		REQUIRE(test_renderer.transform(ofVec3f(0, 0, 0)) == ofVec2f(test_width / 2, test_height / 2));
	}

	SECTION("Point behind the camera") {
		REQUIRE(test_renderer.transform(ofVec3f(0, 0, 100)) == out_of_bounds_point);
	}
}

TEST_CASE("Test void rotateCoords(float& coord0, float& coord1, float& angle)") {
	float coord0 = 1.0f;
	float coord1 = -1.0f;

	SECTION("Rotate two coordinates by 0 degrees") {
		test_renderer.rotateCoords(coord0, coord1, 0.0);
		REQUIRE(nearlyEquivalent(coord0, 1.0f));
		REQUIRE(nearlyEquivalent(coord1, -1.0f));
	}

	SECTION("Rotate two coordinates by 360 degrees") {
		test_renderer.rotateCoords(coord0, coord1, 2 * PI);
		REQUIRE(nearlyEquivalent(coord0, 1.0f));
		REQUIRE(nearlyEquivalent(coord1, -1.0f));
	}

	SECTION("Rotate two coordinates by 180 degrees") {
		test_renderer.rotateCoords(coord0, coord1, PI);
		REQUIRE(nearlyEquivalent(coord0, -1.0f));
		REQUIRE(nearlyEquivalent(coord1, 1.0f));
	}
}

TEST_CASE("Test void updateCamera()") {
	//cam_pos and cam_rot are private!
}

TEST_CASE("Test void updatePhysics()") {
	//scene_models is private!
}

TEST_CASE("Test bool inBounds(ofVec2f point2d)") {

	SECTION("Inside") {
		//Top Left
		REQUIRE(test_renderer.inBounds(ofVec2f(test_width / -4, test_height / -4)));
		//Bottom Left
		REQUIRE(test_renderer.inBounds(ofVec2f(test_width / -4, test_height * 1.25)));
		//Top Right
		REQUIRE(test_renderer.inBounds(ofVec2f(test_width * 1.25, test_height / -4)));
		//Bottom Right
		REQUIRE(test_renderer.inBounds(ofVec2f(test_width * 1.25, test_height * 1.25)));
	}

	SECTION("Outside") {
		//Just Outside Top Left
		REQUIRE(!test_renderer.inBounds(ofVec2f(test_width / -4 - 1, test_height / -4 - 1)));
		//Just Outside Bottom Left
		REQUIRE(!test_renderer.inBounds(ofVec2f(test_width / -4 - 1, test_height * 1.25 + 1)));
		//Just Outside Top Right
		REQUIRE(!test_renderer.inBounds(ofVec2f(test_width * 1.25 + 1, test_height / -4 - 1)));
		//Just Outside Bottom Right
		REQUIRE(!test_renderer.inBounds(ofVec2f(test_width * 1.25 + 1, test_height * 1.25 + 1)));
	}
}

TEST_CASE("void drawModel(Model3D* model)") {
	// This method probably can't be tested directly
}

TEST_CASE("void computeLocalBasis()") {
	//lovcal_basis is private!
}

TEST_CASE("void clearScene()") {
	// scene_models is private!
}

