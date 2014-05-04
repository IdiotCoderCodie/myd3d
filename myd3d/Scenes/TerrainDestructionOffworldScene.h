#pragma once
#define _WINSOCKAPI_ // Just spammed this fucker everywhere.
#include "Scene.h"
#include "../Physics/PhysicsSystem.h"
#include "../Networking/OffworldNetworkManager.h"
#include "WSA.h"
#include "../Components/Visual/VisualBitmapComponent.h"

class TerrainDestructionOffworldScene : public Scene
{
public:
    TerrainDestructionOffworldScene(const std::string& name, SceneManager* sceneMgr, int playerNum);
    ~TerrainDestructionOffworldScene(void);

    void Update(double time);
    void Draw(D3D& d3d);

  /*  std::vector<Entity*>& GetCircles() { return m_circles; }
    std::vector<Entity*>& GetSquares() { return m_squares; }
    std::vector<Entity*>& GetLines()   { return m_lines; }*/

    void AddCircle(float x, float y, float radius, std::string& id);
    void AddSquare(float x, float y, float w, float h, std::string& id);
    
    void LoadNewEnts();

    Entity* GetEntitySafe(const std::string& entID);

private:
    int                     m_screenWidth;
    int                     m_screenHeight;
    //std::vector<Entity*>    m_circles;
    std::vector<Entity*>    m_entsToAdd;
    std::mutex              m_entsToAddMutex;
    std::mutex              m_entitiesMutex;
    //std::vector<Entity*>    m_squares;
    //std::vector<Entity*>    m_lines;

    WSA                     m_wsa;
    OffworldNetworkManager  m_networkManager;
    Entity*                 m_circleEnt;
    VisualBitmapComponent*  m_circleBmp;
};
