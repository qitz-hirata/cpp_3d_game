#pragma once
#include <string>
#include "Vector3.h"

/// <summary>
/// 神がかってるヘッダ「DxLib.h」をmainで読み込ませないためのクラス
/// </summary>
class DxLibCommon
{
public:
	DxLibCommon();
	~DxLibCommon();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 初期化処理の成否
	/// </summary>
	/// <returns>成功したか否か</returns>
	bool FailInitialize();

	/// <summary>
	/// メインループを回し続けるか否か
	/// </summary>
	/// <returns>実行許可</returns>
	bool Running();

	/// /// <summary>
	/// 主に画面の初期化
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 主にScreenFlip
	/// </summary>
	void PostDraw();
	
	/// <summary>
	/// 終了処理
	/// </summary>
	void Shutdown();

	/// <summary>
	/// 指定座標を中心に文字列を描画する
	/// </summary>
	/// <param name="str">描画する文字列</param>
	/// <param name="x">中心X座標</param>
	/// <param name="y">中心Y座標</param>
	void DrawLabel(const char* str, int x, int y);

	int GetScreenWidth() const;
	int GetScreenHeight() const;

	unsigned int MakeColor(int r, int g, int b);
	void SetCamera(Vector3 position, Vector3 target);
	void DrawCube(Vector3 position, Vector3 scale, unsigned int color);

	bool IsLeftKeyDown();
	bool IsRightKeyDown();

#ifdef _DEBUG
	/// <summary>
	/// DxLibのバージョンチェック用。Releaseでは不要
	/// </summary>
	/// <param name="requireVersion">要求するDxLibのバージョン</param>
	/// <returns>一致するか否か</returns>
	bool IsCurrentDxLibVer(int requireVersion);
#endif // _DEBUG

private:
	bool failInitialize = false;
	int screenWidth_ = 0;
	int screenHeight_ = 0;
};