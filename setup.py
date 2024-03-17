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
vulkan_root = os.environ.get('VULKAN_SDK')

project_srcs = project_root + '/src'

include_directories = [
    numpy_include,
    project_srcs,
    vulkan_root + '/include',
    project_root + '/deps/VKL/include',
    #project_root + '/glslang/glslang/Include'
]

sources = [
    'src/wrapper.pyx',
    'src/init.cpp',
    'src/sim.cpp',
    'src/renderer.cpp',
    'deps/VKL/src/VKLBuffer.cpp',
    'deps/VKL/src/VKLCommandBuffer.cpp',
    'deps/VKL/src/VKLDescriptorSet.cpp',
    'deps/VKL/src/VKLDevice.cpp',
    'deps/VKL/src/VKLFramebuffer.cpp',
    'deps/VKL/src/VKLImage.cpp',
    'deps/VKL/src/VKLImageView.cpp',
    'deps/VKL/src/VKLInstance.cpp',
    'deps/VKL/src/VKLPhysicalDevice.cpp',
    'deps/VKL/src/VKLPipeline.cpp',
    'deps/VKL/src/VKLPipelineLayout.cpp',
    'deps/VKL/src/VKLQueue.cpp',
    'deps/VKL/src/VKLRenderPass.cpp',
    'deps/VKL/src/VKLSurface.cpp',
    'deps/VKL/src/VKLStaticAllocator.cpp',
    'deps/VKL/src/VKLSwapChain.cpp'
]

setup(
    name='vktem',
    author='Shahar Sandhaus',
    version='1.0',
    packages=['vktem'],
    ext_modules=[
        Extension('vktem_native_submodule',
                  sources=sources,
                  language='c++',
                  library_dirs=[vulkan_root + '/lib'],
                  libraries=['dl', 'pthread', 'vulkan'],
                  extra_compile_args=['-O0', '-g'],
                  extra_link_args=['-g'],
                  include_dirs=include_directories
        )
    ],
    zip_safe=False
)