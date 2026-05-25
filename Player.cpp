#include "Player.h"
#include "DxLibCommon.h"
#include "BulletManager.h"

void Player::Initialize(DxLibCommon* dxLib) {
    cube.transform.localScale = { 2.0f, 2.0f, 2.0f };
    cube.color = dxLib->MakeColor(255, 200, 100);
}

void Player::Update(DxLibCommon* dxLib, BulletManager& bulletManager, float deltaTime) {
    if (dxLib->IsKeyDown(GameKey::Left))  { cube.transform.position.x -= kMoveSpeed * deltaTime; }
    if (dxLib->IsKeyDown(GameKey::Right)) { cube.transform.position.x += kMoveSpeed * deltaTime; }
    if (dxLib->IsKeyDown(GameKey::Up))    { cube.transform.position.z += kMoveSpeed * deltaTime; }
    if (dxLib->IsKeyDown(GameKey::Down))  { cube.transform.position.z -= kMoveSpeed * deltaTime; }

    bool spaceDown = dxLib->IsKeyDown(GameKey::Space);
    if (spaceDown && !spaceWasDown_) {
        bulletManager.Add(dxLib, cube.transform.position, BulletOwner::Ally, { 0.0f, 0.0f, -kBulletSpeed }, kBulletLife);
    }
    spaceWasDown_ = spaceDown;
}

void Player::Draw(DxLibCommon* dxLib) const {
    cube.Draw(dxLib);
}
