//
// Created by Long Nguyen on 1/23/26.
//

#pragma once
#include "BufferProvider.h"
#include "CPlusPlus_Common.h"
#include "TOP_CPlusPlusBase.h"

class TDBuffer final : public BufferProvider {
public:
    explicit TDBuffer(TD::TOP_Context*);
    void* provide(std::size_t) override;
    TD::OP_SmartRef<TD::TOP_Buffer>* useTOPBuffer();
private:
    TD::TOP_Context* context_;
    TD::OP_SmartRef<TD::TOP_Buffer> buffer_;
};
