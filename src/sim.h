#ifndef SRC_SIM_H
#define SRC_SIM_H

#include "base.h"

struct SimulatorConfig {
    int rows;
    int cols;
    int atomicPotentialsRows;
    int atomicPotentialsCols;
    int atomicPotentialsCount;
    int maxAtomCount;
};

MySimulator* create_simulator_extern(int device_index, struct SimulatorConfig config);

void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols);

#endif // SIM_H