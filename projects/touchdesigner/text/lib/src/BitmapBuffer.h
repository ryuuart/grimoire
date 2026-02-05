//
// Created by Long Nguyen on 2/1/26.
//

#pragma once
#include "Buffer.h"
#include "external/skia+/include/core/SkBitmap.h"


class BitmapBuffer final : public Buffer {
public:
    explicit BitmapBuffer(const SkImageInfo& info);

    void* data() override;
    std::size_t size() override;
    std::size_t rowBytes() override;
    SkImageInfo getImageInfo() const;
    SkPixmap getPixmap() const;

private:
    SkBitmap bitmap_;
};
