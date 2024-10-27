# conan package manager https://conan.io/

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

required_conan_version = ">=2.6.0"


class SamariumConan(ConanFile):
    name = "rocket-ui"
    version = "1.1.0"
    description = "Rocket Telemetry App"
    # homepage = "https://jjbel.github.io/samarium/"
    url = "https://github.com/conan-io/conan-center-index/"
    license = "MIT"
    topics = ("cpp20", "physics", "2d", "simulation")
    generators = "CMakeDeps", "CMakeToolchain"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        deps = [
            "fmt/11.0.2",
            "range-v3/0.12.0",
            "stb/cci.20240531",
            "tl-expected/20190710",
            "tl-function-ref/1.0.0",
            "bshoshany-thread-pool/4.1.0",
            "unordered_dense/4.4.0",
            "svector/1.0.3",
            "glfw/3.4",
            "glm/cci.20230113",
            "glad/0.1.36",
            "freetype/2.13.2",

            "samarium/1.1.0",
            "rapidjson/cci.20230929"
        ]

        for dep in deps:
            self.requires(dep)

    def configure(self):
        self.options["glad"].gl_profile = "core"
        self.options["glad"].gl_version = "4.6"

        self.options["freetype"].with_png = False
        self.options["freetype"].with_zlib = False
        self.options["freetype"].with_bzip2 = False
        self.options["freetype"].with_brotli = False
