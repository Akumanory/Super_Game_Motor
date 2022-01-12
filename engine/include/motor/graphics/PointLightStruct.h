#pragma once

struct PointLightStruct 
{
    DirectX::XMFLOAT3 dynamicLightColor; // 12 byte
    float dynamicLightStrength; // 4 bytes => alight for 16 bytes
    //16

    DirectX::XMFLOAT3 dynamicLightPosition; // 12 byte
    float dynamicLightAttenuation_A; // 4 bytes
    // 16

    float dynamicLightAttenuation_B; // 4 bytes
    float dynamicLightAttenuation_C; // 4 bytes

    double padding; // 8 bytes
    // 16
};