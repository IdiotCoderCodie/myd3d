#pragma once
#include "Scene.h"

class TerrainDestructionScene : public Scene
{
public:
    TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr);
    ~TerrainDestructionScene(void);

    void Update(double time);
    void Draw(D3D& d3d);

private:
    int m_screenWidth;
    int m_screenHeight;
};

