//
// Created by Long Nguyen on 1/23/26.
//

#pragma once
#include "Buffer.h"
#include "CPlusPlus_Common.h"
#include "TOP_CPlusPlusBase.h"

class TDBuffer final : public Buffer {
public:
    explicit TDBuffer(TD::TOP_Context*, std::size_t size, std::size_t row_bytes);

    void *data() override;
    size_t size() override;
    size_t rowBytes() override;

    TD::OP_SmartRef<TD::TOP_Buffer>* useTOPBuffer();
private:
    TD::TOP_Context* context_;
    TD::OP_SmartRef<TD::TOP_Buffer> buffer_;

    std::size_t rowBytes_;
};
