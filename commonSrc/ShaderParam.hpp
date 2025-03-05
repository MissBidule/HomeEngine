//
//  ShaderParam.hpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#pragma once

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

//creates a shader with specific vertex and fragment shader to use
class ShaderParam {
public:
    ShaderParam(const char* vertexFunction, const char* fragmentFunction, CA::MetalLayer* metalLayer, MTL::Device* metalDevice, MTL::Library* metalLibrary, const int sampleCount);
    const MTL::RenderPipelineState* getRenderPipelineState() const;
    
private:
    MTL::RenderPipelineState* renderPipelineState;
};
