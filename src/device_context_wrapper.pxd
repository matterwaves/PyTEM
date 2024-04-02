# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

from libc.stdlib cimport malloc, free

cdef extern from "device_context.h":
    struct MyDeviceContext

    MyDeviceContext* create_device_context_extern(int* device_indicies, int device_count, int submission_thread_count)
    void destroy_device_context_extern(MyDeviceContext* device_context);

cpdef inline create_device_context(list[int] device_indicies, int submission_thread_count=1):
    cdef int len_device_indicies = len(device_indicies)
    cdef int* device_indicies_c = <int*>malloc(len_device_indicies * sizeof(int))

    for i in range(len_device_indicies):
        device_indicies_c[i] = device_indicies[i]

    cdef unsigned long long result = <unsigned long long>create_device_context_extern(device_indicies_c, len_device_indicies, submission_thread_count)

    free(device_indicies_c)

    return result

cpdef inline destroy_device_context(unsigned long long device_context):
    destroy_device_context_extern(<MyDeviceContext*>device_context)