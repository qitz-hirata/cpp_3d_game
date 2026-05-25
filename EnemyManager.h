#pragma once
#include <vector>
#include "Enemy.h"

class DxLibCommon;
class BulletManager;

class EnemyManager {
public:
    void Add(DxLibCommon* dxLib, Vector3 position, Vector3 velocity);
    void Update(float deltaTime, BulletManager& bulletManager);
    void Draw(DxLibCommon* dxLib) const;

private:
    std::vector<Enemy> enemies_;
};
