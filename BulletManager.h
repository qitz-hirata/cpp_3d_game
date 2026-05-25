#pragma once
#include <vector>
#include "Bullet.h"

class DxLibCommon;

class BulletManager {
public:
    void Add(DxLibCommon* dxLib, Vector3 position, BulletOwner owner, Vector3 velocity, float lifetime);
    void Update(float deltaTime);
    void Draw(DxLibCommon* dxLib) const;

private:
    std::vector<Bullet> bullets_;
};
