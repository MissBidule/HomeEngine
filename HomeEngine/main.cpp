//
//  main.cpp
//  HomeEngine
//
//  Created by MissBidule on 19/02/2025.
//
#include <Metal/Metal.hpp>

#include <iostream>

int main(int argc, const char * argv[]) {
    
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    
    std::cout << "Hello, World!\n";
    return 0;
}
