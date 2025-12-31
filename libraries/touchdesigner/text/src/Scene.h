#pragma once

#include "Components.h"
#include "Entity.h"
#include "Painter.h"
#include "absl/container/flat_hash_set.h"
#include <memory>

class Scene {
  public:
    Scene();

    void updateContent(std::string content);
    void render(void *buffer);
    SkImageInfo getImageInfo();

  private:
    absl::flat_hash_set<std::shared_ptr<Entity>> entities_;
    absl::flat_hash_set<std::shared_ptr<ContentComponent>> contents_;

    Painter painter_;
};
