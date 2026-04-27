#include <Windows.h>
#include <cassert>
#include <cmath>
#include <format>

#include "DxLibCommon.h"
#include "Vector3.h"
#include "Cube.h"



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

    // キューブの生成と初期化
    Cube cube;
    cube.transform.position   = { 0.0f, 0.0f, 0.0f };
    cube.transform.localScale = { 2.0f, 2.0f, 2.0f };
    cube.Initialize(dxLibCommon);

    // メインループ
    while (dxLibCommon->Running())
    {

        dxLibCommon->PreDraw();

        // 描画ここから↓

        dxLibCommon->SetCamera({ 0.0f, 5.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
        cube.Draw(dxLibCommon);

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