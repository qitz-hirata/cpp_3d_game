#pragma once
#include "Transform.h"

class DxLibCommon;

class Cube {
public:
    Transform    transform;
    unsigned int color = 0xFFFFFF;

    void Initialize(DxLibCommon* dxLib);
    void Draw(DxLibCommon* dxLib) const;
};
