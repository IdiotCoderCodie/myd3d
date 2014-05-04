#include "TerrainDestructionScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "TerrainDestructionConsts.h"
#include "../InputManager.h"
//#include "../Physics/PhysicsSystem.h"
#include <time.h>
#include <fstream>

#define _NETWORK_DEBUGOUT
#ifndef _NETWORK_DEBUGOUT
std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
#endif 

TerrainDestructionScene::TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

    cerr << "test" << endl;
    if (!m_wsa.isOk())
    {
        cerr << "Error initializing wsa." << endl;
    }
    else
    {
        m_networkManager.SetTerrainDestructionScene(*this);
        m_networkManager.start();
    }

    // Wait for connection to peer, and get the player num to position world.
    float worldOffsetX = 0.0f;
    while (true)
    {
        if (!m_networkManager.HasFoundOpponent())
        {
            Sleep(100); // Sleep for a bit.
        }
        else
        {
            int playerNum = m_networkManager.GetPlayerNum();
            if (playerNum == 1)
            {
                worldOffsetX = -320.5f;
            }
            else
            {
                worldOffsetX = 320.5f;
            }
            break;
        }
    }


	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, BACKGROUND_TEX, 2560, 1600, m_screenWidth, m_screenHeight, "bg");
    
    AddCircle(worldOffsetX + 250.0f, 400.0f, 20.0f, glm::vec2(-20.0f, 0.0f), 1.0f, 0.9f, std::string("circ3"));

    for(int i = 0; i < 20; i++)
    {
        for(int y = 0; y < rand() % 7 + 6; y++)
        {
            AddAABB(worldOffsetX - 320.0f + 11.0f + 15.5f + (31.0f * i), -239.0f + 10.0f + 15.5f + (32.0f * y),
                    glm::vec2(-15.0f, -15.0f), glm::vec2(15.0f, 15.0f),
                    glm::vec2(0.0f), 1.0f, 0.9f, std::string("sqwer") + to_string(i*20 + y));
        }
    }


    // Walls.
    AddAABB(worldOffsetX - 320.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(10.0f, 240.0f), glm::vec2(0.0f),
            0.0f, 0.8f, std::string("leftWall"));
    
    AddAABB(worldOffsetX + 320.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(10.0f, 240.0f), glm::vec2(0.0f),
            0.0f, 0.8f, std::string("rightWall"));

    AddAABB(worldOffsetX, -240.0f, glm::vec2(-310.0f, -10.0f), glm::vec2(310.0f, 10.0f), glm::vec2(0.0f),
            0.0f, 0.8f, std::string("floorWall"));


	Entity* cam = EntityFactory::CreateOrthoFpCameraEntity(*this, -320.0f, 320.0f,
		-240.0f, 240.0f, "testCam");

    cam->SetPos(glm::vec3(-worldOffsetX, 0.0f, 0.0f));

    m_physicsSystem.start();
}


TerrainDestructionScene::~TerrainDestructionScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
    m_physicsSystem.finish();
    m_physicsSystem.waitForFinish();
}


void TerrainDestructionScene::AddCircle(float x, float y, float radius, glm::vec2& vel, 
                                        float mass, float elast, std::string& id)
{
    PhysCircleEntity* newEnt =
        new PhysCircleEntity(*this, id, m_physicsSystem, radius, glm::vec2(x, y), vel, mass, elast);

    //this->AddEntity(newEnt);

    std::lock_guard<std::mutex> lock(m_circlesToAddMutex);
    m_circlesToAdd.push_back(newEnt);

    //m_circles.push_back(newEnt);
}


// Adds a circle to the scene, as well as prepping it to be sent over the network.
void TerrainDestructionScene::AddCircleNetwork(float x, float y, float radius, glm::vec2& vel,
    float mass, float elast, std::string& id)
{
    PhysCircleEntity* newEnt =
        new PhysCircleEntity(*this, id, m_physicsSystem, radius, glm::vec2(x, y), vel, mass, elast);

    //this->AddEntity(newEnt);

    {
        std::lock_guard<std::mutex> lock(m_circlesToAddMutex);
        m_circlesToAdd.push_back(newEnt);
    }

    {
        std::lock_guard<std::mutex> lock2(m_networkCirclesMutex);
        m_newNetworkCircles.push_back(newEnt);
    }

    //m_circles.push_back(newEnt);
}


void TerrainDestructionScene::AddAABB(float x, float y, glm::vec2& min, glm::vec2& max, glm::vec2& vel, 
                                      float mass, float elast, std::string& id)
{
    PhysAABBEntity* newEnt = new PhysAABBEntity(*this, id, m_physicsSystem, max.x - min.x, max.y - min.y, 
                                                glm::vec2(x, y), vel, mass, elast);


    std::lock_guard<std::mutex> lock(m_aabbsToAddMutex);
    m_aabbsToAdd.push_back(newEnt);

   /* this->AddEntity(newEnt);*/

    //m_aabbs.push_back(newEnt);
}


void TerrainDestructionScene::LoadNewCircles()
{
    std::lock_guard<std::mutex> lock(m_circlesToAddMutex);
    // TODO: lock m_circleMutex too. (NetMgr accesses m_circles for info.)
    std::lock_guard<std::mutex> lock2(m_circlesMutex);
    
    for (auto it = m_circlesToAdd.begin(); it != m_circlesToAdd.end();)
    {
        // Add to vector for networking data access and add to scenes entities.
        m_circles.push_back(*it);
        this->AddEntity(*it);

        it = m_circlesToAdd.erase(it);

        if (it == m_circlesToAdd.end())
            break;

        ++it;
    }
}


void TerrainDestructionScene::LoadNewAABBs()
{
    std::lock_guard<std::mutex> lock(m_aabbsToAddMutex); 
    // TODO: lock m_aabbsMutex.
    std::lock_guard<std::mutex> lock2(m_circlesMutex);
    
    for (auto it = m_aabbsToAdd.begin(); it != m_aabbsToAdd.end();)
    {
        // Add to vector for networking data access and add to scenes entities.
        m_aabbs.push_back(*it);
        this->AddEntity(*it);

        it = m_aabbsToAdd.erase(it);

        if (it == m_aabbsToAdd.end())
            break;

        ++it;
    }
}


void TerrainDestructionScene::GetNewNetworkCircles(ostream& out)
{
    std:lock_guard<std::mutex> lock(m_networkCirclesMutex);
    for (auto it = m_newNetworkCircles.begin(); it != m_newNetworkCircles.end(); )
    {
        PhysCircleEntity& ent = *(*it);
        out << "ADD " << ent.GetID() << " CIRC "
            << "X: " << ent.GetPos().x << " " << ent.GetPos().y << " "
            << "R: " << ent.GetRadius() << endl;

        it = m_newNetworkCircles.erase(it);
        if (it == m_newNetworkCircles.end())
            break;

        ++it;
    }
}


void TerrainDestructionScene::Update(double time)
{
    Scene::Update(time);
    LoadNewCircles();
    LoadNewAABBs();

    const float timeBetweenShots = 2.0f;
    static float timeUntilNextShot = 0.0f;
    static int totalShots = 0;
    if (G_InputManager().IsKeyPressed(DIK_1))
    {
        if (timeUntilNextShot < 0.00001f)
        {
            // Fire shot.
            if (m_networkManager.GetNumPeers() > 0)
            {
                AddCircleNetwork(-200.0f, 500.0f, 10.0f, glm::vec2(0.0f, -100.0f), 1.0f, 0.9f,
                    std::string("shot") + to_string(totalShots));
            }
            else
            { // No peers are connected, so no need to signal that new stuff has been added.
                AddCircle(-200.0f, 500.0f, 10.0f, glm::vec2(0.0f, -100.0f), 1.0f, 0.9f,
                    std::string("shot") + to_string(totalShots));
            }
            totalShots++;
            timeUntilNextShot = timeBetweenShots;
        }
    }
    timeUntilNextShot -= (float)time;
   // InputManager

	//m_physicsSystem.Update(time);
}


void TerrainDestructionScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}
