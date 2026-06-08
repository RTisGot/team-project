#include "map/MapDataLoader.h"
#include "map/MapData.h"
#include "thirdparty/json.hpp"
#include <fstream>

using json = nlohmann::json;

namespace
{
    VECTOR JsonToVector(const json& data)
    {
        return VGet(
            data.value("x", 0.0f),
            data.value("y", 0.0f),
            data.value("z", 0.0f));
    }
}

bool MapDataLoader::Load(const std::string& filePath, MapData& mapData)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return false;
    }

    json root;
    file >> root;

    const auto& playerSpawn = root["PlayerSpawn"];
    mapData.PlayerSpawn.Position = JsonToVector(playerSpawn["Position"]);
    mapData.PlayerSpawn.Rotation = JsonToVector(playerSpawn["Rotation"]);

    return true;
}
