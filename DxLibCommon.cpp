#include "DxLibCommon.h"
// cppはincludeされないのでDxLib.hをincludeするのはセーフ
#include <DxLib.h>

DxLibCommon::DxLibCommon()
{
}

DxLibCommon::~DxLibCommon()
{
}

void DxLibCommon::Initialize()
{
	failInitialize = false;
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 背景色を群青色に設定
	SetBackgroundColor(43, 69, 144);

	// DxLibの初期化。初期化の成否についてsucceededInitializeに代入。
	// 「初期化処理」と「成否の取得」は別の仕事なので関数を分ける。
	failInitialize = DxLib_Init() == -1;

	GetDrawScreenSize(&screenWidth_, &screenHeight_);

	// Zバッファを有効にする
	SetUseZBuffer3D(TRUE);
	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(TRUE);

	// マテリアルの自己発光色を暗い青色にする
	MATERIALPARAM material;
	material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	material.Ambient = GetColorF(0.3f, 0.3f, 0.3f, 0.0f);
	material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	material.Power = 20.0f;
	SetMaterialParam(material);


}

bool DxLibCommon::FailInitialize()
{
	return failInitialize;
}

bool DxLibCommon::Running()
{
	// Windowから終了メッセージをもらったら終了
	if (ProcessMessage() != 0) { return  false; }
	// エスケープキーを受け取ったら終了
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { return false; }
	return true;
}

void DxLibCommon::PreDraw()
{
	//画面の消去
	ClearDrawScreen();

}

void DxLibCommon::PostDraw()
{
	// 裏画面の内容を表画面に反映する
	ScreenFlip();
}

void DxLibCommon::Shutdown()
{
	// ＤＸライブラリの後始末
	DxLib_End();
}

void DxLibCommon::DrawLabel(const char* str, int x, int y)
{
	DrawString(x, y, str, GetColor(255, 255, 255));
}

int DxLibCommon::GetScreenWidth() const { return screenWidth_; }
int DxLibCommon::GetScreenHeight() const { return screenHeight_; }

#ifdef _DEBUG
bool DxLibCommon::IsCurrentDxLibVer(int requireVersion)
{
	return requireVersion == DXLIB_VERSION;
}


#endif // _DEBUG