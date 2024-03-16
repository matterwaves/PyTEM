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
    VkDeviceMemory memoryPool;
    VkDeviceSize memoryPoolSize;

    SimulatorConfig config;

    // Renderer
    struct {
        VKLImage renderTarget;
        VKLImageView renderTargetView;
        VKLRenderPass renderpass;
        VKLFramebuffer framebuffer;

        VKLBuffer vertexBuffer;
        VKLBuffer instanceBuffer;
        uint32_t instanceCount;
        VKLPipeline pipeline;
        VKLPipelineLayout pipelineLayout;
        VKLDescriptorSet* descriptorSet;

        VKLImage atomicPotentials;
        VKLImageView atomicPotentialsView;
        VkSampler atomicPotentialsSampler;
    } renderer;
    

};

#endif // BASE_H