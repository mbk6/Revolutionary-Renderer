
#pragma once
#include "renderer.h"

/* Bool for determining near-equivalence of floats */
//Based on https://stackoverflow.com/questions/4548004/how-to-correctly-and-standardly-compare-floats
bool nearlyEquivalent(float num0, float num1);

/* Bool for determining near-equivalence of vectors */
bool nearlyEquivalent(ofVec3f vec0, ofVec3f vec1);