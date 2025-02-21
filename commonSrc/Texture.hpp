//
//  Texture.hpp
//  HomeEngine
//
//  Created by MissBidule on 21/02/2025.
//
#pragma once
#include <Metal/Metal.hpp>
#include <stb_image.h>

class Texture {
public:
    Texture(const char* filepath, MTL::Device* metalDevice);
    ~Texture();
    MTL::Texture* texture;
    int width, height, channels;
    
private:
    MTL::Device* device;
};
