//
//  mtl_engine.cpp
//  HomeEngine
//
//  Created by MissBidule on 19/02/2025.
//
#include "GLFWBridge.h"
#include "mtl_engine.hpp"
#include <iostream>

void MTLEngine::init() {
    initDevice();
    initWindow();
    
    createCube();
    createDefaultLibrary();
    createCommandQueue();
    createRenderPipeline();
}

void MTLEngine::run() {
    while (!glfwWindowShouldClose(glfwWindow)) {
        pPool = NS::AutoreleasePool::alloc()->init();
        metalDrawable = metalLayer->nextDrawable();
        draw();
        pPool->release();
        glfwPollEvents();
    }
}

void MTLEngine::cleanup() {
    glfwTerminate();
    metalDevice->release();
}

void MTLEngine::initDevice() {
    metalDevice = MTL::CreateSystemDefaultDevice();
}

void MTLEngine::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    MTLEngine* engine = (MTLEngine*)glfwGetWindowUserPointer(window);
    engine->resizeFrameBuffer(width, height);
}

void MTLEngine::resizeFrameBuffer(int width, int height) {
    metalLayer->setDrawableSize(CGSizeMake(width, height));
}

void MTLEngine::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindow = glfwCreateWindow(800, 600, "HomeEngine", NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, frameBufferSizeCallback);
    
    metalLayer = CA::MetalLayer::layer();
    metalLayer->setDevice(metalDevice);
    metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    metalLayer->setDrawableSize(CGSizeMake(width, height));
    
    GLFWBridge::AddLayerToWindow(glfwWindow, metalLayer);
}

void MTLEngine::createCube() {
    VertexData cubeVertices[] = {
        //Front face
        {{-0.5f,-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,-0.5f, 0.5f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f,-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}},
        
        //Back face
        {{ 0.5f,-0.5f,-0.5f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f,-0.5f,-0.5f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f,-0.5f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f,-0.5f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, 0.5f,-0.5f, 1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,-0.5f,-0.5f, 1.0f}, {0.0f, 0.0f}},
        
        //Top face
        {{-0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, 0.5f,-0.5f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, 0.5f,-0.5f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f,-0.5f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}},
        
        //Bottom face
        {{-0.5f,-0.5f,-0.5f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,-0.5f,-0.5f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,-0.5f, 0.5f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f,-0.5f, 0.5f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,-0.5f, 0.5f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f,-0.5f,-0.5f, 1.0f}, {0.0f, 0.0f}},
        
        //Left face
        {{-0.5f,-0.5f,-0.5f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f,-0.5f, 0.5f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f,-0.5f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f,-0.5f,-0.5f, 1.0f}, {0.0f, 0.0f}},
        
        //Right face
        {{ 0.5f,-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,-0.5f,-0.5f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, 0.5f,-0.5f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, 0.5f,-0.5f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,-0.5f, 0.5f, 1.0f}, {0.0f, 0.0f}}
    };
    
    cubeVertexBuffer = metalDevice->newBuffer(&cubeVertices, sizeof(cubeVertices), MTL::ResourceStorageModeShared);
    
    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    
    lavaTexture = new Texture("assets/pretty.png", metalDevice);
}

void MTLEngine::createDefaultLibrary() {
    metalDefaultLibrary = metalDevice->newDefaultLibrary();
    if (!metalDefaultLibrary) {
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
}

void MTLEngine::createCommandQueue() {
    metalCommandQueue = metalDevice->newCommandQueue();
}

void MTLEngine::createRenderPipeline() {
    MTL::Function* vertexShader = metalDefaultLibrary->newFunction(NS::String::string("vertexShader", NS::ASCIIStringEncoding));
    assert(vertexShader);
    MTL::Function* fragmentShader = metalDefaultLibrary->newFunction(NS::String::string("fragmentShader", NS::ASCIIStringEncoding));
    assert(fragmentShader);
    
    MTL::RenderPipelineDescriptor* renderPipelineDecriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    renderPipelineDecriptor->setLabel(NS::String::string("Triangle Rendering Pipeline", NS::ASCIIStringEncoding));
    renderPipelineDecriptor->setVertexFunction(vertexShader);
    renderPipelineDecriptor->setFragmentFunction(fragmentShader);
    assert(renderPipelineDecriptor);
    MTL::PixelFormat pixelFormat = metalLayer->pixelFormat();
    renderPipelineDecriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);
    
    NS::Error* error;
    metalRenderPSO = metalDevice->newRenderPipelineState(renderPipelineDecriptor, &error);
    
    renderPipelineDecriptor->release();
}

void MTLEngine::draw() {
    sendRenderCommand();
}

void MTLEngine::sendRenderCommand() {
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    
    MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* cd = renderPassDescriptor->colorAttachments()->object(0);
    cd->setTexture(metalDrawable->texture());
    cd->setLoadAction(MTL::LoadActionClear);
    cd->setClearColor(MTL::ClearColor(41.0f/255.0f, 42.0f/255.0f, 48.0f/255.0f, 1.0));
    cd->setStoreAction(MTL::StoreActionStore);
    
    MTL::RenderCommandEncoder* renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    encodeRenderCommand(renderCommandEncoder);
    renderCommandEncoder->endEncoding();
    
    metalCommandBuffer->presentDrawable(metalDrawable);
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();
    
    renderPassDescriptor->release();
}

void MTLEngine::encodeRenderCommand(MTL::RenderCommandEncoder* renderCommandEncoder) {
    //Moves the cube 2 units down the negative Z-axis
    matrix_float4x4 translationMatrix = matrix4x4_translation(0.0, 0.0, -1.0);
    
    //Moves the cube over time
    float angleInDegrees = glfwGetTime()/2.0 * 45;
    float angleInRadians = angleInDegrees * M_PI / 180.0f;
    matrix_float4x4 rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);
    
    matrix_float4x4 modelMatrix = simd_mul(translationMatrix, rotationMatrix);
    
    simd::float3 R = simd::float3 {1, 0, 0}; //Right to camera
    simd::float3 U = simd::float3 {0, 1, 0}; //Up to camera
    simd::float3 F = simd::float3 {0, 0,-1}; //Forward to camera
    simd::float3 P = simd::float3 {0, 0, 1}; //Position of camera
    
    matrix_float4x4 viewMatrix = matrix_make_rows( R.x, R.y, R.z, simd::dot(-R, P),
                                                   U.x, U.y, U.z, simd::dot(-U, P),
                                                  -F.x,-F.y,-F.z, simd::dot( F, P),
                                                  0.0, 0.0, 0.0, 1);
    
    float fov = 90 * (M_PI / 180.0f);
    float aspectRatio = (metalLayer->drawableSize().width / metalLayer->drawableSize().height);
    float nearZ = 0.1f;
    float farZ = 100.0f;
    
    matrix_float4x4 perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);
    
    TransformationData transformationData = {modelMatrix, viewMatrix, perspectiveMatrix};
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));
    
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setVertexBuffer(cubeVertexBuffer, 0, 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
    NS::UInteger vertexStart = 0;
    NS::UInteger vertexCount = 36;
    renderCommandEncoder->setFragmentTexture(lavaTexture->texture, 0);
    renderCommandEncoder->drawPrimitives(typeTriangle, vertexStart, vertexCount);
}
