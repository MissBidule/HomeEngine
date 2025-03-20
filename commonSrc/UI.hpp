//
//  UI.hpp
//  HomeEngine
//
//  Created by MissBidule on 20/03/2025.
//
#pragma once

#include "imgui.h"
#include <Metal/Metal.hpp>

class MTLEngine;
class GLFWwindow;

class UI {
public:
    UI(GLFWwindow* window, MTL::Device* device);
    virtual ~UI() {};
    void cleanup();
    
    void draw(MTLEngine* mtlEngine, MTL::RenderCommandEncoder* renderCommandEncoder);
    
private:
    ImGuiIO* io;
    
    bool show_demo_window = false;
};
