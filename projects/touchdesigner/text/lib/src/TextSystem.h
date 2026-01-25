//
// Created by Long Nguyen on 1/23/26.
//

#pragma once
#include "Scene.h"
#include "Typography.h"
#include "modules/skunicode/include/SkUnicode_icu.h"


class TextSystem {
public:
    explicit TextSystem(Scene&);

    void update(float width) const;
private:
    Scene& scene_;
    Typography& typography_;
    sk_sp<SkUnicode> unicode_;
};
