# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

assert sizeof(int) == sizeof(np.int32_t)

cdef extern from "init_extern.h":
    void init_extern()

def init():
    init_extern()
