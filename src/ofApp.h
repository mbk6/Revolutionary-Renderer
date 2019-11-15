#pragma once

#include "ofMain.h"

#include <vector>

class Renderer : public ofBaseApp {
   public:
    // Constructor passes in window size
    Renderer(int width, int height);

	// Window parameters
    int win_width;
    int win_height;

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);




};
