#ifndef ENEMY_H_
#define ENEMY_H_	
#include "DxLib.h"

struct Enemy
{
public:

	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化処理
	void Init();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	
	VECTOR m_position;
	int m_modelHandle;	///< モデルハンドル

};
#endif