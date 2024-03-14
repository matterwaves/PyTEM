# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

cpdef inline create_simulator(int device_index, int rows, int cols):
    cdef extern from "sim.h":
        void* create_simulator(int device_index, int rows, int cols)
    
    return <unsigned long long>create_simulator(device_index, rows, cols)