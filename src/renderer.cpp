#include "internal.h"

#include "atoms_spirv.h"

typedef struct {
    float x;
    float y;
    float z;
    int proton_count;
} AtomInstance;

typedef struct {
    float rot3d[16];
    float camx;
    float camy; 
    float pix_size;
    float r_asymptote;
} PC;

void allocate_renderer(MySimulator* sim) {
    printf("queue index: %d\n", sim->queue->getFamilyIndex());

    sim->renderer.renderTarget = new VKLImage(VKLImageCreateInfo()
        .device(sim->device)
        .extent(sim->config.rows, sim->config.cols, 1)
        .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        .format(VK_FORMAT_R32_SFLOAT)
    );

    printf("queue index: %d\n", sim->queue->getFamilyIndex());
    LOG_INFO("Renderer render target created");

    printf("queue: %p\n", sim->queue);

    sim->renderer.renderTarget->transition(sim->queue, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    LOG_INFO("Renderer render target transitioned");
/*
    sim->renderer.vertexBuffer.create(VKLBufferCreateInfo()
        .device(&sim->device)
        .size(3 * 2 * sizeof(float))
        .usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
    );
    
    sim->renderer.instanceBuffer.create(VKLBufferCreateInfo()
        .device(&sim->device)
        .size(sim->config.maxAtomCount * sizeof(AtomInstance))
        .usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
    );
    
    sim->renderer.atomicPotentials.create(VKLImageCreateInfo()
        .device(&sim->device)
        .extent(sim->config.atomicPotentialsRows, sim->config.atomicPotentialsCols, 1)
        .usage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        .format(VK_FORMAT_R32_SFLOAT)
        .arrayLevels(sim->config.atomicPotentialsCount)
    );

    //sim->renderer.renderTargetAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.renderTarget.memoryRequirements());
    //sim->renderer.vertexBufferAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.vertexBuffer.memoryRequirements());
    //sim->renderer.instanceBufferAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.instanceBuffer.memoryRequirements());
    //sim->renderer.atomicPotentialsAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.atomicPotentials.memoryRequirements());
    */
}

void build_renderer(MySimulator* sim) {
    //sim->renderer.renderTarget.bind(sim->allocator.getAllocation(sim->renderer.renderTargetAllocationIndex));
    //sim->renderer.vertexBuffer.bind(sim->allocator.getAllocation(sim->renderer.vertexBufferAllocationIndex));
    //sim->renderer.instanceBuffer.bind(sim->allocator.getAllocation(sim->renderer.instanceBufferAllocationIndex));
    //sim->renderer.atomicPotentials.bind(sim->allocator.getAllocation(sim->renderer.atomicPotentialsAllocationIndex));

    sim->renderer.renderTarget->transition(sim->queue, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    /*

return;
    sim->renderer.renderTargetView.create(VKLImageViewCreateInfo()
        .image(&sim->renderer.renderTarget)
    );

    sim->renderer.renderpass.create(VKLRenderPassCreateInfo()
        .device(&sim->device)
        .addAttachment(VK_FORMAT_R32_SFLOAT)
            .layout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
        .end()
        .addSubpass()
            .addColorAttachment(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        .end()
    );

    sim->renderer.framebuffer.create(VKLFramebufferCreateInfo()
        .renderPass(&sim->renderer.renderpass)
        .addAttachment(&sim->renderer.renderTargetView)
        .extent(sim->config.rows, sim->config.cols, 1)
    );

    float verts[] = {
        -0.5f, -1.5f,
         1.5f,  0.5f,
        -0.5f,  0.5f
    };

    sim->renderer.vertexBuffer.uploadData(sim->queue, verts, 3 * 2 * sizeof(float), 0);

    sim->renderer.pipelineLayout.create(VKLPipelineLayoutCreateInfo()
        .device(&sim->device)
        .addShaderModule((uint32_t*)vert_spirv_code, sizeof(vert_spirv_code), VK_SHADER_STAGE_VERTEX_BIT, "main")
        .addShaderModule((uint32_t*)frag_spirv_code, sizeof(frag_spirv_code), VK_SHADER_STAGE_FRAGMENT_BIT, "main")
        .addDescriptorSet()
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
        .end()
        .addPushConstant(VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PC))
    );

    sim->renderer.pipeline.create(VKLPipelineCreateInfo()
        .layout(&sim->renderer.pipelineLayout)
        .renderPass(&sim->renderer.renderpass, 0)
        .vertexInput
            .addBinding(0, 2 * sizeof(float))
                .addAttrib(0, VK_FORMAT_R32G32_SFLOAT, 0)
            .end()
            .addBinding(1, 4 * sizeof(float))
                .addAttrib(1, VK_FORMAT_R32G32B32_SFLOAT, 0)
                .addAttrib(2, VK_FORMAT_R32_SINT, 3 * sizeof(float))
                .inputRate(VK_VERTEX_INPUT_RATE_INSTANCE)
            .end()
        .end()
    );

    sim->renderer.descriptorSet = new VKLDescriptorSet(&sim->renderer.pipelineLayout, 0);

    sim->renderer.instanceCount = 0;

    sim->renderer.atomicPotentialsView.create(VKLImageViewCreateInfo()
        .image(&sim->renderer.atomicPotentials)
        .type(VK_IMAGE_VIEW_TYPE_2D_ARRAY)
    );

    VkSamplerCreateInfo samplerCreateInfo;

    memset(&samplerCreateInfo, 0, sizeof(VkSamplerCreateInfo));
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.mipLodBias = 0;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1;
    samplerCreateInfo.minLod = 0;
    samplerCreateInfo.maxLod = 1;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    VK_CALL(sim->device.vk.CreateSampler(sim->device.handle(), &samplerCreateInfo, sim->device.allocationCallbacks(), &sim->renderer.atomicPotentialsSampler));

    sim->renderer.descriptorSet->writeImage(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, sim->renderer.atomicPotentialsView.handle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, sim->renderer.atomicPotentialsSampler);

    */
}

void set_atomic_potentials_extern(MySimulator* sim, void* data) {
    /*
    sim->renderer.atomicPotentials.uploadDataBuffer(sim->queue, data, sim->config.atomicPotentialsRows * 
                                                                      sim->config.atomicPotentialsCols * 
                                                                      sim->config.atomicPotentialsCount * 
                                                                      sizeof(float));

    sim->renderer.atomicPotentials.transition(sim->queue, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
                                                */
}

void set_atoms_extern(MySimulator* sim, void* coords, void* proton_counts, int count) {
       /*
    if(count > sim->config.maxAtomCount) {
        LOG_ERROR("Too many atoms for renderer!!!");
        return;
    }

    float* _coords = (float*)coords;
    int* _proton_counts = (int*)proton_counts;

    AtomInstance* instanceData = (AtomInstance*)malloc(count * sizeof(AtomInstance));

    for(int i = 0; i < count; i++) {
        instanceData[i].x = _coords[i*3];
        instanceData[i].y = _coords[i*3+1];
        instanceData[i].z = _coords[i*3+2];
        instanceData[i].proton_count = _proton_counts[i];
    }

    sim->renderer.instanceBuffer.uploadData(sim->queue, instanceData, count * sizeof(AtomInstance), 0);

    free(instanceData);
    */
}

void compute_potential_extern(MySimulator* sim, float* matricies) {
    /*
    VKLCommandBuffer* cmdBuffer = sim->queue->getCmdBuffer();

    VkClearValue clearColorStruct;
    clearColorStruct.color.float32[0] = 0.137777;

    sim->renderer.framebuffer.setClearValue(clearColorStruct, 0);

    cmdBuffer->begin();
    cmdBuffer->beginRenderPass(sim->renderer.framebuffer, VK_SUBPASS_CONTENTS_INLINE);

    cmdBuffer->setViewPort(0, sim->config.rows, sim->config.cols);	
    cmdBuffer->setScissor(0, sim->config.rows, sim->config.cols);

    cmdBuffer->endRenderPass();

    VkBufferImageCopy bufferImageCopy;
    memset(&bufferImageCopy, 0, sizeof(VkBufferImageCopy));
    bufferImageCopy.bufferOffset = 0;
    bufferImageCopy.bufferRowLength = 0;
    bufferImageCopy.bufferImageHeight = 0;
    bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferImageCopy.imageSubresource.layerCount = 1;
    bufferImageCopy.imageExtent = sim->renderer.renderTarget.extent(); 

    //sim->device.vk.CmdCopyImageToBuffer(
    //    cmdBuffer->handle(),
    //    sim->renderer.renderTarget.handle(), 
    //    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 
    //    vkl->stagingBuffer.handle(), 
    //    1,
    //    &bufferImageCopy
    //);

    cmdBuffer->end();

    sim->queue->submitAndWait(cmdBuffer);
    */
}

void get_potential_extern(MySimulator* sim, void* out) {
    //sim->renderer.renderTarget.downloadDataBuffer(sim->queue, out, sim->config.rows * sim->config.cols * sizeof(float));
}