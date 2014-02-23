#include "TextureManager.h"

//TextureManager G_TextureManager;

TextureManager::TextureManager(void)
{

}


TextureManager::~TextureManager(void)
{
}


Texture* TextureManager::GetTexture(const std::string& key)
{
    if(m_TextureMap.find(key) != m_TextureMap.end())
    {
        return &m_TextureMap.find(key)->second;     
    }

    return nullptr;
}


Texture* TextureManager::LoadTexture(D3D& d3d, const WCHAR* filename, const std::string& key)
{
    /*std::wstring ws(filename);
    std::string  ss(ws.begin(), ws.end());*/
    if(m_TextureMap.find(key) != m_TextureMap.end())
    {
        // Texture with provided key already exists.
        return &m_TextureMap[key];
    }
    // Texture doesn't exist with said name, so create it and add to texture map, and return it's
    // address.

    // NOTE: Should probably load this from a config file? Would allow easy moving of assets/textures 
    //       without the need to recompile.
    std::wstring fullFile(L"Assets\\Textures\\");
    fullFile += std::wstring(filename);
    WCHAR* fullWcharFile = &fullFile[0];

    // Create the texture object. Load in the texture. Add to the texture map. return the address.
    m_TextureMap[key] = Texture();
    m_TextureMap[key].Load(d3d, fullWcharFile);
    return &(m_TextureMap.find(key)->second);
}

TextureManager& G_TextureManager()
{
    static TextureManager textureManager;
    return textureManager;
}