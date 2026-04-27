#include "Cube.h"
#include "DxLibCommon.h"

void Cube::Initialize(DxLibCommon* dxLib) {
    color = dxLib->MakeColor(255, 200, 100);
}

void Cube::Draw(DxLibCommon* dxLib) const {
    dxLib->DrawCube(transform.position, transform.localScale, color);
}
