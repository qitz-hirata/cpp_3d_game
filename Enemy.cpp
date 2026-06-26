#include "Enemy.h"
#include "DxLibCommon.h"

void Enemy::Initialize(DxLibCommon* dxLib, Vector3 position, int hp, unsigned int color, float speed) {
    cube.transform.position   = position;
    cube.transform.localScale = { 1.5f, 1.5f, 1.5f };
    cube.color = color;
    velocity   = { 0.0f, 0.0f, speed };
    hp_        = hp;
    speed_     = speed;
    isAlive_   = true;
}

void Enemy::Update(float deltaTime) {
    if (!isAlive_) return;

    cube.transform.position.x += velocity.x * deltaTime;
    cube.transform.position.y += velocity.y * deltaTime;
    cube.transform.position.z += velocity.z * deltaTime;
}

void Enemy::Draw(DxLibCommon* dxLib) const {
    if (!isAlive_) return;
    cube.Draw(dxLib);
}

bool Enemy::IsAlive() const {
    return isAlive_;
}

void Enemy::Kill() {
    isAlive_ = false;
}

void Enemy::TakeDamage(int damage) {
    hp_ -= damage;
    if (hp_ <= 0) isAlive_ = false;
}
