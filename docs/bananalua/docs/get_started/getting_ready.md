# Getting ready

## Requirements

You will need a few things to start using Banana:

- Windows or Linux (sorry Apple users).
- Your favorite text editor
- A C++17 compiler or IDE (Visual Studio on Windows, make on Linux).
- git
- C++ and Lua knowledge.
- (Optionnal) The [Lua interpreter](https://www.lua.org/) for your system. This is not required but is neat for testing your Lua code is you don't want to launch your whole game.

## Downloading Banana

To start to create your game with Banana, you first need to download Banana from the [Github repository](https://github.com/TotoooDev/Banana). You can do so by running the following command:

`git clone https://github.com/TotoooDev/Banana.git`.

This will download the whole engine in a folder named `Banana`. You can rename this folder however you like with your project name.

## Creating a project

Go into the folder you've just downloaded. You should find a multitude of files and folders, but the one we are interested in is `CreateProject.bat` (or `CreateProject.sh` on Linux). This script will create a new Banana project for you.

Launch the script either in a terminal or by double-clicking it. Enter the name of your project, and you will be good to go!

The script creates a folder with the name of your project. All your C++ code will live in the folder.

If you use Windows, you can simply open the solution file `Banana.sln` with Visual Studio to begin your project.

If you use Linux, simply open the whole folder in your favorite text editor.

## Project structure

The directories are structured the following way:

- The `Banana` directory is where the engine code is located. Usually you don't need to alter any source files in here.
- The `BananaLua` directory is where the bindings between Banana and Lua occur. Again, you don't need to modify anything here.
- The `docs` directory contains all the documentation.
- `lib` contains all the libraries Banana depends on. If you want to add a library, this is where you need to place it (and add it to the `premake5.lua` file).
- The `bin` and `bin-intermediate` directories are where your binaries are located (the directories are created when you build your project). You can find the executable of your project in the `bin/<configuration>-<platform>/<your-project>` directory.
- The `dev-assets` directory is where you will put all the assets of your game. All the files are copied automatically after your game is built.

Other files that can be useful:

- `CreateProject` is a script that creates a new Banana project.
- `GenerateProjects` is a script that regenerates all your project files. You need to run it when you add or delete any source file in your project.

## The example scene

Go into the folder named after your project. Under the `src` directory, there should be two files: `Game.cpp` and `ExampleScene.h`.

The `Game.cpp` file defines the `CreateApplication` function that returns a pointer to an application. It creates a new application, creates a new scene and returns the the pointer to the application to Banana. You won't need to modify this file here.

The `ExampleScene.h` contains, well, a scene example. Only two methods are defined: the constructor and a method named `OnWindowResized`, which handles the `WindowResized` event (for more information abour events, see the documentation of the `EventBus` class). This is the file (and class) where everything will take life.

If you build and launch the projet now, you should see a magnificent black window appear on your screen. This is normal, because no entity was created yet.

---

You're now ready to read the [next section](the_first_entity.md).
