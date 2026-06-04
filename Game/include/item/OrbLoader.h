#ifndef ORB_LOADER_H
#define ORB_LOADER_H
#include <string>

class OrbManager;

class OrbLoader
{
public:

    static bool LoadFromJson(const std::string& filePath, OrbManager& orbManager);
};
#endif // ORB_LOADER_H
