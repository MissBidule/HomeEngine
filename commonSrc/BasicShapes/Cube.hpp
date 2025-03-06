//
//  Cube.hpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#pragma once

#include "Element.hpp"

class Cube : public Element {
public:
    Cube(ShaderParam* shaderParam, MTL::Device* metalDevice);
    ~Cube() override {};
    void setDimensions(simd::float3 newDimensions);
    simd::float3 getDimensions();
    
private:
    void createVertexUV(MTL::Device* metalDevice) override;
    float height = 1.0f;
    float width = 1.0f;
    float depth = 1.0f;
};
