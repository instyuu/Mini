# Mini
Small project I've been working on, a few things to note:
- Main file is SUPER messy, will be fixed and re-committed once everything is resolved client side
- Utilizes GLFW3 for window management, including creation and event handling
- It uses old, immediate mode as opposed to the newer OpenGL pipeline mainly due to debugging; this will all eventually be migrated to a more "proper" way of drawing. This causes the program to run at a solid ~4 FPS or less, depending on the amount of faces in the file being loaded.
- It was mainly made quickly, probably over the course of 3-5 days or so, and can only load object files. (For now)
- It has basic lighting, but doesn't rely on object normals, rather, it calculates the normals of each face realtime, which also is a performance hit. Regardless, this is still a huge work in progress.
- Has no support for textures or animation, and most of the main file is messy due to constant testing.

Generally speaking, this is really really barebones, but does it's job (although it may do it at a subpar level)
