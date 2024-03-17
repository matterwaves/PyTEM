#include "internal.h"

#include "renderer.h"

MySimulator* create_simulator_extern(int device_index, struct SimulatorConfig config) {
    MySimulator* sim = new MySimulator();

    sim->device.create(VKLDeviceCreateInfo()
                        .physicalDevice(&_ctx.instance.getPhysicalDevices()[device_index])
                        .queueTypeCount(VKL_QUEUE_TYPE_ALL, 1));

    sim->queue = sim->device.getQueue(VKL_QUEUE_TYPE_ALL, 0);

    sim->config = config;

    sim->allocator.create(&sim->device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    allocate_renderer(sim);

    sim->allocator.allocate();

    return sim;
}

void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols) {

}