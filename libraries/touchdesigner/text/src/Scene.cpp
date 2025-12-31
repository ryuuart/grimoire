#include "Scene.h"
#include "SigilText.h"
#include <memory>
#include <string>

Scene::Scene() {
    auto sigilTextEntity = std::make_shared<SigilText>();
    entities_.emplace(sigilTextEntity);
    painter_.addPaintable(sigilTextEntity);
}

void Scene::updateContent(std::string content) {
    for (auto contentItem : contents_) {
        contentItem->content = content;
    }
}

SkImageInfo Scene::getImageInfo() { return painter_.getImageInfo(); }

void Scene::render(void *buffer) {
    painter_.paint();
    painter_.readPixels(buffer);
}
