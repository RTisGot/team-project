#ifndef DEF_STAGECOLLIDERBASE_H
#define DEF_STAGECOLLIDERBASE_H

/**
 * @file StageColliderBase.h
 * @brief ステージコライダーベースクラスの定義
 */

/**
 * @class StageColliderBase
 * @brief ステージコライダーベースクラス
 */
class StageColliderBase
{
public:
    virtual ~StageColliderBase() = default;

    void SetStageModel(int modelHandle);

protected:
    int m_StageModelHandle;
};

#endif // DEF_STAGECOLLIDERBASE_H
