//
//  Camera.cpp
//  HomeEngine
//
//  Created by MissBidule on 04/03/2025.
//
#include "Camera.hpp"

void Camera::ChangeView() {
    viewType = (View)(((int)viewType + 1) % 2);
}

matrix_float4x4 Camera::getViewMatrix() {
    simd::float3& R = rightVector;
    simd::float3& U = upVector;
    simd::float3& F = forwardVector;
    simd::float3& P = cameraPosition;
    
    return matrix_make_rows( R.x, R.y, R.z, simd::dot(-R, P),
                             U.x, U.y, U.z, simd::dot(-U, P),
                            -F.x,-F.y,-F.z, simd::dot( F, P),
                            0.0, 0.0, 0.0, 1);
}

matrix_float4x4 Camera::getPerspectiveMatrix(float width, float height) {
    switch (viewType) {
        case View::Perspective:
            return matrix_perspective_right_hand(fov, width/height, nearZ, farZ);
            break;
            
        case View::Orthogonal:
            return matrix_ortho_right_hand(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, nearZ, farZ);
            break;
            
        default:
            return matrix4x4_identity();
            break;
    }
}
