Uses 
GLEW
GLM
GLFW
AssImp
AntTweakBar*
they're not huge so I left the parts I need in the repo, in the Dependencies folder.

* I used an AntTweakBar that has been extended to GL 3.3 from this github 'https://github.com/sasmaster/AntTweakBarGLFW3'

CONTROLS:
w,s -> move forward/backward
a,d -> strafe left/right
mouse -> wheel move up/down (relative to camera, not world)
mouse -> movement controls view direction
g -> toggle GUI on and off. When the GUI is on the mouse look around is de-activated
space -> toggle fullscreen. needs to be full screen for 3D to work
1,2,3 -> change specularity type. 1 = normal glossy, 2 = static, 3 = parallel

NVIDIA Control Panel settings
Manage 3D settings -> Stereo -Display Mode -> On-Board DIN connector (with NVIDIA 3D Vision)
Manage 3D settings -> Stereo -Enable -> On

The matlab scripts to create the noise image are in the Matlab directory, along with a sample image

The methods in Main.cpp are fairly self explanitory. The important ones are:
init, initializes glfw and glew (glew sets up OpenGL so there's no include of OpenGL) and sets up the context and creates a window

initGUI, sets up assimp GUI

