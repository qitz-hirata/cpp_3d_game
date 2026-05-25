#pragma once
#include <vector>
#include "Bullet.h"

class DxLibCommon;

class BulletManager {
public:
    void Add(DxLibCommon* dxLib, Vector3 position, BulletOwner owner, Vector3 velocity, float lifetime);
    void Update(float deltaTime);
    void Draw(DxLibCommon* dxLib) const;

    // target と AABB が重なる Ally 弾を1つ消して true を返す。当たりなしは false
    bool CheckAndKillAllyHit(const Cube& target);

private:
    std::vector<Bullet> bullets_;
};
