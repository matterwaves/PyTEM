# distutils: language=c++
import numpy as np
cimport numpy as np
from libcpp cimport bool
import sys

cdef extern from "sim.h":
    struct MySimulator

    void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols)

cdef extern from "renderer.h":
    void set_atomic_potentials_extern(MySimulator* sim, void* data)
    void set_atoms_extern(MySimulator* sim, void* coords, void* proton_counts, int count)

    void compute_potential_extern(MySimulator* sim, float* matricies)

    void get_potential_extern(MySimulator* sim, void* out)

cpdef inline set_atomic_potentials(unsigned long long sim_handle, np.ndarray data):
    cdef np.ndarray[ndim=3, dtype=np.float32_t] in_data = np.ascontiguousarray(data, dtype=np.float32)
    set_atomic_potentials_extern(<MySimulator*>sim_handle, &in_data[0, 0, 0])

cpdef inline set_atoms(unsigned long long sim_handle, np.ndarray coords, np.ndarray proton_counts):
    cdef np.ndarray[ndim=2, dtype=np.float32_t] in_coords = np.ascontiguousarray(coords, dtype=np.float32)
    cdef np.ndarray[ndim=1, dtype=np.int32_t] in_proton_counts = np.ascontiguousarray(proton_counts, dtype=np.int32)
    set_atoms_extern(<MySimulator*>sim_handle, &in_coords[0, 0], &in_proton_counts[0], coords.shape[0])

cpdef inline compute_potential(unsigned long long sim_handle, list[float] angles):
    assert len(angles) == 3, "angles must have shape (3,)"

    cdef np.ndarray[ndim=1, dtype=np.float32_t] in_matricies = np.zeros(shape=(16,), dtype=np.float32)

    cdef float cos_phi   = np.cos(np.deg2rad(angles[0]))
    cdef float sin_phi   = np.sin(np.deg2rad(angles[0]))
    cdef float cos_theta = np.cos(np.deg2rad(angles[1]))
    cdef float sin_theta = np.sin(np.deg2rad(angles[1]))

    cdef float M00 = cos_phi * cos_theta 
    cdef float M01 = -sin_phi 

    cdef float M10 = sin_phi * cos_theta 
    cdef float M11 = cos_phi 

    cdef float M20 = -sin_theta 

    cdef float cos_psi_in_plane   = np.cos(np.deg2rad(-angles[2] - 90)) 
    cdef float sin_psi_in_plane   = np.sin(np.deg2rad(-angles[2] - 90))

    cdef float m00  = cos_psi_in_plane
    cdef float m01 = sin_psi_in_plane
    cdef float m10 = -sin_psi_in_plane
    cdef float m11 = cos_psi_in_plane

    in_matricies[0] = m00 * M00 + m10 * M01
    in_matricies[1] = m01 * M00 + m11 * M01
    
    in_matricies[4] = m00 * M10 + m10 * M11
    in_matricies[5] = m01 * M10 + m11 * M11
    
    in_matricies[8] = m00 * M20 
    in_matricies[9] = m01 * M20 

    compute_potential_extern(<MySimulator*>sim_handle, &in_matricies[0])

cpdef inline get_potential(unsigned long long sim_handle, np.ndarray out):
    cdef int rows, cols
    get_image_dims_extern(<MySimulator*>sim_handle, &rows, &cols)

    cdef np.ndarray[ndim=2, dtype=np.float32_t] out_array = np.zeros(shape=(rows, cols), dtype=np.float32)
    get_potential_extern(<MySimulator*>sim_handle, &out_array[0, 0])

    return out_array
