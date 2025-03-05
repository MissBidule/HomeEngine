//
//  ShaderParam.cpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#include "ShaderParam.hpp"

#include <iostream>

ShaderParam::ShaderParam(const char* vertexFunction, const char* fragmentFunction, CA::MetalLayer* metalLayer, MTL::Device* metalDevice, MTL::Library* metalLibrary, const int sampleCount) {
    
    MTL::Function* vertexShader = metalLibrary->newFunction(NS::String::string(vertexFunction, NS::ASCIIStringEncoding));
    assert(vertexShader);
    MTL::Function* fragmentShader = metalLibrary->newFunction(NS::String::string(fragmentFunction, NS::ASCIIStringEncoding));
    assert(fragmentShader);
    
    MTL::RenderPipelineDescriptor* renderPipelineDecriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    renderPipelineDecriptor->setVertexFunction(vertexShader);
    renderPipelineDecriptor->setFragmentFunction(fragmentShader);
    assert(renderPipelineDecriptor);
    MTL::PixelFormat pixelFormat = metalLayer->pixelFormat();
    renderPipelineDecriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);
    renderPipelineDecriptor->setSampleCount(sampleCount);
    renderPipelineDecriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
    
    NS::Error* error;
    renderPipelineState = metalDevice->newRenderPipelineState(renderPipelineDecriptor, &error);
    
    if (renderPipelineState == nil) {
        std::cerr << "Error creating render pipeline state: " << error << std::endl;
        std::exit(0);
    }
    
    renderPipelineDecriptor->release();
    vertexShader->release();
    fragmentShader->release();
}

const MTL::RenderPipelineState* ShaderParam::getRenderPipelineState() const {
    return renderPipelineState;
}

