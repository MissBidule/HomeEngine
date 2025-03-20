//
//  Camera.hpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#pragma once

#include "AAPLMathUtilities.h"

enum class View {
    Perspective = 0,
    Orthogonal = 1
};

class Camera {
public:
    void changeView();
    View getViewType();
    void setPosition(simd::float3 newPosition);
    simd::float3 getPosition();
    matrix_float4x4 getViewMatrix();
    matrix_float4x4 getPerspectiveMatrix(float aspectRatio);
    //movement with mouse (right clic / left clic / scroll) +
    //movement with function -> imgui +
private:
    simd::float3 cameraPosition = simd::float3 {0, 0, 1};
    simd::float3 rightVector    = simd::float3 {1, 0, 0};
    simd::float3 upVector       = simd::float3 {0, 1, 0};
    simd::float3 forwardVector  = simd::float3 {0, 0,-1};
    float fov   = 90;
    float nearZ = 0.1f;
    float farZ  = 100.0f;
    View viewType = View::Perspective;
    
    friend class UI;
};
