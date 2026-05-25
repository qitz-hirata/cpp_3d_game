#include <Windows.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <format>

#include "DxLibCommon.h"
#include "Vector3.h"
#include "Cube.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"



static const int kColumnWidth = 60;
static const int kRowHeight = 20;

void VectorScreenPrintf(DxLibCommon* dxLibCommon, int x, int y, const Vector3& vector, const char* label) {
    dxLibCommon->DrawLabel(std::format("{:.2f}", vector.x).c_str(), x, y);
    dxLibCommon->DrawLabel(std::format("{:.2f}", vector.y).c_str(), x + kColumnWidth, y);
    dxLibCommon->DrawLabel(std::format("{:.2f}", vector.z).c_str(), x + kColumnWidth * 2, y);
    dxLibCommon->DrawLabel(label, x + kColumnWidth * 3, y);
}

static const int requireDxLibVer = 0x324f;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // DxLibのラッパクラスを生成
    DxLibCommon* dxLibCommon = new DxLibCommon();
#ifdef _DEBUG
    // バージョン確認
    assert(dxLibCommon->IsCurrentDxLibVer(requireDxLibVer));
#endif // _DEBUG

    // dxLibの初期化処理
    dxLibCommon->Initialize();
    //初期化に失敗したら終了
    // 「初期化処理」と「初期化の成否」で2つの仕事を行なっているので分離
    if (dxLibCommon->FailInitialize())
    {
        // 初期化失敗したとしてもちゃんとdelete
        delete dxLibCommon;
        dxLibCommon = nullptr;
        return -1;
    }

    // キューブ1の生成と初期化 (矢印キーで操作)
    Cube cube;
    cube.transform.position   = { 0.0f, 0.0f, 0.0f };
    cube.transform.localScale = { 2.0f, 2.0f, 2.0f };
    cube.Initialize(dxLibCommon);
    unsigned int defaultColor = cube.color;

    // キューブ2の生成と初期化 (固定)
    Cube cube2;
    cube2.transform.position   = { 4.0f, 0.0f, 0.0f };
    cube2.transform.localScale = { 2.0f, 2.0f, 2.0f };
    cube2.Initialize(dxLibCommon);
    unsigned int defaultColor2 = cube2.color;

    unsigned int hitColor = dxLibCommon->MakeColor(255, 50, 50);

    // コリジョンコールバックを登録
    cube.onCollisionEnter = [&](GameObject other) { cube.color  = hitColor;     };
    cube.onCollisionExit  = [&](GameObject other) { cube.color  = defaultColor; };
    cube2.onCollisionEnter = [&](GameObject other) { cube2.color = hitColor;     };
    cube2.onCollisionExit  = [&](GameObject other) { cube2.color = defaultColor2; };

    // Cube を個別に登録 (登録済みの全 Cube とのペアが自動生成される)
    CollisionManager collisionManager;
    //collisonManagerで当たり判定を登録
    collisionManager.Register(&cube);
    collisionManager.Register(&cube2);

    // 試験用Bullet: 原点から-Z方向へ飛ぶ、生存時間5秒
    BulletManager bulletManager;
    bulletManager.Add(dxLibCommon, { 0.0f, 0.0f, 0.0f }, BulletOwner::Ally, { 0.0f, 0.0f, -5.0f }, 5.0f);

    // 試験用Enemy: Z=-8 から+Z方向へゆっくり移動
    EnemyManager enemyManager;
    enemyManager.Add(dxLibCommon, { 0.0f, 0.0f, -8.0f }, { 0.0f, 0.0f, 2.0f });

    auto prevTime = std::chrono::steady_clock::now();

    static const float kMoveSpeed      = 0.05f;
    static const float kCameraSpeed    = 0.02f;
    static const float kCameraDistance = sqrtf(125.0f); // 初期位置 {0,5,-10} の距離
    float cameraYaw   = 0.0f;
    float cameraPitch = atan2f(5.0f, 10.0f); // 初期仰角

    // メインループ
    while (dxLibCommon->Running())
    {
        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - prevTime).count();
        prevTime = now;

        // 矢印キーでCubeを移動 (左右=X軸, 上下=Z軸)
        if (dxLibCommon->IsKeyDown(GameKey::Left))  { cube.transform.position.x -= kMoveSpeed; }
        if (dxLibCommon->IsKeyDown(GameKey::Right)) { cube.transform.position.x += kMoveSpeed; }
        if (dxLibCommon->IsKeyDown(GameKey::Up))    { cube.transform.position.z += kMoveSpeed; }
        if (dxLibCommon->IsKeyDown(GameKey::Down))  { cube.transform.position.z -= kMoveSpeed; }

        // WASDでカメラを回転 (A/D=Y軸, W/S=X軸)
        /* カメラポジションの変更処理コメントアウト
        
        if (dxLibCommon->IsKeyDown(GameKey::A)) { cameraYaw   -= kCameraSpeed; }
        if (dxLibCommon->IsKeyDown(GameKey::D)) { cameraYaw   += kCameraSpeed; }
        if (dxLibCommon->IsKeyDown(GameKey::W)) { cameraPitch += kCameraSpeed; }
        if (dxLibCommon->IsKeyDown(GameKey::S)) { cameraPitch -= kCameraSpeed; }
        cameraPitch = std::clamp(cameraPitch, -1.5f, 1.5f);

        Vector3 cameraPos = {
            kCameraDistance * sinf(cameraYaw) * cosf(cameraPitch),
            kCameraDistance * sinf(cameraPitch),
           -kCameraDistance * cosf(cameraYaw) * cosf(cameraPitch)
        };
        */

        bulletManager.Update(deltaTime);
        enemyManager.Update(deltaTime, bulletManager);

        collisionManager.Update();

        dxLibCommon->PreDraw();

        // 描画ここから↓

        dxLibCommon->SetCamera(cameraPos, { 0.0f, 0.0f, 0.0f });
        cube.Draw(dxLibCommon);
        cube2.Draw(dxLibCommon);
        bulletManager.Draw(dxLibCommon);
        enemyManager.Draw(dxLibCommon);

        // 描画ここまで↑

        // 描画の後処理
        dxLibCommon->PostDraw();

    }

    // 終了処理
    dxLibCommon->Shutdown();

    delete dxLibCommon; 
    dxLibCommon = nullptr;


    // ソフトの終了
    return 0;
}