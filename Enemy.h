#pragma once
#include "Cube.h"
#include "Vector3.h"

class DxLibCommon;

class Enemy {
public:
    Cube    cube;
    Vector3 velocity = { 0.0f, 0.0f, 0.0f };

    void Initialize(DxLibCommon* dxLib, Vector3 position, Vector3 velocity);
    void Update(float deltaTime);
    void Draw(DxLibCommon* dxLib) const;
    bool IsAlive() const;
    void Kill();

private:
    bool isAlive_ = false;
};
