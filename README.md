# QuasarBell
QuasarBell is a small sfx, texture and geometry nodal editor made with openal + imgui

### Preview

![preview_nodal](demo/preview_nodal.png)

![preview_texture](demo/preview_texture.jpg)

### Some rendering results

<img src="demo/results/render01.png" width="400"><img src="demo/results/render02.png" width="400">
<img src="demo/results/anim01.gif" width="400"><img src="demo/results/anim02.gif" width="400">
<img src="demo/results/rot_planet.gif" width="400"><img src="demo/results/lava_torus.gif" width="400">

### Dependencies
- OpenAL (SDK 1.1 under Windows; 1.20 under Linux)
- GLFW3 3.3.2
- OpenGL 3.3 must be supported

### Building from source
Use CMake to generate a makefile. For example with MSYS2 from project root folder:
```bash
mkdir build
cd build
cmake -G "MSYS Makefiles" ..
make -j8
```
