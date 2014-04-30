#include "TerrainDestructionScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/PhysicsSystem.h"

#include <fstream>

#define _NETWORK_DEBUGOUT
#ifndef _NETWORK_DEBUGOUT
std::ofstream debugOut("TerrainDestructionNetworkingLog.txt");
#endif 

TerrainDestructionScene::TerrainDestructionScene(const std::string& name, SceneManager* sceneMgr)
    : Scene(name, sceneMgr)
{
    D3D& d3d = GetParent().GetD3DInstance();

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, L"jan2.dds", 1000, 1000, m_screenWidth, m_screenHeight, "sqwer");


    AddCircle(100.0f, 220.0f, 10.0f, glm::vec2(0.0f, -50.0f), 1.0f, std::string("circ1"));
    AddCircle(0.0f, -50.0f, 10.0f, glm::vec2(0.0f, 0.0f), 1.0f, std::string("circ2"));
    AddCircle(0.0f, -50.0f, 20.0f, glm::vec2(0.0f, 0.0f), 1.0f, std::string("circ3"));

    AddAABB(0.0f, -100.0f, glm::vec2(-20.0f, -20.0f), glm::vec2(20.0f, 20.0f), glm::vec2(0.0f, 0.0f), 1.0f, 
            std::string("square1"));
    AddAABB(20.0f, 100.0f, glm::vec2(-20.0f, -20.0f), glm::vec2(20.0f, 20.0f), glm::vec2(0.0f, 0.0f), 1.0f,
            std::string("square2"));

    AddAABB(150.0f, 0.0f, glm::vec2(-20.0f, -20.0f), glm::vec2(20.0f, 20.0f), glm::vec2(0.0f, 0.0f), 1.0f,
            std::string("square3"));

    // Walls.
    AddAABB(-320.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(10.0f, 240.0f), glm::vec2(0.0f), 0.0f,
            std::string("leftWall"));
    
    AddAABB(+320.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(10.0f, 240.0f), glm::vec2(0.0f), 0.0f,
            std::string("rightWall"));

    AddAABB(0.0f, -240.0f, glm::vec2(-310.0f, -10.0f), glm::vec2(310.0f, 10.0f), glm::vec2(0.0f), 0.0f,
            std::string("floorWall"));


	EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "testCam");
	
    // It's a sqwerrr!!1-!
    
    cerr << "test" << endl;
    if(!m_wsa.isOk())
    {
        cerr << "Error initializing wsa." << endl;
    }
    else
    {
        m_networkManager.SetTerrainDestructionScene(*this);
        m_networkManager.start();
    }
}


TerrainDestructionScene::~TerrainDestructionScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
}


void TerrainDestructionScene::AddCircle(float x, float y, float radius, glm::vec2& vel, float mass, std::string& id)
{
    PhysCircleEntity* newEnt =
        new PhysCircleEntity(*this, id, m_physicsSystem, radius, glm::vec2(x, y), vel, mass, 0.4f);

    this->AddEntity(newEnt);
/*

    Entity* newEnt = 
        EntityFactory::CreateBmpEntity(*this, GetParent().GetD3DInstance(), L"cement.dds", 
                                       L"circleStencil.dds", radius*2.0f, radius*2.0f, 
                                       m_screenWidth, m_screenHeight, id);

    newEnt->SetPos(glm::vec3(x, y, 0.0f));

    m_physicsSystem.AddCircle(newEnt, radius, vel, mass);*/

    m_circles.push_back(newEnt);
}


void TerrainDestructionScene::AddAABB(float x, float y, glm::vec2& min, glm::vec2& max, glm::vec2& vel, 
                                      float mass, std::string& id)
{
    PhysAABBEntity* newEnt = new PhysAABBEntity(*this, id, m_physicsSystem, max.x - min.x, max.y - min.y, 
                                                glm::vec2(x, y), vel, mass, 0.4f);

    this->AddEntity(newEnt);
    /*Entity* newEnt = 
        EntityFactory::CreateBmpEntity(*this, GetParent().GetD3DInstance(), L"cement.dds",
                                       max.x - min.x, max.y - min.y, m_screenWidth, m_screenHeight, 
                                       id);

    newEnt->SetPos(glm::vec3(x, y, 0.0f));
    m_physicsSystem.AddAABB(newEnt, min, max, vel, mass);*/

    m_aabbs.push_back(newEnt);
}


void TerrainDestructionScene::Update(double time)
{
    Scene::Update(time);

	m_physicsSystem.Update(time);
}


void TerrainDestructionScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}
