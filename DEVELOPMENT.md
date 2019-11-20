<h2>Revolutionary Renderer Development Log:</h2>


* 11/15/19 - Generated project, added openFrameworks ofxGui and ofxVectorGraphics, learned how to draw simple 2D shapes and change colors.
* 11/16/19 - Began basic perspective rendering by displaying the vertices of a cube. Added camera movement with wsad/space/shift and camera rotation with arrow keys.
* 11/17/19 - Fixed camera movement to be relative to view angle, added ability to control the camera with the mouse, began using lines to render edges
* 11/18/19 - Fixed visibility issue with off-screen points, but at the cost of some lines becoming out of view too early. Switched to drawing edges of test cube.
* 11/19/19 - Limited the user's vertical camera rotation, created new Model3D class for consolidating behavior for 3D models, tested rendering multiple simple shapes with different positions and colors at once. Also modified Model3D objects to readjust their vertex data to be relative to their centers, and provided a method for rotating Model3D objects about their centers.

**Upcoming:**

* Begin importing OBJ files
* Possibly consolidate camera behavior into a Camera class
* Clean up some messy and repeated code
