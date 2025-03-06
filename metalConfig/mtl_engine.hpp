//
//  mtl_engine.hpp
//  HomeEngine
//
//  Created by MissBidule on 19/02/2025.
//
#pragma once

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "VertexData.hpp"
#include "Texture.hpp"
#include "ShaderParam.hpp"
#include "Camera.hpp"
#include <stb_image.h>

#include <simd/simd.h>
#include <filesystem>

#include "AAPLMathUtilities.h"

class MTLEngine {
public:
    void init();
    void run();
    void cleanup();//TODO
    
private:
    void initDevice();
    void initWindow();
    
    void createBasicShaders();
    void createDefaultLibrary();
    void createCommandQueue();
    void createRenderPipeline();
    void createDepthAndMSAATextures();
    void createRenderPassDescriptor();
    
    //Upon resizing, update Depth and MSAA Textures
    void updateRenderPassDescriptor();
    
    void encodeRenderCommand(MTL::RenderCommandEncoder* renderEncoder);
    void sendRenderCommand();
    void draw();
    
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    void resizeFrameBuffer(int width, int height);
    
    float getAspectRatio();
    
    MTL::Device* metalDevice;
    GLFWwindow* glfwWindow;
    CA::MetalLayer* metalLayer;
    CA::MetalDrawable* metalDrawable;
    
    NS::AutoreleasePool* pPool;
    
    Camera* sceneCamera;
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::DepthStencilState* depthStencilState;
    MTL::RenderPassDescriptor* renderPassDescriptor;
    MTL::Texture* msaaRenderTargetTexture = nullptr;
    MTL::Texture* depthTexture;
    
    const int sampleCount = 4;
    
    //flat color shader
    ShaderParam* textureShader;
    //light shader
};
