from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain


class EmberConan(ConanFile):
    package_type = "application"
    settings = "os", "arch", "compiler", "build_type"
    def requirements(self):
        self.requires("cegui/0.8.7@worldforge")
        self.requires("ogre/13.4.2@worldforge")
        self.requires("sdl/2.26.1")
        self.requires("bullet3/2.89")
        self.requires("openal-soft/1.22.2")

        self.requires("atlas/0.7.0@worldforge")
        self.requires("eris/1.4.0@worldforge")
        self.requires("mercator/0.4.0@worldforge")
        self.requires("wfut/0.2.4@worldforge")
        self.requires("varconf/1.0.3@worldforge")
        self.requires("sigc++/2.10.0@worldforge")
        self.requires("lua/5.3.5")
        if self.settings.compiler != 'msvc':
            self.requires("libxdg-basedir/1.2.3@worldforge")
        # We need to resolve openssl since it's used by both libcurl (used by wfut) and pulseaudio (used by openal presumably)
#        self.requires("openssl/1.1.1t", override=True)
        self.requires("libxml2/2.10.4", override=True)

    def generate(self):
        deps = CMakeDeps(self)
        # OGRE provides its own CMake files which we should use
        deps.set_property("ogre", "cmake_find_mode", "none")
        # CEGUI provides its own CMake files which we should use
        deps.set_property("cegui", "cmake_find_mode", "none")
        deps.generate()

        tc = CMakeToolchain(self)
        tc.variables["CONAN_FOUND"] = "TRUE"
        tc.generate()

    def layout(self):
        cmake_layout(self)
