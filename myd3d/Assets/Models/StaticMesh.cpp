#include "StaticMesh.h"
#include "../../d3d_safe_release.h"
#include <string>
#include <cassert>


StaticMesh::StaticMesh(const std::string& filename, D3D& d3d)
    :   m_filename(filename),
        m_vertexCount(0),
        m_indexCount(0),
        m_vertexBuffer(0),
        m_indexBuffer(0),
        m_modelData(),
        m_modelDataTanBin(),
        m_includeTanBin(false)
{
    LoadObj(filename);
    InitBuffers(d3d);
}


StaticMesh::StaticMesh(const std::string& filename, D3D& d3d, bool includeTanBin)
    : m_filename(filename),
      m_vertexCount(0),
      m_indexCount(0),
      m_vertexBuffer(0),
      m_indexBuffer(0),
      m_modelData(),
      m_modelDataTanBin(),
      m_includeTanBin(includeTanBin)
{
    LoadObj(filename);
    m_includeTanBin ? CalculateModelVectors() : "";
    m_includeTanBin ? InitBuffersTanBin(d3d) : InitBuffers(d3d);
}


StaticMesh::~StaticMesh(void)
{
    try
    {
    d3d_safe_release(m_indexBuffer);
    d3d_safe_release(m_vertexBuffer);
    }
    catch(int& e)
    {
    }
}


bool StaticMesh::LoadObj(const std::string& filename)
{
    std::vector<glm::vec3> tempVerts;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempUVs;

    std::ifstream input(filename);
    if(input.is_open())
    {
        bool readingFaces = false;
        std::string line;
        while(input.good())
        {
            std::getline(input, line);
            
            // these two temp vectors prevent the creation. TEST WHEN COMPLETE.
            /*glm::vec3 tempVec3;
            glm::vec2 tempVec2;*/

            if(line[0] == 'v' && line[1] != 't' && line[1] != 'p' && line[1] != 'n')
            {
                // Read in all vertices.
                glm::vec3 newVert;
                char c;
                sscanf(line.c_str(), "%*c %f %f %f", &newVert.x, &newVert.y, &newVert.z);
                tempVerts.push_back(newVert);
            }
            else if(line[0] == 'v' && line[1] == 'n')
            {
                // Read in all normals.
                glm::vec3 newNorm;
                char c;
                std::string test; 
                sscanf(line.c_str(), "%*c%*c %f %f %f", &newNorm.x, &newNorm.y, &newNorm.z);
                tempNormals.push_back(newNorm);
            }
            else if(line[0] == 'v' && line[1] == 't')
            {
                // Read in all texture coords.
                glm::vec2 newTex;
                char c;
                sscanf(line.c_str(), "%*c%*c %f %f", &newTex.x, &newTex.y);
                tempUVs.push_back(newTex);
            }
            else if(line[0] == 'f')
            {
                static int faceCount = 0;

                char c;
                input >> c; // Remove the 'f'.

                for(int i = 0; i != 3; i++)
                {
                    int v[3];
                    int n[3];
                    int t[3]; // YUK!!
                    sscanf(line.c_str(), "%*c %d%*c%d%*c%d %d%*c%d%*c%d %d%*c%d%*c%d", 
                                                             &v[0], &t[0], &n[0], 
                                                             &v[1], &t[1], &n[1], 
                                                             &v[2], &t[2], &n[2]);
                    // Reverse loop to reverse winding.
                    for(int j = 2; j > -1; j--)
                    {
                        ModelVertex newModVert;
                        // Assign vertex positions.
                        newModVert.x    = tempVerts[v[j]-1].x; 
                        newModVert.y    = tempVerts[v[j]-1].y;
                        newModVert.z    = tempVerts[v[j]-1].z;
                        // Assign texture coords.
                        newModVert.tu   = tempUVs[t[j]-1].x;
                        newModVert.tv   = tempUVs[t[j]-1].y;
                        // Assign normals.
                        newModVert.nx   = tempNormals[n[j]-1].x;
                        newModVert.ny   = tempNormals[n[j]-1].y;
                        newModVert.nz   = tempNormals[n[j]-1].z;

                        m_modelData.push_back(newModVert);
                    }

                    // Read in next 2 lines, don't read in last loop, don't need this line yet.
                    if(i != 2)
                        std::getline(input, line);

                }
                faceCount++;
            }
        }
    }
    else
    {
        return false;
    }

    // Set the vertex and index counts.
    m_vertexCount   = m_modelData.size();
    m_indexCount    = m_vertexCount;

    return true;
}


bool StaticMesh::InitBuffers(D3D& d3d)
{

    VertexFormat* vertices = new VertexFormat[m_vertexCount];
    unsigned long* indices = new unsigned long[m_vertexCount];

    // Convert data from Model type to vertex type.
    // NOTE: Could probably change this, and just get rid of all this, just load straight into 
    //       container and then create buffers from there, instead of loading and then converting.
    int i = 0;
    for (auto it = m_modelData.begin(); it != m_modelData.end(); it++, i++)
    {
        vertices[i].position    = glm::vec3(it->x, it->y, it->z);
        vertices[i].texture     = glm::vec2(it->tu, it->tv);
        vertices[i].normal      = glm::vec3(it->nx, it->ny, it->nz);

        indices[i] = i;
    }

    // Setup description of static vertex buffer.
    D3D11_BUFFER_DESC vertexBufferDesc; 
    vertexBufferDesc.Usage                  = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth              = sizeof(VertexFormat) * m_vertexCount;
    vertexBufferDesc.BindFlags              = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags         = 0;
	vertexBufferDesc.MiscFlags              = 0;
	vertexBufferDesc.StructureByteStride    = 0;
    // Setup subresource data with pointer to the vertex data.
    D3D11_SUBRESOURCE_DATA  vertexData;
    vertexData.pSysMem                      = vertices;
    vertexData.SysMemPitch                  = 0;
    vertexData.SysMemSlicePitch             = 0;

    // Create vertex buffer.
    HRESULT result = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if(FAILED(result))
    {
        assert(true);
        delete[] vertices;
        vertices = 0;
        delete[] indices;
        indices = 0;
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    // vertexCount == indexCount.
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_indexCount; 
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags      = 0;
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;
    // Setup subresource data with pointer to the index data.
    D3D11_SUBRESOURCE_DATA  indexData;
    indexData.pSysMem                   = indices;
    indexData.SysMemPitch               = 0;
    indexData.SysMemSlicePitch          = 0;

    // Create index buffer.
    result = d3d.GetDevice().CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if(FAILED(result))
    {
        assert(true);
        delete[] vertices;
        vertices = 0;
        delete[] indices;
        vertices = 0;
        return false;
    }

    delete[] vertices;
    vertices = 0;
    delete[] indices;
    vertices = 0;

    return true;
}

bool StaticMesh::InitBuffersTanBin(D3D& d3d)
{

    VertexFormatTanBin* vertices = new VertexFormatTanBin[m_vertexCount];
    unsigned long* indices = new unsigned long[m_vertexCount];

    // Convert data from Model type to vertex type.
    // NOTE: Could probably change this, and just get rid of all this, just load straight into 
    //       container and then create buffers from there, instead of loading and then converting.
    int i = 0;
    for (auto it = m_modelDataTanBin.begin(); it != m_modelDataTanBin.end(); it++, i++)
    {
        vertices[i].position    = glm::vec3(it->x, it->y, it->z);
        vertices[i].texture     = glm::vec2(it->tu, it->tv);
        vertices[i].normal      = glm::vec3(it->nx, it->ny, it->nz);
        vertices[i].tangent     = glm::vec3(it->tx, it->ty, it->tx);
        vertices[i].binormal    = glm::vec3(it->bx, it->by, it->bz);

        indices[i] = i;
    }

    // Setup description of static vertex buffer.
    D3D11_BUFFER_DESC vertexBufferDesc; 
    vertexBufferDesc.Usage                  = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth              = sizeof(VertexFormatTanBin) * m_vertexCount;
    vertexBufferDesc.BindFlags              = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags         = 0;
	vertexBufferDesc.MiscFlags              = 0;
	vertexBufferDesc.StructureByteStride    = 0;
    // Setup subresource data with pointer to the vertex data.
    D3D11_SUBRESOURCE_DATA  vertexData;
    vertexData.pSysMem                      = vertices;
    vertexData.SysMemPitch                  = 0;
    vertexData.SysMemSlicePitch             = 0;

    // Create vertex buffer.
    HRESULT result = d3d.GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if(FAILED(result))
    {
        assert(true);
        delete[] vertices;
        vertices = 0;
        delete[] indices;
        indices = 0;
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    // vertexCount == indexCount.
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_indexCount; 
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags      = 0;
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;
    // Setup subresource data with pointer to the index data.
    D3D11_SUBRESOURCE_DATA  indexData;
    indexData.pSysMem                   = indices;
    indexData.SysMemPitch               = 0;
    indexData.SysMemSlicePitch          = 0;

    // Create index buffer.
    result = d3d.GetDevice().CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if(FAILED(result))
    {
        assert(true);
        delete[] vertices;
        vertices = 0;
        delete[] indices;
        vertices = 0;
        return false;
    }

    delete[] vertices;
    vertices = 0;
    delete[] indices;
    vertices = 0;

    return true;
}

void StaticMesh::Render(D3D& d3d)
{
    unsigned int stride = m_includeTanBin ? sizeof(VertexFormatTanBin) : sizeof(VertexFormat);
    unsigned int offset = 0;

    // Set vertex buffer to active in the input assembler.
    d3d.GetDeviceContext().IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set index buffer to active.
    d3d.GetDeviceContext().IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set type of primitive to be rendered.
    d3d.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void StaticMesh::CalculateModelVectors()
{
    int faceCount = m_vertexCount / 3;
    
    VertexFormat vert1, vert2, vert3;

    for(int i = 0; i < faceCount; i++)
    {
        // Get the face data.
        int index = i * 3;
        VertexFormat vert1 = {
            glm::vec3(m_modelData[index].x, m_modelData[index].y, m_modelData[index].z),
            glm::vec2(m_modelData[index].tu, m_modelData[index].tv), 
            glm::vec3(m_modelData[index].nx, m_modelData[index].ny, m_modelData[index].nz)
        };
        index++;
        VertexFormat vert2 = {
            glm::vec3(m_modelData[index].x, m_modelData[index].y, m_modelData[index].z),
            glm::vec2(m_modelData[index].tu, m_modelData[index].tv), 
            glm::vec3(m_modelData[index].nx, m_modelData[index].ny, m_modelData[index].nz)
        };
        index++;
        VertexFormat vert3 = {
            glm::vec3(m_modelData[index].x, m_modelData[index].y, m_modelData[index].z),
            glm::vec2(m_modelData[index].tu, m_modelData[index].tv), 
            glm::vec3(m_modelData[index].nx, m_modelData[index].ny, m_modelData[index].nz)
        };

        glm::vec3 tangent;
        glm::vec3 binormal;

        // Calculate the tangent and binormal for face (vert1, vert2, vert3).
        CalculateTangentBinormal(vert1, vert2, vert3, tangent, binormal);
        tangent = -tangent;
        binormal = -binormal;
        // Calculate the new normal from the tangent and binormal.
        glm::vec3 newNormal;
        CalculateNormal(tangent, binormal, newNormal);
        
        // Create the new vertices with tangent and binormal, add to the m_modelDataTanBin vector.
        ModelVertexTanBin tanBinVert3 = 
        {       
            m_modelData[index].x, m_modelData[index].y, m_modelData[index].z,
            m_modelData[index].tu, m_modelData[index].tv,
            m_modelData[index].nx, m_modelData[index].ny, m_modelData[index].nz,
            tangent.x, tangent.y, tangent.z,
            binormal.x, binormal.y, binormal.z
        };
        index--;
        ModelVertexTanBin tanBinVert2 = 
        {       
            m_modelData[index].x, m_modelData[index].y, m_modelData[index].z,
            m_modelData[index].tu, m_modelData[index].tv,
            m_modelData[index].nx, m_modelData[index].ny, m_modelData[index].nz,
            tangent.x, tangent.y, tangent.z,
            binormal.x, binormal.y, binormal.z
        };
        index--;
        ModelVertexTanBin tanBinVert1 = 
        {       
            m_modelData[index].x, m_modelData[index].y, m_modelData[index].z,
            m_modelData[index].tu, m_modelData[index].tv,
            m_modelData[index].nx, m_modelData[index].ny, m_modelData[index].nz,
            tangent.x, tangent.y, tangent.z,
            binormal.x, binormal.y, binormal.z
        };

        m_modelDataTanBin.push_back(tanBinVert1);
        m_modelDataTanBin.push_back(tanBinVert2);
        m_modelDataTanBin.push_back(tanBinVert3);
    }
}


void StaticMesh::CalculateTangentBinormal(const VertexFormat& vert1, const VertexFormat& vert2, 
                                          const VertexFormat& vert3, glm::vec3& tangent, 
                                          glm::vec3& binormal) const
{
    // Calculate the two vectors for the face.
    glm::vec3 vec1(vert2.position - vert1.position);
    glm::vec3 vec2(vert3.position - vert1.position);

    // Calculate the tu and tv vector space coords.
    glm::vec2 textureVec1 = vert2.texture - vert1.texture;
    glm::vec2 textureVec2 = vert3.texture - vert1.texture;

    // Calculate the denominator of the tangent/binormal equation.
    float den = 1.0f / (textureVec1.x * textureVec2.y - textureVec1.y * textureVec2.x);

    // Calculate the cross products and multiply by the coefficient to get tangent and binormal.
    tangent = vec1 * textureVec2.y - vec2 * textureVec1.y;
   /* tangent.x   = (textureVec2.y * vec1.x - textureVec1.y * vec2.x) * den;
    tangent.y   = (textureVec2.y * vec1.y - textureVec1.y * vec2.y) * den;
    tangent.z   = (textureVec2.y * vec1.z - textureVec1.y * vec2.z) * den;*/

    binormal = vec2 * textureVec1.x - vec1 * textureVec2.x;
   /* binormal.x  = (textureVec1.x * vec2.x - textureVec2.x * vec1.x) * den;
    binormal.y  = (textureVec1.x * vec2.y - textureVec2.x * vec1.y) * den;
    binormal.z  = (textureVec1.x * vec2.z - textureVec2.x * vec1.z) * den;*/

    // Normalize tangent and binormal.
    tangent     = glm::normalize(tangent);
    binormal    = glm::normalize(binormal);    
}


void StaticMesh::CalculateNormal(const glm::vec3& tangent, const glm::vec3& binormal, 
                                 glm::vec3& normal) const
{
    // Cross product of tangent and binormal.
    normal = glm::normalize(glm::cross(tangent, binormal));
}
