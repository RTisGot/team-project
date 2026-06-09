#include "item/OrbLoader.h"
#include "item/OrbManager.h"
#include "thirdparty/json.hpp"
#include <fstream>

using json = nlohmann::json;

bool OrbLoader::LoadFromJson(const std::string& filePath, OrbManager& orbManager)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return false;
    }

    json root;

    file >> root;

    if (!root.contains("orbs"))
    {
        return false;
    }

    // オーブの情報を読み込んでオーブマネージャーに登録する
    for (const auto& orb : root["orbs"])
    {
        uint32_t id = orb["id"];

        float x = orb["position"]["x"];
        float y = orb["position"]["y"];
        float z = orb["position"]["z"];

        orbManager.CreateOrb(id, VGet(x, y, z));
    }

    return true;
}
