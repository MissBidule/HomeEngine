//
//  Element.hpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#pragma once

#include "ShaderParam.hpp"
#include "Camera.hpp"
#include "VertexData.hpp"
#include "Texture.hpp"

#include <stb_image.h>
#include <list>

class Element {
public:
    static std::list<Element*> elementList;
    
    Element(ShaderParam* shader);
    virtual ~Element() {};
    void cleanup();
    
    void draw(MTL::RenderCommandEncoder* renderCommandEncoder, Camera* camera, float aspectRatio);
    void applytexture(const char* texturePath = "", MTL::Device* metalDevice = nullptr);
    void changeShader(ShaderParam& newShader);
    
    void setColor(simd::float4 newColor);
    simd::float4 getColor();
    void setPosition(simd::float3 newPosition);
    simd::float3 getPosition();
    matrix_float4x4 getModelMatrix();
    
protected:
    virtual void createVertexUV(MTL::Device* metalDevice) = 0;
    void createBuffers(MTL::Device* metalDevice);
    
    Texture* texture;
    simd::float4 color = simd::float4 {1,1,1,1};
    simd::float3 position = simd::float3 {0,0,0};
    matrix_float4x4 modelMatrix;
    MTL::PrimitiveType primitiveType = MTL::PrimitiveTypeTriangle;
    MTL::TriangleFillMode fillMode = MTL::TriangleFillModeFill;
    NS::UInteger vertexStart = 0;
    NS::UInteger vertexCount = 0;
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* transformationBuffer;
    ShaderParam* shader = nullptr;
};
