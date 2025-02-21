//
//  VertexData.hpp
//  HomeEngine
//
//  Created by MissBidule on 21/02/2025.
//
#pragma once
#include <simd/simd.h>

struct VertexData {
    simd::float4 position;
    simd::float2 textureCoordinate;
};
