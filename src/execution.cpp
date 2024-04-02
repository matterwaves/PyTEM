#include "internal.h"

VkBufferUsageFlags getBufferStageUsage(ExecutionStageConfig* stageConfig, int buffer_id) {
    VkBufferUsageFlags usage = 0;

    for(int read_buffer_id = 0; read_buffer_id < stageConfig->readBufferCount; read_buffer_id++) {
        if(stageConfig->readBuffers[read_buffer_id].index == buffer_id) {
            usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }

        switch (stageConfig->readBuffers[read_buffer_id].usage) {
            case EXECUTION_BUFFER_USAGE_STORAGE:
                usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
                break;
            case EXECUTION_BUFFER_USAGE_UNIFORM:
                usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                break;
            case EXECUTION_BUFFER_USAGE_INSTANCE:
                usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                break;
            case EXECUTION_BUFFER_USAGE_VERTEX:
                usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                break;
        }
    }

    for(int write_buffer_id = 0; write_buffer_id < stageConfig->writeBufferCount; write_buffer_id++) {
        if(stageConfig->writeBuffers[write_buffer_id].index == buffer_id) {
            usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }

        switch (stageConfig->writeBuffers[write_buffer_id].usage) {
            case EXECUTION_BUFFER_USAGE_STORAGE:
                usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
                break;
            case EXECUTION_BUFFER_USAGE_UNIFORM:
                usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                break;
            case EXECUTION_BUFFER_USAGE_INSTANCE:
                usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                break;
            case EXECUTION_BUFFER_USAGE_VERTEX:
                usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                break;
        }        
    }

    return usage;
}

VkImageUsageFlags getImageStageUsage(ExecutionStageConfig* stageConfig, int image_id) {
    VkImageUsageFlags usage = 0;



    return usage;
}

MyExecutionPipeline* create_execution_pipeline_extern(MyDeviceContext* device_context, struct ExexutionBuffersConfig* buffers, int buffer_count,
                                                                                       struct ExecutionImageConfig* images, int image_count,
                                                                                       struct ExecutionStageConfig* stages, int stage_count) {

    MyExecutionPipeline* pipeline = new MyExecutionPipeline();
    pipeline->deviceContext = device_context;
    pipeline->instances = new MyExecutionPipelineInstance[device_context->deviceCount * device_context->submissinThreadCount];

    for(int device_id = 0; device_id < device_context->deviceCount; device_id++) {
        for(int thread_id = 0; thread_id < device_context->submissinThreadCount; thread_id++) {
            int instance_id = device_id * device_context->submissinThreadCount + thread_id;

            MyExecutionPipelineInstance* instance = &pipeline->instances[instance_id];
            instance->device = &device_context->devices[device_id];
            instance->queue = device_context->queues[instance_id];

            instance->allocator.create(instance->device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            instance->bufferCount = buffer_count;
            instance->imageCount = image_count;
            instance->stageCount = stage_count;

            //First, we copy over all the configuration information
            instance->buffersConfigs = (ExecutionBufferConfig*)malloc(buffer_count * sizeof(ExecutionBufferConfig));
            instance->imagesConfigs = (ExecutionImageConfig*)malloc(image_count * sizeof(ExecutionImageConfig));
            instance->stagesConfigs = (ExecutionStageConfig*)malloc(stage_count * sizeof(ExecutionStageConfig));

            memcpy(instance->buffersConfigs, buffers, buffer_count * sizeof(ExecutionBufferConfig));
            memcpy(instance->imagesConfigs, images, image_count * sizeof(ExecutionImageConfig));

            // Now we loop through the stage configurations make copies of all the data (including allocating new memory for strings and sub structures)
            for(int stage_id = 0; stage_id < instance->stageCount; stage_id++) {
                instance->stagesConfigs[stage_id].type = stages[stage_id].type;

                instance->stagesConfigs[stage_id].sourceCount = stages[stage_id].sourceCount;
                instance->stagesConfigs[stage_id].sources = (const char **)malloc(stages[stage_id].sourceCount * sizeof(const char*));
                
                for(int source_id = 0; source_id < stages[stage_id].sourceCount; source_id++) {
                    instance->stagesConfigs[stage_id].sources[source_id] = (const char*)malloc(strlen(stages[stage_id].sources[source_id]) + 1);
                    strcpy((char*)instance->stagesConfigs[stage_id].sources[source_id], stages[stage_id].sources[source_id]);
                }

                instance->stagesConfigs[stage_id].pcSizes = (unsigned long long*)malloc(stages[stage_id].sourceCount * sizeof(unsigned long long));
                memcpy(instance->stagesConfigs[stage_id].pcSizes, stages[stage_id].pcSizes, stages[stage_id].sourceCount * sizeof(unsigned long long));

                instance->stagesConfigs[stage_id].readImageCount = stages[stage_id].readImageCount;
                instance->stagesConfigs[stage_id].readImages = (ExecutionStageImageConfig*)malloc(stages[stage_id].readImageCount * sizeof(ExecutionStageImageConfig));

                instance->stagesConfigs[stage_id].writeImageCount = stages[stage_id].writeImageCount;
                instance->stagesConfigs[stage_id].writeImages = (ExecutionStageImageConfig*)malloc(stages[stage_id].writeImageCount * sizeof(ExecutionStageImageConfig));

                instance->stagesConfigs[stage_id].readBufferCount = stages[stage_id].readBufferCount;
                instance->stagesConfigs[stage_id].readBuffers = (ExecutionStageBufferConfig*)malloc(stages[stage_id].readBufferCount * sizeof(ExecutionStageBufferConfig));

                instance->stagesConfigs[stage_id].writeBufferCount = stages[stage_id].writeBufferCount;
                instance->stagesConfigs[stage_id].writeBuffers = (ExecutionStageBufferConfig*)malloc(stages[stage_id].writeBufferCount * sizeof(ExecutionStageBufferConfig));

                memcpy(instance->stagesConfigs[stage_id].readImages, stages[stage_id].readImages, stages[stage_id].readImageCount * sizeof(ExecutionStageImageConfig));
                memcpy(instance->stagesConfigs[stage_id].writeImages, stages[stage_id].writeImages, stages[stage_id].writeImageCount * sizeof(ExecutionStageImageConfig));
                memcpy(instance->stagesConfigs[stage_id].readBuffers, stages[stage_id].readBuffers, stages[stage_id].readBufferCount * sizeof(ExecutionStageBufferConfig));
                memcpy(instance->stagesConfigs[stage_id].writeBuffers, stages[stage_id].writeBuffers, stages[stage_id].writeBufferCount * sizeof(ExecutionStageBufferConfig));
            }

            // Now, we create and allocate the buffers
            instance->buffers = new MyExecutionBuffer[buffer_count];

            for(int buffer_id = 0; buffer_id < buffer_count; buffer_id++) {
                VkBufferUsageFlags usage = 0;

                for(int stage_id = 0; stage_id < stage_count; stage_id++) {
                    usage |= getBufferStageUsage(&instance->stagesConfigs[stage_id], buffer_id);
                }

                instance->buffers[buffer_id].buffer.create(
                    VKLBufferCreateInfo()
                    .device(instance->device)
                    .size(instance->buffersConfigs[buffer_id].size)
                    .usage(usage)
                );

                instance->buffers[buffer_id].allocationIndex = instance->allocator.registerMemoryRequirement(instance->buffers[buffer_id].buffer.memoryRequirements());
                
                // TODO: Add support for staging buffers                
            }

            // Now, we create and allocate the images
            instance->images = new MyExecutionImage[image_count];

            for(int image_id = 0; image_id < image_count; image_id++) {
                VkImageUsageFlags usage = 0;

                for(int stage_id = 0; stage_id < stage_count; stage_id++) {
                    usage |= getImageStageUsage(&instance->stagesConfigs[stage_id], image_id);
                }

                VkImageType imageType = VK_IMAGE_TYPE_3D;

                if(instance->imagesConfigs[image_id].depth == 1) {
                    imageType = VK_IMAGE_TYPE_2D;
                }

                if(instance->imagesConfigs[image_id].cols == 1 && instance->imagesConfigs[image_id].depth == 1) {
                    imageType = VK_IMAGE_TYPE_1D;
                }

                VkFormat format = VK_FORMAT_R32_SFLOAT;

                if(instance->imagesConfigs[image_id].channels == 2) {
                    format = VK_FORMAT_R32G32_SFLOAT;
                } else if(instance->imagesConfigs[image_id].channels == 3) {
                    format = VK_FORMAT_R32G32B32_SFLOAT;
                } else if(instance->imagesConfigs[image_id].channels == 4) {
                    format = VK_FORMAT_R32G32B32A32_SFLOAT;
                }

                instance->images[image_id].image.create(
                    VKLImageCreateInfo()
                    .device(instance->device)
                    .imageType(imageType)
                    .format(format)
                    .extent(instance->imagesConfigs[image_id].rows, instance->imagesConfigs[image_id].cols, instance->imagesConfigs[image_id].depth)
                    .arrayLevels(instance->imagesConfigs[image_id].layers)
                    .usage(usage)
                );

                instance->images[image_id].allocationIndex = instance->allocator.registerMemoryRequirement(instance->images[image_id].image.memoryRequirements());
            }


        }
    }    

    return pipeline;
}

void set_execution_buffer_extern(MyExecutionPipeline* execution_pipeline, int bufferIndex, void* data) {

}

void set_execution_image_extern(MyExecutionPipeline* execution_pipeline, int imageIndex, void* data) {
    
}

void queue_execution_pipeline_extern(MyExecutionPipeline* execution_pipeline, void*** pcData) {

}

void get_execution_buffer_extern(MyExecutionPipeline* execution_pipeline, int bufferIndex, void* data) {

}

void get_execution_image_extern(MyExecutionPipeline* execution_pipeline, int imageIndex, void* data) {

}

void destroy_execution_pipeline_extern(MyExecutionPipeline* execution_pipeline) {

}