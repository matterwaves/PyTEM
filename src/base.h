#ifndef SRC_BASE_H
#define SRC_BASE_H

#include <VKL/VKL.h>
#include <vector>

#include "init.h"

typedef struct {
    VKLInstance instance;
    struct MyPhysicalDeviceProperties* devices;
} MyContext;

extern MyContext _ctx;

typedef struct {
    VKLDevice device;
    VKLQueue* queue;
} MySimulator;

#endif // BASE_H