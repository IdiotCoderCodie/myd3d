#pragma once
#include <d3d11.h>

namespace SamplerDesc
{
    static D3D11_SAMPLER_DESC DEFAULT_WRAP = 
    {
        D3D11_FILTER_MIN_MAG_MIP_LINEAR,    // Filter
	    D3D11_TEXTURE_ADDRESS_WRAP,         // AddressU
	    D3D11_TEXTURE_ADDRESS_WRAP,         // AddressV
	    D3D11_TEXTURE_ADDRESS_WRAP,         // AddressW
	    0.0f,                               // MipLODBias
	    4,                                  // MaxAnisotropy
	    D3D11_COMPARISON_ALWAYS,            // ComparisonFunc
	    0,                                  // BorderColor[0]
	    0,                                  // BorderColor[1]
	    0,                                  // BorderColor[2]
	    0,                                  // BorderColor[3]
	    0,                                  // MinLOD
	    D3D11_FLOAT32_MAX                   // MaxLOD
    };


    static D3D11_SAMPLER_DESC SAMPLE_CLAMP =
    {
        D3D11_FILTER_MIN_MAG_MIP_LINEAR,    // Filter
	    D3D11_TEXTURE_ADDRESS_CLAMP,         // AddressU
	    D3D11_TEXTURE_ADDRESS_CLAMP,         // AddressV
	    D3D11_TEXTURE_ADDRESS_CLAMP,         // AddressW
	    0.0f,                               // MipLODBias
	    4,                                  // MaxAnisotropy
	    D3D11_COMPARISON_ALWAYS,            // ComparisonFunc
	    0,                                  // BorderColor[0]
	    0,                                  // BorderColor[1]
	    0,                                  // BorderColor[2]
	    0,                                  // BorderColor[3]
	    0,                                  // MinLOD
	    D3D11_FLOAT32_MAX                   // MaxLOD
    };
};