//
//  Cube.hpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#pragma once

#include "Element.hpp"

class Cube : public Element {
public:
    static int nameNumber;
    Cube(ShaderParam* shaderParam, MTL::Device* metalDevice);
    ~Cube() override {};
    
private:
    void createVertexUV(MTL::Device* metalDevice) override;
};
