#pragma once

#include "Components.h"
#include "Entity.h"
#include "Paintable.h"
#include <memory>

class SigilText : public Entity, public Paintable {
  public:
    SigilText();

    std::shared_ptr<ContentComponent> content;
    std::shared_ptr<StyleComponent> style;

    void paint(SkCanvas *canvas) override;

  private:
};
