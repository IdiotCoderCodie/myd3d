#pragma once
#include  "../../../glm/glm.hpp"

namespace ConstantBuffers
{
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

    const int MAX_SHADOWCASTING_LIGHTS = 4;

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
        bool enabled;
        glm::vec4 position;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float     spotCutoff;
        glm::vec3 spotDirection;
        float     spotExponent;
        glm::vec3 attenuation;
        glm::vec3 padding;
    };
};