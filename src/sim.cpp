#include "base.h"
#include "sim.h"

void* create_simulator(int device_index, int rows, int cols) {
    MySimulator* sim = new MySimulator();

    sim->device.create(VKLDeviceCreateInfo()
                        .physicalDevice(&_ctx.instance.getPhysicalDevices()[device_index])
                        .queueTypeCount(VKL_QUEUE_TYPE_ALL, 1));

    sim->queue = sim->device.getQueue(VKL_QUEUE_TYPE_ALL, 0);

    return sim;
}