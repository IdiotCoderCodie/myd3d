#pragma once
#include <d3d11.h>

class RenderTarget
{
public:
    RenderTarget(ID3D11Device* device, int width, int height);

    ~RenderTarget();


    void SetRenderTarget(ID3D11DeviceContext* devContext, ID3D11DepthStencilView* depthStencilView);

    void ClearRenderTarget(ID3D11DeviceContext* devContext, ID3D11DepthStencilView* depthStencilView
                         , float r, float g, float b, float a) const;

    ID3D11ShaderResourceView* GetShaderResourceView() const { return m_shaderResourceView; }


private:
    ID3D11Texture2D* m_renderTargetTexture;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11ShaderResourceView* m_shaderResourceView;
};

