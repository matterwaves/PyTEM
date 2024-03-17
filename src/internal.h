#ifndef SRC_INTERNAL_H
#define SRC_INTERNAL_H

#include <VKL/VKL.h>
#include <vector>

#include "base.h"
#include "init.h"
#include "sim.h"

typedef struct {
    VKLInstance instance;
    struct MyPhysicalDeviceProperties* devices;
} MyContext;

extern MyContext _ctx;

struct MySimulator {
    // Device handle/context
    VKLDevice device;
    const VKLQueue* queue;

    VKLStaticAllocator allocator;

    struct SimulatorConfig config;

    // Renderer
    struct {
        VKLImage renderTarget;
        int renderTargetAllocationIndex;
        VKLImageView renderTargetView;
        VKLRenderPass renderpass;
        VKLFramebuffer framebuffer;

        VKLBuffer vertexBuffer;
        int vertexBufferAllocationIndex;
        VKLBuffer instanceBuffer;
        int instanceBufferAllocationIndex;
        uint32_t instanceCount;
        VKLPipeline pipeline;
        VKLPipelineLayout pipelineLayout;
        VKLDescriptorSet* descriptorSet;

        VKLImage atomicPotentials;
        int atomicPotentialsAllocationIndex;
        VKLImageView atomicPotentialsView;
        VkSampler atomicPotentialsSampler;
    } renderer;
    

};

#endif // BASE_H