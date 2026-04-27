#pragma once
#include <string>
#include "Transform.h"

class GameObject {
public:
    std::string name            = "";
    std::string tag             = "Untagged";
    bool        activeSelf      = true;  // このオブジェクト自身のアクティブ状態
    Transform   transform;
};
