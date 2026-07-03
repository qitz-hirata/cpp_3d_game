#include <Windows.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <format>
//ローカルで変更！
#include "DxLibCommon.h"
#include "Vector3.h"
#include "Cube.h"
#include "Player.h"
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

    // プレイヤーの生成と初期化
    Player player;
    player.cube.transform.position = { 0.0f, 0.0f, 0.0f };
    player.Initialize(dxLibCommon);
    unsigned int defaultPlayerColor = player.cube.color;



    unsigned int hitColor = dxLibCommon->MakeColor(255, 50, 50);

    // コリジョンコールバックを登録
    player.cube.onCollisionEnter = [&](GameObject other) { player.cube.color = hitColor;            };
    player.cube.onCollisionExit  = [&](GameObject other) { player.cube.color = defaultPlayerColor;  };


    // Cube を個別に登録 (登録済みの全 Cube とのペアが自動生成される)
    CollisionManager collisionManager;
    collisionManager.Register(&player.cube);


    BulletManager bulletManager;

    EnemyManager enemyManager;
    enemyManager.LoadFromCSV(dxLibCommon);

    auto prevTime = std::chrono::steady_clock::now();

    static const float kCameraDistance = sqrtf(125.0f); // 初期位置 {0,5,-10} の距離
    float cameraYaw   = 0.0f;
    float cameraPitch = atan2f(5.0f, 10.0f); // 初期仰角

    // メインループ
    while (dxLibCommon->Running())
    {
        auto now = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(now - prevTime).count();
        prevTime = now;

        player.Update(dxLibCommon, bulletManager, deltaTime);

        Vector3 cameraPos = {
            kCameraDistance * sinf(cameraYaw) * cosf(cameraPitch),
            kCameraDistance * sinf(cameraPitch),
           -kCameraDistance * cosf(cameraYaw) * cosf(cameraPitch)
        };
        

        bulletManager.Update(deltaTime);
        enemyManager.Update(deltaTime, bulletManager);

        collisionManager.Update();

        dxLibCommon->PreDraw();

        // 描画ここから↓

        dxLibCommon->SetCamera(cameraPos, { 0.0f, 0.0f, 0.0f });
        player.Draw(dxLibCommon);
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