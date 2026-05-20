#pragma once

/**
 * @file RoofTop.h
 * @brief 屋上クラス
 */

/**
 * @class RoofTop
 * 
 */
class RoofTop
{
public:
	RoofTop();
	~RoofTop();
	bool Init();
	void Draw();

	int GetModelHandle() const { return m_modelHandle; }
	
private:
	int m_modelHandle;	///< 屋上のモデルハンドル
};