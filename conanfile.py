from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

# FIXME: How to deal with test_requires properly?
ENABLE_VISIBILITY_WORKAROUND = False

class TiffWriterConan(ConanFile):
    name = "tiff_writer"
    version = "0.1.0"

    settings = "os", "compiler", "build_type", "arch"
    options = {"fPIC": [True, False]}
    default_options = {"fPIC": True}

    exports_sources = "CMakeLists.txt", "core/*", "test/*"

    package_type = "static-library"

    def requirements(self):
        self.requires("libtiff/4.7.0")

        if ENABLE_VISIBILITY_WORKAROUND:
            # Unit test dependencies
            self.requires("zlib/1.3.1")
            self.requires("libxml2/2.13.8")

    def build_requirements(self):
        # Tool dependencies
        self.tool_requires("cmake/[^3]")

        # Unit test dependencies
        self.test_requires("gtest/1.14.0")
        if not ENABLE_VISIBILITY_WORKAROUND:
            self.test_requires("zlib/1.3.1")
            self.test_requires("libxml2/2.13.8")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)


    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.components["tiff_core"].libs = ["tiff_core"]
        self.cpp_info.components["tiff_core"].requires = ["libtiff::libtiff"]
