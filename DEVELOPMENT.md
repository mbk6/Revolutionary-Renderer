<h2>Revolutionary Renderer Development Log:</h2>


* 11/15/19 - Generated project, added openFrameworks ofxGui and ofxVectorGraphics, learned how to draw simple 2D shapes and change colors.
* 11/16/19 - Began basic perspective rendering by displaying the vertices of a cube. Added camera movement with wsad/space/shift and camera rotation with arrow keys.
* 11/17/19 - Fixed camera movement to be relative to view angle, added ability to control the camera with the mouse, began using lines to render edges
* 11/18/19 - Fixed visibility issue with off-screen points, but at the cost of some lines becoming out of view too early. Switched to drawing edges of test cube.
* 11/19/19 - Limited the user's vertical camera rotation, created new Model3D class for consolidating behavior for 3D models, tested rendering multiple simple shapes with different positions and colors at once. Also modified Model3D objects to readjust their vertex data to be relative to their centers, and provided a method for rotating Model3D objects about their centers.
* 11/20/19 - Allowed for modifying the field of view with the +/- keys and the scroll wheel, cursor is now hidden when controlling camera with the mouse, added support for importing arbitrary OBJ files, made all camera movements framerate-independent. Also added "Edit Mode", where the user can move objects around the scene by right clicking and dragging them around. Commented all code written so far.
* 11/21/19 - When in Edit Mode, the user can now rotate objects with the mouse while holding middle click. Also added "Walk Mode", where the camera is affected by gravity and can "walk" and jump around the scene.
* 11/23/19 - Began physics simulation: created PhysicsBody class extending Model3D for controlling physical behavior of a model. Used copies of sphere.obj to simulate a simple planetary orbit system.
* 11/29/19 - Added GUI panel for adding and removing planets from the orbit system.
* 11/30/19 - Added extra toggles to the main GUI panel, and added the Models Demo, where the user can add new models to the scene.
* 11/31/19 - Began writing tests for the Renderer and Model3D objects.
* 12/2/19 - Added OBJ importing using a file dialog,  wrote more tests.
* 12/3/19 - Added Mirror Mode, using head tracking to move a wireframe head like the user's reflection.

**Upcoming:**

* **Make most methods more testable**

* Handle collision between PhysicsBody objects

* Improve efficiency of rendering techniques to improve performance

* Possibly consolidate camera behavior into a Camera class

* Fix issue of models being drawn on top of each other despite which should be in front

  
