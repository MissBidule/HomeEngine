//
//  Element.cpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#include "Element.hpp"

std::list<Element*> Element::elementList = std::list<Element*>();

Element::Element(ShaderParam* shaderParam) : shader(shaderParam){
    elementList.emplace_back(this);
}

void Element::cleanup() {
    transformationBuffer->release();
    vertexBuffer->release();
    delete texture;
}

void Element::createBuffers(MTL::Device* metalDevice) {
    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
}

void Element::draw(MTL::RenderCommandEncoder* renderCommandEncoder, Camera* camera, float aspectRatio) {
    TransformationData transformationData = {getModelMatrix(), camera->getViewMatrix(), camera->getPerspectiveMatrix(aspectRatio)};
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));
    
    renderCommandEncoder->setTriangleFillMode(fillMode);
    renderCommandEncoder->setRenderPipelineState(shader->getRenderPipelineState());
    renderCommandEncoder->setVertexBuffer(vertexBuffer, 0, 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    if (texture) renderCommandEncoder->setFragmentTexture(texture->texture, 0);
    renderCommandEncoder->drawPrimitives(primitiveType, vertexStart, vertexCount);
}

void Element::applytexture(const char* texturePath, MTL::Device* metalDevice) {
    if (texture) {
        delete texture;
        texture = nullptr;
    }
    
    if (strcmp(texturePath, "") != 0) texture = new Texture(texturePath, metalDevice);
}

void Element::changeShader(ShaderParam& newShader) {
    shader = &newShader;
}

void Element::setColor(simd::float4 newColor) {
    color = newColor;
}

simd::float4 Element::getColor() {
    return color;
}

void Element::setPosition(simd::float3 newPosition) {
    position = newPosition;
}

simd::float3 Element::getPosition() {
    return position;
}

matrix_float4x4 Element::getModelMatrix() {
    return matrix4x4_translation(getPosition());
    
    //FOR ME ONLY TEMPORARY
    /*//Moves the cube 2 units down the negative Z-axis
     matrix_float4x4 translationMatrix = matrix4x4_translation(0.0, 0.0, -1.0);
     
     //Moves the cube over time
     float angleInDegrees = glfwGetTime()/2.0 * 45;
     float angleInRadians = angleInDegrees * M_PI / 180.0f;
     matrix_float4x4 rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);
     
     matrix_float4x4 modelMatrix = simd_mul(translationMatrix, rotationMatrix);*/
}
