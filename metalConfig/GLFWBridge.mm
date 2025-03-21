//
//  GLFWBridge.mm
//  HomeEngine
//
//  Created by MissBidule on 24/02/2025.
//
#include "GLFWBridge.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.h>
#include <QuartzCore/QuartzCore.h>

namespace GLFWBridge {
void AddLayerToWindow(GLFWwindow* window, CA::MetalLayer* layer) {
    NSWindow* cocoa_window = glfwGetCocoaWindow(window);
    CAMetalLayer* native_layer = (__bridge CAMetalLayer*) layer;
    [[cocoa_window contentView] setLayer:native_layer];
    [[cocoa_window contentView] setWantsLayer:YES];
    [[cocoa_window contentView] setNeedsLayout:YES];
    }
}
