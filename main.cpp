#include <Novice.h>
#include <assert.h>
#include <ResourceIndexer.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LC1C_25_ミヤモト_ナオヤ_ゲームタイトル";

struct Vector2
{
	float x;
	float y;
};

struct Vector2int
{
	int x;
	int y;
};

struct Matrix3x3
{
	float m[3][3];
};

Matrix3x3 MakeAffineMatrix(Vector2 scale, float theta, Vector2 translate)
{
	Matrix3x3 resultMatrix;
	resultMatrix.m[0][0] = scale.x * cosf(theta);
	resultMatrix.m[0][1] = scale.x * sinf(theta);
	resultMatrix.m[0][2] = 0;
	resultMatrix.m[1][0] = scale.y * -sinf(theta);
	resultMatrix.m[1][1] = scale.y * cosf(theta);
	resultMatrix.m[1][2] = 0;
	resultMatrix.m[2][0] = translate.x;
	resultMatrix.m[2][1] = translate.y;
	resultMatrix.m[2][2] = 1;
	return resultMatrix;
};

Vector2 Transform(Vector2 vector, Matrix3x3 matrix)
{
	Vector2 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
};

void QUAD(Vector2 lefttop, Vector2 righttop, Vector2 leftbottom, Vector2 rightbottom, int grahghundle);

struct Player
{
	Vector2 pos;
	Vector2 vel;
	Vector2 acc;
	Vector2 radius;
	bool flag;
	Vector2 scale;
	float scaleSpeed;
	float theta;
	int isPlayerAlive;
};

Player Player1 =
{
	{100,100},
	{0,0},
	{0,0.8f},
	{16,16},
	true,
	{1.0f,1.0f},
	0.1f,
	0.0f,
	1,
};

Player Player2 =
{
	{200,600},
	{0,0},
	{0,0.8f},
	{16,16},
	false,
	{1.0f,1.0f},
	0.1f,
	0.0f,
	1,
};

///
///プレイヤー1原点基準の四隅の座標(回転用)
/// 

Vector2 Player1_Olefttop = { 0 - Player1.radius.x,0 - Player1.radius.y };

Vector2 Player1_Orighttop = { 0 + Player1.radius.x,0 - Player1.radius.y };

Vector2 Player1_Oleftbottom = { 0 - Player1.radius.x,0 + Player1.radius.y };

Vector2 Player1_Orightbottom = { 0 + Player1.radius.x,0 + Player1.radius.y };


///
///プレイヤー2原点基準の四隅の座標(回転用)
/// 

Vector2 Player2_Olefttop = { 0 - Player2.radius.x,0 - Player2.radius.y };

Vector2 Player2_Orighttop = { 0 + Player2.radius.x,0 - Player2.radius.y };

Vector2 Player2_Oleftbottom = { 0 - Player2.radius.x,0 + Player2.radius.y };

Vector2 Player2_Orightbottom = { 0 + Player2.radius.x,0 + Player2.radius.y };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	///
	///Resource置き場ここから↓↓↓
	///

	int  whiteblock = Novice::LoadTexture("white1x1.png");///仮置き

	int red = Novice::LoadTexture("./Resources/images/red1x1.png");///仮置き

	int blue = Novice::LoadTexture("./Resources/images/blue1x1.png");///仮置き

	int subred = Novice::LoadTexture("./Resources/images/subred1x1.png");

	int changeblock = Novice::LoadTexture("./Resources/images/changescaleblock.png");

	///
	///Resource置き場ここまで↑↑↑
	///

	///
	///変数初期化ここから↓↓↓
	///

	int scene = 0;

	enum scene
	{
		Title,
		StageSelect,
		Stage1,
		Stage2,
		Stage3,
		GameOver,
		GameClear,
	};

	int scaletimer = 0;

	///
	///変数初期化ここまで↑↑↑
	///

	int mapchip[23][52] =
	{
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		///プレイヤー1の四隅の座標
		/// 

		Vector2 Player1_Lefttop = { Player1.pos.x - Player1.radius.x,Player1.pos.y - Player1.radius.y };///プレイヤー1左上

		Vector2 Player1_Righttop = { Player1.pos.x + 15,Player1.pos.y - Player1.radius.y };///プレイヤー1右上

		Vector2 Player1_Leftbottom = { Player1.pos.x - Player1.radius.x,Player1.pos.y + 15 };///プレイヤー1左下

		Vector2 Player1_Rightbottom = { Player1.pos.x + 15,Player1.pos.y + 15 };///プレイヤー1右下

		///
		///プレイヤー1の四隅のマップチップ
		/// 

		Vector2int Player1_Lefttopmap = { int(Player1_Lefttop.x) / 32,int(Player1_Lefttop.y) / 32 };

		Vector2int Player1_Righttopmap = { int(Player1_Righttop.x) / 32,int(Player1_Righttop.y) / 32 };

		Vector2int Player1_Leftbottommap = { int(Player1_Leftbottom.x) / 32,int(Player1_Leftbottom.y) / 32 };

		Vector2int Player1_Rightbottommap = { int(Player1_Rightbottom.x) / 32, int(Player1_Rightbottom.y) / 32 };

		///
		///プレイヤー1の影
		/// 

		Vector2 SScale1 = { 0,0 };

		Vector2 SPlayer1_Lefttop = { (Player1.pos.x - Player1.radius.x) - SScale1.x,(Player1.pos.y - Player1.radius.y) + 353 - SScale1.y };///プレイヤー1左上 S = Shadow

		Vector2 SPlayer1_Righttop = { (Player1.pos.x + 15) + SScale1.x,(Player1.pos.y - Player1.radius.y) + 353 - SScale1.y };///プレイヤー1右上

		Vector2 SPlayer1_Leftbottom = { (Player1.pos.x - Player1.radius.x) - SScale1.x,(Player1.pos.y + 15) + 353 + SScale1.y };///プレイヤー1左下

		Vector2 SPlayer1_Rightbottom = { (Player1.pos.x + 15) + SScale1.x,(Player1.pos.y + 15) + 353 + SScale1.y };///プレイヤー1右下

		///
		///プレイヤー2の四隅の座標
		/// 

		Vector2 Player2_Lefttop = { Player2.pos.x - Player2.radius.x,Player2.pos.y - Player2.radius.y };///プレイヤー2左上

		Vector2 Player2_Righttop = { Player2.pos.x + 15,Player2.pos.y - Player2.radius.y };///プレイヤー2右上

		Vector2 Player2_Leftbottom = { Player2.pos.x - Player2.radius.x,Player2.pos.y + 15 };///プレイヤー2左下

		Vector2 Player2_Rightbottom = { Player2.pos.x + 15,Player2.pos.y + 15 };///プレイヤー2右下

		///
		///プレイヤー2の四隅のマップチップ
		/// 

		Vector2int Player2_Lefttopmap = { int(Player2_Lefttop.x) / 32,int(Player2_Lefttop.y) / 32 };

		Vector2int Player2_Righttopmap = { int(Player2_Righttop.x) / 32,int(Player2_Righttop.y) / 32 };

		Vector2int Player2_Leftbottommap = { int(Player2_Leftbottom.x) / 32,int(Player2_Leftbottom.y) / 32 };

		Vector2int Player2_Rightbottommap = { int(Player2_Rightbottom.x) / 32, int(Player2_Rightbottom.y) / 32 };




		switch (scene)
		{
		case Title:

			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0)
			{
				scene = Stage1;
			}

			break;

		case StageSelect:

			break;

		case Stage1:

			//プレイヤーの変更



			///プレイヤ－1フラグ

			if (Player1.flag == true)
			{
				if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0)
				{
					Player1.flag = false;
					Player2.flag = true;
				}

				//-----プレイヤー１生存フラグ
				if (Player1.isPlayerAlive == 0)
				{
					if (Player1.scale.x <= 0.5f) { Player1.scaleSpeed = +0.1f; }
					if (Player1.scale.x >= 2.0f) { Player1.scaleSpeed = -0.1f; }

					if (Player1.scale.y <= 0.5f) { Player1.scaleSpeed = +0.1f; }
					if (Player1.scale.y >= 2.0f) { Player1.scaleSpeed = -0.1f; }

					Player1.scale.x += Player1.scaleSpeed;
					Player1.scale.y += Player1.scaleSpeed;

					Player1.theta -= 1.0f / 128.0f * (float)M_PI;
				}

				///
				///移動処理ここから↓↓↓
				/// 

				///左移動の処理

				if (keys[DIK_A])
				{
					Player1.vel.x = -4;

					Vector2int Player1_preLefttopmap = { int(Player1_Lefttop.x + Player1.vel.x) / 32,int(Player1_Lefttop.y) / 32 };///プレイヤーの1マス左のマップチップ(左上)

					Vector2int Player1_preLeftbottommap = { int(Player1_Leftbottom.x + Player1.vel.x) / 32,int(Player1_Leftbottom.y) / 32 };///プレイヤーの1マス左のマップチップ(左下)

					if (mapchip[Player1_preLefttopmap.y][Player1_preLefttopmap.x] == 0 && mapchip[Player1_preLeftbottommap.y][Player1_preLeftbottommap.x] == 0)
					{
						Player1.pos.x += Player1.vel.x;
					}
					if (mapchip[Player1_preLefttopmap.y][Player1_preLefttopmap.x] == 2 && mapchip[Player1_preLeftbottommap.y][Player1_preLeftbottommap.x] == 2)
					{
						Player1.pos.x += Player1.vel.x;
					}
				}

				///右移動の処理

				if (keys[DIK_D])
				{
					Player1.vel.x = 4;

					Vector2int Player1_preRighttopmap = { int(Player1_Righttop.x + Player1.vel.x) / 32,int(Player1_Righttop.y) / 32 };///プレイヤーの1マス右のマップチップ(右上)

					Vector2int Player1_preRightbottommap = { int(Player1_Rightbottom.x + Player1.vel.x) / 32,int(Player1_Rightbottom.y) / 32 };///プレイヤーの1マス右のマップチップ(右下)

					if (mapchip[Player1_preRighttopmap.y][Player1_preRighttopmap.x] == 0 && mapchip[Player1_preRightbottommap.y][Player1_preRightbottommap.x] == 0)
					{
						Player1.pos.x += Player1.vel.x;
					}
					if (mapchip[Player1_preRighttopmap.y][Player1_preRighttopmap.x] == 2 && mapchip[Player1_preRightbottommap.y][Player1_preRightbottommap.x] == 2)
					{
						Player1.pos.x += Player1.vel.x;
					}
				}

				///落下処理

				Player1.pos.y += Player1.vel.y;

				Vector2int Player1_preleftBottommap = { int(Player1_Leftbottom.x) / 32,int(Player1_Leftbottom.y + Player1.vel.y) / 32 };///プレイヤー1の1マス下のマップチップ(左下)

				Vector2int Player1_prerightBottommap = { int(Player1_Rightbottom.x) / 32,int(Player1_Rightbottom.y + Player1.vel.y) / 32 };///プレイヤー1の1マス下のマップチップ(右下)

				if (mapchip[Player1_preleftBottommap.y][Player1_preleftBottommap.x] == 0 && mapchip[Player1_prerightBottommap.y][Player1_prerightBottommap.x] == 0 &&
					mapchip[int(Player1_Leftbottom.y + 1) / 32][int(Player1_Leftbottom.x) / 32] == 0 && mapchip[int(Player1_Rightbottom.y + 1) / 32][int(Player1_Rightbottom.x) / 32] == 0)
				{
					Player1.vel.y += Player1.acc.y;
				}
				else if (mapchip[Player1_preleftBottommap.y][Player1_preleftBottommap.x] == 1 && mapchip[Player1_prerightBottommap.y][Player1_prerightBottommap.x] == 1)
				{
					Player1.pos.y = float(Player1_preleftBottommap.y - 1) * 32 + 16;

					Player1.vel.y = 0;
				}
				else if (mapchip[Player1_preleftBottommap.y][Player1_preleftBottommap.x] == 1)
				{
					Player1.pos.y = float(Player1_preleftBottommap.y - 1) * 32 + 16;

					Player1.vel.y = 0;
				}
				else if (mapchip[Player1_prerightBottommap.y][Player1_prerightBottommap.x] == 1)
				{
					Player1.pos.y = float(Player1_prerightBottommap.y - 1) * 32 + 16;

					Player1.vel.y = 0;
				}

				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && Player1.vel.y == 0)
				{
					Player1.vel.y = -15;
				}

				///
				///移動処理ここまで↑↑↑
				/// 



			}


			///プレイヤー2フラグ

			else if (Player2.flag == true)
			{

				if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0)
				{
					Player2.flag = false;
					Player1.flag = true;
				}

				//-----プレイヤー2生存フラグ
				if (Player2.isPlayerAlive == 0)
				{
					if (Player2.scale.x <= 0.5f) { Player2.scaleSpeed = +0.1f; }
					if (Player2.scale.x >= 2.0f) { Player2.scaleSpeed = -0.1f; }

					if (Player2.scale.y <= 0.5f) { Player2.scaleSpeed = +0.1f; }
					if (Player2.scale.y >= 2.0f) { Player2.scaleSpeed = -0.1f; }

					Player2.scale.x += Player2.scaleSpeed;
					Player2.scale.y += Player2.scaleSpeed;

					Player2.theta -= 1.0f / 128.0f * (float)M_PI;
				}

				///
				///移動処理ここから↓↓↓
				/// 

				///左移動の処理

				if (keys[DIK_A])
				{
					Player2.vel.x = -4;

					Vector2int Player2_preLefttopmap = { int(Player2_Lefttop.x + Player2.vel.x) / 32,int(Player2_Lefttop.y) / 32 };///プレイヤーの1マス左のマップチップ(左上)

					Vector2int Player2_preLeftbottommap = { int(Player2_Leftbottom.x + Player2.vel.x) / 32,int(Player2_Leftbottom.y) / 32 };///プレイヤーの1マス左のマップチップ(左下)

					if (SPlayer1_Lefttop.y > Player2_Rightbottom.y || SPlayer1_Leftbottom.y < Player2_Righttop.y)
					{
						if (mapchip[Player2_preLefttopmap.y][Player2_preLefttopmap.x] == 0 && mapchip[Player2_preLeftbottommap.y][Player2_preLeftbottommap.x] == 0)
						{
							Player2.pos.x += Player2.vel.x;
						}
						else if (mapchip[Player2_preLefttopmap.y][Player2_preLefttopmap.x] == 2 && mapchip[Player2_preLeftbottommap.y][Player2_preLeftbottommap.x] == 2)
						{
							Player2.pos.x += Player2.vel.x;
						}
					}
					else if (SPlayer1_Righttop.y < Player2_Leftbottom.y || SPlayer1_Rightbottom.y > Player2_Lefttop.y)
					{
						if (SPlayer1_Righttop.y < Player2_Leftbottom.y)
						{
							if (SPlayer1_Lefttop.x > Player2_Rightbottom.x || SPlayer1_Righttop.x < Player2_Leftbottom.x + Player2.vel.y)
							{
								if (mapchip[Player2_preLefttopmap.y][Player2_preLefttopmap.x] == 0 && mapchip[Player2_preLeftbottommap.y][Player2_preLeftbottommap.x] == 0)
								{
									Player2.pos.x += Player2.vel.x;
								}
								else if (mapchip[Player2_preLefttopmap.y][Player2_preLefttopmap.x] == 2 && mapchip[Player2_preLeftbottommap.y][Player2_preLeftbottommap.x] == 2)
								{
									Player2.pos.x += Player2.vel.x;
								}
							}
							else if (SPlayer1_Righttop.x > Player2_Leftbottom.x + Player2.vel.y)
							{
								Player2.pos.x = Player1.pos.x + 31 + SScale1.x;
							}
						}
						else if (SPlayer1_Rightbottom.y > Player2_Lefttop.y)
						{
							if (SPlayer1_Leftbottom.x > Player2_Righttop.x || SPlayer1_Rightbottom.x < Player2_Lefttop.x + Player2.vel.y)
							{
								if (mapchip[Player2_preLefttopmap.y][Player2_preLefttopmap.x] == 0 && mapchip[Player2_preLeftbottommap.y][Player2_preLeftbottommap.x] == 0)
								{
									Player2.pos.x += Player2.vel.x;
								}
								else if (mapchip[Player2_preLefttopmap.y][Player2_preLefttopmap.x] == 2 && mapchip[Player2_preLeftbottommap.y][Player2_preLeftbottommap.x] == 2)
								{
									Player2.pos.x += Player2.vel.x;
								}
							}
							else if (SPlayer1_Rightbottom.x < Player2_Lefttop.x + Player2.vel.y)
							{
								Player2.pos.x = Player1.pos.x + 31 + SScale1.x;
							}
						}

					}
					

				}

				///右移動の処理

				if (keys[DIK_D])
				{
					Player2.vel.x = 4;

					Vector2int Player2_preRighttopmap = { int(Player2_Righttop.x + Player2.vel.x) / 32,int(Player2_Righttop.y) / 32 };///プレイヤーの1マス右のマップチップ(右上)

					Vector2int Player2_preRightbottommap = { int(Player2_Rightbottom.x + Player2.vel.x) / 32,int(Player2_Rightbottom.y) / 32 };///プレイヤーの1マス右のマップチップ(右下)

					if (SPlayer1_Lefttop.y > Player2_Rightbottom.y || SPlayer1_Leftbottom.y < Player2_Righttop.y)
					{
						if (mapchip[Player2_preRighttopmap.y][Player2_preRighttopmap.x] == 0 && mapchip[Player2_preRightbottommap.y][Player2_preRightbottommap.x] == 0)
						{
							Player2.pos.x += Player2.vel.x;
						}
						else if (mapchip[Player2_preRighttopmap.y][Player2_preRighttopmap.x] == 2 && mapchip[Player2_preRightbottommap.y][Player2_preRightbottommap.x] == 2)
						{
							Player2.pos.x += Player2.vel.x;
						}
					}
					else if (SPlayer1_Lefttop.y < Player2_Rightbottom.y || SPlayer1_Leftbottom.y > Player2_Righttop.y)
					{
						if (SPlayer1_Lefttop.y < Player2_Rightbottom.y)
						{
							if (SPlayer1_Righttop.x < Player2_Leftbottom.x || SPlayer1_Lefttop.x > Player2_Rightbottom.x + Player2.vel.y)
							{
								if (mapchip[Player2_preRighttopmap.y][Player2_preRighttopmap.x] == 0 && mapchip[Player2_preRightbottommap.y][Player2_preRightbottommap.x] == 0)
								{
									Player2.pos.x += Player2.vel.x;
								}
								else if (mapchip[Player2_preRighttopmap.y][Player2_preRighttopmap.x] == 2 && mapchip[Player2_preRightbottommap.y][Player2_preRightbottommap.x] == 2)
								{
									Player2.pos.x += Player2.vel.x;
								}
							}
							else if (SPlayer1_Lefttop.x < Player2_Rightbottom.x + Player2.vel.y)
							{
								Player2.pos.x = Player1.pos.x - 35 - SScale1.x;
							}
						}
						else if (SPlayer1_Leftbottom.y > Player2_Righttop.y)
						{
							if (SPlayer1_Rightbottom.x > Player2_Lefttop.x || SPlayer1_Leftbottom.x < Player2_Righttop.x + Player2.vel.y)
							{
								if (mapchip[Player2_preRighttopmap.y][Player2_preRighttopmap.x] == 0 && mapchip[Player2_preRightbottommap.y][Player2_preRightbottommap.x] == 0)
								{
									Player2.pos.x += Player2.vel.x;
								}
								else if (mapchip[Player2_preRighttopmap.y][Player2_preRighttopmap.x] == 2 && mapchip[Player2_preRightbottommap.y][Player2_preRightbottommap.x] == 2)
								{
									Player2.pos.x += Player2.vel.x;
								}
							}
							else if (SPlayer1_Leftbottom.x < Player2_Righttop.x + Player2.vel.y)
							{
								Player2.pos.x = Player1.pos.x + 35 + SScale1.x;
							}
						}

					}
				}

				///落下処理

				Player2.pos.y += Player2.vel.y;

				Vector2int Player2_preleftBottommap = { int(Player2_Leftbottom.x) / 32,int(Player2_Leftbottom.y + Player2.vel.y) / 32 };///プレイヤー1の1マス下のマップチップ(左下)

				Vector2int Player2_prerightBottommap = { int(Player2_Rightbottom.x) / 32,int(Player2_Rightbottom.y + Player2.vel.y) / 32 };///プレイヤー1の1マス下のマップチップ(右下)

				if (mapchip[Player2_preleftBottommap.y][Player2_preleftBottommap.x] == 0 && mapchip[Player2_prerightBottommap.y][Player2_prerightBottommap.x] == 0 &&
					mapchip[int(Player2_Leftbottom.y + 1) / 32][int(Player2_Leftbottom.x) / 32] == 0 && mapchip[int(Player2_Rightbottom.y + 1) / 32][int(Player2_Rightbottom.x) / 32] == 0)
				{
					Player2.vel.y += Player2.acc.y;
				}
				else if (mapchip[Player2_preleftBottommap.y][Player2_preleftBottommap.x] == 1 && mapchip[Player2_prerightBottommap.y][Player2_prerightBottommap.x] == 1)
				{
					Player2.pos.y = float(Player2_preleftBottommap.y - 1) * 32 + 16;

					Player2.vel.y = 0;
				}
				else if (mapchip[Player2_preleftBottommap.y][Player2_preleftBottommap.x] == 1)
				{
					Player2.pos.y = float(Player2_preleftBottommap.y - 1) * 32 + 16;

					Player2.vel.y = 0;
				}
				else if (mapchip[Player2_prerightBottommap.y][Player2_prerightBottommap.x] == 1)
				{
					Player2.pos.y = float(Player2_prerightBottommap.y - 1) * 32 + 16;

					Player2.vel.y = 0;
				}

				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && Player2.vel.y == 0)
				{
					Player2.vel.y = -15;
				}

				///
				///移動処理ここまで↑↑↑
				/// 


			}




			break;

		case Stage2:

			///プレイヤ－1フラグ

			if (Player1.flag == true)
			{


				if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0)
				{
					Player1.flag = false;
					Player2.flag = true;
				}
			}
			else if (Player1.flag == false)
			{

			}

			///プレイヤー2フラグ

			if (Player2.flag == true)
			{


				if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0)
				{
					Player1.flag = true;
					Player2.flag = false;
				}
			}
			else if (Player2.flag == false)
			{

			}

			break;

		case Stage3:

			///プレイヤ－1フラグ

			if (Player1.flag == true)
			{


				if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0)
				{
					Player1.flag = false;
					Player2.flag = true;
				}
			}
			else if (Player1.flag == false)
			{

			}

			///プレイヤー2フラグ

			if (Player2.flag == true)
			{


				if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0)
				{
					Player1.flag = true;
					Player2.flag = false;
				}
			}
			else if (Player2.flag == false)
			{

			}

			break;

		case GameOver:

			break;

		case GameClear:

			break;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (scene)
		{
		case Title:

			break;

		case StageSelect:

			break;

		case Stage1:


			QUAD(Player1_Lefttop, Player1_Righttop, Player1_Leftbottom, Player1_Rightbottom, red);

			QUAD(Player2_Lefttop, Player2_Righttop, Player2_Leftbottom, Player2_Rightbottom, blue);

			QUAD(SPlayer1_Lefttop, SPlayer1_Righttop, SPlayer1_Leftbottom, SPlayer1_Rightbottom, subred);

			//-----上段のステージの描画-----

			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < 52; j++)
				{
					if (mapchip[i][j] == 1)
					{
						Novice::DrawSprite(j * 32, i * 32, whiteblock, 32, 32, 0.0f, WHITE);
					}

					///test

					if (mapchip[i][j] == 2)
					{
						scaletimer += 1;

						if (scaletimer == 200)
						{
							scaletimer = 0;
						}

						if (scaletimer < 50)
						{
							Novice::DrawSpriteRect(j * 32, i * 32, 0, 0, 32, 32, changeblock, 0.25f, 1, 0.0f, 0xFFFFFFFF);
						}
						else if (scaletimer < 100)
						{
							Novice::DrawSpriteRect(j * 32, i * 32, 32, 0, 32, 32, changeblock, 0.25f, 1, 0.0f, 0xFFFFFFFF);
						}
						else if (scaletimer < 150)
						{
							Novice::DrawSpriteRect(j * 32, i * 32, 64, 0, 32, 32, changeblock, 0.25f, 1, 0.0f, 0xFFFFFFFF);
						}
						else if (scaletimer < 200)
						{
							Novice::DrawSpriteRect(j * 32, i * 32, 96, 0, 32, 32, changeblock, 0.25f, 1, 0.0f, 0xFFFFFFFF);
						}

					}

				}
			}

			//-----下段のステージの描画-----

			for (int i = 12; i < 23; i++)
			{
				for (int j = 0; j < 52; j++)
				{
					if (mapchip[i][j] == 1)
					{
						Novice::DrawSprite(j * 32, i * 32, whiteblock, 32, 32, 0.0f, WHITE);
					}

				}
			}




			if (Player1.flag == true)
			{
				Novice::ScreenPrintf(100, 100, "Player1.vel.x = %f", Player1.vel.x);
			}
			else if (Player2.flag == true)
			{
				Novice::ScreenPrintf(100, 100, "Player2.vel.x = %f", Player2.vel.x);
			}

			Novice::ScreenPrintf(100, 200, "%f", Player1.pos.x);

			Novice::ScreenPrintf(100, 300, "%f", Player1_Rightbottom.x);

			break;

		case Stage2:

			break;

		case Stage3:

			break;

		case GameOver:

			break;

		case GameClear:

			break;
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void QUAD(Vector2 lefttop, Vector2 righttop, Vector2 leftbottom, Vector2 rightbottom, int grahghundle)
{
	Novice::DrawQuad(int(lefttop.x), int(lefttop.y), int(righttop.x), int(righttop.y), int(leftbottom.x), int(leftbottom.y), int(rightbottom.x), int(rightbottom.y), 0, 0, 32, 32, grahghundle, WHITE);

}
