# Revolutionary Renderer

For my final project for this class, I intend to write a 3D wireframe graphics renderer in openFrameworks. I plan on implementing real time 3D rendering to the application window, a controllable camera to move around the 3D space, and the ability to load in and view OBJ files, a standard filetype for simple 3D models. In addition to this, I intend to run one or more simple physics simulations of some kind in this 3D space, such as a pendulum, planetary orbits, or a bouncing ball.  One of the extra features of my project will be head-controlled camera movement, where I will use face tracking provided by the addon ofxCv to allow the user to control the camera with their head movement. 

I am aware that OpenFrameworks also comes with functions that could do some 3D rendering for me, *but I do not intend to use them*. **I mean for all 3D effects to be computed manually, using nothing but my own code and OpenFrameworks' 2D drawing tools**.

I've always had a special interest in computer graphics, and I think the idea of representing a view of 3D space on a 2D screen will prove to be an interesting challenge. I'm also interested in animation and physics simulation, but I've only worked with it in 2D, so the extra dimension will also add a fun challenge.

The external openFrameworks addon ofxCv comes with some useful methods for tracking a person's face with the webcam. I will use this for my head-controlled camera feature.

**Rough Timeline:**

* Week 1: Using openFrameworks' 2D drawing tools, basic perspective rendering, camera controls, visibility, and rendering basic 3D shapes.
* Week 2: Rendering wireframe objects, writing functions for reading and interpreting OBJ files. The user should be able to import and view arbitrary OBJ files by end of week 2.
* Week 3: Beginning physics simulation: creating and controlling on-screen motion and physical interaction between objects.
* Week 4: Finishing physics simulation and polishing the project, optimizing or downsizing features to improve performance, and implementing stretch goals.

**Stretch Goals:** 

* Enhanced and customizable color palette.
* Allowing the user to browse for an OBJ file with a GUI instead of giving a file path on the command line.
* Allowing for multiple user-imported objects in the scene.
* GUI sliders for adjusting camera speed, world scale, object size, and field of view.
* Allowing the user to modify the simulated physics system (specify number and masses of objects, initial positions and velocities, force magnitudes, etc.).
* Allowing the user to save the current screen view to an image file.
* Upgrading from wireframe modeling to solid modeling.
