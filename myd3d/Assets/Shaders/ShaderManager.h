#pragma once
#include <unordered_map>

#include "Shader.h"
#include "ShaderResources\poly_layouts.h"
#include "ShaderResources\constant_buffers.h"
#include "ShaderResources\sampler_desc.h"


class ShaderManager
{
public:
    ShaderManager();

    bool LoadShaders(D3D& d3d, const std::string& configFilename);

    Shader* GetShader(const std::string& id);

    bool IsLoaded() const { return m_loaded; }

    ~ShaderManager(void);

private:
    bool m_loaded;
    std::unordered_map<std::string, Shader> m_shaders;
};

ShaderManager& G_ShaderManager();

