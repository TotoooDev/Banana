# Building the documentation

If you haven't already, you need to download Banana from the [Github repository](https://github.com/TotoooDev/Banana).

The documentation is located in the `docs` folder. The `bananalua` directory contains the code of this user guide, and the `banana` directory contains the Doxygen specification of the documentation.

## Requirements

- Having a web browser installed on your system.
- Having Doxygen installed on your system.

## Building with Doxywizard

First, launch Doxywizard and open the file named `Doxyfile` located in the `docs/banana/` directory. All you need to do now is to click on the `Run doxygen` button and wait a few moments while the documentation is generated.

Once the generation is finished, you can click the `Show HTML output` button to open the HTML documentation in your default browser. You can also open the `index.html` file inside the `build` directory with your favorite browser.

## Building with Doxygen

Open a terminal in the `docs/banana/` directory, and run the following command: `doxygen Doxyfile`. Wait a few moments while the documentation is generated, and open the `index.html` file located in the `build` directory in your favorite browser.
