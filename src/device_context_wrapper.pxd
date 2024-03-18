# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

from libc.stdlib cimport malloc, free

cdef extern from "device_context.h":
    struct MyDeviceContext

    MyDeviceContext* create_device_context_extern(int* device_indicies, int device_count)

cpdef inline create_device_context(list[int] device_indicies):
    cdef int len_device_indicies = len(device_indicies)
    cdef int* device_indicies_c = <int*>malloc(len_device_indicies * sizeof(int))

    for i in range(len_device_indicies):
        device_indicies_c[i] = device_indicies[i]

    cdef unsigned long long result = <unsigned long long>create_device_context_extern(device_indicies_c, len_device_indicies)

    free(device_indicies_c)

    return result
