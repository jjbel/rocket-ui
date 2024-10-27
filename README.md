# Rocket UI 🚀

A blazing fast and dead-simple telemetry app, to show and plot data.
The data is received live, for example from a Raspberry Pi connected over serial or WiFi.

[](https://github.com/user-attachments/assets/ed1bcfde-352b-4792-b66d-fdcae2a5544c)

The app is build from the ground up to be high performant and easy to use:

- ⚡ The app consistently runs at **500fps**
- ⏱️ [Time to first draw](https://developer.chrome.com/docs/lighthouse/performance/first-contentful-paint) is just 200ms
- 📥The executable is a mere **1MB**. This was the motivation to move on from ~300MB [electron apps](https://github.com/jjbel/rocket-ui-js).
- 🌐 Single executable file, run from anywhere, no dependencies
- 🖥️ Windows and Linux support
- 🖹 the source code is < 200 lines, since it relies heavily on the samarium library for plotting.
- ✨ flexibly specify data format in a `format.json` file

### [Download the latest release now!](https://github.com/jjbel/rocket-ui/releases)

## Building

### Prerequisites

You will need git, python and a C++ compiler.
The app is build using [samarium](https://github.com/jjbel/samarium); install it:

```sh
git clone --depth 1 https://github.com/jjbel/samarium.git
conan create ./samarium/ -b missing
```

### Building

In a new directory, run:

```sh
git clone https://github.com/jjbel/rocket-ui.git . # Get code

cmake --preset=default # Configure CMake
cmake --build --preset=default # Compile example

# Run example
./build/bin/rocket-ui # on Linux
.\build\Release\rocket-ui # on Windows
```
