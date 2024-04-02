#ifndef SRC_INTERNAL_H
#define SRC_INTERNAL_H

#include <VKL/VKL.h>
#include <vkFFT.h>
#include <vector>

#include "base.h"
#include "init.h"
#include "sim.h"
#include "device_context.h"
#include "execution.h"

typedef struct {
    VKLInstance instance;
    struct MyPhysicalDeviceProperties* devices;
} MyContext;

extern MyContext _ctx;

struct MyDeviceContext {
    uint32_t deviceCount;
    uint32_t submissinThreadCount;
    VKLDevice* devices;
    const VKLQueue** queues;
};

struct MyExecutionBuffer {
    VKLBuffer buffer;
    int allocationIndex;
    VKLAllocation allocation;
    
    int stagingEnabled;
    VKLBuffer stagingBuffer;
    int stagingAllocationIndex;
    VKLAllocation stagingAllocation;
};

struct MyExecutionImage {
    VKLImage image;
    int allocationIndex;
    VKLAllocation allocation;

    VKLImageView imageView;
    
    unsigned int rows;
    unsigned int cols;
    unsigned int depth;
    unsigned int layers;
    unsigned int channels;
};

struct MyExecutionStageGraphics {
    VKLRenderPass renderpass;
    VKLFramebuffer framebuffer;

    VKLPipelineLayout pipelineLayout;
    VKLPipeline pipeline;
    VKLDescriptorSet* descriptorSet;
};

struct MyExecutionStageCompute {
    VKLPipelineLayout pipelineLayout;
    VKLPipeline pipeline;
    VKLDescriptorSet* descriptorSet;
};

struct MyExecutionStageFFT {
    VkFFTApplication fftApp = {};
    VkFFTConfiguration fftConfig = {};
    VkFFTLaunchParams launchParams = {}; 
};

struct MyExecutionStage {
    ExecutionType type;
    void* stage; 
};

struct MyExecutionPipelineInstance {
    VKLDevice* device;
    const VKLQueue* queue;

    VKLStaticAllocator allocator;

    unsigned int stageCount;
    struct ExecutionStageConfig* stagesConfigs;
    MyExecutionStage* stages;

    unsigned int bufferCount;
    struct ExecutionBufferConfig* buffersConfigs;
    MyExecutionBuffer* buffers;

    unsigned int imageCount;
    struct ExecutionImageConfig* imagesConfigs;
    MyExecutionImage* images;
};

struct MyExecutionPipeline {
    MyDeviceContext* deviceContext;
    MyExecutionPipelineInstance* instances;
};

struct MySimulator {
    // Device handle/context
    VKLDevice device;
    const VKLQueue* queue;

    VKLStaticAllocator allocator;

    VkFFTApplication fftApp = {};
    struct {
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue queue;
        VkFence fence;
        VkCommandPool commandPool;
        VkBuffer buffer;
        uint64_t bufferSize;
    } fftvulkanhandles;
    VkFFTConfiguration fftConfig = {};
    VkFFTLaunchParams launchParams = {};

    struct SimulatorConfig config;

    VKLBuffer baseBuffer;

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

        VKLCommandBuffer* commandBuffer[2];
        uint32_t currentCommandBuffer;
    } renderer;
};

#endif // BASE_H