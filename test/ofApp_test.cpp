#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ofApp.h"

int test_width = 1920;
int test_height = 1080;
Renderer test_renderer(test_width, test_height);

TEST_CASE("bool inBounds(ofVec2f point2d)") {

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

