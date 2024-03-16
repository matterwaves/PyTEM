#include "internal.h"

MySimulator* create_simulator(int device_index, SimulatorConfig config) {
    MySimulator* sim = new MySimulator();

    sim->device.create(VKLDeviceCreateInfo()
                        .physicalDevice(&_ctx.instance.getPhysicalDevices()[device_index])
                        .queueTypeCount(VKL_QUEUE_TYPE_ALL, 1));

    sim->queue = sim->device.getQueue(VKL_QUEUE_TYPE_ALL, 0);

    return sim;
}

void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols) {

}