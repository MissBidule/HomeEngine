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
    
    {//elements
        ImGui::Begin("Add element :", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        
        if (ImGui::Button("Cube")) {
            Cube* cube = new Cube(mtlEngine->textureShader, mtlEngine->metalDevice);
            cube->applytexture("assets/pretty.png", mtlEngine->metalDevice);
        }
        
        ImGui::NewLine();
        
        for (std::list<Element*>::iterator it = Element::elementList.begin(); it != Element::elementList.end(); ++it)
        {
            //change position
            ImGui::DragFloat3(("position of " + (*it)->getName()).c_str(), (float*)&(*it)->position, 0.1f);
            
            //change dimension
            ImGui::DragFloat3(("scale of " + (*it)->getName()).c_str(), (float*)&(*it)->scale, 0.1f, 0.0f, FLT_MAX);
            
            //change rotation in degrees
            ImGui::DragFloat3(("rotation of " + (*it)->getName()).c_str(), (float*)&(*it)->rotation, 1.0f);
            
            if (ImGui::Button(("delete " + (*it)->getName()).c_str())) {
                (*it)->cleanup();
                delete (*it);
                it = Element::elementList.erase(it);
            }

            ImGui::NewLine();
        }
        
        ImGui::NewLine();
        
        ImGui::Text("Application frame (%.1f FPS)", io->Framerate);
        
        ImGui::Checkbox("Demo Window", &show_demo_window);
        
        ImGui::End();
    }
    
    {//camera
        ImGui::Begin("Camera setting :", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        
        if (ImGui::Button("Change view")) {
            mtlEngine->sceneCamera->changeView();
        }
        
        ImGui::NewLine();
        
        if (mtlEngine->sceneCamera->getViewType() == View::Perspective) {
            ImGui::Text("Perspective view parameters :");
            ImGui::DragFloat("FOV", &mtlEngine->sceneCamera->fov, 0.1f, 0.0f, 360.0f);
        }
        else {
            ImGui::Text("Orthogonal view parameters :");
        }
        ImGui::DragFloat("nearZ", &mtlEngine->sceneCamera->nearZ, 0.01f, 0.0f, mtlEngine->sceneCamera->farZ);
        ImGui::DragFloat("farZ", &mtlEngine->sceneCamera->farZ, 0.1f, 0.0f, FLT_MAX);
        
        ImGui::NewLine();
        
        ImGui::Text("Global camera parameters :");
        ImGui::DragFloat3("Camera position", (float*)&mtlEngine->sceneCamera->cameraPosition, 0.1f);
        ImGui::DragFloat3("Camera right vector", (float*)&mtlEngine->sceneCamera->rightVector, 0.1f);
        ImGui::DragFloat3("Camera up vector", (float*)&mtlEngine->sceneCamera->upVector, 0.1f);
        ImGui::DragFloat3("Camera forward vector", (float*)&mtlEngine->sceneCamera->forwardVector, 0.1f);
        
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
