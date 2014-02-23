#pragma once
#include <d3d11.h>

namespace PolyLayouts
{
    static D3D11_INPUT_ELEMENT_DESC POS3[1] =
    {
        { 
            "POSITION",                     // Semantic Name
            0,                              // Semantic Index
            DXGI_FORMAT_R32G32B32_FLOAT,    // Format
            0,                              // Input Slot
            0,                              // Aligned Byte Offset
            D3D11_INPUT_PER_VERTEX_DATA,    // Input Slot Class
            0                               // Instance Data Step Rate
        }
    };

    static D3D11_INPUT_ELEMENT_DESC POS3_TEX2[2] =
    {
        { 
            "POSITION",                     // Semantic Name
            0,                              // Semantic Index
            DXGI_FORMAT_R32G32B32_FLOAT,    // Format
            0,                              // Input Slot
            0,                              // Aligned Byte Offset
            D3D11_INPUT_PER_VERTEX_DATA,    // Input Slot Class
            0                               // Instance Data Step Rate
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    static D3D11_INPUT_ELEMENT_DESC POS3_TEX2_COL4[3] =
    {
        { 
            "POSITION",                     // Semantic Name
            0,                              // Semantic Index
            DXGI_FORMAT_R32G32B32_FLOAT,    // Format
            0,                              // Input Slot
            0,                              // Aligned Byte Offset
            D3D11_INPUT_PER_VERTEX_DATA,    // Input Slot Class
            0                               // Instance Data Step Rate
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    static D3D11_INPUT_ELEMENT_DESC POS3_TEX2_NORM3[3] = 
    {
        { 
            "POSITION",                     // Semantic Name
            0,                              // Semantic Index
            DXGI_FORMAT_R32G32B32_FLOAT,    // Format
            0,                              // Input Slot
            0,                              // Aligned Byte Offset
            D3D11_INPUT_PER_VERTEX_DATA,    // Input Slot Class
            0                               // Instance Data Step Rate
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "NORMAL",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

	static D3D11_INPUT_ELEMENT_DESC POS3_TEX2_NORM3_TAN3_BIN3[5] = 
    {
        { 
            "POSITION",                     // Semantic Name
            0,                              // Semantic Index
            DXGI_FORMAT_R32G32B32_FLOAT,    // Format
            0,                              // Input Slot
            0,                              // Aligned Byte Offset
            D3D11_INPUT_PER_VERTEX_DATA,    // Input Slot Class
            0                               // Instance Data Step Rate
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "NORMAL",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
		{
            "TANGENT",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
		{
            "BINORMAL",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };
}