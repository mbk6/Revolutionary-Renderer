# **Revolutionary Renderer**

## Overview

My final project for CS126 fall 2019 is *Revolutionary Renderer*, a 3D wireframe graphics renderer that can view and manipulate 3D models, as well as run simple physics simulations.

## Download / Installation

* Download the latest release, or use openFrameworks and Visual Studio 2019 to compile the project. The project requires openFrameworks and the addons ofxGui, ofxOpenCv, and ofxCv.

The program includes lots of fun features and focuses most of them into three demonstration modes showing off it's capabilities.

## Demos

* The "Model" demo - an open 3D space in which a user can import and manipulate models in the .OBJ file format. The program comes with nine simple models, found in the `models` folder. Documentation for the source of each one can be found in the first lines of each OBJ file.
* The "Planets" demo - an sandbox environment for simulating planetary orbits. Includes a graphical interface for creating new planets.
* The "Box" demo - a demonstration of the renderer's collision simulation, featuring a box filled with bouncing balls. Includes an graphical interface for changing the size of the box and number of balls.

## Extra Features

* Ability to manipulate objects in the scene with the mouse
* Head-tracking controlled camera
* Walk Mode - allowing the user to walk around on the floor and jump with the spacebar
* On-screen display showing frame rate, frame time, and other program variables
* Toggleable floor

## Dependencies

- A moderately-powerful computer. See warning below.

## Warnings

- **This project was created and tested on a laptop with a Intel i7 9750H and an Nvidia RTX 2060 GPU, and while it could reach a consistent 144fps in most of the medium-cost simulations and medium-poly models during development, I CANNOT GUARANTEE that this program will run smoothly on all devices. The program does its best to keep movement and physics simulations independent of framerate, but it's not without it's errors if it slows down.**

## Controls

#### Keyboard

| Key      | Action                              |
| -------- | ----------------------------------- |
| `w`      | Move forward                        |
| `s`      | Move backward                       |
| `a`      | Move to the left                    |
| `d`      | Move to the right                   |
| `SPACE`  | Move upward,   (In Walk Mode): Jump |
| `SHIFT`  | Move downward                       |
| `UP`     | Look up                             |
| `DOWN`   | Look down                           |
| `LEFT`   | Look left                           |
| `RIGHT`  | Look right                          |
| `+`      | Zoom in                             |
| `-`      | Zoom out                            |
| `ESCAPE` | Exit Program                        |

#### Mouse

| Control             | Action                                            |
| ------------------- | ------------------------------------------------- |
| `Left Click Drag`   | Turn camera                                       |
| `Right Click Drag`  | (Near center of 3D model): Grab and move object   |
| `Middle Click Drag` | (While holding an object): Rotate object          |
| `Forward Scroll`    | Zoom in,  (When holding an object): Bring further |
| `Backward Scroll`   | Zoom out,  (When holding an object): Bring closer |

Created with openFrameworks (https://openframeworks.cc/), and Visual Studio 2019 (https://visualstudio.microsoft.com/vs/)

Created by Michael Korenchan, University of Illinois at Urbana-Champaign,  CS126 Fall 2019