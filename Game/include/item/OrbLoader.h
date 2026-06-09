#ifndef ORB_LOADER_H
#define ORB_LOADER_H

#include <string>

/**
 * @file OrbLoader.h
 * @brief オーブのロードクラスを定義するヘッダーファイル
 */

class OrbManager;

/**
 * @class OrbLoader
 * @brief オーブのロードクラス
 */
class OrbLoader
{
public:

    /**
     * @brief JSONファイルからオーブの情報を読み込んでオーブマネージャーに登録する
     * @param filePath JSONファイルのパス
     * @param orbManager オーブマネージャーへの参照
     * @return 読み込みが成功した場合はtrue、失敗した場合はfalse
     */
    static bool LoadFromJson(const std::string& filePath, OrbManager& orbManager);
};
#endif // ORB_LOADER_H
