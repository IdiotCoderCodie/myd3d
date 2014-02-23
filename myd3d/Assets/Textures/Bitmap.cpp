#include "Bitmap.h"
#include "../../d3d_safe_release.h"

Bitmap::Bitmap(D3D& d3d, const Texture& texture, int width, int height, int screenWidth, int screenHeight)
    :   m_texture(texture.GetTexture()),
        m_vertexBuffer(0), m_indexBuffer(0),
        m_vertexCount(0), m_indexCount(0),
        m_screenWidth(screenWidth), m_screenHeight(screenHeight),      
        m_bitmapWidth(width), m_bitmapHeight(height)       
{
    Init(&d3d.GetDevice());
}

Bitmap::Bitmap(D3D& d3d, ID3D11ShaderResourceView* texture, int width, int height, int screenWidth,
               int screenHeight)
    : m_texture(texture),
      m_vertexBuffer(0), m_indexBuffer(0),
      m_vertexCount(0), m_indexCount(0),
      m_screenWidth(screenWidth), m_screenHeight(screenHeight),
      m_bitmapWidth(width), m_bitmapHeight(height)
{
    Init(&d3d.GetDevice());
}


Bitmap::~Bitmap(void)
{
    try
    {
        d3d_safe_release(m_vertexBuffer);
        d3d_safe_release(m_indexBuffer);
    }
    catch (int& e)
    {
    }
    // Not responsible for deleting the shader resource view. May be being used elsewhere.
    m_texture = 0;
}


bool Bitmap::Init(ID3D11Device* device)
{
    m_indexCount = m_vertexCount = 6;

    // Create vertex array.
    bitmap::VertexStruct* vertices;
    vertices = new bitmap::VertexStruct[m_vertexCount];
    if(!vertices)
        assert(true);

    // Create index array.
    unsigned long* indices = new unsigned long[m_indexCount];
    if(!indices)
        assert(true);

    // Init vertices info to all 0's, indices to 1-6.
    for(int i = 0; i < m_vertexCount; i++)
    {
        vertices[i].position    = glm::vec3(0.0f);
        vertices[i].uv          = glm::vec2(0.0f);
        indices[i] = i;
    }

    // Set up description.
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage                  = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth              = sizeof(bitmap::VertexStruct) * m_vertexCount;
	vertexBufferDesc.BindFlags              = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags         = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags              = 0;
	vertexBufferDesc.StructureByteStride    = 0;

    // Give subresource structure pointer to data.
    D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem          = vertices;
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

    // Create vertex buffer.
    if(FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
        assert(true);

    // Create index buffer description.
    D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags      = 0;
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;

    // Give pointer to data.
    D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem           = indices;
	indexData.SysMemPitch       = 0;
	indexData.SysMemSlicePitch  = 0;

    if(FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
        assert(true);

    delete[] vertices;
    vertices = 0;
    delete[] indices;
    indices = 0;

    return true;
}


bool Bitmap::UpdateBuffers(D3D& d3d, int positionX, int positionY) const
{
    float left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

    float right = left + (float)m_bitmapWidth;

    float top   = (float)((m_screenHeight / 2.0f) - (float)positionY);

    float bot   = top - (float)m_bitmapHeight;

    bitmap::VertexStruct* vertices = new bitmap::VertexStruct[m_vertexCount];
    if(!vertices)
    {
        assert(true);
        return false;
    }

    // Tri 1.
    vertices[0].position = glm::vec3(left, top, 0.0f);
    vertices[0].uv       = glm::vec2(0.0f, 0.0f);
    vertices[1].position = glm::vec3(right, bot, 0.0f);
    vertices[1].uv       = glm::vec2(1.0f, 1.0f);
    vertices[2].position = glm::vec3(left, bot, 0.0f);
    vertices[2].uv       = glm::vec2(0.0f, 1.0f);

    // Tri 2.
    vertices[3].position = glm::vec3(left, top, 0.0f);
    vertices[3].uv       = glm::vec2(0.0f, 0.0f);
    vertices[4].position = glm::vec3(right, top, 0.0f);
    vertices[4].uv       = glm::vec2(1.0f, 0.0f);
    vertices[5].position = glm::vec3(right, bot, 0.0f);
    vertices[5].uv       = glm::vec2(1.0f, 1.0f);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if(FAILED(d3d.GetDeviceContext().Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, 
              &mappedResource)))
    {
        assert(true);
        return false;
    }

    bitmap::VertexStruct* verticesPtr = (bitmap::VertexStruct*)mappedResource.pData;

    memcpy(verticesPtr, (void*)vertices, (sizeof(bitmap::VertexStruct) * m_vertexCount));

    d3d.GetDeviceContext().Unmap(m_vertexBuffer, 0);

    delete[] vertices;
    vertices = 0;

    return true;
}

void Bitmap::Draw(D3D& d3d)
{
    unsigned int stride = sizeof(bitmap::VertexStruct);
    unsigned int offset = 0;

    // Set vertex buffer active.
    d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set index buffer active
    d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set primitive type.
    d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}