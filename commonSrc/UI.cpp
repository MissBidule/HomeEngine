//
//  UI.cpp
//  HomeEngine
//
//  Created by MissBidule on 20/03/2025.
//
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"

#include "UI.hpp"
#include "../metalConfig/mtl_engine.hpp"
#include "Cube.hpp"

UI::UI(GLFWwindow* window, MTL::Device* device) {
    
    //setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    
    //init
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplMetal_Init(device);
}

void UI::cleanup() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::draw(MTLEngine* mtlEngine, MTL::RenderCommandEncoder* renderCommandEncoder) {
    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(mtlEngine->renderPassDescriptor);
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    
    {
        ImGui::Begin("Add element :", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        
        if (ImGui::Button("Cube")) {
            Cube* cube = new Cube(mtlEngine->textureShader, mtlEngine->metalDevice);
            cube->applytexture("assets/pretty.png", mtlEngine->metalDevice);
        }
        
        ImGui::NewLine();
        
        for (Element* element : Element::elementList) {
            //change position
            ImGui::DragFloat3(("position of " + element->getName()).c_str(), (float*)&element->position, 0.1f);
            
            //change dimension
            ImGui::DragFloat3(("scale of " + element->getName()).c_str(), (float*)&element->scale, 0.1f, 0.0f, std::numeric_limits<float>::max());
            
            //change rotation
            ImGui::DragFloat3(("rotation of " + element->getName()).c_str(), (float*)&element->rotation, 0.1f);

            ImGui::NewLine();
        }
        
        ImGui::NewLine();
        
        ImGui::Text("Application frame (%.1f FPS)", io->Framerate);
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), mtlEngine->metalCommandBuffer, renderCommandEncoder);
}

/* 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
{
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    ImGui::End();
}*/
