#include "raylib.h"
#include "rlgl.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// State for our plugins
Vector2 redDotOffset = { 0, 0 };
Vector2 greenDotOffset = { 0, 0 };
float redAmplitude = 50.0f;    
float greenAmplitude = 50.0f;  
float totalTime = 0;

// State for the Demo Component
Color demoColor = BLUE;
bool demoShowGrid = true;
float demoRotation = 0.0f;

// Helper to render a Raylib Texture into an ImGui Window with mixed UI controls
void DrawPluginViewport(const char* name, RenderTexture2D& target, float* amplitude, Vector2* offset, bool horizontal) {
    ImGui::Begin(name);
    
    // 1. MIXING IMGUI CONTROLS
    ImGui::Text("Animation Settings");
    ImGui::SliderFloat("Amplitude", amplitude, 0.0f, 200.0f);
    ImGui::Separator();

    // 2. VIEWPORT RENDERING
    ImVec2 size = ImGui::GetContentRegionAvail();
    if (size.x <= 0 || size.y <= 0) size = ImVec2(100, 100);

    if (target.texture.width != (int)size.x || target.texture.height != (int)size.y) {
        UnloadRenderTexture(target);
        target = LoadRenderTexture((int)size.x, (int)size.y);
    }

    ImGui::Image(
        (ImTextureID)(intptr_t)target.texture.id, 
        size, 
        ImVec2(0, 1), 
        ImVec2(1, 0)
    );

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImGui::SetWindowFocus();
    }

    if (horizontal) {
        offset->x = sinf(totalTime * 2.0f) * (*amplitude);
        offset->y = 0;
    } else {
        offset->x = 0;
        offset->y = sinf(totalTime * 2.0f) * (*amplitude);
    }

    ImGui::End();
}

// Dedicated function for the new Demo Component
void DrawDemoComponent(RenderTexture2D& target) {
    ImGui::Begin("Demo Component");

    // ImGui Controls for this specific plugin
    ImGui::Text("3D Visualization Controls");
    ImGui::Checkbox("Show Grid", &demoShowGrid);
    
    float col[3] = { (float)demoColor.r/255.0f, (float)demoColor.g/255.0f, (float)demoColor.b/255.0f };
    if (ImGui::ColorEdit3("Cube Color", col)) {
        demoColor.r = (unsigned char)(col[0] * 255);
        demoColor.g = (unsigned char)(col[1] * 255);
        demoColor.b = (unsigned char)(col[2] * 255);
    }
    
    ImGui::SliderFloat("Rotation Speed", &demoRotation, 0.0f, 5.0f);
    ImGui::Separator();

    // Viewport rendering
    ImVec2 size = ImGui::GetContentRegionAvail();
    if (size.x <= 0 || size.y <= 0) size = ImVec2(100, 100);

    if (target.texture.width != (int)size.x || target.texture.height != (int)size.y) {
        UnloadRenderTexture(target);
        target = LoadRenderTexture((int)size.x, (int)size.y);
    }

    ImGui::Image((ImTextureID)(intptr_t)target.texture.id, size, ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImGui::SetWindowFocus();
    }

    ImGui::End();
}

int main() {
    // 1. Initialize Raylib
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Raylib + ImGui: Mixed Controls");
    SetTargetFPS(60);

    // 2. Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true; 

    ImGui::StyleColorsDark();

    GLFWwindow* window = (GLFWwindow*)GetWindowHandle();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 3. Create Plugin Render Targets
    RenderTexture2D pluginA = LoadRenderTexture(800, 600);
    RenderTexture2D pluginB = LoadRenderTexture(800, 600);
    RenderTexture2D pluginDemo = LoadRenderTexture(800, 600);

    // Setup 3D camera for the Demo Component
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        totalTime += GetFrameTime();

        // --- STEP A: Plugin Rendering ---
        
        BeginTextureMode(pluginA);
            ClearBackground(BLACK);
            DrawCircle(pluginA.texture.width/2 + (int)redDotOffset.x, pluginA.texture.height/2 + (int)redDotOffset.y, 60, RED);
            DrawText("SYSTEM A", 10, 10, 20, RAYWHITE);
        EndTextureMode();

        BeginTextureMode(pluginB);
            ClearBackground(Color{20, 20, 40, 255});
            DrawCircle(pluginB.texture.width/2 + (int)greenDotOffset.x, pluginB.texture.height/2 + (int)greenDotOffset.y, 60, GREEN);
            DrawText("SYSTEM B", 10, 10, 20, RAYWHITE);
        EndTextureMode();

        // Rendering for the Demo Component (3D)
        BeginTextureMode(pluginDemo);
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                if (demoShowGrid) DrawGrid(10, 1.0f);
                rlPushMatrix();
                    rlRotatef(totalTime * 50.0f * demoRotation, 0, 1, 0); // Rotate around Y axis
                    DrawCube((Vector3){0, 0, 0}, 2.0f, 2.0f, 2.0f, demoColor);
                    DrawCubeWires((Vector3){0, 0, 0}, 2.1f, 2.1f, 2.1f, DARKGRAY);
                rlPopMatrix();
            EndMode3D();
            DrawText("3D PREVIEW", 10, 10, 20, DARKGRAY);
        EndTextureMode();

        // --- STEP B: ImGui Frame ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiID dockId = ImGui::GetID("MainDockSpace");
        ImGui::DockSpaceOverViewport(dockId, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        DrawPluginViewport("Red Component", pluginA, &redAmplitude, &redDotOffset, true);
        DrawPluginViewport("Green Component", pluginB, &greenAmplitude, &greenDotOffset, false);
        DrawDemoComponent(pluginDemo);

        ImGui::ShowDemoWindow();

        // --- STEP C: Final Output ---
        BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("Full Hybrid System: 2D Logic + 3D Viewport + ImGui Controls", 10, GetScreenHeight() - 30, 20, LIGHTGRAY);

            rlDrawRenderBatchActive();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        EndDrawing();
    }

    UnloadRenderTexture(pluginA);
    UnloadRenderTexture(pluginB);
    UnloadRenderTexture(pluginDemo);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    CloseWindow();

    return 0;
}