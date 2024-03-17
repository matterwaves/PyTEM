#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

#include "base.h"

void allocate_renderer(MySimulator* sim);
void build_renderer(MySimulator* sim);

void set_atomic_potentials_extern(MySimulator* sim, void* data);
void set_atoms_extern(MySimulator* sim, void* coords, void* proton_counts, int count);

void compute_potential_extern(MySimulator* sim, float* matricies);

void get_potential_extern(MySimulator* sim, void* out);

#endif // SRC_RENDERER_H