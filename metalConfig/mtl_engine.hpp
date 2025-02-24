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
#include <stb_image.h>

#include <simd/simd.h>
#include <filesystem>

class MTLEngine {
public:
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    void resizeFrameBuffer(int width, int height);
    
    void init();
    void run();
    void cleanup();
    
private:
    void initDevice();
    void initWindow();
    
    void createSquare();
    void createDefaultLibrary();
    void createCommandQueue();
    void createRenderPipeline();
    
    void encodeRenderCommand(MTL::RenderCommandEncoder* renderEncoder);
    void sendRenderCommand();
    void draw();
    
    MTL::Device* metalDevice;
    GLFWwindow* glfwWindow;
    CA::MetalLayer* metalLayer;
    CA::MetalDrawable* metalDrawable;
    
    NS::AutoreleasePool* pPool;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* squareVertexBuffer;
    
    Texture* lavaTexture;
};
