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

	Entity* noPhysSphere = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", L"circleStencil.dds", 100, 100, m_screenWidth, m_screenHeight,
		"noPhysSphere");

	//m_physicsSystem.AddCircle(testSphere, 50.0f, glm::vec2(00.0f, 100.0f));

    m_circles.push_back(&(*noPhysSphere));

	Entity* testSphere = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", L"circleStencil.dds", 100, 100, m_screenWidth, m_screenHeight,
		"sphere1");
	testSphere->MoveUp(200.0f);
    testSphere->MoveRight(40.0f);

	m_physicsSystem.AddCircle(testSphere, 50.0f, glm::vec2(0.0f, -50.0f), 1.0f);

    m_circles.push_back(&(*testSphere));

    testSphere = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", L"circleStencil.dds", 100, 100, m_screenWidth, m_screenHeight,
		"sphere2");

    testSphere->MoveRight(-50.0f);
    testSphere->MoveUp(50.0f);

	m_physicsSystem.AddCircle(testSphere, 50.0f, glm::vec2(0.0f, 0.0f), 0.0f);

    m_circles.push_back(&(*testSphere));

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


void TerrainDestructionScene::Update(double time)
{
    Scene::Update(time);

	m_physicsSystem.Update(time);
}


void TerrainDestructionScene::Draw(D3D& d3d)
{
    Scene::Draw(d3d);
}
