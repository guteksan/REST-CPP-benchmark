from conans import ConanFile, CMake, tools


class BasicRestinioConan(ConanFile):
    name = "basic-restinio"
    version = "1.0.0"
    author = "guteksan"
    description = "Basic REST service using Restinio"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=True"
    generators = "cmake"
    requires = "restinio/0.6.2@stiffstream/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()


