# Rocket UI 🚀

A blazing fast and dead-simple telemetry app, to show and plot data.
The data is received live, for example from a Raspberry Pi connected over serial or WiFi.

The app is build from the ground up to be high performant and easy to use:

- ⚡ Time to first draw is just 200ms
- 🌐 Single executable file, run from anywhere, no dependencies
- 📥The executable is a mere **1MB**. This was the motivation to move on from ~300MB electron apps.
- 🖥️ Windows and Linux support

## Prerequisites

You will need git, python and a C++ compiler.
The app is build using [samarium](https://github.com/jjbel/samarium); install it:

```sh
git clone --depth 1 https://github.com/jjbel/samarium.git
conan create ./samarium/ -b missing
```

## Building

In a new directory, run:

```sh
git clone https://github.com/jjbel/rocket-ui.git . # Get code

cmake --preset=default # Configure CMake
cmake --build --preset=default # Compile example

# Run example
./build/bin/rocket-ui # on Linux
.\build\Release\rocket-ui # on Windows
```
