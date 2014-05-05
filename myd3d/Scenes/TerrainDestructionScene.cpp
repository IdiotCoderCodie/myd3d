#include "TerrainDestructionScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "TerrainDestructionConsts.h"
#include "../InputManager.h"
//#include "../Physics/PhysicsSystem.h"
#include <time.h>
#include <fstream>
#include <algorithm>

#define _NETWORK_DEBUGOUT
#ifndef _NETWORK_DEBUGOUT
std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
#endif 

TerrainDestructionScene::TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr),
    m_cannonAim(0.0f, 1.0f, 0.0f),
    m_cannonPower(400.0f),
    m_worldOffsetX(0.0f)
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
    m_worldOffsetX = 0.0f;
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
                m_worldOffsetX = (-HOME_WIDTH / 2.0f) - 10.0f;
            }
            else
            {
                m_worldOffsetX = (HOME_WIDTH / 2.0f) + 10.0f;
            }
            break;
        }
    }

    int cannonPosX = rand() % 9 + 6;

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, BACKGROUND_TEX, 2560, 1600, m_screenWidth, m_screenHeight, "bg");
    
    //AddCircle(m_worldOffsetX + 250.0f, 400.0f, 20.0f, glm::vec2(-20.0f, 0.0f), 1.0f, 0.9f, std::string("circ3"));

    for(int i = 0; i < 20; i++)
    {
        float xPos = m_worldOffsetX + HOME_LEFT + UNIT_SIZE / 2.0f + (UNIT_SIZE * i);
        float yPos;
        for(int y = 0; y < rand() % 7 + 6; y++)
        {
            yPos = HOME_BOT + HALF_UNIT_SIZE + (UNIT_SIZE * y);
            AddAABB(xPos, yPos,                                         // Position
                    glm::vec2(-HALF_UNIT_SIZE, -HALF_UNIT_SIZE), 
                    glm::vec2(HALF_UNIT_SIZE, HALF_UNIT_SIZE),          // Dimensions
                    glm::vec2(0.0f),                                    // Velocity
                    10.0f,                                               // Mass
                    0.9f,                                               // Elasticity
                    std::string("sqwer") + to_string(i*20 + y));        // ID
        }

        if (cannonPosX == i)
        {
            m_cannon = EntityFactory::CreateBmpEntity(*this, d3d, CANNON_TEX, CANNON_STENCIL,
                UNIT_SIZE, UNIT_SIZE, 0, 0, "P" + to_string(m_networkManager.GetPlayerNum()) + "cannon");
            m_cannon->SetPos(glm::vec3(xPos, yPos + UNIT_SIZE, 0.0f));
        }
    }


    // Walls.
    AddAABB(m_worldOffsetX - 320.0f - 5.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(0.0f, 240.0f), glm::vec2(0.0f),
        0.0f, 0.8f, "P" + to_string(m_networkManager.GetPlayerNum()) + std::string("leftWall"));
    
    AddAABB(m_worldOffsetX + 320.0f + 5.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(0.0f, 240.0f), glm::vec2(0.0f),
        0.0f, 0.8f, "P" + to_string(m_networkManager.GetPlayerNum()) + std::string("rightWall"));

    AddAABB(m_worldOffsetX, -240.0f - 5.0f, glm::vec2(-320.0f, -10.0f), glm::vec2(320.0f, 0.0f), glm::vec2(0.0f),
        0.0f, 0.8f, "P" + to_string(m_networkManager.GetPlayerNum()) + std::string("floorWall"));


	Entity* cam = EntityFactory::CreateOrthoFpCameraEntity(*this, -320.0f, 320.0f,
		-240.0f, 240.0f, "testCam");

    cam->SetPos(glm::vec3(-m_worldOffsetX, 0.0f, 0.0f));

    m_physicsSystem.start();


    m_tweakBar = TwNewBar("Game");
    TwAddVarRW(m_tweakBar, "cannonPower", TW_TYPE_FLOAT, &m_cannonPower, "step = 5.0");
    TwAddVarRW(m_tweakBar, "cannonDir", TW_TYPE_DIR3F, &m_cannonAim, "");
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
        new PhysCircleEntity(*this,
        "P" + to_string(m_networkManager.GetPlayerNum()) + id,
        m_physicsSystem, radius, glm::vec2(x, y), vel, mass, elast);

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
        new PhysCircleEntity(*this, 
        "P" + to_string(m_networkManager.GetPlayerNum()) + id,
        m_physicsSystem, radius, glm::vec2(x, y), vel, mass, elast);

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
    PhysAABBEntity* newEnt = new PhysAABBEntity(*this, 
        "P" + to_string(m_networkManager.GetPlayerNum()) + id,
        m_physicsSystem, max.x - min.x, max.y - min.y,
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


// Add to the ostream data of any circles that need to be transferred over.
void TerrainDestructionScene::GetTransferCircles(ostream& out)
{
    std::lock_guard<std::mutex> toTransLock(m_circlesToTransferMutex);
    for (auto it = m_circlesToTransfer.begin(); it != m_circlesToTransfer.end();)
    {
        Circle& circ = it->second;
        out << "LOSTCTRL " << it->first << " CIRC "
            /*<< "X: "*/ << circ.GetPos().x << " " << circ.GetPos().y << " "
            /*<< "R: " */<< circ.GetRadius() << " "
            /*<< "V: "*/ << circ.GetVel().x << " " << circ.GetVel().y << " "
            /*<< "E: "*/ << circ.GetElasticity() << " "
            /*<< "F: "*/ << circ.GetStaticFriction() << " " << circ.GetDynamicFriction() << " "
            /*<< "M: "*/ << circ.GetMass()
            << endl;
    }
}


void TerrainDestructionScene::FireHeavyRound(int shotNumber)
{
    if (m_networkManager.GetNumPeers() > 0)
    {
        AddCircleNetwork(m_cannon->GetPos().x, m_cannon->GetPos().y,    // StartPos
            HALF_UNIT_SIZE / 2.0f,    // Radius
            glm::vec2(m_cannonAim.x, m_cannonAim.y) * m_cannonPower, // Vel.
            4.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber));  // ID
    }
    else
    { // No peers are connected, so no need to signal that new stuff has been added.
        AddCircle(m_cannon->GetPos().x, m_cannon->GetPos().y + 10.0f,    // StartPos
            HALF_UNIT_SIZE / 2.0f,    // Radius
            glm::vec2(m_cannonAim.x, m_cannonAim.y) * m_cannonPower,  // Velocity
            4.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber));
    }
}

void TerrainDestructionScene::FireGrapeShot(int shotNumber)
{
    if (m_networkManager.GetNumPeers() > 0)
    {
        glm::vec2 centrePos(m_cannon->GetPos().x, m_cannon->GetPos().y);
        // First ball.
        AddCircleNetwork(centrePos.x, centrePos.y,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x, m_cannonAim.y)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber));  // ID
        // Second.
        AddCircleNetwork(centrePos.x, centrePos.y + HALF_UNIT_SIZE / 2.0f,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x, m_cannonAim.y * 0.9)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber+1));  // ID
        // Third.
        AddCircleNetwork(centrePos.x + HALF_UNIT_SIZE / 2.0f, centrePos.y + HALF_UNIT_SIZE / 2.0f,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x * 0.9, m_cannonAim.y)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber+2));  // ID
        // Fourth
        AddCircleNetwork(centrePos.x + HALF_UNIT_SIZE / 2.0f, centrePos.y,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x * 0.9, m_cannonAim.y * 0.9)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber+3));  // ID
    }
    else
    {
        glm::vec2 centrePos(m_cannon->GetPos().x, m_cannon->GetPos().y);
        // First ball.
        AddCircle(centrePos.x, centrePos.y,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x, m_cannonAim.y)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber));  // ID
        // Second.
        AddCircle(centrePos.x, centrePos.y + HALF_UNIT_SIZE / 2.0f,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x, m_cannonAim.y * 0.9)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber+1));  // ID
        // Third.
        AddCircle(centrePos.x + HALF_UNIT_SIZE / 2.0f, centrePos.y + HALF_UNIT_SIZE / 2.0f,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x * 0.9, m_cannonAim.y)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber+2));  // ID
        // Fourth
        AddCircle(centrePos.x + HALF_UNIT_SIZE / 2.0f, centrePos.y,    // StartPos
            HALF_UNIT_SIZE / 4.0f,    // Radius
            glm::normalize(glm::vec2(m_cannonAim.x * 0.9, m_cannonAim.y * 0.9)) * m_cannonPower, // Vel.
            1.0f, 0.9f,    // Mass, Elast
            std::string("shot") + to_string(shotNumber+3));  // ID
    }
}


void TerrainDestructionScene::CheckForHandovers()
{
    // Check for any circles that need to be handed over.
    // First, Checks if they're over the boundary,
    // If it is then get the circle's data and add the entity to be removed.
    std::lock_guard<std::mutex> lock(m_circlesMutex);

    for (auto it = m_circles.begin(); it != m_circles.end();)
    {
        if ((*it)->GetPos().x > m_worldOffsetX + HOME_RIGHT)
        {
            // Handover.
            std::lock_guard<std::mutex> toTransLock(m_circlesToTransferMutex);
            m_circlesToTransfer[(*it)->GetID()] = ((*it)->GetCircle());

            std::lock_guard<std::mutex> toRemoveLock(m_circlesToRemoveMutex);

            m_circlesToRemove.push_back(*it); // add the circle entity to be removed.
        }
    }
}


void TerrainDestructionScene::DeleteToBeRemoved()
{
    { // Circles.
        std::lock_guard<std::mutex> lock(m_circlesToRemoveMutex);

        for (auto it = m_circlesToRemove.begin(); it != m_circlesToRemove.end();)
        {
            // Marks it to be deleted in the physics system...
            (*it)->DetachPhysicsObject();

            Entity* toRemove = (*it);

            auto& entsVec = this->GetEntities();

            // Remove entity from the vector list.
            entsVec.erase(std::remove(entsVec.begin(), entsVec.end(), toRemove), entsVec.end());

            // Also remove from list of circles. (these are used for network).
            std::lock_guard<std::mutex> lock2(m_circlesMutex);
            m_circles.erase(std::remove(m_circles.begin(), m_circles.end(), toRemove), m_circles.end());

            delete toRemove; // Destruct the entity from memory.
            toRemove = 0;

            // No move on.
            it = m_circlesToRemove.erase(it);
            if (it == m_circlesToRemove.end())
            {
                break; // No more to remove.
            }

            ++it;

        }
    }

    { // AABBs.
        std::lock_guard<std::mutex> lock(m_aabbsToRemoveMutex);

        for (auto it = m_aabbsToRemove.begin(); it != m_aabbsToRemove.end();)
        {
            // Marks it to be deleted in the physics system...
            (*it)->DetachPhysicsObject();

            Entity* toRemove = (*it);

            auto& entsVec = this->GetEntities();

            // Remove entity from the vector list.
            entsVec.erase(std::remove(entsVec.begin(), entsVec.end(), toRemove), entsVec.end());

            // Remove the pointer from m_aabbs (this is vector obtained by NetMgr).
            std::lock_guard<std::mutex> lock2(m_aabbsMutex);
            m_aabbs.erase(std::remove(m_aabbs.begin(), m_aabbs.end(), toRemove), m_aabbs.end());

            delete toRemove; // Destruct the entity from memory.
            toRemove = 0;

            // No move on.
            it = m_aabbsToRemove.erase(it);
            if (it == m_aabbsToRemove.end())
            {
                break; // No more to remove.
            }
            ++it;
        }
    }
}


void TerrainDestructionScene::Update(double time)
{
    DeleteToBeRemoved();
    Scene::Update(time);

    CheckForHandovers();

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
            FireHeavyRound(totalShots);
            totalShots++;
            timeUntilNextShot = timeBetweenShots;
        }
    }
    else if (G_InputManager().IsKeyPressed(DIK_2))
    {
        if (timeUntilNextShot < 0.00001f)
        {
            // Fire shot.
            FireGrapeShot(totalShots);
            totalShots+=4;
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
