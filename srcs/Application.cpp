#include "Application.h"

Application::Application()
{
    // Create Window
    mainWindow = Window(WIN_WIDTH, WIN_HEIGHT);
    mainWindow.initWindow();

    // Create Camera
    front_camera = Camera(glm::vec3(0.0f, 1.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -20.0f, 5.0f, 0.5f);
    back_camera = Camera(glm::vec3(0.0f, 1.5f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -20.0f, 5.0f, 0.5f);
    left_camera = Camera(glm::vec3(-4.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -20.0f, 5.0f, 0.5f);
    right_camera = Camera(glm::vec3(4.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, -20.0f, 5.0f, 0.5f);
    top_camera = Camera(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -89.9f, 5.0f, 0.5f);

    // Set projection
    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(66.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Set lights
    mainLight = new DirectionalLight(1.0f, 1.0f, 1.0f, 0.5f,        // r, g, b, ambientIntensity
                                    0.0f, -10.0f, 0.0f, 0.5f,      // xDir, yDir, zDir, diffuseIntensity
                                    10000, 10000);                    // shadowWidth, shadowHeight

    memset(pointLights, 0, sizeof(pointLights));
    memset(spotLights, 0, sizeof(spotLights));
    pointLights[0] = new PointLight(1.0f, 0.0f, 0.0f,               // r, g, b
                                0.0f, 0.0f,                     // ambientIntensity, diffuseIntensity
                                0.0f, 5.0f, -2.5f,              // xPos, yPos, zPos
                                1.0f, 0.02f, 0.01f,             // constant, linear, exponent
                                2048, 2048,                     // shadowWidth, shadowHeight
                                0.1f, 100.0f);                  // nearPlane, farPlane
    pointLights[1] = new PointLight(0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f,
                                2.0f, 5.0f, 2.5f,
                                1.0f, 0.02f, 0.01f,
                                2048, 2048,
                                0.1f, 100.0f);
    pointLights[2] = new PointLight(0.0f, 0.0f, 1.0f,
                                0.0f, 0.0f,
                                -2.0f, 5.0f, 2.5f,
                                1.0f, 0.02f, 0.01f,
                                2048, 2048,
                                0.1f, 100.0f);
    spotLights[0] = new SpotLight(1.0f, 1.0f, 1.0f,                 // r, g, b
                                1.0f, 1.0f,                     // ambientIntensity, diffuseIntensity
                                0.0f, 4.0f, 0.0f,              // xPos, yPos, zPos
                                1.0f, 0.2f, 0.1f,               // constant, linear, exponent
                                0.0f, -1.0f, -0.0f,             // xDir, yDir, zDir
                                20.0f,                          // edgeAngle
                                2048, 2048,                     // shadowWidth, shadowHeight
                                0.1f, 100.0f);                  // nearPlane, farPlane
    pointLightCount = spotLightCount = 0;
    while (pointLights[pointLightCount] && pointLightCount < MAX_POINT_LIGHTS)
        ++pointLightCount;
    while (spotLights[spotLightCount] && spotLightCount < MAX_SPOT_LIGHTS)
        ++spotLightCount;

    // Set skyboxs
    skyboxFaces.push_back("textures/skybox/cupertin-lake_rt.tga");
    skyboxFaces.push_back("textures/skybox/cupertin-lake_lf.tga");
    skyboxFaces.push_back("textures/skybox/cupertin-lake_up.tga");
    skyboxFaces.push_back("textures/skybox/cupertin-lake_dn.tga");
    skyboxFaces.push_back("textures/skybox/cupertin-lake_bk.tga");
    skyboxFaces.push_back("textures/skybox/cupertin-lake_ft.tga");
    sunsetLake = new Skybox(skyboxFaces);
    std::vector<const char*>().swap(skyboxFaces);
    skyboxFaces.push_back("textures/skybox/castle_rt.jpg");
    skyboxFaces.push_back("textures/skybox/castle_lf.jpg");
    skyboxFaces.push_back("textures/skybox/castle_up.jpg");
    skyboxFaces.push_back("textures/skybox/castle_dn.jpg");
    skyboxFaces.push_back("textures/skybox/castle_bk.jpg");
    skyboxFaces.push_back("textures/skybox/castle_ft.jpg");
    castle = new Skybox(skyboxFaces);
    std::vector<const char*>().swap(skyboxFaces);
    skyboxFaces.push_back("textures/skybox/clearsky_rt.jpg");
    skyboxFaces.push_back("textures/skybox/clearsky_lf.jpg");
    skyboxFaces.push_back("textures/skybox/clearsky_up.jpg");
    skyboxFaces.push_back("textures/skybox/clearsky_dn.jpg");
    skyboxFaces.push_back("textures/skybox/clearsky_bk.jpg");
    skyboxFaces.push_back("textures/skybox/clearsky_ft.jpg");
    clearOcean = new Skybox(skyboxFaces);

    // Set textures
    grid = Texture("textures/grid.png");
    grid.LoadTexture();

    // Set materials
    metal = Material(10.0f, 256); // shininess is usually set to power of 2
    mat = Material();

    // Create Objects
    CreateObjects();
    couch_body.LoadModel("models/couch.obj");
    couch_cushions1.LoadModel("models/couch_1.obj");
    sphere.LoadModel("models/sphere.obj");
    box.LoadModel("models/box.obj");

    // Create Shaders
    CreateShaders();

    // Set time
    time = Time();

    // Set IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(mainWindow.mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
    unsigned int iLength, unsigned int vLength, unsigned int normalOffset)
{
    for (unsigned int i = 0; i < indiceCount; i += iLength)
    {
        unsigned int in[3];
        in[0] = indices[i] * vLength;
        in[1] = indices[i + 1] * vLength;
        in[2] = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in[1]] - vertices[in[0]], vertices[in[1] + 1] - vertices[in[0] + 1], vertices[in[1] + 2] - vertices[in[0] + 2]);
        glm::vec3 v2(vertices[in[2]] - vertices[in[0]], vertices[in[2] + 1] - vertices[in[0] + 1], vertices[in[2] + 2] - vertices[in[0] + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        for (int i = 0; i < 3; i++)
        {
            vertices[in[i] + normalOffset] -= normal.x;
            vertices[in[i] + normalOffset + 1] -= normal.y;
            vertices[in[i] + normalOffset + 2] -= normal.z;
        }
    }

    for (unsigned int i = 0; i < verticeCount; i += vLength)
    {
        glm::vec3 normal(vertices[i + normalOffset], vertices[i + normalOffset + 1], vertices[i + normalOffset + 2]);
        normal = glm::normalize(normal);
        vertices[i + normalOffset] = normal.x;
        vertices[i + normalOffset + 1] = normal.y;
        vertices[i + normalOffset + 2] = normal.z;
    }
}

void Application::CreateObjects()
{
    GLuint floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
//   x      y     z         u      v         nx    ny    nz
    -5.0f, 0.0f, -5.0f,   0.0f,  0.0f,     0.0f, 1.0f, 0.0f,
     5.0f, 0.0f, -5.0f,   1.0f,  0.0f,     0.0f, 1.0f, 0.0f,
    -5.0f, 0.0f,  5.0f,   0.0f,  1.0f,     0.0f, 1.0f, 0.0f,
     5.0f, 0.0f,  5.0f,   1.0f,  1.0f,     0.0f, 1.0f, 0.0f
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(floorVertices, floorIndices, 32, 6); // Create proper VAO, VBO, IBO of obj1
    meshList.push_back(obj1);

    return;
}

void Application::CreateShaders()
{
    mainShader.CreateFromFiles(vShader, fShader);
    directionalShadowShader.CreateFromFiles(vDirectionalShadowShader, fDirectionalShadowShader);
    omniShadowShader.CreateFromFiles(vOmniShadowShader, gOmniShadowShader, fOmniShadowShader);
}

void Application::RenderScene(Shader* shader)
{
    // uniformModel should be initialized per mesh
    glm::mat4 model(1.0f);
    glm::mat4 m_rotation(1.0f);
    /*
    * [EXAMPLE]
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
    model = glm::rotate(model, rot * toRadian, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    silver.UseTexture();
    metal.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();
    metal.UnuseMaterial(uniformSpecularIntensity, uniformShininess);
   */
    shader->SetColor(myGui.mesh_color);
    if (myGui.current_mesh_type == 0)
    {
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, myGui.mesh_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        shader->SetModel(model);
        mat.ChangeMaterial(myGui.mesh_specular, myGui.mesh_shininess);
        mat.UseMaterial(shader);
        sphere.RenderModel();
        mat.UnuseMaterial(shader);
    }
    else if (myGui.current_mesh_type == 1)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, myGui.mesh_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader->SetModel(model);
        mat.ChangeMaterial(myGui.mesh_specular, myGui.mesh_shininess);
        mat.UseMaterial(shader);
        box.RenderModel();
        mat.UnuseMaterial(shader);
    }
    else if (myGui.current_mesh_type == 2)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, myGui.mesh_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
        shader->SetModel(model);
        mat.ChangeMaterial(myGui.mesh_specular, myGui.mesh_shininess);
        mat.UseMaterial(shader);
        couch_body.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, myGui.mesh_rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::rotate(model, myGui.mesh_rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
        shader->SetModel(model);
        couch_cushions1.RenderModel();
        mat.UnuseMaterial(shader);
    }
    
    if (myGui.show_ground)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        shader->SetModel(model);
        shader->SetColor(glm::vec3(1.0f,1.0f,1.0f));
        grid.UseTexture();
        mat.ChangeMaterial(myGui.ground_specular, myGui.ground_shininess);
        mat.UseMaterial(shader);
        meshList[0]->RenderMesh();
        mat.UnuseMaterial(shader);
    }
}

void Application::DirectionalShadowMapPass(DirectionalLight* light)
{
    ShadowMap* shadowMap = light->GetShadowMap();
    glViewport(0, 0, shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());
    shadowMap->Write();

    glClear(GL_DEPTH_BUFFER_BIT);

    directionalShadowShader.UseShader();
    directionalShadowShader.SetDirectionalLightSpaceTransform(light->CalculateLightSpaceTransform());

    directionalShadowShader.Validate();

    RenderScene(&directionalShadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Application::OmniShadowMapPass(PointLight* light)
{
    ShadowMap* shadowMap = light->GetShadowMap();
    glViewport(0, 0, shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());
    shadowMap->Write();

    glClear(GL_DEPTH_BUFFER_BIT);

    omniShadowShader.UseShader();
    omniShadowShader.SetOmniLightPos(light->GetPosition());
    omniShadowShader.SetNearPlane(light->GetNearPlane());
    omniShadowShader.SetFarPlane(light->GetFarPlane());
    omniShadowShader.SetLightMatrices(light->CalculateLightMatrices());

    omniShadowShader.Validate();

    RenderScene(&omniShadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::RenderPass(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
{
    glViewport(0, 0, (int)mainWindow.getBufferWidth(), (int)mainWindow.getBufferHeight());

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (myGui.current_skybox == 1)
        (*sunsetLake).DrawSkybox(glm::mat4(glm::mat3(viewMatrix)), projectionMatrix);
    else if (myGui.current_skybox == 2)
        (*castle).DrawSkybox(glm::mat4(glm::mat3(viewMatrix)), projectionMatrix);
    else if (myGui.current_skybox == 3)
        (*clearOcean).DrawSkybox(glm::mat4(glm::mat3(viewMatrix)), projectionMatrix);

    mainShader.UseShader();
    mainShader.SetEyePosition(camera.getCameraPosition());
    mainShader.SetProjection(projectionMatrix);
    mainShader.SetView(viewMatrix);
    mainShader.SetDirectionalLight(mainLight, myGui.d_ambientIntensity, myGui.d_diffuseIntensity, myGui.d_color, myGui.d_direction);
    mainShader.SetPointLight(pointLights, pointLightCount, 3, 0, myGui.p_ambientIntensity, myGui.p_diffuseIntensity, myGui.p_color, myGui.p_position);
    mainShader.SetDirectionalLightSpaceTransform(mainLight->CalculateLightSpaceTransform());
    if (myGui.flash_activated)
        spotLightCount = 1;
    else
        spotLightCount = 0;
    mainShader.SetSpotLight(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    glm::vec3 flashLightPosition = camera.getCameraPosition();
    flashLightPosition.y -= 0.3f;
    spotLights[0]->SetFlash(flashLightPosition, camera.getCameraDirection());
        
    
    mainShader.Validate();

    RenderScene(&mainShader);
}

void Application::SetCamera()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    {
        if (myGui.current_camera == 0)
            camera = front_camera;
        else if (myGui.current_camera == 1)
            camera = back_camera;
        else if (myGui.current_camera == 2)
            camera = top_camera;
        else if (myGui.current_camera == 3)
            camera = right_camera;
        else if (myGui.current_camera == 4)
            camera = left_camera;
        else if (myGui.current_camera == 5)
        {
            if (!io.WantCaptureMouse)
                camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
            if (!io.WantCaptureKeyboard)
                camera.keyControl(mainWindow.getKeys(), time.getDeltaTime());
        }
    }
}

bool Application::AppRun()
{  
    while (!mainWindow.getShouldClose())
    {
        SetCamera();
        
        DirectionalShadowMapPass(mainLight);
        for (unsigned int i = 0; i < pointLightCount; ++i)
        {
            OmniShadowMapPass(pointLights[i]);
        }
        for (unsigned int i = 0; i < spotLightCount; ++i)
        {
            OmniShadowMapPass(spotLights[i]);
        }
        RenderPass(projection, camera.calculateViewMatrix());
        
        glUseProgram(0);

        //ImGui::ShowDemoWindow();
        myGui.ActivateGui();
        

        mainWindow.swapBuffers();

        glfwPollEvents();
    }

    return 0;
}

Application::~Application()
{
    if (meshList.size()) 
    {
        for (unsigned int i = 0; i < meshList.size(); ++i)
        {
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }
    std::vector<Mesh*>().swap(meshList);

    if (mainLight)
    {
        delete mainLight;
        mainLight = nullptr;
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        if (pointLights[i])
        {
            delete pointLights[i];
            pointLights[i] = nullptr;
        }
        else
            break;
    }

    for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
    {
        if (spotLights[i])
        {
            delete spotLights[i];
            spotLights[i] = nullptr;
        }
        else
            break;
    }

    if (sunsetLake)
    {
        delete sunsetLake;
        sunsetLake = nullptr;
    }
    if (castle)
    {
        delete castle;
        castle = nullptr;
    }
    if (clearOcean)
    {
        delete clearOcean;
        castle = nullptr;
    }
    std::vector<const char*>().swap(skyboxFaces);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}