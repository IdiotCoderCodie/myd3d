#include "AdvRenderingScene.h"

#include "../Entities/EntityFactory.h"
#include "../Components/Visual/VisualMeshComponent.h"
#include "../Components/Visual/VisualTessellatedPlanetComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "SceneManager.h"

AdvRenderingScene::AdvRenderingScene(const std::string& name, SceneManager* sceneMgr)
: Scene(name, sceneMgr)
{
	D3D& d3d = GetParent().GetD3DInstance();

	int screenWidth = GetParent().GetD3DInstance().GetScreenWidth();
	int screenHeight = GetParent().GetD3DInstance().GetScreenHeight();
	float aspect = screenWidth / (float)screenHeight;

	EntityFactory::CreatePerspectiveFpCameraEntity(*this, 60.0f, aspect, 0.1f, 500.0f, "mmainCam");

	EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cube.obj", L"cement.dds",
		GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f),
		"testCube");

	EntityFactory::CreateTessellatedTerrainEntity(*this, d3d, "Assets\\Models\\quad.obj", L"cement.dds",
		L"noisyHeightmap.dds", GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), "testTessellation");

    Entity* torusJesus = EntityFactory::CreateTessellatedTorusEntity(*this, d3d, "Assets\\Models\\quad.obj", L"cement.dds",
		L"noisyHeightmap.dds", GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), "TorusJesus");

    torusJesus->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(120.0f, 60.0f, 15.0f)));

     Entity* ellipsoidEnt = EntityFactory::CreateTessellatedEllipsoidEntity(*this, d3d, "Assets\\Models\\quad.obj", L"cement.dds",
		L"noisyHeightmap.dds", GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), "Ellipsoid");

     ellipsoidEnt->SetComponent(new PhysicsComponent(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(120.0f, 0.0f, 0.0f)));

	Entity* light = EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.05f, 0.05f, 0.05f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),
		glm::vec3(2.0f, 10.0f, 0.0f),
		10.0f,
		12.0f,
		"mainLight");	

	/*Entity* light2 = EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.1f, 0.05f, 0.05f, 1.0f),
		glm::vec4(0.5f, 0.1f, 0.1f, 1.0f),
		glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),
		glm::vec3(0.0f, 0.0f, 7.0f),
		10.0f,
		12.0f,
		"mainLight2");*/

	light->RotateGlobalY(180.0f);
}


AdvRenderingScene::~AdvRenderingScene()
{
}


void AdvRenderingScene::Update(double time)
{
	Scene::Update(time);
}


void AdvRenderingScene::Draw(D3D& d3d)
{
	for (int i = 0; i < GetShadowMaps().size(); i++)
	{
		GetShadowMaps()[i]->ClearRenderTarget(&d3d.GetDeviceContext(), d3d.GetDepthStencilView(),
			1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Do shadow pass.
	for (auto ent : GetEntities())
	{
		Component* vmcCheck = ent->GetComponent("VisualComponent", "VisualMeshComponent");
		if (vmcCheck)
		{
			// entity has a VMC.
			VisualMeshComponent* vmc = static_cast<VisualMeshComponent*>(vmcCheck);
			//vmc->ShadowPass(d3d);
		}
	}

	d3d.BeginScene(0.6f, 0.6f, 0.6f, 1.0f);

	d3d.SetBackBufferRenderTarget();

	Scene::Draw(d3d);
}