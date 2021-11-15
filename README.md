# prms
PRMS: A simple Phong Reflection Model Simulator(PRMS) written in C++ and OpenGL

## Phong Reflection Model Simulator
Phong Reflection Model Simulator is a simple 3D graphics simulator written in C++ and OpenGL.<br>
This is a simulator in order to simulate phong reflection model.<br>
<br>
It is a brief program that simulates how phong reflection models work.<br>
Phong refelction model utilizes phong lighting model in order to create reflection on surface.<br>
Phong lighting model consists of three components: ambient lighting, diffuse lighting, and specular lighting.<br>
In this simulator, you can control ambient lighting and diffuse lighting by modifying ambient intensity and diffuse intensity of lights in the \"Light\" tab.<br>
Furthermore, specular lighting can be controlled by modifying specular powers of materials that meshs have in the \"Material\" section under the \"Mesh\" tab.<br>
Addtional features of lights and meshs as well as those of a camera and a skybox are available as well.<br>
I am working on with CMake for you to build the code by yourself with your own IDE.  
Until I then, the repository contains an executable file that runs on both 32bit and 64bit environment. (release x86)

## Tech Stack
The project was done on Windows 10, Visual Studio 2019 with Modern C++. OpenGL was used as a 3D graphics API.<br>

## Libraries
For OpenGL extension loading library, [GLEW](http://glew.sourceforge.net/) is used.<br>
For OpenGL utility library, [GLFW](https://www.glfw.org/) is used.<br>
For mathematics library, [GLM](https://github.com/g-truc/glm) is used.<br>
For asset importer, [ASSIMP](https://github.com/assimp/assimp) is used.<br>
For image importer, [STB(stb_image)](https://github.com/nothings/stb/blob/master/stb_image.h) is used.<br>
For the graphical user interface, [Dear ImGui](https://github.com/ocornut/imgui) is used.<br>

## Preview
![ver1 0](https://user-images.githubusercontent.com/60923302/111743121-9043cb00-88cc-11eb-82d6-4ae34674bc62.png)

Youtube [Video ver.](https://youtu.be/HNKLdebsa9c)

### License
MIT License

## Contact 
e-mail: kyoungwhankim.dev@gmail.com  
website: kyoungwhankim@github.io
