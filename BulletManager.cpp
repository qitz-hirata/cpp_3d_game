#include "BulletManager.h"
#include "DxLibCommon.h"
#include <algorithm>
#include <cmath>

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

bool BulletManager::CheckAndKillAllyHit(const Cube& target) {
    for (auto& bullet : bullets_) {
        if (!bullet.IsAlive() || bullet.owner != BulletOwner::Ally) continue;

        const Cube& bc = bullet.cube;
        bool hit =
            fabsf(bc.transform.position.x - target.transform.position.x) <= (bc.transform.localScale.x + target.transform.localScale.x) * 0.5f &&
            fabsf(bc.transform.position.y - target.transform.position.y) <= (bc.transform.localScale.y + target.transform.localScale.y) * 0.5f &&
            fabsf(bc.transform.position.z - target.transform.position.z) <= (bc.transform.localScale.z + target.transform.localScale.z) * 0.5f;

        if (hit) {
            bullet.Kill();
            return true;
        }
    }
    return false;
}
