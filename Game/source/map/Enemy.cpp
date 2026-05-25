#include "enemy/Enemy.h"

Enemy::Enemy()
:m_modelHandle(-1)
{
		m_position = VGet(200.0f, 400.0f, 0.0f);	
}

Enemy::~Enemy()
{
	// ƒ‚ƒfƒ‹‚Ì‰ğ•ú
	if (m_modelHandle != -1)
	{
		MV1DeleteModel(m_modelHandle);
		m_modelHandle = -1;
	}
}

void Enemy::Init()
{
	// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	m_modelHandle = MV1LoadModel("Game/assets/models/enemy/enemy.mv1");
	if (m_modelHandle != -1)
	{
		MV1SetScale(m_modelHandle, VGet(0.01f, 0.01f, 0.01f));
	}
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	if (m_modelHandle != -1)
	{
		MV1SetPosition(m_modelHandle, m_position);
		MV1DrawModel(m_modelHandle);
	}
	
}