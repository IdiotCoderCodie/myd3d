#include "TerrainDestructionScene.h"
#include "SceneManager.h"
#include "../Entities/EntityFactory.h"
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

	m_screenWidth	= d3d.GetScreenWidth();
	m_screenHeight	= d3d.GetScreenHeight();

    Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, L"spaceBG.dds", 1280, 800, m_screenWidth, m_screenHeight, "sqwer");
    
    //AddCircle(101.0f, 400.0f, 10.0f, glm::vec2(50.0f, 0.0f), 5.0f, 0.7f, std::string("circ1"));
    ////
    //AddCircle(100.0f, 220.0f, 50.0f, glm::vec2(200.0f, -50.0f), 10.0f, 0.7f, std::string("circ1"));
    //AddCircle(0.0f, -50.0f, 10.0f, glm::vec2(-300.0f, 400.0f), 1.0f, 0.7f, std::string("circ2"));
    AddCircle(250.0f, 400.0f, 20.0f, glm::vec2(-20.0f, 0.0f), 1.0f, 0.9f, std::string("circ3"));

    /*for (int i = 0; i < 15; i++)
    {
        AddCircle(-250.0f + (i+1 * 2.0f * 2.6f), 220.0f - (i+1 * 2.0f * 2.6f), i * 2.5f, glm::vec2(0.0f, 0.0f), i * 1.0f, 0.7f, 
                  std::string("circle" + to_string(i + 4)));

    }*/

    for(int i = 0; i < 20; i++)
    {
        for(int y = 0; y < rand() % 7 + 6; y++)
        {
            AddAABB(-320.0f + 11.0f + 15.5f + (31.0f * i), -239.0f + 10.0f + 15.5f + (32.0f * y),
                    glm::vec2(-15.0f, -15.0f), glm::vec2(15.0f, 15.0f),
                    glm::vec2(0.0f), 1.0f, 0.9f, std::string("sqwer") + to_string(i*y));
        }
    }

  /* AddAABB(0.0f, -100.0f, glm::vec2(-30.0f, -30.0f), glm::vec2(30.0f, 30.0f), 
            glm::vec2(100.0f, 0.0f), 1.0f, 0.5f, std::string("square1"));
    AddAABB(20.0f, 100.0f, glm::vec2(-20.0f, -20.0f), glm::vec2(20.0f, 20.0f), 
            glm::vec2(0.0f, 0.0f), 1.0f, 0.5f, std::string("square2"));

    AddAABB(150.0f, 0.0f, glm::vec2(-20.0f, -20.0f), glm::vec2(20.0f, 20.0f), 
            glm::vec2(0.0f, 0.0f), 1.0f, 0.5f, std::string("square3"));*/

    // Walls.
    AddAABB(-320.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(10.0f, 240.0f), glm::vec2(0.0f), 
            0.0f, 0.8f, std::string("leftWall"));
    
    AddAABB(+320.0f, 0.0f, glm::vec2(-10.0f, -240.0f), glm::vec2(10.0f, 240.0f), glm::vec2(0.0f), 
            0.0f, 0.8f, std::string("rightWall"));

    AddAABB(0.0f, -240.0f, glm::vec2(-310.0f, -10.0f), glm::vec2(310.0f, 10.0f), glm::vec2(0.0f), 
            0.0f, 0.8f, std::string("floorWall"));


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

    m_physicsSystem.start();
}


TerrainDestructionScene::~TerrainDestructionScene(void)
{
    m_networkManager.finish();
    m_networkManager.waitForFinish();
}


void TerrainDestructionScene::AddCircle(float x, float y, float radius, glm::vec2& vel, 
                                        float mass, float elast, std::string& id)
{
    PhysCircleEntity* newEnt =
        new PhysCircleEntity(*this, id, m_physicsSystem, radius, glm::vec2(x, y), vel, mass, elast);

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
                                      float mass, float elast, std::string& id)
{
    PhysAABBEntity* newEnt = new PhysAABBEntity(*this, id, m_physicsSystem, max.x - min.x, max.y - min.y, 
                                                glm::vec2(x, y), vel, mass, elast);

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

	//m_physicsSystem.Update(time);
}


void TerrainDestructionScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}
