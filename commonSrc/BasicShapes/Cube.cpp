//
//  Cube.cpp
//  HomeEngine
//
//  Created b1.0f MissBidule on 04/03/2025.
//
#include "Cube.hpp"

int Cube::nameNumber = 1;

Cube::Cube(ShaderParam* shaderParam, MTL::Device* metalDevice) : Element(shaderParam) {
    name = "Cube";
    id = nameNumber++;
    createVertexUV(metalDevice);
    createBuffers(metalDevice);
}


void Cube::createVertexUV(MTL::Device* metalDevice) {
    
    VertexData cubeVertices[] = {
        //Front face
        {{-1.0f,-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        
        //Back face
        {{ 1.0f,-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1.0f}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f}},
        
        //Top face
        {{-1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        
        //Bottom face
        {{-1.0f,-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f,-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f}},
        
        //Left face
        {{-1.0f,-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f,-1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f,-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f}},
        
        //Right face
        {{ 1.0f,-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 1.0f,-1.0f,-1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f,-1.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{ 1.0f,-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
    };
    
    vertexBuffer = metalDevice->newBuffer(&cubeVertices, sizeof(cubeVertices), MTL::ResourceStorageModeShared);
    
    vertexCount = 36;
}
