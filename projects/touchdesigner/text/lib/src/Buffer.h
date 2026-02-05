//
// Created by Long Nguyen on 2/1/26.
//

#pragma once
#include <cstddef>
#include <memory>

class Buffer {
public:
    virtual ~Buffer() = default;

    virtual void* data() = 0;
    virtual std::size_t size() = 0;
    virtual std::size_t rowBytes() = 0;
};
