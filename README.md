SuperSauce Engine
===============

SauceEngine is a game engine built on the [Simple DirectMedia Library](https://www.libsdl.org/). The engine adds high-level classes and functionality for easier handling of the graphics, input, window, etc.

# Features
**Engine**
* Logging functionality
* Resource managing

**Graphics:**
* Bitmap fonts
* Sprite batching
* Render targets/Frame buffer objects
* Indexed and non-indexed primitive rendering
* Textured primitives
* Texture atlases
* Vertex buffers and index buffers
* OpenGL 3.1 shaders
* Multiple render-targets
* Integer textures

**Input** 
* Keyboard input
* Mouse input
* Input configuration files
* Joystick/Gamepad input
 
**Utility**
* Extra math functions
* Matrix and vector classes
* Rectangle packer class

# Samples Included
* **DrawText**: Shows how to draw text from using [BMFont](http://www.angelcode.com/products/bmfont/). This includes drawing text bound to a box and colored text.
* **Gui**: Shows an implementation of a Graphical User Interface. This sample includes a line-edit which supports most of the common features of a single-line text-editor. Also features a button. The GUI-element supports relative positioning and anchoring.
* **Keybinds**: Shows how to bind keys to methods/functions, and how to use InputContexts.
* **RandomNoiseGPU**: Shows how to render random noise on the GPU using shaders.
* **SceneGraph**: Shows how to construct a scene graph by creating object which inherits the SceneObject class, and then adds them to the scene graph. Also exports the object to a graphviz file which visualizes the scene graph in a tree.
* **Shaders**: Shows how to create and load an GLSL shader. This shader visualizes the Mandelbrot set.
* **ShadowCasting**: Shows how to create 2D shadow casting, using [this tutorial](https://github.com/mattdesl/lwjgl-basics/wiki/2D-Pixel-Perfect-Shadows).
* **TextureAtlas**: Shows how to programatically create and use a texture atlas, created run-time from a list of pixmaps.
 
# Missing Features
* Audio playback
* Networking
* Multi-platform support
* Multi-language support
