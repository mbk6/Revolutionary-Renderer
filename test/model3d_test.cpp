#include "catch.hpp"
#include "test_utils.h"

Model3D test_model = Model3D("..\\models\\cube.obj", ofColor::white, ofVec3f(0, 0, 0), 2);


TEST_CASE("Test Proper Construction of Model3D") {

	SECTION("Test Proper Instance Variables") {
		//REQUIRE(test_model.color == ofColor::white);
		REQUIRE(test_model.position == ofVec3f(0, 0, 0));
	}

	SECTION("Test Proper Vertex Set") {
		//A Cube should have 8 verticies, but different from those defined in cube.obj.
		//Their average should be the origin and they should be multiplied by 2
		REQUIRE(test_model.vertices.size() == 8);
		REQUIRE(test_model.vertices[0] == ofVec3f(-1, -1, -1));
		REQUIRE(test_model.vertices[1] == ofVec3f(-1, -1, 1));
		REQUIRE(test_model.vertices[2] == ofVec3f(-1, 1, -1));
		REQUIRE(test_model.vertices[3] == ofVec3f(-1, 1, 1));
		REQUIRE(test_model.vertices[4] == ofVec3f(1, -1, -1));
		REQUIRE(test_model.vertices[5] == ofVec3f(1, -1, 1));
		REQUIRE(test_model.vertices[6] == ofVec3f(1, 1, -1));
		REQUIRE(test_model.vertices[7] == ofVec3f(1, 1, 1));
	}

	SECTION("Test Proper Edge Set") {
		//A Cube defined with triangles has 12 triangles, with 18 distinct edges
		REQUIRE(test_model.edges.size() == 18);
		REQUIRE(test_model.edges[0] == ofVec2f(0, 6));
		REQUIRE(test_model.edges[1] == ofVec2f(6, 4));
		REQUIRE(test_model.edges[2] == ofVec2f(4, 0));
		REQUIRE(test_model.edges[3] == ofVec2f(0, 2));
		REQUIRE(test_model.edges[4] == ofVec2f(2, 6));
		REQUIRE(test_model.edges[5] == ofVec2f(0, 3));
		REQUIRE(test_model.edges[6] == ofVec2f(3, 2));
		REQUIRE(test_model.edges[7] == ofVec2f(0, 1));
		REQUIRE(test_model.edges[8] == ofVec2f(1, 3));
		REQUIRE(test_model.edges[9] == ofVec2f(2, 7));
		REQUIRE(test_model.edges[10] == ofVec2f(7, 6));
		REQUIRE(test_model.edges[11] == ofVec2f(3, 7));
		REQUIRE(test_model.edges[12] == ofVec2f(7, 4));
		REQUIRE(test_model.edges[13] == ofVec2f(7, 5));
		REQUIRE(test_model.edges[14] == ofVec2f(5, 4));
		REQUIRE(test_model.edges[15] == ofVec2f(5, 0));
		REQUIRE(test_model.edges[16] == ofVec2f(5, 1));
		REQUIRE(test_model.edges[17] == ofVec2f(7, 1));
	}
}

TEST_CASE("Test Rotation") {
	
	SECTION("No Rotation") {
		test_model.rotate(ofVec3f(0, 0, 0));

		REQUIRE(test_model.vertices[0] == ofVec3f(-1, -1, -1));
		REQUIRE(test_model.vertices[1] == ofVec3f(-1, -1, 1));
		REQUIRE(test_model.vertices[2] == ofVec3f(-1, 1, -1));
		REQUIRE(test_model.vertices[3] == ofVec3f(-1, 1, 1));
		REQUIRE(test_model.vertices[4] == ofVec3f(1, -1, -1));
		REQUIRE(test_model.vertices[5] == ofVec3f(1, -1, 1));
		REQUIRE(test_model.vertices[6] == ofVec3f(1, 1, -1));
		REQUIRE(test_model.vertices[7] == ofVec3f(1, 1, 1));
	}

	SECTION("90 Degree Rotation about X-axis") {
		test_model.rotate(ofVec3f(PI / 2, 0, 0));

		REQUIRE(nearlyEquivalent(test_model.vertices[0], ofVec3f(-1, 1, -1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[1], ofVec3f(-1, -1, -1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[2], ofVec3f(-1, 1, 1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[3], ofVec3f(-1, -1, 1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[4], ofVec3f(1, 1, -1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[5], ofVec3f(1, -1, -1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[6], ofVec3f(1, 1, 1)));
		REQUIRE(nearlyEquivalent(test_model.vertices[7], ofVec3f(1, -1, 1)));
	}
}

//Methods readFromOBJ, addEdge, fixVertices, and rotateVector cannot be tested
//directly and are tested thorugh the "Proper Construction" test case