from conan import ConanFile
from conan.tools.cmake import cmake_layout


class EmberConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = ["cegui/0.8.7@worldforge",
                "ogre/13.4.2@worldforge",
                "sdl/2.26.1",
                "bullet3/2.89",
                "openal-soft/1.22.2",
                "libxdg-basedir/1.2.3@worldforge",
                "atlas/0.7.0@worldforge",
                "eris/1.4.0@worldforge",
                "mercator/0.4.0@worldforge",
                "libwfut/0.2.4@worldforge",
                "varconf/1.0.3@worldforge",
                "lua/5.3.5",
                # We need to resolve openssl since it's used by both libcurl (used by wfut) and pulseaudio (used by openal presumably)
                "openssl/1.1.1t"
                ]

    generators = "CMakeToolchain"

    def layout(self):
        cmake_layout(self)
