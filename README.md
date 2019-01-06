Sauce2D
===============

Sauce2D is a game engine built on the [Simple DirectMedia Library](https://www.libsdl.org/). The engine adds high-level classes and functionality for easier handling of the graphics, input, window, etc.

# Features
**Engine**
* Logging functionality
* Automatic resource management

**Graphics:**
* Bitmap fonts
* Sprite batching
* Render targets/Frame buffer objects
* Indexed and non-indexed primitive rendering
* Textured primitives
* Texture atlases
* Vertex buffers and index buffers
* Vertex, fragment and geometry shaders
* Multiple render-targets
* Integer textures
* Can use any OpenGL verison as backend

**Input** 
* Keyboard input
* Mouse input
* Input configuration files
* Joystick/Gamepad input
 
**Utility**
* Extra math functions
* Matrix and vector classes
* Rectangle packer class

# Samples
* **DrawText**: Shows how to draw text from using [BMFont](http://www.angelcode.com/products/bmfont/). This includes drawing text bound to a box and colored text.
![Text Sample](https://i.imgur.com/fAb0Frt.png)

* **GameController**: Shows how handle and use controller input. Tested using an wired Xbox 360 controller.
* **Gui**: Shows an implementation of a graphical user interface. This sample includes a line-edit which supports most of the common features and shortcuts of a single-line text-editor. Also features a Button class, a Slider class, dialog boxes and GUI naviation with cross-fade. The GUI-element supports relative positioning and anchoring.
![Gui Sample](https://i.imgur.com/7InN3bW.png)
* **Keybinds**: Shows how to bind keys to functions, use the InputConfig file to create keybindings and how to use the InputContext class.
* **PhysicsEngine**: Shows how to make a simple physics engine (work-in-progress).
![Physics Sample](https://i.imgur.com/uAwkvXh.png?1)
* **RandomNoiseGPU**: Shows how to render random noise on the GPU using shaders.
![Random Noise Sample](https://i.imgur.com/3I5mVXm.png)

* **SceneGraph**: Shows how to construct a scene graph by creating object which inherits the SceneObject class, and then adds them to the scene graph. Also exports the object to a graphviz file which visualizes the scene graph in a tree.
![Scene Graph](https://i.imgur.com/6ln8Wpx.png)

Traversal Order
![Traversal Order](https://i.imgur.com/A6P6aq9.png)

* **Shaders**: Shows how to create and load an GLSL shader. This shader visualizes the Mandelbrot set.
![Shader Sample](https://i.imgur.com/VLms5pD.png)
* **ShadowCasting**: Shows how to create 2D shadow casting using the technique described [by this article](https://github.com/mattdesl/lwjgl-basics/wiki/2D-Pixel-Perfect-Shadows).
![Shadow Casting Sample](https://i.imgur.com/jtyPEgA.png)
* **TextureAtlas**: Shows how to programatically create and use a texture atlas, created run-time from a list of images.
![Texture Atlas Sample](https://i.imgur.com/nF6soSY.png)
 
# Missing Features
* Audio playback
* ~~Networking~~ (example of networking throught RakNet in my other project, Overworld)
* Multi-platform support
* DirectX and Vulkan support
* Multi-language support
