# Banana 🍌

Banana is a 3D game engine written with C++ and OpenGL.

## How to create a game with Banana

### No editor?

The first thing to know about Banana is that there is no editor like with engine such as Unity, Unreal or Godot. Everything has to be implemented in the code of your game. To help you with this, Banana comes with the spectacular [Dear ImGui](https://github.com/ocornut/imgui) library.

The reason why there is no editor (and probably will never be) is that I didn't want to lose time creating a cute UI for no one to use it (I mean who would use Banana when you can just download Godot).

### Build Banana

Banana uses [Premake](https://premake.github.io/) as its configuration builder. At this point in time I was too lazy to add Linux support so good luck with that penguin users.

1. First, download the source code.
2. Then, depending on you OS, run the `GenerateProjects` script (the `.bat` one on Windows or the `.sh` one on Linux). This will either create a Visual Studio solution file or a Makefile depending on you OS.
3. Open the solution (or the whole folder if you don't use Windows). Your game will live in the `GameProject` folder.
4. Congratulations, you are now ready to create the most awesome game ever made!