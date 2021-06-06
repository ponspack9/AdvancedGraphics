# AdvancedGraphics
 
Simple Rendering Engine for Advanced Graphics Programming subject of the Degree in Videogames Design & Development at CITM, UPC.

Made by Òscar Pons and David Tello.

The techniques implemented are Deferred Shading, Screen Space Ambient Occlusion (SSAO) and Relief Mapping/Parallax Mapping.

## How to use
The engine has three windows: 
* **Console Window:** for debug purposes.
* **Main Window:** where the scene is rendered.
* **Options Window:** modify the objects in the scene, switch between the textures of the Gbuffer and toggle the implemented techniques.

## Camera
**Orbital:**
* WASD to orbit around the center point.
* Mouse Scroll to zoom in/out.

**Free Moving:**
* WASD to move.
* QE to move up/down.
* Mouse Scroll to zoom in/out;
* Click and drag to rotate.

## Renders
<p align="center"><img src="/2.%20OpenGL%20foundations/Renders/main.gif" width="75%" height="75%"/></p>


**SSAO**


![SSAO](https://github.com/ponspack9/AdvancedGraphics/blob/main/2.%20OpenGL%20foundations/Renders/SSAOFull.png?raw=true)

You can enable or disable these effects with this options

![SSAOOptions](https://github.com/ponspack9/AdvancedGraphics/blob/main/2.%20OpenGL%20foundations/Renders/SSAOinspector.png?raw=true)

The corresponding shader file is **SSAO.glsl**


**Relief Mapping**

![Relief Mapping](https://github.com/ponspack9/AdvancedGraphics/blob/main/2.%20OpenGL%20foundations/Renders/RMFull.png?raw=true)

You can enable or disable these effects with this options inside the cube.fbx3 panel (only this model features relief mapping)

![SSAOOptions](https://github.com/ponspack9/AdvancedGraphics/blob/main/2.%20OpenGL%20foundations/Renders/RMInspector.png?raw=true)


The corresponding shader file is **relief_mapping.glsl**
## Shader Files
* **light_shader.glsl:** Light shader, contains Directional Light shader and Point Light shader.
* **SSAO.glsl:** Screen Space Ambient Occlusion shader.
* **relief_mapping.glsl:** Relief Mapping/Parallax Occlusion shader.
