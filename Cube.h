#pragma once
#include <functional>
#include "Transform.h"
#include "GameObject.h"

class DxLibCommon;

class Cube {
public:
    GameObject   gameObject;
    Transform    transform;
    unsigned int color = 0xFFFFFF;

    // Unity の OnCollisionXxx に相当するコールバック。引数は衝突相手の GameObject
    std::function<void(GameObject)> onCollisionEnter; // 衝突開始フレーム
    std::function<void(GameObject)> onCollisionStay;  // 衝突継続中の毎フレーム
    std::function<void(GameObject)> onCollisionExit;  // 衝突終了フレーム

    void Initialize(DxLibCommon* dxLib);
    void Draw(DxLibCommon* dxLib) const;
};
