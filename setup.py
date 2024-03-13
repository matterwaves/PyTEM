from setuptools import setup
from distutils.extension import Extension
import numpy
from pathlib import Path
import os

try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()


project_root = os.environ.get('PROJECT_ROOT', os.path.abspath(os.path.dirname(__file__)))


project_srcs = project_root + '/src'

include_directories = [
    numpy_include,
    project_srcs,
    #project_root + '/VkFFT/vkFFT',
    #project_root + '/glslang/glslang/Include'
]

sources = [
    'src/init_wrapper.pyx',
    'src/init.cpp'
]

setup(
    name='pytem',
    author='Shahar Sandhaus',
    version='1.0',
    packages=['pytem'],
    ext_modules=[
        Extension('pytem_native_submodule',
                  sources=sources,
                  language='c++',
                  ibraries=['dl', 'pthread', 'vulkan'],
                  extra_compile_args=['-O2'],
                  extra_link_args=['-g'],
                  include_dirs=include_directories
        )
    ],
    zip_safe=False
)