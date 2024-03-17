#include "internal.h"

typedef struct {
    float x;
    float y;
    float z;
    int proton_count;
} AtomInstance;

void allocate_renderer(MySimulator* sim) {
    sim->renderer.renderTarget.create(VKLImageCreateInfo()
        .device(&sim->device)
        .extent(sim->config.rows, sim->config.cols, 1)
        .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        .format(VK_FORMAT_R32_SFLOAT)
    );

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

    sim->renderer.renderTargetAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.renderTarget.memoryRequirements());
    sim->renderer.vertexBufferAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.vertexBuffer.memoryRequirements());
    sim->renderer.instanceBufferAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.instanceBuffer.memoryRequirements());
    sim->renderer.atomicPotentialsAllocationIndex = sim->allocator.registerMemoryRequirement(sim->renderer.atomicPotentials.memoryRequirements());
}

void build_renderer(MySimulator* sim) {
    sim->renderer.renderTarget.bind(sim->allocator.getAllocation(sim->renderer.renderTargetAllocationIndex));
    sim->renderer.vertexBuffer.bind(sim->allocator.getAllocation(sim->renderer.vertexBufferAllocationIndex));
    sim->renderer.instanceBuffer.bind(sim->allocator.getAllocation(sim->renderer.instanceBufferAllocationIndex));
    sim->renderer.atomicPotentials.bind(sim->allocator.getAllocation(sim->renderer.atomicPotentialsAllocationIndex));

}

void set_atomic_potentials_extern(MySimulator* sim, void* data) {

}

void set_atoms_extern(MySimulator* sim, void* coords, void* proton_counts, int count) {

}

void compute_potential_extern(MySimulator* sim, float* matricies) {

}

void get_potential_extern(MySimulator* sim, void* out) {

}