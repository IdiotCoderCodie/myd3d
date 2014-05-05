#include "TerrainDestructionCannBallScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/PhysicsSystem.h"
#include "../Entities/Entity.h"
#include "../Assets/Textures/TextureManager.h"
#include "TerrainDestructionConsts.h"

#include <fstream>

//#ifndef _NETWORK_DEBUGOUT
//#define _NETWORK_DEBUGOUT
//std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
//#endif 

TerrainDestructionCannBallScene::TerrainDestructionCannBallScene(const std::string& name, SceneManager* sceneMgr,
                                                                 int playerNum)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

    m_screenWidth = d3d.GetScreenWidth();
    m_screenHeight = d3d.GetScreenHeight();

    EntityFactory::CreateOrthoFpCameraEntity(*this, -320.0f / 5.0f, 320.0f / 5.0f,
        -240.0f / 5.0f, 240.0f / 5.0f, "mainCamera");

    if (!m_wsa.isOk())
    {
        cerr << "Error initializing wsa." << endl;
    }
    else
    {
        m_networkManager.SetTerrainDestructionCannBallScene(*this);
        m_networkManager.SetPlayerNum(playerNum);
        m_networkManager.start();
        //Sleep(5000);
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
            /*if (playerNum == 1)
            {
                worldOffsetX = -320.5f;
            }
            else
            {
                worldOffsetX = 320.5f;
            }*/
            break;
        }
    }

    Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, BACKGROUND_TEX, 2560, 1600, m_screenWidth, m_screenHeight, "sqwer");
    
}


TerrainDestructionCannBallScene::~TerrainDestructionCannBallScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
}


void TerrainDestructionCannBallScene::Update(double time)
{
    LoadNewEnts();
    Scene::Update(time);

	//m_physicsSystem.Update(time);
}


void TerrainDestructionCannBallScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}


void TerrainDestructionCannBallScene::AddCircle(float x, float y, float radius, std::string& id)
{
    Entity* newEnt = new Entity(*this, id);

    newEnt->SetComponent(new VisualBitmapComponent(GetParent().GetD3DInstance(), PHYS_CIRC_TEX,
        L"circleStencil.dds", radius * 2.0f, radius * 2.0f, m_screenWidth, m_screenHeight));

    newEnt->SetPos(glm::vec3(x, y, 0.0f));

    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    m_entsToAdd.push_back(newEnt);

    //Entity* newEnt = 
    //    EntityFactory::CreateBmpEntity(*this, GetParent().GetD3DInstance(), PHYS_CIRC_TEX, 
    //                                   L"circleStencil.dds", radius*2.0f, radius*2.0f, 
    //                                   m_screenWidth, m_screenHeight, id);

    //newEnt->SetPos(glm::vec3(x, y, 0.0f));
}

void TerrainDestructionCannBallScene::AddSquare(float x, float y, float w, float h, std::string& id)
{
    Entity* newEnt = new Entity(*this, id);

    newEnt->SetComponent(new VisualBitmapComponent(GetParent().GetD3DInstance(), PHYS_BLOCK_TEX,
        w, h, m_screenWidth, m_screenHeight));

    newEnt->SetPos(glm::vec3(x, y, 0.0f));

    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    m_entsToAdd.push_back(newEnt);
    //Entity* newEnt =
    //    EntityFactory::CreateBmpEntity(*this, GetParent().GetD3DInstance(), PHYS_BLOCK_TEX, 
    //    w, h, m_screenWidth, m_screenHeight, id);

    //newEnt->SetPos(glm::vec3(x, y, 0.0f));
}


void TerrainDestructionCannBallScene::LoadNewEnts()
{
    std::lock_guard<std::mutex> lock(m_entsToAddMutex);
    std::lock_guard<std::mutex> lock2(m_entitiesMutex);

    for (auto it = m_entsToAdd.begin(); it != m_entsToAdd.end();)
    {
        this->AddEntity(*it);

        it = m_entsToAdd.erase(it);
        if (it == m_entsToAdd.end())
            break;

        ++it;
    }
}


Entity* TerrainDestructionCannBallScene::GetEntitySafe(const std::string& entID)
{
    std::lock_guard<std::mutex> lock(m_entitiesMutex);

    return GetEntity(entID);
}