#ifndef DEF_PLAYOBJECT_H
#define DEF_PLAYOBJECT_H

#include <DxLib.h>
#include <vector>

class PlayObject
{
    public:
        PlayObject();
        ~PlayObject();

        bool Init();

        void Update();
        void Draw();

    private:
        std::vector<int> m_ModelHandles;
};
#endif // DEF_PLAYOBJECT_H
