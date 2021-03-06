#include "MyGui.h"

MyGui::MyGui()
{
    window_flags        = 0;
    //window_flags        = ImGuiWindowFlags_MenuBar;
    p_open              = false;
    show_about          = false;
    show_help           = false;
    show_license        = false;
    current_skybox      = 0;
    current_mesh_type   = 0;
    current_camera      = 0;
    mesh_rotationX      = 0;
    mesh_rotationY      = 0;
    mesh_rotationZ      = 0;
    mesh_shininess      = 1.0f;
    mesh_specular       = 0.0f;
    
    ground_shininess    = 1.0f;
    ground_specular     = 0.0f;

    d_ambientIntensity  = 0.5f;
    d_diffuseIntensity  = 0.5f;
    d_color             = glm::vec3(1.0f, 1.0f, 1.0f);
    d_direction         = glm::vec3(0.0f, -10.0f, 0.0f);

    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        p_ambientIntensity[i] = 0.0f;
        p_diffuseIntensity[i] = 0.0f;
    }
    p_color[0]          = glm::vec3(1.0f, 0.0f, 0.0f);
    p_color[1]          = glm::vec3(0.0f, 1.0f, 0.0f);
    p_color[2]          = glm::vec3(0.0f, 0.0f, 1.0f);
    p_position[0]       = glm::vec3(0.0f, 5.0f, -2.5f);
    p_position[1]       = glm::vec3(2.0f, 5.0f, 2.5f);
    p_position[2]       = glm::vec3(-2.0f, 5.0f, 2.5f);
    mesh_color          = glm::vec3(1.0f, 1.0f, 1.0f);

    flash_activated     = false;
    show_ground         = true;
}

void MyGui::HelpMarker(const char* desc)
{
    ImGui::TextDisabled("?");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void MyGui::ShowAbout(bool* p_open)
{
    if (!ImGui::Begin("About", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Tony's Phong Reflection Model Simulator %s", TONYS_VERSION);
    ImGui::Separator();
    ImGui::Text("This Phong Reflection Model Simulator is a brief program that simulates how phong reflection models work.\n"
                "Phong reflection model utilizes phong lighting model in order to create reflection on surface.\n"
                "Phong lighting model consists of three components: ambient lighting, diffuse lighting, and specular lighting.\n"
                "In this simulator, you can control ambient lighting and diffuse lighting by modifying ambient intensity and diffuse intensity of lights in the \"Light\" tab.\n"
                "Furthermore, specular lighting can be controlled by modifying specular powers of materials that meshs have in the \"Material\" section under the \"Mesh\" tab.\n"
                "Addtional features of lights and meshs as well as those of a camera and a skybox is available as well.\n"
                "Dear Imgui %s is used for the graphical user interface.", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("By Tony Kim");
    ImGui::Text("Tony's Phong Reflection Model Simulator is licensed under the MIT License. See License menu for more information.");
    ImGui::Text("Homepage: kyoungwhankim.github.io");
    ImGui::Text("Contact: kyoungwhankim.dev@gmail.com");
    ImGui::End();
}

void MyGui::ShowLicense(bool* p_open)
{
    if (!ImGui::Begin("License", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("MIT License\n"
        "\n"
        "Copyright(c) 2021 Kyoungwhan Kim\n"
        "\n"
        "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
        "of this softwareand associated documentation files(the \"Software\"), to deal\n"
        "in the Software without restriction, including without limitation the rights\n"
        "to use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n"
        "copies of the Software, and to permit persons to whom the Software is\n"
        "furnished to do so, subject to the following conditions:\n"
        "\n"
        "The above copyright noticeand this permission notice shall be included in all\n"
        "copies or substantial portions of the Software.\n"
        "\n"
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n"
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
        "SOFTWARE.");
    ImGui::End();
}

void MyGui::ShowHelp(bool* p_open)
{
    if (!ImGui::Begin("Help", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Simple Instructions");
    if (ImGui::CollapsingHeader("Navigation"))
    {
        ImGui::Text("You can move around the scene by setting the camera to \'Controllable\'.");
        ImGui::Text("If you select a camera view, it will always be reset to the one you selected.");
        ImGui::Text("Therefore, make sure to change the camera view first to what you want to start from, and select \'Controllable\'.");
        ImGui::Text("You can move the camera with W, A, S, and D keys on your keyboard.");
        ImGui::Text("You can left mouse click and drag in order to control the camera direction, which is the point of view.");
    }
    if (ImGui::CollapsingHeader("Sliders"))
    {
        ImGui::Text("Left click and drag the sliders in order to change the values.");
        ImGui::Text("Double click the sliders or CTRL + left mouse click them to type in values.");
    }
    ImGui::End();
}

void MyGui::CreateMainMenu()
{
    if (show_about)     ShowAbout(&show_about);
    if (show_help)      ShowHelp(&show_help);
    if (show_license)   ShowLicense(&show_license);
   
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("About"))
        {
            ImGui::MenuItem("About", NULL, &show_about);
            ImGui::MenuItem("License", NULL, &show_license);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("Help", NULL, &show_help);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MyGui::CollapsingHeader_Camera()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::RadioButton("Front View", &current_camera, 0); ImGui::SameLine();
        ImGui::RadioButton("Back View", &current_camera, 1);
        ImGui::RadioButton("Top View", &current_camera, 2);
        ImGui::RadioButton("Right View", &current_camera, 3); ImGui::SameLine();
        ImGui::RadioButton("Left View", &current_camera, 4);
        ImGui::RadioButton("Controllable", &current_camera, 5);
        ImGui::SameLine(); HelpMarker("Set to Controllable to move around the scene. \nWASD + L Mouse Click & Drag");
    }
}

void MyGui::CollapsingHeader_Mesh()
{
    if (ImGui::CollapsingHeader("Mesh"))
    {
        if (ImGui::TreeNode("Type"))
        {
            static ImGuiComboFlags flags = 0;
            const char* items[] = { "Sphere", "Box", "Couch" };
            const char* combo_label = items[current_mesh_type];  // Label to preview before opening the combo (technically it could be anything)
            if (ImGui::BeginCombo("Mesh Type", combo_label, flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = (current_mesh_type == n);
                    if (ImGui::Selectable(items[n], is_selected))
                        current_mesh_type = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Color"))
        {
            ImGui::ColorEdit3("Mesh Color", &mesh_color.r);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Rotation"))
        {
            //ImGui::SliderAngle("Rotate X", &mesh_rotationX);
            //ImGui::SameLine(); HelpMarker("CTRL + L Mouse Click to type in a value.");
            ImGui::SliderAngle("Rotate Y", &mesh_rotationY);
            ImGui::SameLine(); HelpMarker("CTRL + L Mouse Click to type in a value.");
            //ImGui::SliderAngle("Rotate Z", &mesh_rotationZ);
            //ImGui::SameLine(); HelpMarker("CTRL + L Mouse Click to type in a value.");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Material"))
        {
            ImGui::DragFloat("Specular", &mesh_specular, 0.005f, 0.0f, 10.0f);
            ImGui::SameLine(); HelpMarker("The intensity of mirror-like reflection of lights.");
            ImGui::DragFloat("Shininess", &mesh_shininess, 1.0f, 0.0f, 1024.0f);
            ImGui::SameLine(); HelpMarker("Higher the shininess, lesser the amount of reflected lights scattered.");
            ImGui::TreePop();
        }
    }
}

void MyGui::CollapsingHeader_Ground()
{
    if (ImGui::CollapsingHeader("Ground"))
    {
        ImGui::Checkbox("Show Ground", &show_ground);
        ImGui::Spacing();
        ImGui::DragFloat("Specular", &ground_specular, 0.005f, 0.0f, 10.0f);
        ImGui::SameLine(); HelpMarker("The intensity of mirror-like reflection of lights.");
        ImGui::DragFloat("Shininess", &ground_shininess, 1.0f, 0.0f, 1024.0f);
        ImGui::SameLine(); HelpMarker("Higher the shininess, lesser the amount of reflected lights scattered.");
    }
}

void MyGui::CollapsingHeader_Light()
{
    if (ImGui::CollapsingHeader("Light"))
    {
        if (ImGui::TreeNode("Directional Light"))
        {
            ImGui::DragFloat("Ambient", &d_ambientIntensity, 0.005f, 0.0f, 10.0f);
            ImGui::SameLine(); HelpMarker("Ambient light is light that is always present.");
            ImGui::DragFloat("Diffuse", &d_diffuseIntensity, 0.005f, 0.0f, 10.0f);
            ImGui::SameLine(); HelpMarker("Diffuse light is light that directly strikes an object and scatters in all directions.");
            ImGui::ColorEdit3("Color", &d_color.x);
            ImGui::DragFloat3("Direction", &d_direction.x, 0.01f, -100.0f, 100.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Point Lights"))
        {
            if (ImGui::TreeNode("Red Point Light"))
            {
                ImGui::DragFloat("Ambient", &p_ambientIntensity[0], 0.005f, 0.0f, 10.0f);
                ImGui::SameLine(); HelpMarker("Ambient light is light that is always present.");
                ImGui::DragFloat("Diffuse", &p_diffuseIntensity[0], 0.005f, 0.0f, 10.0f);
                ImGui::SameLine(); HelpMarker("Diffuse light is light that directly strikes an object and scatters in all directions.");
                ImGui::ColorEdit3("Color", &p_color[0].x);
                ImGui::DragFloat3("Position", &p_position[0].x, 0.01f, -100.0f, 100.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Green Point Light"))
            {
                ImGui::DragFloat("Ambient", &p_ambientIntensity[1], 0.005f, 0.0f, 10.0f);
                ImGui::SameLine(); HelpMarker("Ambient light is light that is always present.");
                ImGui::DragFloat("Diffuse", &p_diffuseIntensity[1], 0.005f, 0.0f, 10.0f);
                ImGui::SameLine(); HelpMarker("Diffuse light is light that directly strikes an object and scatters in all directions.");
                ImGui::ColorEdit3("Color", &p_color[1].x);
                ImGui::DragFloat3("Position", &p_position[1].x, 0.01f, -100.0f, 100.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Blue Point Light"))
            {
                ImGui::DragFloat("Ambient", &p_ambientIntensity[2], 0.005f, 0.0f, 10.0f);
                ImGui::SameLine(); HelpMarker("Ambient light is light that is always present.");
                ImGui::DragFloat("Diffuse", &p_diffuseIntensity[2], 0.005f, 0.0f, 10.0f);
                ImGui::SameLine(); HelpMarker("Diffuse light is light that directly strikes an object and scatters in all directions.");
                ImGui::ColorEdit3("Color", &p_color[2].x);
                ImGui::DragFloat3("Position", &p_position[2].x, 0.01f, -100.0f, 100.0f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Flash Light"))
        {
            ImGui::Checkbox("Activate Flash Light", &flash_activated);
            ImGui::SameLine(); HelpMarker("Flash light is a spot light attached to the camera facing forward.");
            ImGui::TreePop();
        }
    }
}

void MyGui::CollapsingHeader_Skybox()
{
    if (ImGui::CollapsingHeader("Skybox"))
    {
        ImGui::RadioButton("No Skybox", &current_skybox, 0); ImGui::SameLine();
        ImGui::RadioButton("Sunset Lake", &current_skybox, 1); ImGui::SameLine();
        ImGui::RadioButton("Castle", &current_skybox, 2); ImGui::SameLine();
        ImGui::RadioButton("Clear Ocean", &current_skybox, 3);
    }
}

void MyGui::ActivateGui()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 1190, main_viewport->WorkPos.y + 90), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 700), ImGuiCond_Once);

    if (!ImGui::Begin("Phong Reflection Model Simulator", &p_open, window_flags))
    {
        ImGui::End();
        return;
    }

    // Menu bar
    CreateMainMenu();

    // Collapsing Headers
    CollapsingHeader_Camera();
    CollapsingHeader_Mesh();
    CollapsingHeader_Ground();
    CollapsingHeader_Light();
    CollapsingHeader_Skybox();
    
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

MyGui::~MyGui()
{
}
