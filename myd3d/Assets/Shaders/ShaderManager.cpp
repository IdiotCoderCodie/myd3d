#include "ShaderManager.h"

//ShaderManager G_ShaderManager;

ShaderManager::ShaderManager()
    :  m_loaded(false), 
       m_shaders()
{
}


ShaderManager::~ShaderManager()
{
}


bool ShaderManager::LoadShaders(D3D& d3d, const std::string& configFilename)
{
    // For now, don't load from config file. Just hard-code.
    configFilename;
    //----------------------------------------------------------------------------------------------
    // Create shader for render pass to create shadows.
    m_shaders["Depth"] = Shader();
    m_shaders["Depth"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\depth_vs.hlsl", 
                                   "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Depth"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\depth_ps.hlsl", 
                                   "ps_main", "ps_5_0");

    m_shaders["Depth"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                   sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                   D3D11_CPU_ACCESS_WRITE, 0, 0);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create shader for bitmap rendering.
    m_shaders["Bitmap"] = Shader();
    m_shaders["Bitmap"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\bitmap_vs.hlsl", "vp_main", 
                                        "vs_5_0", &PolyLayouts::POS3_TEX2[0], 2);
    m_shaders["Bitmap"].SetPixelShader (d3d, 0, L"Assets\\Shaders\\bitmap_ps.hlsl", "ps_main", 
                                        "ps_5_0");

    m_shaders["Bitmap"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
                                  sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                  D3D11_CPU_ACCESS_WRITE, 0, 0);
    m_shaders["Bitmap"].AddSamplerState(d3d, "BitmapTextureSampler", SamplerDesc::DEFAULT_WRAP);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create mesh render shader using shadow, using 1 light and 1 texture.
    m_shaders["Mesh_Shadows"] = Shader();
    m_shaders["Mesh_Shadows"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\shadowStruct_vs.hlsl", 
                                        "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Mesh_Shadows"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\shadowStruct_ps.hlsl", 
                                        "ps_main", "ps_5_0");

    // Add all the buffers.
    m_shaders["Mesh_Shadows"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::ShadowMatrixBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);  

    m_shaders["Mesh_Shadows"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::LightPositionBuffer), 
    								      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_Shadows"].AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::CameraPosBuffer),
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_Shadows"].AddStructuredBuffer(d3d, "LightBuffer", 
                                                         sizeof(ConstantBuffers::Light), 
                                                         ConstantBuffers::MAX_SHADOWCASTING_LIGHTS);

    // Add the sampler states.
    m_shaders["Mesh_Shadows"].AddSamplerState(d3d, "ModelTextureSampler", 
                                                      SamplerDesc::DEFAULT_WRAP);
    m_shaders["Mesh_Shadows"].AddSamplerState(d3d, "ShadowMapSampler", 
                                                      SamplerDesc::SAMPLE_CLAMP);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Create mesh render shader using shadow, using 1 light and 1 texture.
    m_shaders["Allen_Like"] = Shader();
    m_shaders["Allen_Like"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\allenLike_vs.hlsl", 
                                        "vp_main", "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);
    m_shaders["Allen_Like"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\shadowStruct_ps.hlsl", 
                                        "ps_main", "ps_5_0");

    // Add all the buffers.
    m_shaders["Allen_Like"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                      sizeof(ConstantBuffers::ShadowMatrixBuffer), 
                                      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);  

    m_shaders["Allen_Like"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::LightPositionBuffer), 
    								      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Allen_Like"].AddBuffer(d3d, "TimeBuffer", D3D11_USAGE_DYNAMIC, 
                                          sizeof(ConstantBuffers::TimeBuffer), 
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Allen_Like"].AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::CameraPosBuffer),
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Allen_Like"].AddStructuredBuffer(d3d, "LightBuffer", 
                                                         sizeof(ConstantBuffers::Light), 
                                                         ConstantBuffers::MAX_SHADOWCASTING_LIGHTS);

    // Add the sampler states.
    m_shaders["Allen_Like"].AddSamplerState(d3d, "ModelTextureSampler", 
                                                      SamplerDesc::DEFAULT_WRAP);
    m_shaders["Allen_Like"].AddSamplerState(d3d, "ShadowMapSampler", 
                                                      SamplerDesc::SAMPLE_CLAMP);
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
	// Load in the test normal map shader, with 2 lights and shadows.
	m_shaders["Mesh_Bump_Shadows"] = Shader();
	m_shaders["Mesh_Bump_Shadows"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\shadowStruct_bump_vs.hlsl", 
                                                "vp_main", "vs_5_0", 
                                                &PolyLayouts::POS3_TEX2_NORM3_TAN3_BIN3[0], 5);

	m_shaders["Mesh_Bump_Shadows"].SetPixelShader (d3d, 0, L"Assets\\Shaders\\shadowStruct_bump_ps.hlsl", 
												"ps_main", "ps_5_0");

	m_shaders["Mesh_Bump_Shadows"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
										  sizeof(ConstantBuffers::ShadowMatrixBuffer), 
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders["Mesh_Bump_Shadows"].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::LightPositionBuffer), 
    								      D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_Bump_Shadows"].AddBuffer(d3d, "CameraBuffer", D3D11_USAGE_DYNAMIC,
                                          sizeof(ConstantBuffers::CameraPosBuffer),
                                          D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Mesh_Bump_Shadows"].AddStructuredBuffer(d3d, "LightBuffer", 
                                                         sizeof(ConstantBuffers::Light), 
                                                         ConstantBuffers::MAX_SHADOWCASTING_LIGHTS);

	// Add the sampler states.  
	m_shaders["Mesh_Bump_Shadows"].AddSamplerState(d3d, "ModelTextureSampler", 
                                                      SamplerDesc::DEFAULT_WRAP);

	m_shaders["Mesh_Bump_Shadows"].AddSamplerState(d3d, "ShadowMapSampler", 
                                                      SamplerDesc::SAMPLE_CLAMP);    
	//----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // Particle Shader
    m_shaders["Particle"] = Shader();

    m_shaders["Particle"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\particle_vs.hlsl", "vp_main",
                                          "vs_5_0", &PolyLayouts::POS3_TEX2_COL4[0], 3);

    m_shaders["Particle"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\particle_ps.hlsl", "ps_main",
                                         "ps_5_0");

    m_shaders["Particle"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC, 
                                    sizeof(ConstantBuffers::MVPBuffer),
                                    D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

    m_shaders["Particle"].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------
    // Lines
    m_shaders["Lines"] = Shader();
    m_shaders["Lines"].SetVertexShader(d3d, 0, L"Assets\\Shaders\\lines_vs.hlsl", "vp_main",
                                       "vs_5_0", &PolyLayouts::POS3[0], 1);
    
    m_shaders["Lines"].SetPixelShader(d3d, 0, L"Assets\\Shaders\\lines_ps.hlsl", "ps_main",
                                      "ps_5_0");

    m_shaders["Lines"].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
                                 sizeof(ConstantBuffers::MVPBuffer), D3D11_BIND_CONSTANT_BUFFER, 
                                 D3D11_CPU_ACCESS_WRITE, 0, 0);
    //----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Planet
	std::string name = "PlanetTerrain";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_hs.hlsl", "main",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TerrainBuffer", D3D11_USAGE_DYNAMIC,
							  sizeof(ConstantBuffers::TerrainBuffer),
							  D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Planet Fractional_even
	name = "PlanetTerrain_FractionalEven";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_hs.hlsl", "main_fractional_even",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TerrainBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::TerrainBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Planet Fractional_odd
	name = "PlanetTerrain_FractionalOdd";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_hs.hlsl", "main_fractional_odd",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TerrainBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::TerrainBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Planet pow2
	name = "PlanetTerrain_Pow2";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_hs.hlsl", "main_pow2",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\terrainPlanet_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TerrainBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::TerrainBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	
	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::SAMPLE_CLAMP);
	//----------------------------------------------------------------------------------------------

    LoadTorusTessellationShaders(d3d);
    LoadEllipsoidTessellationShaders(d3d);

    m_loaded = true;
    // NOTE: should be checking all of the above worked fine.
    return true;
}


bool ShaderManager::LoadTorusTessellationShaders(D3D& d3d)
{
    //----------------------------------------------------------------------------------------------
	// Planet
	std::string name = "TorusTessellation";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\torus_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\torus_hs.hlsl", "main",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\torus_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\torus_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TorusBuffer", D3D11_USAGE_DYNAMIC,
							  sizeof(ConstantBuffers::TorusBuffer),
							  D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Planet Fractional_even
	name = "TorusTessellation_FractionalEven";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\torus_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\torus_hs.hlsl", "main_fractional_even",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\torus_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\torus_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TorusBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::TorusBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Planet Fractional_odd
	name = "TorusTessellation_FractionalOdd";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\torus_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\torus_hs.hlsl", "main_fractional_odd",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\torus_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\torus_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TorusBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::TorusBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Planet pow2
	name = "TorusTessellation_Pow2";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\torus_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\torus_hs.hlsl", "main_pow2",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\torus_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\torus_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "TorusBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::TorusBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	
	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::SAMPLE_CLAMP);
	//----------------------------------------------------------------------------------------------

    return false;
}


bool ShaderManager::LoadEllipsoidTessellationShaders(D3D& d3d)
{
    //----------------------------------------------------------------------------------------------
	// Ellipsoid
	std::string name = "EllipsoidTessellation";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_hs.hlsl", "main",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "EllipsoidBuffer", D3D11_USAGE_DYNAMIC,
							  sizeof(ConstantBuffers::EllipsoidBuffer),
							  D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// Ellipsoid Fractional_even
	name = "EllipsoidTessellation_FractionalEven";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_hs.hlsl", "main_fractional_even",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "EllipsoidBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::EllipsoidBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Ellipsoid Fractional_odd
	name = "EllipsoidTessellation_FractionalOdd";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_hs.hlsl", "main_fractional_odd",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "EllipsoidBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::EllipsoidBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::DEFAULT_WRAP);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	// Ellipsoid pow2
	name = "EllipsoidTessellation_Pow2";
	m_shaders[name] = Shader();
	m_shaders[name].SetVertexShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_vs.hlsl", "main",
											   "vs_5_0", &PolyLayouts::POS3_TEX2_NORM3[0], 3);

	m_shaders[name].SetHullShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_hs.hlsl", "main_pow2",
											 "hs_5_0");

	m_shaders[name].SetDomainShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ds.hlsl", "main",
											   "ds_5_0");

	m_shaders[name].SetPixelShader(d3d, 0, L"Assets\\Shaders\\ellipsoid_ps.hlsl", "main",
										      "ps_5_0");

	m_shaders[name].AddBuffer(d3d, "TessellationBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::TessellationBufferDist), 
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "MatrixBuffer", D3D11_USAGE_DYNAMIC,
										 sizeof(ConstantBuffers::MVPBuffer),
										 D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "EllipsoidBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::EllipsoidBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "LightPositionBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::LightPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D10_CPU_ACCESS_WRITE, 0, 0);

	m_shaders[name].AddBuffer(d3d, "CameraPosBuffer", D3D11_USAGE_DYNAMIC,
		sizeof(ConstantBuffers::CameraPosBuffer),
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	
	m_shaders[name].AddStructuredBuffer(d3d, "LightBuffer", sizeof(ConstantBuffers::Light), 1);

	m_shaders[name].AddSamplerState(d3d, "SampleTypeWrap", SamplerDesc::SAMPLE_CLAMP);
	//----------------------------------------------------------------------------------------------

    return false;
}

Shader* ShaderManager::GetShader(const std::string& id) 
{
    if(m_shaders.find(id) != m_shaders.end())
    {
        // Shader exists.
        return &(m_shaders.find(id)->second);
    }
    
    return 0;
}

ShaderManager& G_ShaderManager()
{
    static ShaderManager shaderManager;
    return shaderManager;
}
