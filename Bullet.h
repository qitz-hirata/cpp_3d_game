#pragma once
#include "Cube.h"
#include "Vector3.h"

class DxLibCommon;

enum class BulletOwner { Ally, Enemy };

class Bullet {
public:
    Cube        cube;
    Vector3     velocity = { 0.0f, 0.0f, 0.0f };
    BulletOwner owner    = BulletOwner::Ally;

    void Initialize(DxLibCommon* dxLib, BulletOwner ownerType, Vector3 vel, float lifetime);
    void Update(float deltaTime);
    void Draw(DxLibCommon* dxLib) const;
    bool IsAlive() const;
    void Kill();

private:
    float lifetime_ = 0.0f;
    bool  isAlive_  = false;
};
