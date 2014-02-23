#include "Texture.h"
#include "../../d3d_safe_release.h"

Texture::Texture()
    : m_texture(0)
{
}

Texture::Texture(D3D& d3d, WCHAR* filename)
    : m_texture(0)
{
	HRESULT result = 
                DirectX::CreateDDSTextureFromFile(&d3d.GetDevice(), filename, nullptr, &m_texture);
    if(FAILED(result))
    {
        // Failed. So load the error texture.
        DirectX::CreateDDSTextureFromFile(&d3d.GetDevice(), L"Assets\\Textures\\texture_error.dds",
                                           nullptr, &m_texture);
    }
}


Texture::~Texture(void)
{
    try
    {
	    d3d_safe_release(m_texture);
    }
    catch(int& e)
    {
    }
}


bool Texture::Load(D3D& d3d, WCHAR* filename)
{
    HRESULT result = 
                DirectX::CreateDDSTextureFromFile(&d3d.GetDevice(), filename, nullptr, &m_texture);
    if(FAILED(result))
    {
        // Failed. So load the error texture.
        DirectX::CreateDDSTextureFromFile(&d3d.GetDevice(), L"Assets\\Textures\\texture_error.dds",
                                           nullptr, &m_texture);
        return false;
    }

    return true;
}