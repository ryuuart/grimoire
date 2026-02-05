//
// Created by Long Nguyen on 1/23/26.
//

#include "TDBuffer.h"

TDBuffer::TDBuffer(TD::TOP_Context * context, std::size_t size, std::size_t row_bytes) : context_(context),
    rowBytes_(row_bytes) {
    buffer_ = context_->createOutputBuffer(size, TD::TOP_BufferFlags::None, nullptr);
}

void *TDBuffer::data() {
    return buffer_->data;
}

size_t TDBuffer::size() {
    return buffer_->size;
}

size_t TDBuffer::rowBytes() {
    return rowBytes_;
}

TD::OP_SmartRef<TD::TOP_Buffer>* TDBuffer::useTOPBuffer() {
    return &buffer_;
}
