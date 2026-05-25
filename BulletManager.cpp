#include "BulletManager.h"
#include "DxLibCommon.h"
#include <algorithm>

void BulletManager::Add(DxLibCommon* dxLib, Vector3 position, BulletOwner owner, Vector3 velocity, float lifetime) {
    Bullet bullet;
    bullet.cube.transform.position = position;
    bullet.Initialize(dxLib, owner, velocity, lifetime);
    bullets_.push_back(std::move(bullet));
}

void BulletManager::Update(float deltaTime) {
    for (auto& bullet : bullets_) {
        bullet.Update(deltaTime);
    }
    bullets_.erase(
        std::remove_if(bullets_.begin(), bullets_.end(),
            [](const Bullet& b) { return !b.IsAlive(); }),
        bullets_.end()
    );
}

void BulletManager::Draw(DxLibCommon* dxLib) const {
    for (const auto& bullet : bullets_) {
        bullet.Draw(dxLib);
    }
}
