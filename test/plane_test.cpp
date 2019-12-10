#include "catch.hpp"
#include "test_utils.h"


Plane plane = Plane(ofVec3f(0, 0, 0), ofVec3f(1, 0, 0), ofColor::white, 4);

TEST_CASE("Test proper construction of Plane") {

	SECTION("Check instance variables") {
		REQUIRE(plane.position == ofVec3f(0, 0, 0));
		REQUIRE(plane.normal == ofVec3f(1, 0, 0));
		REQUIRE(plane.size == 4);
	}

	SECTION("Check vertices") {
		REQUIRE(nearlyEquivalent(plane.vertices[0], ofVec3f(0.0f, -1.5f, -1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[1], ofVec3f(0.0f, -1.5f, -0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[2], ofVec3f(0.0f, -1.5f, 0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[3], ofVec3f(0.0f, -1.5f, 1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[4], ofVec3f(0.0f, -0.5f, -1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[5], ofVec3f(0.0f, -0.5f, -0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[6], ofVec3f(0.0f, -0.5f, 0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[7], ofVec3f(0.0f, -0.5f, 1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[8], ofVec3f(0.0f, 0.5f, -1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[9], ofVec3f(0.0f, 0.5f, -0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[10], ofVec3f(0.0f, 0.5f, 0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[11], ofVec3f(0.0f, 0.5f, 1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[12], ofVec3f(0.0f, 1.5f, -1.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[13], ofVec3f(0.0f, 1.5f, -0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[14], ofVec3f(0.0f, 1.5f, 0.5f)));
		REQUIRE(nearlyEquivalent(plane.vertices[15], ofVec3f(0.0f, 1.5f, 1.5f)));
	}
	
	SECTION("Check edges") {
		REQUIRE(plane.edges[0] == ofVec2f(0, 4));
		REQUIRE(plane.edges[1] == ofVec2f(4, 8));
		REQUIRE(plane.edges[2] == ofVec2f(8, 12));
		REQUIRE(plane.edges[3] == ofVec2f(1, 5));
		REQUIRE(plane.edges[4] == ofVec2f(5, 9));
		REQUIRE(plane.edges[5] == ofVec2f(9, 13));
		REQUIRE(plane.edges[6] == ofVec2f(2, 6));
		REQUIRE(plane.edges[7] == ofVec2f(6, 10));
		REQUIRE(plane.edges[8] == ofVec2f(10, 14));
		REQUIRE(plane.edges[9] == ofVec2f(3, 7));
		REQUIRE(plane.edges[10] == ofVec2f(7, 11));
		REQUIRE(plane.edges[11] == ofVec2f(11, 15));
		REQUIRE(plane.edges[12] == ofVec2f(0, 1));
		REQUIRE(plane.edges[13] == ofVec2f(1, 2));
		REQUIRE(plane.edges[14] == ofVec2f(2, 3));
		REQUIRE(plane.edges[15] == ofVec2f(4, 5));
		REQUIRE(plane.edges[16] == ofVec2f(5, 6));
		REQUIRE(plane.edges[17] == ofVec2f(6, 7));
		REQUIRE(plane.edges[18] == ofVec2f(8, 9));
		REQUIRE(plane.edges[19] == ofVec2f(9, 10));
		REQUIRE(plane.edges[20] == ofVec2f(10, 11));
		REQUIRE(plane.edges[21] == ofVec2f(12, 13));
		REQUIRE(plane.edges[22] == ofVec2f(13, 14));
		REQUIRE(plane.edges[23] == ofVec2f(14, 15));
	}
}

//Methods generatePlane and rotateToNormal cannot be tested directly, but are tested
//by the "Proper Construction" test case