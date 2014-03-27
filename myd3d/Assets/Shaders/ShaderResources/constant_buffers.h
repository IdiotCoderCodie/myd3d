#pragma once
#include  "../../../glm/glm.hpp"

namespace ConstantBuffers
{
	typedef glm::vec3 float3;
	typedef glm::vec2 float2;
	typedef glm::vec4 float4;
	typedef glm::mat4 matrix;

    struct MVPBuffer
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };

	struct ModelMatrixBuffer
	{
		glm::mat4 modelMatrix;
	};

    struct ProjectionOnlyBuffer
    {
        glm::mat4 projectionMat;
    };


    struct MVPShadowBuffer
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 lightViewMatrix;
        glm::mat4 lightProjectionMatrix;
    };

    struct MVPShadowBuffer2
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 lightViewMatrix;
        glm::mat4 lightProjectionMatrix;
        glm::mat4 lightViewMatrix2;
        glm::mat4 lightProjectionMatrix2;
    };

    const int MAX_SHADOWCASTING_LIGHTS = 1;

    struct ShadowMatrixBuffer
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 lightViewMatrix[MAX_SHADOWCASTING_LIGHTS];
        glm::mat4 lightProjectionMatrix[MAX_SHADOWCASTING_LIGHTS];
    };

    struct LightPositionBuffer
    {
        glm::vec4 lightPosition[MAX_SHADOWCASTING_LIGHTS];
    };

    struct LightShadowColorBuffer2
    {
        glm::vec4 ambientColor;
        glm::vec4 diffuseColor;
        glm::vec4 diffuseColor2;
    };

    struct CameraPosBuffer
    {
        glm::vec3 cameraPos;
        float padding;
    };


    struct LightColorBuffer
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specPow;
        glm::vec3 padding;
    };


    struct LightColorBuffer2
    {
        glm::vec4 ambient1;
        glm::vec4 diffuse1;
        glm::vec4 specular1;
        float specPow1;
        glm::vec3 padding1;

        glm::vec4 ambient2;
        glm::vec4 diffuse2;
        glm::vec4 specular2;
        float specPow2;
        glm::vec3 padding2;
    };

    struct LightAmbientDiffuseColorBuffer
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
    };

    struct LightAmbientDiffuse2ColorBuffer
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 diffuse2;
    };

    struct LightPosBuffer
    {
        glm::vec4 lightPosition;
    };

    struct LightPosBuffer2
    {
        glm::vec4 lightPosition;
        glm::vec4 lightPosition2;
    };

    struct Light
    {
        int enabled;
		int shadows;
        glm::vec4 position;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float     spotCutoff;
        glm::vec3 spotDirection;
        float     spotExponent;
        glm::vec3 attenuation;
        glm::vec2 padding;
    };


    struct TimeBuffer
    {
        float     time;
        glm::vec3 padding;
    };


	struct TessellationBuffer
	{
		float		tesselationAmount;
		glm::vec3	padding;
	};

    struct TessellationBufferDist
    {
        int distanceBased;
        float tessellationAmount;
        glm::vec4 eyePos;
        glm::mat4 modelMatrix;
        glm::vec2 padding;
    };

	struct TerrainBuffer
	{
		float terrainHeight;
		float terrainHeightMin;
		float terrainHeightMax;
		float padding;
	};

    struct TorusBuffer
    {
        float innerRadius;
        float tubeRadius;
        glm::vec2 padding;
    };

    struct EllipsoidBuffer
    {
        float a;
        float b;
        float c;
        float padding;
    };

    struct GeometryParticlesBuffer
    {
        int particleCount;
        float particleSize;
		int effectId;
        float padding;
    };

	struct RayMarchCameraBuffer
	{
		glm::vec3	eyePos;
		float	nearPlane;
		float	farPlane;
		float	viewportW;
		float	viewportH;
		glm::mat4	viewInverse;
		float padding;
	};

	struct RayMarchLightBuffer
	{
		float3	lightPosition;
		float4	lightColor;
		float	padding;
	};

	struct RayMarchBackgroundColorBuffer
	{
		float4 backgroundColor;
	};
};