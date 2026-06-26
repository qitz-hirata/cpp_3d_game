#pragma once
#include <vector>
#include "Enemy.h"

class DxLibCommon;
class BulletManager;

struct EnemyMasterData {
    int          id;
    unsigned int color;
    int          hp;
    float        speed;
};

class EnemyManager {
public:
    void Add(DxLibCommon* dxLib, Vector3 position, int hp, unsigned int color, float speed);
    void LoadFromCSV(DxLibCommon* dxLib);
    void Update(float deltaTime, BulletManager& bulletManager);
    void Draw(DxLibCommon* dxLib) const;

private:
    std::vector<Enemy> enemies_;
};
