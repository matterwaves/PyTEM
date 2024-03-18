#include "internal.h"

MyDeviceContext* create_device_context_extern(int* device_indicies, int device_count) {
    MyDeviceContext* ctx = new MyDeviceContext();
    ctx->deviceCount = device_count;
    ctx->devices = new VKLDevice[device_count];
    ctx->queues = (const VKLQueue**)malloc(sizeof(VKLQueue*) * device_count);

    for (int i = 0; i < device_count; i++) {
        ctx->devices[i].create(VKLDeviceCreateInfo()
                                .physicalDevice(&_ctx.instance.getPhysicalDevices()[device_indicies[i]])
                                .queueTypeCount(VKL_QUEUE_TYPE_ALL, 1));
        
        ctx->queues[i] = ctx->devices[i].getQueue(VKL_QUEUE_TYPE_ALL, 0);
    }

    return ctx;
}