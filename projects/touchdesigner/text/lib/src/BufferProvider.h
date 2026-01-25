//
// Created by Long Nguyen on 1/23/26.
//
#pragma once
#include <cstddef>

class BufferProvider {
public:
    virtual ~BufferProvider() = default;

    virtual void* provide(std::size_t) = 0;
};
