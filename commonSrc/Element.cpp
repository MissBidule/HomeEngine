//
//  Element.cpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#include "Element.hpp"

std::list<Element*> Element::elementList = std::list<Element*>();

bool Element::cmp(const Element* a, const Element* b) {
    return a->position.z < b->position.z;
}

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

std::string Element::getName() {
    return name + std::to_string(id);
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

void Element::setScale(simd::float3 newScale) {
    scale = newScale;
}

simd::float3 Element::getScale() {
    return scale;
}

void Element::setRotation(simd::float3 newRotation) {
    rotation = newRotation;
}

simd::float3 Element::getRotation() {
    return rotation;
}

matrix_float4x4 Element::getModelMatrix() {
    matrix_float4x4 translationMatrix = matrix4x4_translation(getPosition());
    matrix_float4x4 scaleMatrix = matrix4x4_scale(getScale());
    matrix_float4x4 XrotationMatrix = matrix4x4_rotation(rotation.x * M_PI / 180.0f, 1, 0, 0);
    matrix_float4x4 YrotationMatrix = matrix4x4_rotation(rotation.y * M_PI / 180.0f, 0, 1, 0);
    matrix_float4x4 ZrotationMatrix = matrix4x4_rotation(rotation.z * M_PI / 180.0f, 0, 0, 1);
    
    matrix_float4x4 rotationMatrix = simd_mul(XrotationMatrix, simd_mul(YrotationMatrix, ZrotationMatrix));
    
    //T*R*S
    return simd_mul(translationMatrix, simd_mul(rotationMatrix, scaleMatrix));
    
    //FOR ME ONLY TEMPORARY
    /*//Moves the cube 2 units down the negative Z-axis
     matrix_float4x4 translationMatrix = matrix4x4_translation(0.0, 0.0, -1.0);
     
     //Moves the cube over time
     float angleInDegrees = glfwGetTime()/2.0 * 45;
     float angleInRadians = angleInDegrees * M_PI / 180.0f;
     matrix_float4x4 rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);
     
     matrix_float4x4 modelMatrix = simd_mul(translationMatrix, rotationMatrix);*/
}
