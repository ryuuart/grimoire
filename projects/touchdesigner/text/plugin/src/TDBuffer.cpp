//
// Created by Long Nguyen on 1/23/26.
//

#include "TDBuffer.h"

TDBuffer::TDBuffer(TD::TOP_Context * context) : context_(context) {
}


void *TDBuffer::provide(std::size_t size) {
    buffer_ = context_->createOutputBuffer(size, TD::TOP_BufferFlags::Readable, nullptr);

    return buffer_->data;
}

TD::OP_SmartRef<TD::TOP_Buffer>* TDBuffer::useTOPBuffer() {
    return &buffer_;
}
