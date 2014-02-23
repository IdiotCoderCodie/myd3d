#pragma once
// System Includes.
#include <map>
#include <string>

// Project Includes.
#include "Texture.h"

class TextureManager
{
public:
    TextureManager(void);
    ~TextureManager(void);

    /** 
     * Attempts to load the texture at "filename" and store it in the texture map.
     * Returns: The loaded texture, or nullptr if it could not be loaded.
     */
    Texture* LoadTexture(D3D& d3d, const WCHAR* filename, const std::string& key);
    /**
     * Fetches the texture with matching "key". 
     * Returns nullptr if a matching texture was not found.
     */
    Texture* GetTexture(const std::string& key);

private:
    std::map<std::string, Texture> m_TextureMap;
};

TextureManager& G_TextureManager();