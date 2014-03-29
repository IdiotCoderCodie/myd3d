#include "EntityFactory.h"
#include "../Assets/Textures/Texture.h"
#include "../Assets/Textures/TextureManager.h"
#include "../Scenes/Scene.h"
#include "../Components/Camera/PerspectiveCamComponent.h"
#include "../Components/Camera/OrthoCamComponent.h"
#include "../Components/Physics/FreeRoamFpComponent.h"
#include "../Components/Light/LightComponent.h"
#include "../Components/Visual/VisualMeshComponent.h"
#include "../Components/Visual/VisualTessellatedPlanetComponent.h"
#include "../Components/Visual/VisualTessellatedTorusComponent.h"
#include "../Components/Visual/VisualTessellatedEllipsoidComponent.h"
#include "../Components/Visual/VisualGeomParticlesComponent.h"
#include "../Components/Visual/VisualBezierPatchComponent.h"
#include "../Components/Visual/VisualBitmapComponent.h"
#include "../Components/Visual/ParticleSystemComponent.h"
#include "../Components/Collision/CollisionComponent.h"
#include "../Components/BehaviourControllers/PaperPlaneBC.h"
#include "../Components/Visual/VisualRaymarchComponent.h"


//extern TextureManager G_TextureManager;
Entity* EntityFactory::CreatePerspectiveFpCameraEntity(Scene& scene, float fov, float aspect, 
                                                     float pNear, float pFar, const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new PerspectiveCamComponent(fov, aspect, pNear, pFar));
    newEntity->SetComponent(new FreeRoamFpComponent(30.0f, 75.0f, 75.0f));
    scene.AddEntity(newEntity);
    newEntity->MoveForward(-10.0f);
    return newEntity;
}


Entity* EntityFactory::CreateOrthoFpCameraEntity(Scene& scene, float left, float right, float bot,
	float top, const std::string& id)
{
	Entity* newEntity = new Entity(scene, id);
	newEntity->SetComponent(new OrthoCamComponent(left, right, bot, top));
	newEntity->SetComponent(new FreeRoamFpComponent(30.0f, 75.0f, 75.0f));
	scene.AddEntity(newEntity);
	//newEntity->MoveForward(-10.0f);
	return newEntity;
}

Entity* EntityFactory::CreatePerspectiveCameraEntity(Scene& scene, float fov, float aspect, 
                                                     float pNear, float pFar, 
                                                     const glm::vec3& position, const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new PerspectiveCamComponent(fov, aspect, pNear, pFar));
    scene.AddEntity(newEntity);
    newEntity->MoveGlobalX(position.x);
    newEntity->MoveGlobalY(position.y);
    newEntity->MoveGlobalZ(position.z);
    return newEntity;
}


Entity* EntityFactory::CreatePointlightEntity(Scene& scene, const glm::vec4& ambient, 
                                              const glm::vec4& diffuse, const glm::vec4& specular,
                                              const glm::vec3& position,
                                              const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    LightComponent* lightComp = new LightComponent(ambient, diffuse, specular);
    lightComp->GenerateProjectionMatrix(5.0f, 200.0f);
    newEntity->SetComponent(lightComp);
    scene.AddEntity(newEntity);
    newEntity->MoveForward(position.z);
    newEntity->MoveRight(position.x);
    newEntity->MoveUp(position.y);

    return newEntity;
}

Entity* EntityFactory::CreatePointlightEntity(Scene& scene, const glm::vec4& ambient, 
                                          const glm::vec4& diffuse, const glm::vec4& specular,
                                          const std::string& id)
{
        return CreatePointlightEntity(scene, ambient, diffuse, specular, glm::vec3(0.0f), id);
}


Entity* EntityFactory::CreateSpotlightEntity(Scene& scene, const glm::vec4& ambient, 
                                              const glm::vec4& diffuse, const glm::vec4& specular,
                                              const glm::vec3& position, float spotCutoff, 
                                              float spotExponent, const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    LightComponent* lightComp = new LightComponent(ambient, diffuse, specular, 
                                                   spotCutoff, spotExponent);
    lightComp->GenerateProjectionMatrix(5.0f, 200.0f);
    newEntity->SetComponent(lightComp);
    scene.AddEntity(newEntity);
    newEntity->MoveForward(position.z);
    newEntity->MoveRight(position.x);
    newEntity->MoveUp(position.y);

    return newEntity;
}


Entity* EntityFactory::CreateMeshEntity(Scene& scene, D3D& d3d, const std::string& objFilename, 
                                        WCHAR* textureName, std::vector<RenderTarget*>& shadowMaps,
                                        const glm::vec3& position, const glm::vec3& scale,
                                        const std::string& id)
{
    // Create the entity.
    Entity* newEntity = new Entity(scene, id);

    // Get the standard string from WCHAR*.
    std::wstring ws(textureName);
    std::string  ssTexName(ws.begin(), ws.end());

    // Check if texture is already loaded...
    Texture* tex = G_TextureManager().GetTexture(ssTexName);
    if(!tex)
    {
        // It's not, so load it.
        tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
    }

    // Create the mesh component, enable shadows (both cast and recieve).
    VisualMeshComponent* mesh = new VisualMeshComponent(d3d, objFilename, *tex, shadowMaps);
    mesh->EnableCastShadows();
    mesh->EnableRecieveShadows();
    newEntity->SetComponent(mesh);

    // Move to requested position.
    newEntity->MoveForward(position.z);
    newEntity->MoveRight(position.x);
    newEntity->MoveUp(position.y);

    // Scale to requested scale.
    newEntity->SetScaleX(scale.x);
    newEntity->SetScaleY(scale.y);
    newEntity->SetScaleZ(scale.z);

    // Add to the scene.
    scene.AddEntity(newEntity);

    return newEntity;
}


Entity* EntityFactory::CreateRaymarchEntity(Scene& scene, D3D& d3d, const std::string& objFilename,
	WCHAR* textureName, std::vector<RenderTarget*>& shadowMaps,
	const glm::vec3& position, const glm::vec3& scale,
	const std::string& id)
{
	// Create the entity.
	Entity* newEntity = new Entity(scene, id);

	// Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

	// Check if texture is already loaded...
	Texture* tex = G_TextureManager().GetTexture(ssTexName);
	if (!tex)
	{
		// It's not, so load it.
		tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	}

	// Create the mesh component, enable shadows (both cast and recieve).
	VisualRaymarchComponent* mesh = new VisualRaymarchComponent(d3d, objFilename, *tex, shadowMaps);
	mesh->EnableCastShadows();
	mesh->EnableRecieveShadows();
	newEntity->SetComponent(mesh);

	// Move to requested position.
	newEntity->MoveForward(position.z);
	newEntity->MoveRight(position.x);
	newEntity->MoveUp(position.y);

	// Scale to requested scale.
	newEntity->SetScaleX(scale.x);
	newEntity->SetScaleY(scale.y);
	newEntity->SetScaleZ(scale.z);

	// Add to the scene.
	scene.AddEntity(newEntity);

	return newEntity;
}


Entity* EntityFactory::CreateBumpMappedMeshEntity(Scene& scene, D3D& d3d, const std::string& objFilename, 
                                        WCHAR* textureName, WCHAR* bmpMapTextureName, 
                                        std::vector<RenderTarget*>& shadowMaps,
                                        const glm::vec3& position, const glm::vec3& scale,
                                        const std::string& id)
{
    // Create the entity.
    Entity* newEntity = new Entity(scene, id);

    // Get the standard string from WCHAR*.
    std::wstring ws(textureName);
    std::string  ssTexName(ws.begin(), ws.end());

    // Check if texture is already loaded.
    Texture* tex = G_TextureManager().GetTexture(ssTexName);
    if(!tex)
    {
        // It's not, so load it.
        tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
    }

    // Attempt to load the bump map texture.
    std::wstring wsBump(bmpMapTextureName);
    std::string  ssBump(wsBump.begin(), wsBump.end());

    Texture* bumpTex = G_TextureManager().GetTexture(ssBump);
    if(!bumpTex)
    {
        bumpTex = G_TextureManager().LoadTexture(d3d, bmpMapTextureName, ssBump);
    }

    

    // Create the mesh component, enable shadows (both cast and recieve).
    VisualMeshComponent* mesh = new VisualMeshComponent(d3d, objFilename, *tex, *bumpTex, 
                                                        shadowMaps);
    mesh->EnableCastShadows();
    mesh->EnableRecieveShadows();
    newEntity->SetComponent(mesh);

    // Move to requested position.
    newEntity->MoveForward(position.z);
    newEntity->MoveRight(position.x);
    newEntity->MoveUp(position.y);

    // Scale to requested scale.
    newEntity->SetScaleX(scale.x);
    newEntity->SetScaleY(scale.y);
    newEntity->SetScaleZ(scale.z);

    // Add to the scene.
    scene.AddEntity(newEntity);

    return newEntity;
}


Entity* EntityFactory::CreateTessellatedTerrainEntity(Scene& scene, D3D& d3d, const std::string& objFilename,
	WCHAR* textureName, WCHAR* heightMapTexture, std::vector<RenderTarget*>& shadowMaps,
	const glm::vec3& position, const glm::vec3& scale,
	const std::string& id)
{
	// Create the entity.
	Entity* newEntity = new Entity(scene, id);

	// Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

	// Check if texture is already loaded...
	//Texture* tex = G_TextureManager().GetTexture(ssTexName);
	//if (!tex)
	//{
	//	// It's not, so load it.
	//	tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	//}

	std::wstring ws2(heightMapTexture);
	std::string heightMapString(ws2.begin(), ws2.end());

	Texture* heightMap = G_TextureManager().GetTexture(heightMapString);
	if (!heightMap)
	{ // It's not loaded... so load it.
		heightMap = G_TextureManager().LoadTexture(d3d, heightMapTexture, heightMapString);
	}

	Texture* grassTex = G_TextureManager().GetTexture("grassTexture.dds");
	if (!grassTex)
	{
		grassTex = G_TextureManager().LoadTexture(d3d, L"grassTexture.dds", "grassTexture.dds");
	}

	Texture* rockTex = G_TextureManager().GetTexture("rockTexture.dds");
	if (!rockTex)
	{
		rockTex = G_TextureManager().LoadTexture(d3d, L"rockTexture.dds", "rockTexture.dds");
	}

	Texture* snowTex = G_TextureManager().GetTexture("snowTexture.dds");
	if (!snowTex)
	{
		snowTex = G_TextureManager().LoadTexture(d3d, L"snowTexture.dds", "snowTexture.dds");
	}

	// Create the mesh component, enable shadows (both cast and recieve).
	VisualTessellatedPlanetComponent* mesh = 
		new VisualTessellatedPlanetComponent(d3d, objFilename, *grassTex, *rockTex, *snowTex,
											 *heightMap, shadowMaps);

	mesh->EnableCastShadows();
	mesh->EnableRecieveShadows();
	newEntity->SetComponent(mesh);

	// Move to requested position.
	newEntity->MoveForward(position.z);
	newEntity->MoveRight(position.x);
	newEntity->MoveUp(position.y);

	// Scale to requested scale.
	newEntity->SetScaleX(scale.x);
	newEntity->SetScaleY(scale.y);
	newEntity->SetScaleZ(scale.z);

	// Add to the scene.
	scene.AddEntity(newEntity);

	return newEntity;
}


Entity* EntityFactory::CreateTessellatedTorusEntity(Scene& scene, D3D& d3d, const std::string& objFilename,
	WCHAR* textureName, WCHAR* heightMapTexture, std::vector<RenderTarget*>& shadowMaps,
	const glm::vec3& position, const glm::vec3& scale,
	const std::string& id)
{
	// Create the entity.
	Entity* newEntity = new Entity(scene, id);

	// Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

	// Check if texture is already loaded...
	Texture* tex = G_TextureManager().GetTexture(ssTexName);
	if (!tex)
	{
		// It's not, so load it.
		tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	}

	std::wstring ws2(heightMapTexture);
	std::string heightMapString(ws2.begin(), ws2.end());

	Texture* heightMap = G_TextureManager().GetTexture(heightMapString);
	if (!heightMap)
	{ // It's not loaded... so load it.
		heightMap = G_TextureManager().LoadTexture(d3d, heightMapTexture, heightMapString);
	}

	// Create the mesh component, enable shadows (both cast and recieve).
	VisualTessellatedTorusComponent* mesh = 
		new VisualTessellatedTorusComponent(d3d, objFilename, *tex, *heightMap, shadowMaps);


	mesh->EnableCastShadows();
	mesh->EnableRecieveShadows();
	newEntity->SetComponent(mesh);

	// Move to requested position.
	newEntity->MoveForward(position.z);
	newEntity->MoveRight(position.x);
	newEntity->MoveUp(position.y);

	// Scale to requested scale.
	newEntity->SetScaleX(scale.x);
	newEntity->SetScaleY(scale.y);
	newEntity->SetScaleZ(scale.z);

	// Add to the scene.
	scene.AddEntity(newEntity);

	return newEntity;
}

Entity* EntityFactory::CreateTessellatedEllipsoidEntity(Scene& scene, D3D& d3d, const std::string& objFilename,
	WCHAR* textureName, WCHAR* heightMapTexture, std::vector<RenderTarget*>& shadowMaps,
	const glm::vec3& position, const glm::vec3& scale,
	const std::string& id)
{
	// Create the entity.
	Entity* newEntity = new Entity(scene, id);

	// Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

	// Check if texture is already loaded...
	Texture* tex = G_TextureManager().GetTexture(ssTexName);
	if (!tex)
	{
		// It's not, so load it.
		tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	}

	std::wstring ws2(heightMapTexture);
	std::string heightMapString(ws2.begin(), ws2.end());

	Texture* heightMap = G_TextureManager().GetTexture(heightMapString);
	if (!heightMap)
	{ // It's not loaded... so load it.
		heightMap = G_TextureManager().LoadTexture(d3d, heightMapTexture, heightMapString);
	}

	// Create the mesh component, enable shadows (both cast and recieve).
	VisualTessellatedEllipsoidComponent* mesh = 
		new VisualTessellatedEllipsoidComponent(d3d, objFilename, *tex, *heightMap, shadowMaps);


	mesh->EnableCastShadows();
	mesh->EnableRecieveShadows();
	newEntity->SetComponent(mesh);

	// Move to requested position.
	newEntity->MoveForward(position.z);
	newEntity->MoveRight(position.x);
	newEntity->MoveUp(position.y);

	// Scale to requested scale.
	newEntity->SetScaleX(scale.x);
	newEntity->SetScaleY(scale.y);
	newEntity->SetScaleZ(scale.z);

	// Add to the scene.
	scene.AddEntity(newEntity);

	return newEntity;
}


Entity* EntityFactory::CreateGeometryParticleEntity(Scene& scene, D3D& d3d, const std::string& objFilename,
                                    WCHAR* textureName, std::vector<RenderTarget*>& shadowMaps,
                                    const glm::vec3& position, const glm::vec3& scale,
                                    const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);

    // Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

    Texture* tex = G_TextureManager().GetTexture(ssTexName);
	if (!tex)
	{
		// It's not, so load it.
		tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	}

    VisualGeomParticlesComponent* visCom = 
        new VisualGeomParticlesComponent(d3d, objFilename, *tex, shadowMaps);

    visCom->EnableCastShadows();
    visCom->EnableRecieveShadows();
    newEntity->SetComponent(visCom);

    // Move to requested position.
	newEntity->MoveForward(position.z);
	newEntity->MoveRight(position.x);
	newEntity->MoveUp(position.y);

    // Scale to requested scale.
	newEntity->SetScaleX(scale.x);
	newEntity->SetScaleY(scale.y);
	newEntity->SetScaleZ(scale.z);

    // Add to the scene.
	scene.AddEntity(newEntity);

	return newEntity;
}


Entity* EntityFactory::CreateBezierPatchEntity(Scene& scene, D3D& d3d, WCHAR* textureName,
	std::vector<RenderTarget*>&shadowMaps,
	const glm::vec3& position, const glm::vec3& scale,
	const std::string& id)
{
	Entity* newEntity = new Entity(scene, id);

	// Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

	Texture* tex = G_TextureManager().GetTexture(ssTexName);
	if (!tex)
	{
		// It's not, so load it.
		tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	}

	VisualBezierPatchComponent* bezierComp =
		new VisualBezierPatchComponent(d3d, *tex, shadowMaps);
	bezierComp->EnableCastShadows();
	bezierComp->EnableRecieveShadows();
	newEntity->SetComponent(bezierComp);

	// Move to requested position.
	newEntity->MoveForward(position.z);
	newEntity->MoveRight(position.x);
	newEntity->MoveUp(position.y);

	// Scale to requested scale.
	newEntity->SetScaleX(scale.x);
	newEntity->SetScaleY(scale.y);
	newEntity->SetScaleZ(scale.z);

	// Add to the scene.
	scene.AddEntity(newEntity);

	return newEntity;
}

Entity* EntityFactory::CreateBmpEntity(Scene& scene, D3D& d3d, ID3D11ShaderResourceView* srcTexture,
                                       int width, int height, int screenWidth, int screenHeight,
                                       const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new VisualBitmapComponent(d3d, srcTexture, width, height, screenWidth,
                                                      screenHeight));
    scene.AddEntity(newEntity);
    return newEntity;
}


Entity* EntityFactory::CreateBmpEntity(Scene& scene, D3D& d3d, ID3D11ShaderResourceView* srcTexture,
                                       int width, int height, int screenWidth, int screenHeight,
                                       int xPos, int yPos, const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new VisualBitmapComponent(d3d, srcTexture, width, height, screenWidth,
                                                      screenHeight)); 
    newEntity->MoveRight(-xPos);
    newEntity->MoveUp(-yPos);
    scene.AddEntity(newEntity);
    return newEntity;
}


Entity* EntityFactory::CreateBmpEntity(Scene& scene, D3D& d3d, WCHAR* textureName,
	int width, int height, int screenWidth, int screenHeight,
	const std::string& id)
{
	Entity* newEntity = new Entity(scene, id);

	// Get the standard string from WCHAR*.
	std::wstring ws(textureName);
	std::string  ssTexName(ws.begin(), ws.end());

	// Check if texture is already loaded.
	Texture* tex = G_TextureManager().GetTexture(ssTexName);
	if (!tex)
	{
		// It's not, so load it.
		tex = G_TextureManager().LoadTexture(d3d, textureName, ssTexName);
	}
	newEntity->SetComponent(new VisualBitmapComponent(d3d, tex->GetTexture(), width, height, screenWidth,
												      screenHeight));

	scene.AddEntity(newEntity);

	return newEntity;
}


Entity* EntityFactory::CreateParticleSystemEntity(Scene& scene, D3D& d3d, const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new ParticleSystemComponent(d3d));
    scene.AddEntity(newEntity);
    //newEntity->RotateGlobalY(180.0f);
    return newEntity;
}

Entity* EntityFactory::CreateParticleSystemEntity(Scene& scene, D3D& d3d, const std::string& file,
                                                  
                                                  const std::string& id)
{
    Entity* newEntity = new Entity(scene, id);
    newEntity->SetComponent(new ParticleSystemComponent(d3d, file));
    scene.AddEntity(newEntity);
    //newEntity->RotateGlobalY(180.0f);
    return newEntity;
}


Entity* EntityFactory::CreatePaperPlaneEntity(Scene& scene, D3D& d3d, const glm::vec3& position, 
                                              std::vector<RenderTarget*>& shadowMaps, 
                                              const std::vector<FollowPathComponent::Node>& pathNodes,
                                              const std::string& id)
{
    static int numPaperPlanes = 0;
    numPaperPlanes++;
    Entity* newEntity = new Entity(scene, id);
    //newEntity->SetComponent(new ParticleSystemComponent(d3d, "flameParticleEffect.txt"));

    // Load texture.
    Texture* tex = G_TextureManager().GetTexture("crumpledPaper1024.dds");
    if(!tex)
    {
        tex = G_TextureManager().LoadTexture(d3d, L"crumpledPaper1024.dds", "crumpledPaper1024.dds");
    }

    const std::string plane1 = "Assets\\Models\\plane.obj";
    const std::string plane2 = "Assets\\Models\\plane2.obj";

    // Create the mesh component.
    VisualMeshComponent* mesh = new VisualMeshComponent(d3d, 
                                                ((numPaperPlanes % 2) == 0 ? plane1 : plane2), 
                                                *tex, shadowMaps);
    mesh->EnableCastShadows();
    mesh->EnableRecieveShadows();
    newEntity->SetComponent(mesh);
    mesh = 0;
    tex = 0;

    // Move to designated position in the world.
    newEntity->MoveGlobalX(position.x);
    newEntity->MoveGlobalY(position.y);
    newEntity->MoveGlobalZ(position.z);

    newEntity->SetScaleX(3.0f);
    newEntity->SetScaleY(3.0f);
    newEntity->SetScaleZ(3.0f);

    //----------------------------------------------------------------------------------------------
    // Add Collision Component.
    CollisionComponentDesc collisionDesc = 
    {
        CollisionType::BoundingSphere,
        1.0f,
        0.5f,
        0.5f,
        0.5f
    };
    newEntity->SetComponent(new CollisionComponent(collisionDesc));
    //----------------------------------------------------------------------------------------------

    newEntity->SetComponent(new PaperPlaneBC(d3d));

    //----------------------------------------------------------------------------------------------
    // Setup the path component.
    FollowPathComponent* pathComp = new FollowPathComponent();
    newEntity->SetComponent(pathComp);
    // Add all the path points.
    for(const FollowPathComponent::Node& node : pathNodes)
    {
        pathComp->AddNode(node.position, node.timeToReach, node.delay);
    }
    //----------------------------------------------------------------------------------------------

    pathComp->InitDebugBuffers(d3d);
    scene.AddEntity(newEntity);

    return newEntity;
}