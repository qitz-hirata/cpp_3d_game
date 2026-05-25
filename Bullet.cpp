#include "Bullet.h"
#include "DxLibCommon.h"

void Bullet::Initialize(DxLibCommon* dxLib, BulletOwner ownerType, Vector3 vel, float lifetime) {
    owner     = ownerType;
    velocity  = vel;
    lifetime_ = lifetime;
    isAlive_  = true;

    cube.transform.localScale = { 0.3f, 0.3f, 0.3f };
    cube.color = (owner == BulletOwner::Ally)
        ? dxLib->MakeColor(100, 200, 255)
        : dxLib->MakeColor(255, 80, 80);
}

void Bullet::Update(float deltaTime) {
    if (!isAlive_) return;

    cube.transform.position.x += velocity.x * deltaTime;
    cube.transform.position.y += velocity.y * deltaTime;
    cube.transform.position.z += velocity.z * deltaTime;

    lifetime_ -= deltaTime;
    if (lifetime_ <= 0.0f) {
        isAlive_ = false;
    }
}

void Bullet::Draw(DxLibCommon* dxLib) const {
    if (!isAlive_) return;
    cube.Draw(dxLib);
}

bool Bullet::IsAlive() const {
    return isAlive_;
}

void Bullet::Kill() {
    isAlive_ = false;
}
