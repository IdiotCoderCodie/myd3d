#include "RenderTarget.h"
#include "d3d_safe_release.h"

RenderTarget::RenderTarget(ID3D11Device* device, int width, int height)
    : m_renderTargetTexture(0),
      m_renderTargetView(0),
      m_shaderResourceView(0)
{
    // Create texture description and then use it to create the texture.
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    
	textureDesc.Width               = width;
	textureDesc.Height              = height;
	textureDesc.MipLevels           = 1;
	textureDesc.ArraySize           = 1;
	textureDesc.Format              = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count    = 1;
	textureDesc.Usage               = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags           = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags      = 0;
	textureDesc.MiscFlags           = 0;

    HRESULT result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
    if(FAILED(result))
    {
        return;
    }

    // Create description of render target view.
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format             = textureDesc.Format;
    renderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc,
                                            &m_renderTargetView);
    if(FAILED(result))
    {
        return;
    }

    // Create description for shader resource view, and use it to create the shader resource view.
    D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
    resourceViewDesc.Format                     = textureDesc.Format;
    resourceViewDesc.ViewDimension              = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceViewDesc.Texture2D.MostDetailedMip  = 0;
    resourceViewDesc.Texture2D.MipLevels        = 1;

    result = device->CreateShaderResourceView(m_renderTargetTexture, &resourceViewDesc, 
                                              &m_shaderResourceView);
    if(FAILED(result))
    {
        return;
    }
}


RenderTarget::~RenderTarget(void)
{
    try
    {
        d3d_safe_release(m_shaderResourceView);
        d3d_safe_release(m_renderTargetView);
        d3d_safe_release(m_renderTargetTexture);
    }
    catch(int& e)
    {
    }
}


void RenderTarget::SetRenderTarget(ID3D11DeviceContext* deviceContext,
                                   ID3D11DepthStencilView* depthStencilView)
{
    // Bind the texture view and depth stencil buffer to output render pipeline.
    deviceContext->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
}


void RenderTarget::ClearRenderTarget(ID3D11DeviceContext* deviceContext, 
                                     ID3D11DepthStencilView* depthStencilView,
                                     float r, float g, float b, float a) const
{
    float color[4];
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;

    // Clear render target buffer.
    deviceContext->ClearRenderTargetView(m_renderTargetView, color);

    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
