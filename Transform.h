#pragma once
#include <vector>
#include "Vector3.h"
#include "Quaternion.h"

class Transform {
public:
    // ワールド空間
    Vector3    position      = { 0.0f, 0.0f, 0.0f };
    Quaternion rotation      = { 0.0f, 0.0f, 0.0f, 1.0f }; // identity

    // 親からの相対値
    Vector3    localPosition = { 0.0f, 0.0f, 0.0f };
    Quaternion localRotation = { 0.0f, 0.0f, 0.0f, 1.0f }; // identity
    Vector3    localScale    = { 1.0f, 1.0f, 1.0f };

    // 親子関係
    Transform*              parent   = nullptr;
    std::vector<Transform*> children = {};
};
