#include "internal.h"

#include "renderer.h"

MySimulator* create_simulator_extern(int device_index, struct SimulatorConfig config) {
    MySimulator* sim = new MySimulator();

    sim->device = new VKLDevice(VKLDeviceCreateInfo()
                        .physicalDevice(&_ctx.instance.getPhysicalDevices()[device_index])
                        .queueTypeCount(VKL_QUEUE_TYPE_ALL, 1));

    sim->queue = sim->device->getQueue(VKL_QUEUE_TYPE_ALL, 0);

    printf("queue: %p\n", sim->queue);
    printf("cmd queue: %p\n", sim->queue->getCmdBuffer()->m_queue);
    printf("queue index: %d\n", sim->queue->getFamilyIndex());
    sim->config = config;

    printf("cmd queue: %p\n", sim->queue->getCmdBuffer()->m_queue);
    sim->allocator = new VKLStaticAllocator(sim->device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    printf("cmd queue: %p\n", sim->queue->getCmdBuffer()->m_queue);
    allocate_renderer(sim);

    printf("cmd queue: %p\n", sim->queue->getCmdBuffer()->m_queue);
    sim->allocator->allocate();

    printf("cmd queue: %p\n", sim->queue->getCmdBuffer()->m_queue);
    build_renderer(sim);

    printf("cmd queue: %p\n", sim->queue->getCmdBuffer()->m_queue);
    return sim;
}

void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols) {

}