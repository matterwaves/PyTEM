#ifndef SRC_DEVICE_CONTEXT_H
#define SRC_DEVICE_CONTEXT_H

#include "base.h"

MyDeviceContext* create_device_context_extern(int* device_indicies, int device_count, int submission_thread_count);
void destroy_device_context_extern(MyDeviceContext* device_context);

#endif // DEVICE_CONTEXT_H