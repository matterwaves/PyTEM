#ifndef SRC_EXECUTION_PIPELINE_H
#define SRC_EXECUTION_PIPELINE_H

#include "base.h"

enum ExecutionStageType {
    EXECUTION_TYPE_GRAPHICS = 1,
    EXECUTION_TYPE_COMPUTE = 2,
    EXECUTION_TYPE_TRANSFER = 3,
    EXECUTION_TYPE_FFT = 4
};

enum ExecutionBufferUsage {
    EXECUTION_BUFFER_USAGE_UNIFORM = 1,
    EXECUTION_BUFFER_USAGE_STORAGE = 2,
    EXECUTION_BUFFER_USAGE_VERTEX = 3,
    EXECUTION_BUFFER_USAGE_INSTANCE = 4
};

enum ExecutionImageUsage {
    EXECUTION_IMAGE_USEAGE_SAMPLED = 1,
    EXECUTION_IMAGE_USEAGE_STORAGE = 2,
    EXECUTION_IMAGE_USEAGE_COLOR_ATTACHMENT = 3
};

struct ExecutionBufferConfig {
    unsigned long long size;
    unsigned int createStagingBuffer;
};

struct ExecutionImageConfig {
    unsigned int rows;
    unsigned int cols;
    unsigned int depth;
    unsigned int layers;
    unsigned int channels;
};

struct ExecutionStageImageConfig {
    int index;
    ExecutionImageUsage usage;
    unsigned int rows_offset;
    unsigned int cols_offset;
    unsigned int depth_offset;
    
    unsigned int rows_extent;
    unsigned int cols_extent;
    unsigned int depth_extent;

    unsigned int layers_base;
    unsigned int layers_count;
};

struct ExecutionStageBufferConfig {
    int index;
    int stride;
    ExecutionBufferUsage usage;

    unsigned long long size;
    unsigned long long offset;
};

struct ExecutionStageConfig {
    ExecutionStageType type;
    int sourceCount;
    const char** sources;
    unsigned long long* pcSizes;

    int readImageCount;
    ExecutionStageImageConfig* readImages;
    int writeImageCount;
    ExecutionStageImageConfig* writeImages;

    int readBufferCount;
    ExecutionStageBufferConfig* readBuffers;
    int writeBufferCount;
    ExecutionStageBufferConfig* writeBuffers;
};

struct ExecutionStageParams {
    void** pcDatas;
    int fft_inverse;
};

MyExecutionPipeline* create_execution_pipeline_extern(MyDeviceContext* device_context, struct ExecutionBufferConfig* buffers, int buffer_count,
                                                                                       struct ExecutionImageConfig* images, int image_count,
                                                                                       struct ExecutionStageConfig* stages, int stage_count);

void set_execution_buffer_extern(MyExecutionPipeline* execution_pipeline, int bufferIndex, void* data);
void set_execution_image_extern(MyExecutionPipeline* execution_pipeline, int imageIndex, void* data);

void queue_execution_pipeline_extern(MyExecutionPipeline* execution_pipeline, struct ExecutionStageParams* params);

void get_execution_buffer_extern(MyExecutionPipeline* execution_pipeline, int bufferIndex, void* data);
void get_execution_image_extern(MyExecutionPipeline* execution_pipeline, int imageIndex, void* data);

void destroy_execution_pipeline_extern(MyExecutionPipeline* execution_pipeline);

#endif // EXECUTION_PIPELINE_H