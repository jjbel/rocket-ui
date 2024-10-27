# Samarium Example

This is a fully self-contained example project for [samarium](https://github.com/strangeQuark1041/samarium), a 2-D physics simulation engine written in modern C++

## Prerequistes

| Dependency | URL | Documentation |
| ---        | --- | --- |
| python     | <https://www.python.org/downloads/> | https://www.python.org/doc/ |
| git        | <https://git-scm.com/downloads/> | https://git-scm.com/docs/ |
| cmake      | <https://cmake.org/download/> | https://cmake.org/cmake/help/latest/ |
| conan      | <https://conan.io/downloads.html/> | https://docs.conan.io/en/latest/ |

## Install and Run

In a new directory, run:

```sh
git clone --depth 1 https://github.com/strangeQuark1041/samarium_example.git . # Get code

cmake --preset=default # Configure CMake
cmake --build --preset=default # Compile example

./build/bin/example # Run example
```

For more details see [the library](https://github.com/strangeQuark1041/samarium/) and [the docs](https://strangequark1041.github.io/samarium_docs/)
