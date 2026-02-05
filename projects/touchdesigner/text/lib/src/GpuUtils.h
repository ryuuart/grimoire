//
// Created by Long Nguyen on 1/25/26.
//
#pragma once
#include "include/gpu/graphite/Context.h"

std::unique_ptr<skgpu::graphite::Context> MakeGpuContext();
