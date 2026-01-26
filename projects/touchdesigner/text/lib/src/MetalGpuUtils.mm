//
// Created by Long Nguyen on 1/25/26.
//

#include "GpuUtils.h"
#include "include/gpu/graphite/ContextOptions.h"
#include "include/gpu/graphite/mtl/MtlBackendContext.h"
#include "include/gpu/graphite/mtl/MtlGraphiteTypes.h"
#include <Metal/Metal.h>
using namespace skgpu::graphite;

struct Initializer {
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    id<MTLCommandQueue> queue = [device newCommandQueue];
    MtlBackendContext mtl_backend_context{};
    ContextOptions context_options;

    Initializer() {
        mtl_backend_context.fDevice.retain((__bridge CFTypeRef)device);
        mtl_backend_context.fQueue.retain((__bridge CFTypeRef)queue);
    }
};

std::unique_ptr<Context> MakeGpuContext() {
    static Initializer initializer;

    return ContextFactory::MakeMetal(initializer.mtl_backend_context, initializer.context_options);
}

std::unique_ptr<Context> MainGpuContext = MakeGpuContext();
