from setuptools import setup
from distutils.extension import Extension
import numpy

try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

setup(
    name='pyvktm',
    author='Shahar Sandhaus',
    version='1.0',
    packages=['pyvktm'],
    ext_modules=[
        Extension('pyvktm_native_submodule', 
                  sources=[],
        )
    ],
    zip_safe=False
)