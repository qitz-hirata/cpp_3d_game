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

	// ライトをカメラ方向から当たるよう設定（デフォルトの真下ライトだとカメラ側の面が暗くなる）
	ChangeLightTypeDir(VGet(0.0f, -1.0f, 1.0f));

	// カメラのクリップ距離を明示設定
	SetCameraNearFar(0.1f, 1000.0f);
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

unsigned int DxLibCommon::MakeColor(int r, int g, int b) {
	return GetColor(r, g, b);
}

void DxLibCommon::SetCamera(Vector3 position, Vector3 target) {
	SetCameraPositionAndTarget_UpVecY(
		VGet(position.x, position.y, position.z),
		VGet(target.x,   target.y,   target.z)
	);
}

void DxLibCommon::DrawCube(Vector3 position, Vector3 scale, unsigned int color) {
	VECTOR minPos = VGet(position.x - scale.x * 0.5f, position.y - scale.y * 0.5f, position.z - scale.z * 0.5f);
	VECTOR maxPos = VGet(position.x + scale.x * 0.5f, position.y + scale.y * 0.5f, position.z + scale.z * 0.5f);
	DrawCube3D(minPos, maxPos, color, GetColor(255, 255, 255), TRUE);
}

bool DxLibCommon::IsKeyDown(GameKey key) {
	switch (key) {
		case GameKey::Left:  return CheckHitKey(KEY_INPUT_LEFT)  != 0;
		case GameKey::Right: return CheckHitKey(KEY_INPUT_RIGHT) != 0;
		case GameKey::Up:    return CheckHitKey(KEY_INPUT_UP)    != 0;
		case GameKey::Down:  return CheckHitKey(KEY_INPUT_DOWN)  != 0;
		case GameKey::W:     return CheckHitKey(KEY_INPUT_W)     != 0;
		case GameKey::A:     return CheckHitKey(KEY_INPUT_A)     != 0;
		case GameKey::S:     return CheckHitKey(KEY_INPUT_S)     != 0;
		case GameKey::D:     return CheckHitKey(KEY_INPUT_D)     != 0;
		default:             return false;
	}
}

#ifdef _DEBUG
bool DxLibCommon::IsCurrentDxLibVer(int requireVersion)
{
	return requireVersion == DXLIB_VERSION;
}


#endif // _DEBUG