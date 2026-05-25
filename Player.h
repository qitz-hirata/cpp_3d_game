#pragma once
#include "Cube.h"

class DxLibCommon;
class BulletManager;

class Player {
public:
    Cube cube;

    void Initialize(DxLibCommon* dxLib);
    void Update(DxLibCommon* dxLib, BulletManager& bulletManager, float deltaTime);
    void Draw(DxLibCommon* dxLib) const;

private:
    bool  spaceWasDown_  = false;

    static constexpr float kMoveSpeed   = 3.0f;
    static constexpr float kBulletSpeed = 10.0f;
    static constexpr float kBulletLife  = 3.0f;
};
