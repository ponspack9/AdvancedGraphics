# AdvancedGraphics
 
Simple Rendering Engine for Advanced Graphics Programming subject of the Degree in Videogames Design & Development at CITM, UPC.

Made by Óscar Pons and David Tello.

## How to use
The engine has three windows: 
* Console Window: for debug purposes.
* Main Window: where the scene is rendered.
* Options Window: modify the objects in the scene, switch between the textures of the Gbuffer and toggle the implemented techniques.

## Controls
The camera has two options:
* Orbital:
WASD to orbit around the center point.
QE to zoom in/out.

* Free Moving:
WASD to move.
Left-Click to rotate.

## Renders


## Shader Files
* Shader.glsl: basic shader, used for geometry pass.
* Light_shader.glsl: light shader, contains Directional Light shader and Point Light shader.
* SSAO.glsl: Screen Space Ambient Occlusion shader.
* WaterPlane.glsl: Water Plane shader.
