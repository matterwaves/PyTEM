# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

cimport init_wrapper
cimport device_context_wrapper
cimport sim_wrapper
cimport renderer_wrapper


assert sizeof(int) == sizeof(np.int32_t)