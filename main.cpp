//
//  main.cpp
//  HomeEngine
//
//  Created by MissBidule on 19/02/2025.
//
#include <Metal/Metal.hpp>
#include "metalConfig/mtl_engine.hpp"

int main() {
    
    MTLEngine engine;
    engine.init();
    engine.run();
    engine.cleanup();
    
    return 0;
}
