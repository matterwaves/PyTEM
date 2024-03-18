# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

cdef extern from "sim.h":
    struct MySimulator

    struct SimulatorConfig:
        int rows
        int cols
        int atomicPotentialsRows
        int atomicPotentialsCols
        int atomicPotentialsCount
        int maxAtomCount
        float pix_size
    
    MySimulator* create_simulator_extern(int device_index, SimulatorConfig config)

    void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols)

cpdef inline create_simulator(int device_index, dict config_dict):
    cdef SimulatorConfig config
    config.rows = config_dict['rows']
    config.cols = config_dict['cols']
    config.atomicPotentialsRows = config_dict['atomicPotentialsRows']
    config.atomicPotentialsCols = config_dict['atomicPotentialsCols']
    config.atomicPotentialsCount = config_dict['atomicPotentialsCount']
    config.maxAtomCount = config_dict['maxAtomCount']
    config.pix_size = config_dict['pixelSize']

    return <unsigned long long>create_simulator_extern(device_index, config)

cpdef inline get_image_dims(unsigned long long sim):
    cdef int rows, cols
    get_image_dims_extern(<MySimulator*>sim, &rows, &cols)
    return rows, cols

