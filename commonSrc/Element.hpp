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
    static bool cmp(const Element* a, const Element* b); //sort the elements from farthest to nearest
    
    Element(ShaderParam* shader);
    virtual ~Element() {};
    void cleanup();
    
    void draw(MTL::RenderCommandEncoder* renderCommandEncoder, Camera* camera, float aspectRatio);
    void applytexture(const char* texturePath = "", MTL::Device* metalDevice = nullptr);
    void changeShader(ShaderParam& newShader);
    
    std::string getName();
    void setColor(simd::float4 newColor);
    simd::float4 getColor();
    void setPosition(simd::float3 newPosition);
    simd::float3 getPosition();
    void setScale(simd::float3 newScale);
    simd::float3 getScale();
    void setRotation(simd::float3 newRotation);
    simd::float3 getRotation();
    
    matrix_float4x4 getModelMatrix();
    
protected:
    virtual void createVertexUV(MTL::Device* metalDevice) = 0;
    void createBuffers(MTL::Device* metalDevice);
    
    std::string name = "";
    int id = 0;
    Texture* texture;
    simd::float4 color = simd::float4 {1,1,1,1};
    simd::float3 position = simd::float3 {0,0,-4};
    simd::float3 scale = simd::float3 {1,1,1};
    simd::float3 rotation = simd::float3 {0,0,0};
    matrix_float4x4 modelMatrix;
    MTL::PrimitiveType primitiveType = MTL::PrimitiveTypeTriangle;
    MTL::TriangleFillMode fillMode = MTL::TriangleFillModeFill;
    NS::UInteger vertexStart = 0;
    NS::UInteger vertexCount = 0;
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* transformationBuffer;
    ShaderParam* shader = nullptr;
    
    friend class UI;
};
