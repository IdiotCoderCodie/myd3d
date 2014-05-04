#pragma once
#define _WINSOCKAPI_ // Just spammed this fucker everywhere.
#include "Scene.h"
#include "../Physics/PhysicsSystem.h"
#include "../Networking/NetworkManager.h"
#include "../Entities/PhysCircleEntity.h"
#include "../Entities/PhysAABBEntity.h"
#include "WSA.h"

class TerrainDestructionScene : public Scene
{
public:
    TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr);
    ~TerrainDestructionScene(void);

    void Update(double time);
    void Draw(D3D& d3d);

    void AddCircle(float x, float y, float radius, glm::vec2& vel, float mass, float elast,
                   std::string& id);
    void AddAABB(float x, float y, glm::vec2& min, glm::vec2& max, glm::vec2& vel, 
                 float mass, float elast, std::string& id);

    void LoadNewCircles();
    void LoadNewAABBs();

    std::vector<PhysCircleEntity*>& GetCircles() { return m_circles; }
    std::vector<PhysAABBEntity*>& GetAABBs() { return m_aabbs; }
    std::vector<Entity*>& GetLines()   { return m_lines; }

    std::mutex& GetCirclesMutex() { return m_circlesMutex;  }
    std::mutex& GetAABBsMutex()   { return m_aabbsMutex; }

private:
    int                             m_screenWidth;
    int                             m_screenHeight;
    std::vector<PhysCircleEntity*>  m_circles;
    std::vector<PhysCircleEntity*>  m_circlesToAdd;
    std::vector<PhysAABBEntity*>    m_aabbs;
    std::vector<PhysAABBEntity*>    m_aabbsToAdd;
    std::vector<Entity*>            m_lines;

    std::mutex                      m_circlesToAddMutex;
    std::mutex                      m_aabbsToAddMutex;
    std::mutex                      m_circlesMutex;
    std::mutex                      m_aabbsMutex;
    //std::mutex                      

	PhysicsSystem   m_physicsSystem;
    WSA             m_wsa;
    NetworkManager  m_networkManager;
};
