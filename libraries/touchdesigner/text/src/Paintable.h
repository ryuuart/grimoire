#pragma once

#include "include/core/SkCanvas.h"

class Paintable {
  public:
    virtual void paint(SkCanvas *canvas) = 0;
};
