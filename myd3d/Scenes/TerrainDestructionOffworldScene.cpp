#include "TerrainDestructionOffworldScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/PhysicsSystem.h"
#include "../Entities/Entity.h"
#include "../Assets/Textures/TextureManager.h"

#include "TerrainDestructionConsts.h"

#include <fstream>

#ifndef _NETWORK_DEBUGOUT
#define _NETWORK_DEBUGOUT
std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
#endif 

TerrainDestructionOffworldScene::TerrainDestructionOffworldScene(const std::string& name, SceneManager* sceneMgr,
                                                                 int playerNum)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

    if (!m_wsa.isOk())
    {
        cerr << "Error initializing wsa." << endl;
    }
    else
    {
        m_networkManager.SetTerrainDestructionOffworldScene(*this);
        m_networkManager.SetPlayerNum(playerNum);
        m_networkManager.start();
    }

    float worldOffsetX = 0.0f;
    // Wait until connection is made before doing anything stupid.
    while (true)
    {
        if (!m_networkManager.IsConnected())
        {
            Sleep(100);
        }
        else
        {
            // Don't really need this bit here, as it doesn't rely on netMgr connecting. ah well.
            if (playerNum == 1)
            { 
                worldOffsetX = (-HOME_WIDTH / 2.0f) - 10.0f;
            }
            else
            {
                worldOffsetX = (HOME_WIDTH / 2.0f) + 10.0f;
            }
            break;
        }
    }

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    EntityFactory::CreateBmpEntity(*this, d3d, BACKGROUND_TEX, 2560, 1600, m_screenWidth, m_screenHeight, "sqwer");

    // Camera.
    Entity* cam = EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "testCam");

    cam->SetPos(glm::vec3(-worldOffsetX, 0.0f, 0.0f));
}


TerrainDestructionOffworldScene::~TerrainDestructionOffworldScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
}


void TerrainDestructionOffworldScene::Update(double time)
{
    LoadNewEnts();
    Scene::Update(time);
}


void TerrainDestructionOffworldScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}


void TerrainDestructionOffworldScene::AddCircle(float x, float y, float radius, std::string& id)
{
    Entity* newEnt = new Entity(*this, id);

    newEnt->SetComponent(new VisualBitmapComponent(GetParent().GetD3DInstance(), PHYS_CIRC_TEX,
        L"circleStencil.dds", radius * 2.0f, radius * 2.0f, m_screenWidth, m_screenHeight));
    
    newEnt->SetPos(glm::vec3(x, y, 0.0f));

    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    m_entsToAdd.push_back(newEnt);
}


void TerrainDestructionOffworldScene::AddSquare(float x, float y, float w, float h, std::string& id)
{
    Entity* newEnt = new Entity(*this, id);

    newEnt->SetComponent(new VisualBitmapComponent(GetParent().GetD3DInstance(), PHYS_BLOCK_TEX, 
                         w, h, m_screenWidth, m_screenHeight));

    newEnt->SetPos(glm::vec3(x, y, 0.0f));

    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    m_entsToAdd.push_back(newEnt);
}


void TerrainDestructionOffworldScene::AddCannon(float x, float y, std::string& id)
{
    Entity* newEnt = new Entity(*this, id);
    newEnt->SetComponent(new VisualBitmapComponent(GetParent().GetD3DInstance(),
                         CANNON_TEX, CANNON_STENCIL, UNIT_SIZE, UNIT_SIZE, 0, 0));

    newEnt->SetPos(glm::vec3(x, y, 0.0f));

    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    m_entsToAdd.push_back(newEnt);
}


void TerrainDestructionOffworldScene::LoadNewEnts()
{
    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    std::lock_guard<std::mutex> lock2(m_entitiesMutex);

    for (auto it = m_entsToAdd.begin(); it != m_entsToAdd.end(); )
    {
        this->AddEntity(*it);

        it = m_entsToAdd.erase(it);
        if (it == m_entsToAdd.end())
            break;

        ++it;
    }
}


Entity* TerrainDestructionOffworldScene::GetEntitySafe(const std::string& entID)
{
    std::lock_guard<std::mutex> lock(m_entitiesMutex);

    return GetEntity(entID);
}
