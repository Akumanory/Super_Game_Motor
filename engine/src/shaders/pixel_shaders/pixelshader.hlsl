#define MAX_POINT_LIGHTS 100

struct PointLight
{
    float3 dynamicLightColor;
    float dynamicLightStrength;
    
    float3 dynamicLightPosition;
    float dynamicLightAttenuation_A;
    
    float dynamicLightAttenuation_B;
    float dynamicLightAttenuation_C;
    double padding;
};


cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    PointLight lights[MAX_POINT_LIGHTS];
    
    int size;
}


struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
    //float3 inColor : COLOR;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    
    float3 appliedLight = ambientLight;
    
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if(i != size)
        {
            float3 vectorToLight = normalize(lights[i].dynamicLightPosition - input.inWorldPos);
    
            float3 diffuseLightIntensivity = max(dot(vectorToLight, input.inNormal), 0);
    
            float distanceToLight = distance(lights[i].dynamicLightPosition, input.inWorldPos);
    
            float attenuationFactor = 1 / (lights[i].dynamicLightAttenuation_A + lights[i].dynamicLightAttenuation_B * distanceToLight + lights[i].dynamicLightAttenuation_C * pow(distanceToLight, 2));
    
            diffuseLightIntensivity *= attenuationFactor;
    
            float3 diffuseLight = diffuseLightIntensivity * lights[i].dynamicLightStrength * lights[i].dynamicLightColor;
    
            appliedLight += diffuseLight;
        }
        else
        {
            break;
        }
    }
    
    //float3 sampleColor = input.inNormal; // Display normals
    
    float3 finalColor = sampleColor * appliedLight;
    //float3 finalColor = sampleColor * saturate(appliedLight);
    
    return float4(
        finalColor,
        1.0f  // A
    );
}