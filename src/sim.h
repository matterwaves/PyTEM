#ifndef SRC_SIM_H
#define SRC_SIM_H

#include "base.h"

typedef struct SimulatorConfig {
    int rows;
    int cols;
    int atomicPotentialsRows;
    int atomicPotentialsCols;
    int atomicPotentialsCount;
    int maxAtomCount;
} SimulatorConfig;

MySimulator* create_simulator_extern(int device_index, SimulatorConfig config);

void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols);

#endif // SIM_H