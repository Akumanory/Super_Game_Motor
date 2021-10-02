cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float dynamicLightStrength;
    
    float3 dynamicLightPosition;
    
    float dynamicLightAttenuation_A; 
    float dynamicLightAttenuation_B; 
    float dynamicLightAttenuation_C;
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
    
    //float3 sampleColor = input.inNormal; // Display normals
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    
    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
    
    float3 diffuseLightIntensivity = max(dot(vectorToLight, input.inNormal), 0);
    
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
    
    float attenuationFactor = 1 / (dynamicLightAttenuation_A + dynamicLightAttenuation_B * distanceToLight + dynamicLightAttenuation_C * pow(distanceToLight, 2));
    
    diffuseLightIntensivity *= attenuationFactor;
    
    float3 diffuseLight = diffuseLightIntensivity * dynamicLightStrength * dynamicLightColor;
    
    appliedLight += diffuseLight;
    
    float3 finalColor = sampleColor * saturate(appliedLight);
    //float3 finalColor = sampleColor * appliedLight;
    
    return float4(
        finalColor,
        1.0f  // A
    );
}