#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>


class D3D
{
public:
    D3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
    ~D3D(void);

private:
    D3D(const D3D&);
    D3D& operator=(const D3D&);

    bool InitializeD3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
    void Shutdown();

public:
    void BeginScene(float r, float g, float b, float a);
    void EndScene();

    ID3D11Device& GetDevice();
    ID3D11DeviceContext& GetDeviceContext();

    void GetVideoCardInfo(char* cardName, int& memory) const;

    void TurnZBufferOn();
    void TurnZBufferOff();

    int GetScreenWidth() const   { return m_screenWidth; }
    int GetScreenHeight() const  { return m_screenHeight; }

    bool IsInitialized() const   { return m_initialized; }

    void ClearDepthStencilView();
    ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView; }
    void SetBackBufferRenderTarget();

    void EnableAlphaBlending();
    void DisableAlphaBlending();

private:
    bool                        m_initialized;
    bool                        m_vsyncEnabled;
    int                         m_videoCardMemory;
    char                        m_videoCardDesc[128];
    IDXGISwapChain*             m_swapChain;
    ID3D11Device*               m_device;
    ID3D11DeviceContext*        m_deviceContext;
    ID3D11RenderTargetView*     m_renderTargetView;
    ID3D11Texture2D*            m_depthStencilBuffer;
    ID3D11DepthStencilState*    m_depthStencilState;
    ID3D11DepthStencilState*    m_depthDisabledStencilState;
    ID3D11DepthStencilView*     m_depthStencilView;
    ID3D11RasterizerState*      m_rasterState;
    int                         m_screenWidth;
    int                         m_screenHeight;

    ID3D11BlendState*           m_alphaEnableBlendingState;
    ID3D11BlendState*           m_alphaDisableBlendingState;
};

