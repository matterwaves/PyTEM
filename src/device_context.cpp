#include "internal.h"

MyDeviceContext* create_device_context_extern(int* device_indicies, int device_count, int submission_thread_count) {
    MyDeviceContext* ctx = new MyDeviceContext();
    ctx->deviceCount = device_count;
    ctx->devices = new VKLDevice[device_count];
    ctx->queues = (const VKLQueue**)malloc(sizeof(VKLQueue*) * device_count * submission_thread_count);

    for (int i = 0; i < device_count; i++) {
        ctx->devices[i].create(
            VKLDeviceCreateInfo()
            .physicalDevice(&_ctx.instance.getPhysicalDevices()[device_indicies[i]])
            .queueTypeCount(VKL_QUEUE_TYPE_ALL, submission_thread_count)
        );
        
        for (int j = 0; j < submission_thread_count; j++) {
            ctx->queues[i * submission_thread_count + j] = ctx->devices[i].getQueue(VKL_QUEUE_TYPE_ALL, j);
        }
    }

    return ctx;
}

void destroy_device_context_extern(MyDeviceContext* device_context) {
    for (int i = 0; i < device_context->deviceCount; i++) {
        device_context->devices[i].destroy();
    }

    delete[] device_context->devices;
    free((void*)device_context->queues);
    delete device_context;
}
