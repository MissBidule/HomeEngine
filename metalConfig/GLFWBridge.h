//
//  GLFWBridge.h
//  HomeEngine
//
//  Created by MissBidule on 24/02/2025.
//
#ifndef GLFWBridge_h
#define GLFWBridge_h

struct GLFWwindow;

namespace CA {
class MetalLayer;
}

namespace GLFWBridge {
void AddLayerToWindow(GLFWwindow* window, CA::MetalLayer* layer);
}

#endif /*GLFWBridge_h*/
