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

    sim->baseBuffer.create(VKLBufferCreateInfo()
                            .device(&sim->device)
                            .size(2 * sim->config.rows * sim->config.cols * sizeof(float))
                            .usage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
    
    int baseBufferMemoryIndex = sim->allocator.registerMemoryRequirement(sim->baseBuffer.memoryRequirements());

    allocate_renderer(sim);

    sim->allocator.allocate();

    sim->baseBuffer.bind(sim->allocator.getAllocation(baseBufferMemoryIndex));

    build_renderer(sim);

    sim->fftvulkanhandles.physicalDevice = sim->device.physical()->handle();
    sim->fftvulkanhandles.device = sim->device.handle();
    sim->fftvulkanhandles.queue = sim->queue->handle();
    sim->fftvulkanhandles.commandPool = sim->queue->getCmdBuffer()->pool();
    sim->fftvulkanhandles.fence = sim->queue->fence();
    sim->fftvulkanhandles.buffer = sim->baseBuffer.handle();
    sim->fftvulkanhandles.bufferSize = 2 * sim->config.rows * sim->config.cols * sizeof(float);

    sim->fftConfig.FFTdim = 2;
    sim->fftConfig.size[0] = sim->config.rows;
    sim->fftConfig.size[1] = sim->config.cols;

    sim->fftConfig.device = &sim->fftvulkanhandles.device;
    sim->fftConfig.physicalDevice = &sim->fftvulkanhandles.physicalDevice;
    sim->fftConfig.queue = &sim->fftvulkanhandles.queue;
    sim->fftConfig.commandPool = &sim->fftvulkanhandles.commandPool;
    sim->fftConfig.fence = &sim->fftvulkanhandles.fence;
    sim->fftConfig.isCompilerInitialized = true;
    sim->fftConfig.bufferSize = &sim->fftvulkanhandles.bufferSize;

    VkFFTResult resFFT = initializeVkFFT(&sim->fftApp, sim->fftConfig);
	if (resFFT != VKFFT_SUCCESS) {
        LOG_ERROR("Failed to initialize VkFFT %d", resFFT);
        return NULL;
    }

    sim->launchParams.buffer = &sim->fftvulkanhandles.buffer;

    return sim;
}

void get_image_dims_extern(MySimulator* sim, int* pRows, int* pCols) {
    *pRows = sim->config.rows;
    *pCols = sim->config.cols;
}