#include "EnemyManager.h"
#include "BulletManager.h"
#include "DxLibCommon.h"
#include <algorithm>

void EnemyManager::Add(DxLibCommon* dxLib, Vector3 position, Vector3 velocity) {
    Enemy enemy;
    enemy.Initialize(dxLib, position, velocity);
    enemies_.push_back(std::move(enemy));
}

void EnemyManager::Update(float deltaTime, BulletManager& bulletManager) {
    for (auto& enemy : enemies_) {
        if (!enemy.IsAlive()) continue;

        enemy.Update(deltaTime);

        if (bulletManager.CheckAndKillAllyHit(enemy.cube)) {
            enemy.Kill();
        }
    }

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const Enemy& e) { return !e.IsAlive(); }),
        enemies_.end()
    );
}

void EnemyManager::Draw(DxLibCommon* dxLib) const {
    for (const auto& enemy : enemies_) {
        enemy.Draw(dxLib);
    }
}
