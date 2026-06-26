#pragma once
#include "Cube.h"
#include "Vector3.h"

class DxLibCommon;

class Enemy {
public:
    Cube    cube;
    Vector3 velocity = { 0.0f, 0.0f, 0.0f };

    void Initialize(DxLibCommon* dxLib, Vector3 position, int hp, unsigned int color, float speed);
    void Update(float deltaTime);
    void Draw(DxLibCommon* dxLib) const;
    bool IsAlive() const;
    void Kill();
    void TakeDamage(int damage);

private:
    bool isAlive_ = false;
    int  hp_      = 0;
    float speed_  = 0.0f;
};
