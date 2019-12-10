#include "test_utils.h"

bool nearlyEquivalent(float num0, float num1) {
	float difference = std::abs(num0 - num1);
	return (difference <= 0.00001);
}

bool nearlyEquivalent(ofVec3f vec0, ofVec3f vec1) {
	ofVec3f difference = vec0 - vec1;
	return (difference.length() <= 0.00001);
}