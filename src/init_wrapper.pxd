# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

cdef extern from "init.h":
    struct MyPhysicalDeviceProperties:
        int version_variant;
        int version_major;
        int version_minor;
        int version_patch;

        int driver_version;
        int vendor_id;
        int device_id;

        int device_type;

        const char* device_name;

cpdef inline init(bool debug):
    cdef extern from "init.h":
        void init(bool debug)
    init(debug)

cpdef inline get_devices():
    cdef extern from "init.h":
        MyPhysicalDeviceProperties* get_devices(int* count)
    
    cdef int count = 0
    cdef MyPhysicalDeviceProperties* devices = get_devices(&count)

    if not devices:
        return []
    
    device_list = []

    for i in range(count):
        device = devices[i]
        device_info = (
            device.version_variant,
            device.version_major,
            device.version_minor,
            device.version_patch,
            device.driver_version,
            device.vendor_id,
            device.device_id,
            device.device_type,
            device.device_name.decode('utf-8') if device.device_name is not None else None  # Convert C string to Python string, handling null pointers
        )
        device_list.append(device_info)

    return device_list