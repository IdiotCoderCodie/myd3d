#pragma once
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "../../D3D.h"

class Texture
{
public:
    Texture();
	Texture(D3D& d3d, WCHAR* filename);
	~Texture(void);

    bool Load(D3D& d3d, WCHAR* filename);
	// Returns a pointer to the ShaderResourceView of the texture.
	ID3D11ShaderResourceView* GetTexture() const { return m_texture; }

private:
	ID3D11ShaderResourceView* m_texture;
};

