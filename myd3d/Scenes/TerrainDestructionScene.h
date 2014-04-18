#pragma once
#define _WINSOCKAPI_ // Just spammed this fucker everywhere.
#include "Scene.h"
#include "../Physics/PhysicsSystem.h"
#include "../Networking/NetworkManager.h"
#include "WSA.h"

class TerrainDestructionScene : public Scene
{
public:
    TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr);
    ~TerrainDestructionScene(void);

    void Update(double time);
    void Draw(D3D& d3d);

    std::vector<Entity*>& GetCircles() { return m_circles; }
    std::vector<Entity*>& GetSquares() { return m_squares; }
    std::vector<Entity*>& GetLines()   { return m_lines; }
    
private:
    int             m_screenWidth;
    int             m_screenHeight;
    std::vector<Entity*> m_circles;
    std::vector<Entity*> m_squares;
    std::vector<Entity*> m_lines;

	PhysicsSystem   m_physicsSystem;
    WSA             m_wsa;
    NetworkManager  m_networkManager;
};
