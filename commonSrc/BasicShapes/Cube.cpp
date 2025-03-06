//
//  Cube.cpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#include "Cube.hpp"

Cube::Cube(ShaderParam* shaderParam, MTL::Device* metalDevice) : Element(shaderParam) {
    createVertexUV(metalDevice);
    createBuffers(metalDevice);
}

void Cube::setDimensions(simd::float3 newDimensions) {
    width   = newDimensions.x;
    height  = newDimensions.y;
    depth   = newDimensions.z;
}

simd::float3 Cube::getDimensions() {
    return simd::float3 {width, height, depth};
}

void Cube::createVertexUV(MTL::Device* metalDevice) {
    float x = width/2.0f;
    float y = height/2.0f;
    float z = depth/2.0f;
    
    VertexData cubeVertices[] = {
        //Front face
        {{-x,-y, z, 1.0f}, {0.0f, 0.0f}},
        {{ x,-y, z, 1.0f}, {1.0f, 0.0f}},
        {{ x, y, z, 1.0f}, {1.0f, 1.0f}},
        {{ x, y, z, 1.0f}, {1.0f, 1.0f}},
        {{-x, y, z, 1.0f}, {0.0f, 1.0f}},
        {{-x,-y, z, 1.0f}, {0.0f, 0.0f}},
        
        //Back face
        {{ x,-y,-z, 1.0f}, {0.0f, 0.0f}},
        {{-x,-y,-z, 1.0f}, {1.0f, 0.0f}},
        {{-x, y,-z, 1.0f}, {1.0f, 1.0f}},
        {{-x, y,-z, 1.0f}, {1.0f, 1.0f}},
        {{ x, y,-z, 1.0f}, {0.0f, 1.0f}},
        {{ x,-y,-z, 1.0f}, {0.0f, 0.0f}},
        
        //Top face
        {{-x, y, z, 1.0f}, {0.0f, 0.0f}},
        {{ x, y, z, 1.0f}, {1.0f, 0.0f}},
        {{ x, y,-z, 1.0f}, {1.0f, 1.0f}},
        {{ x, y,-z, 1.0f}, {1.0f, 1.0f}},
        {{-x, y,-z, 1.0f}, {0.0f, 1.0f}},
        {{-x, y, z, 1.0f}, {0.0f, 0.0f}},
        
        //Bottom face
        {{-x,-y,-z, 1.0f}, {0.0f, 0.0f}},
        {{ x,-y,-z, 1.0f}, {1.0f, 0.0f}},
        {{ x,-y, z, 1.0f}, {1.0f, 1.0f}},
        {{ x,-y, z, 1.0f}, {1.0f, 1.0f}},
        {{-x,-y, z, 1.0f}, {0.0f, 1.0f}},
        {{-x,-y,-z, 1.0f}, {0.0f, 0.0f}},
        
        //Left face
        {{-x,-y,-z, 1.0f}, {0.0f, 0.0f}},
        {{-x,-y, z, 1.0f}, {1.0f, 0.0f}},
        {{-x, y, z, 1.0f}, {1.0f, 1.0f}},
        {{-x, y, z, 1.0f}, {1.0f, 1.0f}},
        {{-x, y,-z, 1.0f}, {0.0f, 1.0f}},
        {{-x,-y,-z, 1.0f}, {0.0f, 0.0f}},
        
        //Right face
        {{ x,-y, z, 1.0f}, {0.0f, 0.0f}},
        {{ x,-y,-z, 1.0f}, {1.0f, 0.0f}},
        {{ x, y,-z, 1.0f}, {1.0f, 1.0f}},
        {{ x, y,-z, 1.0f}, {1.0f, 1.0f}},
        {{ x, y, z, 1.0f}, {0.0f, 1.0f}},
        {{ x,-y, z, 1.0f}, {0.0f, 0.0f}}
    };
    
    vertexBuffer = metalDevice->newBuffer(&cubeVertices, sizeof(cubeVertices), MTL::ResourceStorageModeShared);
    
    vertexCount = 36;
}
