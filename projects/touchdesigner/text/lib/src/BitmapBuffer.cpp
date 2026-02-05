//
// Created by Long Nguyen on 2/1/26.
//

#include "BitmapBuffer.h"

BitmapBuffer::BitmapBuffer(const SkImageInfo& info) {
    bitmap_.allocPixels(info);
}

void *BitmapBuffer::data() {
    return bitmap_.getAddr(0,0);
}

std::size_t BitmapBuffer::size() {
    return bitmap_.computeByteSize();
}

std::size_t BitmapBuffer::rowBytes() {
    return bitmap_.rowBytes();
}

SkImageInfo BitmapBuffer::getImageInfo() const {
    return bitmap_.info();
}

SkPixmap BitmapBuffer::getPixmap() const {
    return bitmap_.pixmap();
}






