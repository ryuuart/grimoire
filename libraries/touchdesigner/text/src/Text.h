#pragma once

#include "Components.h"
#include <memory>

class Text {
  public:
    Text();

    std::shared_ptr<TextContentComponent> content;
    std::shared_ptr<StyleComponent> style;
};
