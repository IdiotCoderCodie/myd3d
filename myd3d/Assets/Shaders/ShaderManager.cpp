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

    m_loaded = true;
    // NOTE: should be checking all of the above worked fine.
    return true;
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
