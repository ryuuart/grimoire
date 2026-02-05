//
// Created by Long Nguyen on 1/25/26.
//

#include "GpuUtils.h"
#include "include/gpu/graphite/ContextOptions.h"
#include "include/gpu/graphite/mtl/MtlBackendContext.h"
#include "include/gpu/graphite/mtl/MtlGraphiteTypes.h"
#include <Metal/Metal.h>
using namespace skgpu::graphite;

std::unique_ptr<Context> MakeGpuContext() {
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    id<MTLCommandQueue> queue = [device newCommandQueue];
    MtlBackendContext mtl_backend_context{};
    ContextOptions context_options;

    mtl_backend_context.fDevice.retain((__bridge CFTypeRef)device);
    mtl_backend_context.fQueue.retain((__bridge CFTypeRef)queue);

    return ContextFactory::MakeMetal(mtl_backend_context, context_options);
}
