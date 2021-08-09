import os
import re
import platform
import subprocess

from distutils.version import LooseVersion
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: "
                + ", ".join(e.name for e in self.extensions)
            )

        if platform.system() == "Windows":
            cmake_version = LooseVersion(
                re.search(r"version\s*([\d.]+)", out.decode()).group(1)
            )
            if cmake_version < "3.1.0":
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        import subprocess

        cfg = "Debug" if self.debug else "Release"
        build_args = ["--config", cfg]

        # define build arguments
        # build_args = self.cmake_build_options.split()

        env = os.environ.copy()
        # env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
        #     env.get('CXXFLAGS', ''),
        #     self.distribution.get_version())
        # if not os.path.exists(self.build_temp):
        #     os.makedirs(self.build_temp)

        # call cmake and build
        subprocess.check_call(["cmake", ext.sourcedir], env=env)
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args,
        )

        print()  # Add empty line for nicer output


setup(
    name="wicked",
    version="0.1.0",
    author="Francesco Evangelista",
    description="An automated operator algebra program",
    long_description="An automated operator algebra program",
    packages=["wicked"],
    # tell setuptools that all packages will be under the '.' directory
    package_dir={"": "."},
    # add an extension module named 'forte' to the package
    ext_modules=[CMakeExtension("wicked")],
    # add custom build_ext command
    cmdclass=dict(build_ext=CMakeBuild),
    test_suite="tests",
    zip_safe=False,
)
