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
    createBuffers();
    createDefaultLibrary();
    createCommandQueue();
    createRenderPipeline();
    createDepthAndMSAATextures();
    createRenderPassDescriptor();
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
    transformationBuffer->release();
    transformationBuffer2->release();
    msaaRenderTargetTexture->release();
    depthTexture->release();
    renderPassDescriptor->release();
    metalDevice->release();
    delete lavaTexture;
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
    //Deallocate the textures if they have been created
    if (msaaRenderTargetTexture) {
        msaaRenderTargetTexture->release();
        msaaRenderTargetTexture = nullptr;
    }
    if (depthTexture) {
        depthTexture->release();
        depthTexture = nullptr;
    }
    createDepthAndMSAATextures();
    metalDrawable = metalLayer->nextDrawable();
    updateRenderPassDescriptor();
}

float MTLEngine::getAspectRatio() {
    return (metalLayer->drawableSize().width / metalLayer->drawableSize().height);
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
    
    metalDrawable = metalLayer->nextDrawable();
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
    
    lavaTexture = new Texture("assets/pretty.png", metalDevice);
}

void MTLEngine::createBuffers() {
    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    transformationBuffer2 = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
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
    renderPipelineDecriptor->setVertexFunction(vertexShader);
    renderPipelineDecriptor->setFragmentFunction(fragmentShader);
    assert(renderPipelineDecriptor);
    MTL::PixelFormat pixelFormat = metalLayer->pixelFormat();
    renderPipelineDecriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);
    renderPipelineDecriptor->setSampleCount(sampleCount);
    renderPipelineDecriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
    
    NS::Error* error;
    metalRenderPSO = metalDevice->newRenderPipelineState(renderPipelineDecriptor, &error);
    
    if (metalRenderPSO == nil) {
        std::cerr << "Error creating render pipeline state: " << error << std::endl;
        std::exit(0);
    }
    //-//
    MTL::DepthStencilDescriptor* depthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
    depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLessEqual);
    depthStencilDescriptor->setDepthWriteEnabled(true);
    depthStencilState = metalDevice->newDepthStencilState(depthStencilDescriptor);
    //-//
    renderPipelineDecriptor->release();
    vertexShader->release();
    fragmentShader->release();
}

void MTLEngine::createDepthAndMSAATextures() {
    MTL::TextureDescriptor* msaaTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    msaaTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    msaaTextureDescriptor->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    msaaTextureDescriptor->setWidth(metalLayer->drawableSize().width);
    msaaTextureDescriptor->setHeight(metalLayer->drawableSize().height);
    msaaTextureDescriptor->setSampleCount(sampleCount);
    msaaTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    
    msaaRenderTargetTexture = metalDevice->newTexture(msaaTextureDescriptor);
    
    MTL::TextureDescriptor* depthTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthTextureDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthTextureDescriptor->setWidth(metalLayer->drawableSize().width);
    depthTextureDescriptor->setHeight(metalLayer->drawableSize().height);
    depthTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    depthTextureDescriptor->setSampleCount(sampleCount);
    
    depthTexture = metalDevice->newTexture(depthTextureDescriptor);
    
    msaaTextureDescriptor->release();
    depthTextureDescriptor->release();
}

void MTLEngine::createRenderPassDescriptor() {
    renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = renderPassDescriptor->depthAttachment();
    
    colorAttachment->setTexture(msaaRenderTargetTexture);
    colorAttachment->setResolveTexture(metalDrawable->texture());
    colorAttachment->setLoadAction(MTL::LoadActionClear);
    colorAttachment->setClearColor(MTL::ClearColor(41.0f/255.0f, 42.0f/255.0f, 48.0f/255.0f, 1.0));
    colorAttachment->setStoreAction(MTL::StoreActionMultisampleResolve);
    
    depthAttachment->setTexture(depthTexture);
    depthAttachment->setLoadAction(MTL::LoadActionClear);
    depthAttachment->setStoreAction(MTL::StoreActionDontCare);
    depthAttachment->setClearDepth(1.0);
}

void MTLEngine::updateRenderPassDescriptor() {
    renderPassDescriptor->colorAttachments()->object(0)->setTexture(msaaRenderTargetTexture);
    renderPassDescriptor->colorAttachments()->object(0)->setResolveTexture(metalDrawable->texture());
    renderPassDescriptor->depthAttachment()->setTexture(depthTexture);
}

void MTLEngine::draw() {
    sendRenderCommand();
}

void MTLEngine::sendRenderCommand() {
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    
    updateRenderPassDescriptor();
    MTL::RenderCommandEncoder* renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    encodeRenderCommand(renderCommandEncoder);
    renderCommandEncoder->endEncoding();
    
    metalCommandBuffer->presentDrawable(metalDrawable);
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();
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
    float aspectRatio = getAspectRatio();
    float nearZ = 0.1f;
    float farZ = 100.0f;
    
    matrix_float4x4 perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);
    
    TransformationData transformationData = {modelMatrix, viewMatrix, perspectiveMatrix};
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));
    
    renderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
    renderCommandEncoder->setCullMode(MTL::CullModeBack);
    renderCommandEncoder->setDepthStencilState(depthStencilState);
    
    renderCommandEncoder->setTriangleFillMode(MTL::TriangleFillModeFill);
    //here which shader
    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    //vertex pos
    renderCommandEncoder->setVertexBuffer(cubeVertexBuffer, 0, 0);
    //vertex transform
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    //how to draw
    MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
    NS::UInteger vertexStart = 0;
    NS::UInteger vertexCount = 36;
    //frag texture if needed
    renderCommandEncoder->setFragmentTexture(lavaTexture->texture, 0);
    //drawing step
    renderCommandEncoder->drawPrimitives(typeTriangle, vertexStart, vertexCount);
    //example2
    matrix_float4x4 translationMatrix2 = matrix4x4_translation(1.0, 0.0, -1.0);
    matrix_float4x4 modelMatrix2 = simd_mul(translationMatrix2, rotationMatrix);
    transformationData.modelMatrix = modelMatrix2;
    memcpy(transformationBuffer2->contents(), &transformationData, sizeof(transformationData));
    renderCommandEncoder->setVertexBuffer(transformationBuffer2, 0, 1);
    renderCommandEncoder->drawPrimitives(typeTriangle, vertexStart, vertexCount);
    
}
