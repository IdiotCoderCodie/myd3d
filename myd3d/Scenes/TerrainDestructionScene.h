#pragma once
#define _WINSOCKAPI_ // Just spammed this fucker everywhere.
#include "Scene.h"
#include "../Physics/PhysicsSystem.h"
#include "../Networking/NetworkManager.h"
#include "../Entities/PhysCircleEntity.h"
#include "../Entities/PhysAABBEntity.h"
#include "WSA.h"

#include <ostream>

class TerrainDestructionScene : public Scene
{
public:
    TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr);
    ~TerrainDestructionScene(void);

    void Update(double time);
    void Draw(D3D& d3d);

    void AddCircle(float x, float y, float radius, glm::vec2& vel, float mass, float elast,
                   std::string& id);
    // Adds circle to scene and preps to be added to any peers over the network.
    void AddCircleNetwork(float x, float y, float radius, glm::vec2& vel, float mass, float elast,
                   std::string& id);

    void AddAABB(float x, float y, glm::vec2& min, glm::vec2& max, glm::vec2& vel, 
                 float mass, float elast, std::string& id);

    void LoadNewCircles();
    void LoadNewAABBs();

    std::vector<PhysCircleEntity*>& GetCircles()    { return m_circles; }
    std::vector<PhysAABBEntity*>&   GetAABBs()      { return m_aabbs; }
    Entity&                         GetCannon()     { return *m_cannon;  }

    std::mutex& GetCirclesMutex() { return m_circlesMutex;  }
    std::mutex& GetAABBsMutex()   { return m_aabbsMutex; }

    void GetNewNetworkCircles(ostream& out);

private:
    int                             m_screenWidth;
    int                             m_screenHeight;
    std::vector<PhysCircleEntity*>  m_circles;
    std::vector<PhysCircleEntity*>  m_circlesToAdd;
    std::vector<PhysAABBEntity*>    m_aabbs;
    std::vector<PhysAABBEntity*>    m_aabbsToAdd;

    Entity*                         m_cannon;

    std::vector<PhysCircleEntity*>  m_newNetworkCircles;

    std::mutex                      m_circlesToAddMutex;
    std::mutex                      m_aabbsToAddMutex;
    std::mutex                      m_circlesMutex;
    std::mutex                      m_aabbsMutex;

    std::mutex                      m_networkCirclesMutex;
    std::mutex                      m_networkAABBsMutex;
    //std::mutex                      

	PhysicsSystem   m_physicsSystem;
    WSA             m_wsa;
    NetworkManager  m_networkManager;

    // Cannon Stuff.
    glm::vec3       m_cannonAim;
    float           m_cannonPower;

    // Tweak Bar
    TwBar*          m_tweakBar;
};
