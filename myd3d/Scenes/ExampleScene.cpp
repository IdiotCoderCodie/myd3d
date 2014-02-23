#include "ExampleScene.h"
#include "SceneManager.h"
#include "../Components/Visual/VisualMeshComponent.h"
#include "../Entities/EntityFactory.h"

ExampleScene::ExampleScene(const std::string& name, SceneManager* sceneMgr)
: Scene(name, sceneMgr)
{
	D3D& d3d = GetParent().GetD3DInstance();

	float aspect = GetParent().GetD3DInstance().GetScreenWidth() 
					/ (float)GetParent().GetD3DInstance().GetScreenHeight();

	EntityFactory::CreatePerspectiveFpCameraEntity(*this, 60.0f, aspect, 0.1f, 500.0f, "mmainCam");
	
	EntityFactory::CreateMeshEntity(*this, d3d, "Assets\\Models\\cubeInv.obj", L"cement.dds",
									GetShadowMaps(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), 
									"testCube");

	// Currently only spotlights working. And have to cast shadows currently also.
	Entity* light =
		EntityFactory::CreateSpotlightEntity(*this, glm::vec4(0.01f, 0.01f, 0.02f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.7f, 0.9f, 0.7f, 0.5f),
		glm::vec3(0.0f, 0.0f, -5.0f),
		30.0f,
		12.0f,
		"testLight");

	//EntityFactory::CreateBmpEntity(*this, d3d, G)
}


ExampleScene::~ExampleScene()
{
}


void ExampleScene::Update(double time)
{
	Scene::Update(time);
}


void ExampleScene::Draw(D3D& d3d)
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
				vmc->ShadowPass(d3d);
			}
	}

	d3d.BeginScene(0.6f, 0.6f, 0.6f, 1.0f);

	d3d.SetBackBufferRenderTarget();

	Scene::Draw(d3d);
}
