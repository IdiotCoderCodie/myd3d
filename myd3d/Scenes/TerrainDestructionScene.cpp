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

	Entity* testSphere = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", L"circleStencil.dds", 100, 100, m_screenWidth, m_screenHeight,
		"sphere1");

	//m_physicsSystem.AddCircle(testSphere, 50.0f, glm::vec2(00.0f, 100.0f));

    m_circles.push_back(&(*testSphere));

	/*Entity* testSphere2 = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", L"circleStencil.dds", 100, 100, m_screenWidth, m_screenHeight,
		"sphere2");
	testSphere2->MoveUp(200.0f);
    testSphere2->MoveRight(40.0f);

	m_physicsSystem.AddCircle(testSphere2, 50.0f, glm::vec2(0.0f, -50.0f));

    m_circles.push_back(&(*testSphere2));*/

    /*Entity* testSphere3 = EntityFactory::CreateBmpEntity(*this, d3d, L"cement.dds", L"circleStencil.dds", 150, 150, m_screenWidth, m_screenHeight,
		"sphere3");

    testSphere3->MoveRight(-50.0f);
    testSphere3->MoveUp(50.0f);

	m_physicsSystem.AddCircle(testSphere3, 75.0f, glm::vec2(50.0f, 0.0f));

    m_circles.push_back(&(*testSphere3));*/

	EntityFactory::CreateOrthoFpCameraEntity(*this, -m_screenWidth / 2.0f, m_screenWidth / 2.0f,
		-m_screenHeight / 2.0f, m_screenHeight / 2.0f, "testCam");
	
    // It's a sqwerrr!!1-!
    //Entity* sqwer = EntityFactory::CreateBmpEntity(*this, d3d, L"jan2.dds", 100, 100, m_screenWidth, m_screenHeight, "sqwer");


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
