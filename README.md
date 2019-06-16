# OpenGL Project

Firstly, I basically just copied over the code from the 10th iteration of the practice works, and deleted a bunch of the code to get me to a headstart. This led me to refactoring the code in general pretty nicely, and clearing up glitches that I couldn't deal with initially (like the camera jumping to 90 degrees clockwise as soon as the mouse was moved, and the problem was that the yaw in the camera class (where the function which the callback function calls is defined), was set to 0 rather than -90 degrees (anticlockwise) to compensate for the likely jump).

I got right down to finding models that I could use in my game, so I downloaded a simple crate, and used a nanosuit model which was provided by the website I was learning from initially, before I decided that I wanted my own models instead, and then downloaded blender, and made a simple UV textured cube. Everything was going smoothly except for the fact that the textures wouldn't load from my .obj file, which was in fact due to the .mtl file that accompanied the .obj file did not contain the name of the texture file, which I added, and the model was loaded. I plan to use this model, transformed in various ways, as the basic platform.

And I've been gradually developing the ball game since those.
