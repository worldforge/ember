from conans import ConanFile


class EmberConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    requires = ["cegui/0.8.7@worldforge/stable",
                "ogre/13.4.2@worldforge/stable",
                "sdl/2.26.1",
                "boost/1.81.0",
                "bullet3/2.89",
                # This version builds, whereas other variants of 1.22.2 don't.
                # See https://github.com/conan-io/conan-center-index/issues/13772
                "openal/1.22.2@#8773a97077efe8b58581998255778f1f",
                "libxdg-basedir/1.2.3@worldforge/stable",
                "atlas/0.7.0@worldforge/testing",
                "eris/1.4.0@worldforge/testing",
                "mercator/0.4.0@worldforge/testing",
                "libwfut/0.2.4@worldforge/testing",
                "varconf/1.0.3@worldforge/testing",
                "lua/5.3.5",
                "zlib/1.2.13",
                "bzip2/1.0.8",
                # We need to resolve libiconv
                "libiconv/1.17",
                # We need to resolve openssl since it's used by both libcurl (used by wfut) and pulseaudio (used by openal presumably)
                "openssl/1.1.1s",
                # Resolve to fix conflict: "'xkbcommon/1.4.1' requires 'libxml2/2.9.14' while 'wayland/1.21.0' requires 'libxml2/2.10.3'."
                "libxml2/2.10.3"]

    generators = ["cmake"]

    def requirements(self):
        if self.settings.os == "Linux":
            # We need to resolve libalsa
            self.requires("libalsa/1.2.7.2")

    def imports(self):
        self.copy("*.dll", src="bin", dst="bin")
        self.copy("*.dylib", src="lib", dst="bin")
