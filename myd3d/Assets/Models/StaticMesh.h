#pragma once
#include "../../glm/glm.hpp"
#include <d3d11.h>
#include <vector>
#include <fstream>

#include "../../D3D.h"

class StaticMesh
{
private:
    struct VertexFormat
    {
        glm::vec3 position;
        glm::vec2 texture;
        glm::vec3 normal;
    };

    struct ModelVertex
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

    struct VertexFormatTanBin
    {
        glm::vec3 position;
        glm::vec2 texture;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 binormal;
    };

    struct ModelVertexTanBin
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
        float tx, ty, tz;
        float bx, by, bz;
    };

public:
    //StaticMesh();
    StaticMesh(const std::string& filename, D3D& d3d);
    StaticMesh(const std::string& filename, D3D& d3d, bool includeTanBin);
    ~StaticMesh(void);

    bool LoadObj(const std::string& filename);
    bool InitBuffers(D3D& d3d);
    bool InitBuffersTanBin(D3D& d3d);

    void Render(D3D& d3d);

    int GetVertexCount() const { return m_vertexCount; }
    int GetIndexCount() const { return m_indexCount;}

    void SetVertexCount(int vertexCount) { m_vertexCount = vertexCount; }
    void SetIndexCount(int indexCount) { m_indexCount = indexCount; }

    bool DoesContainTanBin() const { return m_includeTanBin; }

    /**
     * Uses CalculateTangentBinormal and CalculateNormal functions to calculate the tangent,
     * binormal and new normal for each vertex of the model.
     */
    void CalculateModelVectors();
    /** 
     * Given the three vertices (vert1, vert2, and vert3), calculates the tangent and binormal for
     * the polygon. Stores results in 'tangent' and 'binormal' params.
     */
    void CalculateTangentBinormal(const VertexFormat& vert1, const VertexFormat& vert2, 
                                  const VertexFormat& vert3, glm::vec3& tangent, 
                                  glm::vec3& binormal) const;
    /**
     * Calculates the normal from the given tangent and binormal. Result stored in 'normal' param.
     */
    void CalculateNormal(const glm::vec3& tangent, const glm::vec3& binormal, glm::vec3& normal) const;

private:
    std::string                     m_filename;
    int                             m_vertexCount;
    int                             m_indexCount;
    ID3D11Buffer*                   m_vertexBuffer;
    ID3D11Buffer*                   m_indexBuffer;

    std::vector<ModelVertex>        m_modelData; // Holds the model data if tangents and binormals are not stored.
    std::vector<ModelVertexTanBin>  m_modelDataTanBin; // Holds the model data if tangents and binormals are stored.

    bool                            m_includeTanBin;
};

