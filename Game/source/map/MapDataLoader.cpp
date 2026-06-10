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

    // プレイヤースポーン地点の読み込み
    const auto& playerSpawn = root["PlayerSpawn"];
    mapData.PlayerSpawn.Position = JsonToVector(playerSpawn["Position"]);
    mapData.PlayerSpawn.Rotation = JsonToVector(playerSpawn["Rotation"]);

    // カメラ設定の読み込み
    if (root.contains("Camera"))
    {
        const auto& camera = root["Camera"];

        mapData.PlayerSpawn.Camera.Yaw = camera.value("Yaw", 0.0f);
        mapData.PlayerSpawn.Camera.Pitch = camera.value("Pitch", 0.3f);
        mapData.PlayerSpawn.Camera.Distance = camera.value("Distance", 30.0f);
    }

    // 壁コライダーの読み込み
    if (root.contains("WallColliders"))
    {
        for (const auto& wall : root["WallColliders"])
        {
            BoxColliderData collider;

            collider.m_Min =
            {
                wall["Min"][0],
                wall["Min"][1],
                wall["Min"][2]
            };

            collider.m_Max =
            {
                wall["Max"][0],
                wall["Max"][1],
                wall["Max"][2]
            };

            mapData.BoxColliders.push_back(collider);
        }
    }
    return true;
}
