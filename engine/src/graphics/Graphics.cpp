#include <motor/graphics/Graphics.h>
#include <motor/task_system.hpp>
#include <imfilebrowser/imfilebrowser.hpp>

#include "ImGuizmo.h"

using namespace DirectX;

bool Graphics::Initialize(HWND hWnd, int width, int height) {
    Logs::Debug("Initialize Graphics"); // Тестовый лог

    windowWidth = width;
    windowHeight = height;

    fps_timer.Start();

    if (!InitializeDirectX(hWnd)) {
        Logs::Error("	InitializeDirectX is not initialized");
        return false;
    }

    //if (!InitializeShaders()) {
    //    Logs::Error("	InitializeShaders is not initialized");
    //    return false;
    //}

    //if (!InitializeScene()) {
    //    Logs::Error("	InitializeScene is not initialized");
    //    return false;
    //}

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
    ImGui::StyleColorsDark();

    ImFont* font = io.Fonts->AddFontFromFileTTF("./Data/Fonts/Cascadia Mono PL Light 300.otf", 15.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(font != nullptr);

    return true;
}


void Graphics::RenderFrame() {
    std::lock_guard guard{ renderable_objects_mtx };
    // При Flip_Discard нужно при отображение кадра каждый раз устанавливать Render Targets
    deviceContext->OMSetRenderTargets(
      1,
      renderTargetView.GetAddressOf(),
      depth_stencil_view.Get());

    float bgcolor[] = {
        0.0f, // R
        0.0f, // G
        0.0f, // B
        1.0f // A
    };
    // Clear window
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
    deviceContext->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    // -------------------------------------------------------------------
    // TODO: Перенести в InitializeScene или InitializeScene
    // Setup before draw
    //    deviceContext->IASetInputLayout(vertex_shader.GetInputLayout());
    //    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TrianleList
    //
    if (loadedScene_) {
        deviceContext->IASetInputLayout(vertex_shader.GetInputLayout());
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TrianleList

        deviceContext->RSSetState(rasterizer_state.Get());
        deviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
        //
        deviceContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
        deviceContext->VSSetShader(vertex_shader.GetShader(), nullptr, 0);
        deviceContext->PSSetShader(pixel_shader.GetShader(), nullptr, 0);

        // ------------------------------------------------------------------
      //  UINT offset = 0;
//
      //  cb_ps_light.data.dynamicLightColor = light.lightColor;
      //  cb_ps_light.data.dynamicLightStrength = light.lightStrength;
      //  cb_ps_light.data.dynamicLightPosition = light.GetPositionFloat3();
      //  cb_ps_light.data.dynamicLightAttenuation_A = light.attennuation_A;
      //  cb_ps_light.data.dynamicLightAttenuation_B = light.attennuation_B;
      //  cb_ps_light.data.dynamicLightAttenuation_C = light.attennuation_C;
      //  cb_ps_light.ApplyChanges();
      //  deviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddressOf());
    }
    /*model1.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	model2.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	model3.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());*/
    //test_mesh_model.DrawMeshes(camera.GetViewMatrix() * camera.GetProjectionMatrix());

    //DrawObjects(true);

    /// -------------------------------------------------------------------

    //test_entt_scene.DrawSceneEntt(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix(), localSpaceFrustum);
    /// -------------------------------------------------------------------

    if (loadedScene_) {
        UpdatePhysics_();
    }

    if (loadedScene_) {
        test_entt_scene.OnRednerUpdate();
        DrawScene(test_entt_scene, cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
    }
    /*
	// object
	XMMATRIX world = renderable_objects[0].GetWorldMatrix();
    XMVECTOR Det2 = XMMatrixDeterminant(world);
    XMMATRIX invWorld = XMMatrixInverse(&Det2, world);
    XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);
    FrustumCulling local_culling; 
	f_culling.Transform(local_culling, viewToLocal);
    if (local_culling.ContainsByPositonPoint(renderable_objects[0].GetPositionVector())) 
	{
		renderable_objects[0].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
    } 
	else 
	{
        Logs::Debug("Object not rendered");    
	}


	// object1
    world = renderable_objects[1].GetWorldMatrix();
    Det2 = XMMatrixDeterminant(world);
    invWorld = XMMatrixInverse(&Det2, world);
    viewToLocal = XMMatrixMultiply(invView, invWorld);
    FrustumCulling local_culling2; 
    f_culling.Transform(local_culling2, viewToLocal);
    if (local_culling.ContainsByPositonPoint(renderable_objects[1].GetPositionVector())) {
        renderable_objects[1].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
    } 
	else {
        Logs::Debug("Object1 not rendered");
    }
	
	//gameObject.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
	
	//gameObject1.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
    renderable_objects[2].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
    renderable_objects[3].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
    renderable_objects[4].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());

	*/

    /*Entity primary_camera = test_entt_scene.GetPrimaryCamera();

    if (primary_camera && state == States::Simulate) 
    {
        auto transform = primary_camera.GetComponent<TransformComponent>().GetLocalTransformMatrix();
        XMVECTOR Det = XMMatrixDeterminant(transform);
        XMMATRIX view = XMMatrixInverse(&Det, transform);
        XMMATRIX t_viewProjectionMatrix = view * primary_camera.GetComponent<CameraComponent>().camera.GetProjection();
        m_effect->SetVertexColorEnabled(true);
        m_effect->SetView(view);
        m_effect->SetProjection(primary_camera.GetComponent<CameraComponent>().camera.GetProjection());
    } else {
        m_effect->SetVertexColorEnabled(true);
        m_effect->SetView(cam_container.GetCurrentCamera().GetViewMatrix());
        m_effect->SetProjection(cam_container.GetCurrentCamera().GetProjectionMatrix());
    }*/
    if (loadedScene_) {
        {
            void const* shaderByteCode;
            size_t byteCodeLength;
    
            m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
    
            device->CreateInputLayout(
              VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
              shaderByteCode, byteCodeLength,
              m_inputLayout.ReleaseAndGetAddressOf());
        }
    
        deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
        deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
        deviceContext->RSSetState(m_states->CullNone());
    
        m_effect->Apply(deviceContext.Get());
    
        deviceContext->IASetInputLayout(m_inputLayout.Get());
    
        DrawDebugScene(test_entt_scene);
    }
    /*m_batch->Begin();

    DrawDebugScene(test_entt_scene);
    DrawRay(m_batch.get(), renderable_objects[1].GetPositionVector(), renderable_objects[1].GetForwardVector(), true, DirectX::Colors::LightGreen);
    DrawRay(m_batch.get(), renderable_objects[1].GetPositionVector(), renderable_objects[1].GetRightVector(), true, DirectX::Colors::Red);
    DrawRay(m_batch.get(), renderable_objects[1].GetPositionVector(), renderable_objects[1].GetUpVector(), true, DirectX::Colors::LightBlue);
    Draw(m_batch.get(), renderable_objects[0].GetBoundingBox(), DirectX::Colors::Green);
    Draw(m_batch.get(), renderable_objects[1].GetBoundingBox(), DirectX::Colors::Blue);
    Draw(m_batch.get(), renderable_objects[2].GetBoundingBox(), DirectX::Colors::Olive);
    Draw(m_batch.get(), renderable_objects[3].GetBoundingBox(), DirectX::Colors::DarkSeaGreen);
    Draw(m_batch.get(), renderable_objects[4].GetBoundingBox(), DirectX::Colors::Yellow);

    m_batch->End();*/

    // Draw Text and fps
    static int fps_counter = 0;
    static std::string fps_string = "FPS: 0";
    fps_counter += 1;
    if (fps_timer.GetMilisecondsElapsed() > 1000.0) {
        fps_string = "FPS: " + std::to_string(fps_counter);
        fps_counter = 0;
        fps_timer.Restart();
    }

    sprite_batch->Begin();
    sprite_font->DrawString(
      sprite_batch.get(),
      StringConverter::StringToWide(fps_string).c_str(),
      DirectX::XMFLOAT2(0, 0), // Position
      DirectX::Colors::White, // Color
      0.0f, // Rotation
      DirectX::XMFLOAT2(0.0f, 0.0f), // Origin
      DirectX::XMFLOAT2(0.8f, 0.8f) // Scale
    );
    sprite_batch->End();

    // Start ImGui Frame
    static int counter = 0;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

#pragma region Imgui(Simulate, Stop, Pause, Continue)
    if (loadedScene_) {
        // Simulate, Stop, Pause, Imgui
        ImGui::Begin("Simulate");
    
        if (state == States::Editor) {
            if (ImGui::Button("Simulate")) {
                state = States::Simulate;
                // TODO: Сменить на CurrentScene воследствии
                test_entt_scene.Save();
            }
        }
    
        if (state == Pause) {
            if (ImGui::Button("Continue")) {
                state = States::Simulate;
            }
        }
    
        if (state == States::Simulate) {
            if (ImGui::Button("Pause")) {
                if (state == States::Simulate) {
                    state = States::Pause;
                }
            }
        }
    
        if (state == States::Simulate || state == States::Pause) {
            ImGui::SameLine();
            if (ImGui::Button("Stop")) {
                state = States::Editor;
                // TODO: Сменить на CurrentScene воследствии
                test_entt_scene.Load();
            }
        }
        //ImGui::NewLine();
        switch (state) {
        case Editor:
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4{ 0.1f, 0.8f, 0.5f, 1.0f });
            ImGui::Text("Editor_State");
            ImGui::PopStyleColor();
            break;
        case Simulate:
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4{ 0.1f, 0.8f, 0.5f, 1.0f });
            ImGui::Text("Simulation_State");
            ImGui::PopStyleColor();
            break;
        case Pause:
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4{ 0.1f, 0.8f, 0.5f, 1.0f });
            ImGui::Text("Pause_State");
            ImGui::PopStyleColor();
            break;
        }
        ImGui::End();
    }
#pragma endregion

#pragma region ImguiRender
    // Imgui render
    if (state == States::Editor) 
    {
        // Creater ImGui test window
        ImGui::Begin("Ambietnt Light Controls");

        ImGui::ColorEdit3("Ambient Light Color", (float*)&cb_ps_light.data.ambientLightColor);

        ImGui::DragFloat(
          "Ambient Light strength",
          &cb_ps_light.data.ambientLightStrength,
          0.01f,
          0.0f,
          1.0f);
        ImGui::End();

        cam_container.ImGUIWindow();
        scene_hierachy.OnImguiRender();

        if (showConsole_ != nullptr && consoleUI_ != nullptr) {
            if (*showConsole_ and loadedProject_) {
                consoleUI_->Draw("Lua Console", showConsole_);
            }
        }
    }
#pragma endregion


    if (showAssetViewer_ != nullptr and assetViewerUI_ != nullptr) {
        if (*showAssetViewer_ and loadedProject_) {
            assetViewerUI_->Draw("Asset Viewer", showAssetViewer_);
        }
    }

    if (showProject_ != nullptr and projectUI_ != nullptr) {
        if (not project_->IsOpened()) {
            if (not *showProject_) {
                *showProject_ = true;
                ImGui::OpenPopup("Project");
            }
            projectUI_->Draw("Project", showProject_);
        } else {
            if (not loadedProject_) {
                LoadProject_();
            }
            if (ImGui::Button("Close project")) {
                UnloadProject_();
            }
        }
    }

    if (showScenes_) {
        if (loadedProject_) {
            ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin("Scenes", &showScenes_)) {
                ImGui::End();
                return;
            }

            static char newSceneName[128]{};
            ImGui::Text("Create scene:");
            ImGui::SameLine();
            ImGui::InputText("NewSceneName", newSceneName, IM_ARRAYSIZE(newSceneName));
            ImGui::SameLine();
            if (ImGui::Button("Create")) {
                project_->CreateScene(newSceneName);
            }

            auto scenes = project_->GetScenes();

            for (auto& scene : scenes) {
                ImGui::Text(scene.name.c_str());
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left) and ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    motor::utils::debug_write::info("Opening scene {} ...\n", scene.name);
                    UnloadScene_();
                    scenePath_ = "scenes" / scene.path / "config.json";
                    InitializeScene();
                    loadedScene_ = true;
                }
            }

            ImGui::End();
        }
    }

    {
        auto flags = ImGuiWindowFlags_NoDecoration
          | ImGuiWindowFlags_NoDocking
          | ImGuiWindowFlags_NoNav
          | ImGuiWindowFlags_NoSavedSettings
          | ImGuiWindowFlags_NoBackground
          | ImGuiWindowFlags_NoMove
          | ImGuiWindowFlags_NoBringToFrontOnFocus
          | ImGuiWindowFlags_NoFocusOnAppearing;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::Begin("Background", nullptr, flags);
        ImGui::BeginChild("BackgroundChild", { 0, 0 }, false, flags);
        ImGui::EndChild();
        {
            if (ImGui::BeginDragDropTarget()) {
                auto payload = ImGui::AcceptDragDropPayload("Asset##Payload");
                if (payload != nullptr) {
                    auto path = assetViewerUI_->GetDragDrop();
                    if (model_loader.GetModelByFilePath(motor::converters::u8_to_mb(std::filesystem::absolute(path).generic_u8string()))) {

                    } else {
                        motor::ThreadPool().submit([this, path]() {
                            //model_loader.LoadModel("", "");
                            auto id = loadingUI_.AddStr(motor::converters::u8_to_mb(path.filename().generic_u8string()));
                            std::this_thread::sleep_for(std::chrono::seconds(5));
                            loadingUI_.RemoveStr(id);
                        });
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
        ImGui::End();
    }

    loadingUI_.Draw("Loading", nullptr);

    if (loadedScene_) {
        ImGui::Begin("Serialization");
        if (ImGui::Button("Save")) {
            test_entt_scene.Save(scenePath_);
        }
        if (ImGui::Button("Load")) {
            test_entt_scene.Load(scenePath_);
        }
        ImGui::End();
    }

    if (loadedScene_) {
        if (ImGui::Button("Start/stop")) {
            simulate_ = not simulate_;
            if (simulate_) {
                prevTime_ = std::chrono::steady_clock::now();
            }
        }
        if (ImGui::Button("Setup")) {
            auto entityGround = test_entt_scene.CreateEntity("Ground");
            ComponentSystems::SetPosition(entityGround, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
            ComponentSystems::SetRotation(entityGround, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
            auto& t = entityGround.GetComponent<TransformComponent>();
            t.local_scale = XMFLOAT3{ 10.f, 1.f, 10.f };
            entityGround.AddComponent<MeshComponent>();
            entityGround.AddComponent<PhysicsComponent>();
            auto& p = entityGround.GetComponent<PhysicsComponent>();
            p.extent = XMFLOAT3{ 10.f, 1.f, 10.f };
            p.mass = 0.;
            p.id = physics_.AddCube(
              btVector3{t.world_position.x, t.world_position.y, t.world_position.z},
              btVector3{t.world_rotation.x, t.world_rotation.y, t.world_rotation.z},
              btVector3{p.extent.x, p.extent.y, p.extent.z},
              p.mass);
            int count = 1;
            for (int i = -3; i < 4; ++i) {
                for (int j = -4; j < 4; ++j) {
                    auto e = test_entt_scene.CreateEntity("Cube" + std::to_string(count));
                    ComponentSystems::SetPosition(e, DirectX::XMFLOAT3(i, (i * j + 14) * 3, j));
                    ComponentSystems::SetRotation(e, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
                    auto& t = e.GetComponent<TransformComponent>();
                    t.local_scale = XMFLOAT3{ 1.f, 1.f, 1.f };
                    e.AddComponent<MeshComponent>();
                    e.AddComponent<PhysicsComponent>();
                    auto& p = e.GetComponent<PhysicsComponent>();
                    p.extent = XMFLOAT3{ 1.f, 1.f, 1.f };
                    p.mass = 1.;
                    p.id = physics_.AddCube(
                      btVector3{ t.world_position.x, t.world_position.y, t.world_position.z },
                      btVector3{ t.world_rotation.x, t.world_rotation.y, t.world_rotation.z },
                      btVector3{ p.extent.x, p.extent.y, p.extent.z },
                      p.mass);
                }
            }
        }
    }

    // Assemble together Draw Data
    ImGui::Render();
    // Render draw data
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hWnd) {
    try {
        Logs::Debug("	Initialize DirectX");

        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

        scd.BufferDesc.Width = windowWidth;
        scd.BufferDesc.Height = windowHeight;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;

        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 2;
        scd.OutputWindow = hWnd;
        scd.Windowed = true;
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        HRESULT hr;
        hr = D3D11CreateDeviceAndSwapChain(
          nullptr,
          D3D_DRIVER_TYPE_HARDWARE,
          nullptr,
          NULL,
          nullptr,
          0,
          D3D11_SDK_VERSION,
          &scd,
          swapchain.GetAddressOf(),
          device.GetAddressOf(),
          nullptr,
          deviceContext.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain");

        // Creating backbuffer
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        hr = swapchain->GetBuffer(
          0,
          __uuidof(ID3D11Texture2D),
          reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        COM_ERROR_IF_FAILED(hr, "Falied GetBuffer");

        // render target view
        hr = device->CreateRenderTargetView(
          backBuffer.Get(), // backbuffer
          nullptr,
          renderTargetView.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Falied CreateRenderTargetView");

        // Depth stencil (z - buffer)
        CD3D11_TEXTURE2D_DESC depth_stencil_tex_desc(
          DXGI_FORMAT_D24_UNORM_S8_UINT,
          windowWidth,
          windowHeight);
        depth_stencil_tex_desc.MipLevels = 1;
        depth_stencil_tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = device->CreateTexture2D(
          &depth_stencil_tex_desc,
          nullptr,
          depth_stencil_buffer.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

        hr = device->CreateDepthStencilView(
          depth_stencil_buffer.Get(),
          nullptr,
          depth_stencil_view.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view. ");

        deviceContext->OMSetRenderTargets(
          1,
          renderTargetView.GetAddressOf(),
          depth_stencil_view.Get());

        // Create depth stencil state
        CD3D11_DEPTH_STENCIL_DESC depth_stencil_desc(D3D11_DEFAULT);
        depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

        hr = device->CreateDepthStencilState(
          &depth_stencil_desc,
          depth_stencil_state.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

        // Create Viewport
        CD3D11_VIEWPORT viewport(
          0.0f,
          0.0f,
          static_cast<float>(windowWidth),
          static_cast<float>(windowHeight));

        deviceContext->RSSetViewports(1, &viewport);

        // Rasterizer
        CD3D11_RASTERIZER_DESC rasterizer_desc(D3D11_DEFAULT);
        rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; // Wireframe(D3D11_FILL_WIREFRAME) is a possible considiration
        rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; // Для того что бы не рендерилась задняя часть при риосвании против часовой трелки(короче если нормаль смотрит от нас)
        //rasterizer_desc.FrontCounterClockwise = TRUE // Для того что бы рисовать треугольники против часовой стрелки

        hr = device->CreateRasterizerState(
          &rasterizer_desc,
          rasterizer_state.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

        sprite_batch = std::make_unique<DirectX::SpriteBatch>(deviceContext.Get());
        sprite_font = std::make_unique<DirectX::SpriteFont>(device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

        // Sampler state
        CD3D11_SAMPLER_DESC sampler_desc(D3D11_DEFAULT);
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        hr = device->CreateSamplerState(
          &sampler_desc,
          sampler_state.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
    } catch (COMException& ex) {
        Logs::Error(ex);
        return false;
    }
    return true;
}

bool Graphics::InitializeShaders() {
    Logs::Debug("	Initialize shaders");

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION",
          0,
          DXGI_FORMAT_R32G32B32_FLOAT,
          0,
          0,
          D3D11_INPUT_PER_VERTEX_DATA,
          0 },
        { "TEXCOORD",
          0,
          DXGI_FORMAT_R32G32_FLOAT,
          0,
          D3D11_APPEND_ALIGNED_ELEMENT,
          D3D11_INPUT_PER_VERTEX_DATA,
          0 },
        { "NORMAL",
          0,
          DXGI_FORMAT_R32G32B32_FLOAT,
          0,
          D3D11_APPEND_ALIGNED_ELEMENT,
          D3D11_INPUT_PER_VERTEX_DATA,
          0 }
    };

    UINT numElements = ARRAYSIZE(layoutDesc);

    if (!vertex_shader.Initialize(device, L"base_assets/vertex_shaders/vertexshader.hlsl", layoutDesc, numElements)) {
        Logs::Error("		Vertex shader not initialized");
        return false;
    }

    if (!pixel_shader.Initialize(device, L"base_assets/pixel_shaders/pixelshader.hlsl")) {
        Logs::Error("		Pixel shader not initialized");
        return false;
    }

    if (!pixel_shader_no_light.Initialize(device, L"base_assets/pixel_shaders/pixelshader_nolight.hlsl")) {
        Logs::Error("		Pixel shader not initialized");
        return false;
    }

    //deviceContext->IASetInputLayout(vertex_shader.GetInputLayout());
    //deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TrianleList
    //
    //deviceContext->RSSetState(rasterizer_state.Get());
    //deviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
    //
    //deviceContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
    //deviceContext->VSSetShader(vertex_shader.GetShader(), nullptr, 0);
    //deviceContext->PSSetShader(pixel_shader.GetShader(), nullptr, 0);

    return true;
}

bool Graphics::InitializeScene() {
    //try {
        //std::lock_guard guard{ renderable_objects_mtx };
        Logs::Debug("	Initialize scene");

        if (!InitializeShaders()) {
            Logs::Error("	InitializeShaders is not initialized");
            return false;
        }

        // Texture
        HRESULT hr;//
       // = DirectX::CreateWICTextureFromFile(
       //   device.Get(),
       //   L"Data\\Textures\\hylics_texture.jpg",
       //   nullptr,
       //   texture.GetAddressOf());
       // COM_ERROR_IF_FAILED(hr, "Failed to load wic texture from file.");

        // Constant buffer
        hr = cb_vs_vertex_shader.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        hr = cb_ps_light.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
        cb_ps_light.data.ambientLightStrength = 0.2f;

        // LoadModels test
        // ---------------
        model_loader.Initialize(device.Get());

        model_loader.LoadModel("base_assets/Cube/Cube.obj", "Cube");
       // model_loader.LoadModel("Data\\Objects\\Wayne_pog_v2\\wayne_pog_v2.obj", "WayneCube");
       // model_loader.LoadModel("Data\\Objects\\BOTTLE_V1.fbx", "Bottle");
       // model_loader.LoadModel("Data\\Objects\\RubikCube.fbx", "RubikCube");
        
        
        // ---------------

        // Draw debug
        m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(deviceContext.Get());
        m_states = std::make_unique<CommonStates>(device.Get());
        m_effect = std::make_unique<BasicEffect>(device.Get());

      //  RenderableGameObject gameObject;
      //
      //  if (!gameObject.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
      //      return false;
      //  }
      //
      //  renderable_objects.push_back(gameObject);
      //
      //  RenderableGameObject gameObject1;
      //
      //  if (!gameObject1.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
      //      return false;
      //  }
      //
      //  renderable_objects.push_back(gameObject1);
      //
      //  RenderableGameObject gameObject2;
      //
      //  if (!gameObject2.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
      //      return false;
      //  }
      //
      //  renderable_objects.push_back(gameObject2);
      //
      //  RenderableGameObject gameObject3;
      //
      //  if (!gameObject3.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
      //      return false;
      //  }
      //
      //  renderable_objects.push_back(gameObject3);
      //
      //  RenderableGameObject gameObject4;
      //
      //  if (!gameObject4.Initialize("Data\\Objects\\Wayne_pog_v2\\wayne_pog_v2.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
      //      return false;
      //  }
      //
      //  renderable_objects.push_back(gameObject4);

        /*if (!light.Initialize(device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }*/

      //  if (!solar_system_scene.Initialize(device.Get(), deviceContext.Get(), texture.Get(), cb_vs_vertex_shader)) {
      //      return false;
      //  }

      //  renderable_objects[1].SetPosition(4.0f, 0.0f, 0.0f);
      //  //----------------------
      //  renderable_objects[1].SetRotation(0.0f, 0.5f, 0.0f);
      //  // ---------------------
      //  renderable_objects[2].SetPosition(6.0f, 4.0f, 2.0f);
      //  renderable_objects[3].SetPosition(-8.0f, -2.0f, -2.0f);
      //  renderable_objects[4].SetPosition(3.0f, -4.0f, -6.0f);

        //gameObject1.SetPosition(4.0f, 0.0f, 0.0f);
        //gameObject2.SetPosition(6.0f, 4.0f, 2.0f);
        //gameObject3.SetPosition(-8.0f, -2.0f, -2.0f);
        //gameObject4.SetPosition(3.0f, -4.0f, -6.0f);

        // Camera 1
        Camera camera;
        camera.SetCameraName("Frustrum culling camera");
        camera.SetPosition(0.0f, 0.0f, -2.0f);
        camera.SetProjectionValues(
          90.0f, // Fov
          static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
          0.1f,
          1000.0f);

        // Camera 2
        Camera camera2;
        camera2.SetCameraName("Non Frustrum Culling camera");
        camera2.SetPosition(5.0f, 5.0f, -2.0f);
        camera2.SetProjectionValues(
          90.0f, // Fov
          static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
          0.1f,
          1000.0f);

        // Add to camera container
        cam_container.AddCamera(camera);
        cam_container.AddCamera(camera2);

        // Entt scene
        //test_entt_scene.InitializeSceneEntt("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader);
        //test_entt_scene.Initialize(model_loader);

        

        /*for (size_t i = 1; i < 5; i++) {
            test_entt_scene.AddSimpleCube("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader, XMFLOAT3(i * 3, 6.0f, 0.0f));
        }*/

        // Create Entities for Scene

        test_entt_scene.SetModelLoader(&model_loader);
        test_entt_scene.SetAspectRatioParams(windowHeight, windowWidth);

      //  entity1 = test_entt_scene.CreateEntity("First Entity");
      //  ComponentSystems::SetPosition(entity1, DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f));
      //  ComponentSystems::SetRotation(entity1, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
      //  entity1.AddComponent<MeshComponent>();
      //  //ComponentSystems::SetModel(entity1, model_loader.GetModelById(0));
//
      //  entity2 = test_entt_scene.CreateEntity("Second Entity");
      //  ComponentSystems::SetPosition(entity2, DirectX::XMFLOAT3(0.0f, 6.0f, 4.0f));
      //  ComponentSystems::SetRotation(entity2, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
      //  entity2.AddComponent<MeshComponent>();
      //  //entity2.AddComponent<PointLightComponent>();
      //  //ComponentSystems::SetModel(entity2, model_loader.GetModelById(0));
//
      //  entity3 = test_entt_scene.CreateEntity("Point Light Entity");
      //  ComponentSystems::SetPosition(entity3, DirectX::XMFLOAT3(0.0f, 15.0f, 0.0f));
      //  ComponentSystems::SetRotation(entity3, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
      //  entity3.AddComponent<PointLightComponent>();
        //entity3.AddComponent<MeshComponent>();
        //ComponentSystems::SetModel(entity3, model_loader.GetModelById(0));


        //entity3.AddComponent<ParentComponent>(entity1);

        //ComponentSystems::SetChildEntity(entity1, entity2);
        //ComponentSystems::SetChildEntity(entity1, entity3);

        physics_.Init();

        scene_hierachy.SetContext(&test_entt_scene);
        scene_hierachy.SetPhysics(&physics_);

        test_entt_scene.Load(scenePath_);


    //} catch (COMException& ex) {
    //    Logs::Error(ex);
    //    return false;
    //}
    return true;
}

//void Graphics::DrawObjects(bool f_culling_enabled) {
//    // Check Frustrum culling
//    viewMatrix = cam_container.GetCameraById(0).GetViewMatrix();
//    XMVECTOR Det = XMMatrixDeterminant(viewMatrix);
//    XMMATRIX invView = XMMatrixInverse(&Det, viewMatrix);
//
//    BoundingFrustum localSpaceFrustum = cam_container.GetCameraById(0).GetLocalBoundingFrustum();
//
//    for (size_t i = 0; i < renderable_objects.size(); i++) {
//
//        if (f_culling_enabled) { /*
//            XMMATRIX world = renderable_objects[i].GetWorldMatrix();
//            XMVECTOR Det2 = XMMatrixDeterminant(world);
//            XMMATRIX invWorld = XMMatrixInverse(&Det2, world);
//            XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);
//
//            BoundingFrustum localSpaceFrustum;
//            f_culling.Transform(localSpaceFrustum, viewToLocal);*/
//
//            if (localSpaceFrustum.Contains(renderable_objects[i].GetBoundingBox()) != DirectX::DISJOINT) {
//                renderable_objects[i].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
//            } else {
//                //Logs::Debug("Object not rendered");
//            }
//        } else {
//            renderable_objects[i].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
//        }
//    }
//}

void Graphics::setWidgets(
  motor::ui_system::ConsoleUI* console, bool* showConsole,
  motor::ui_system::AssetViewerUI* assetViewer, bool* showAssetViewer,
  motor::ui_system::ProjectUI* projectUI, bool* showProject, motor::Project* project) {
    consoleUI_ = console;
    showConsole_ = showConsole;

    assetViewerUI_ = assetViewer;
    showAssetViewer_ = showAssetViewer;

    projectUI_ = projectUI;
    showProject_ = showProject;
    project_ = project;
}

void Graphics::addCube(float x, float y, float z) {
    //test_entt_scene.AddSimpleCube("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader, XMFLOAT3(x, y, z));
    motor::ThreadPool().submit([this, x, y, z] {
        RenderableGameObject gameObject;

        if (!gameObject.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            throw std::runtime_error("could not load Cube.obj");
        }

        gameObject.SetPosition(x, y, z);
        std::lock_guard guard{ renderable_objects_mtx };
        renderable_objects.push_back(gameObject);
    });
}

void Graphics::addLightCube(float x, float y, float z) {
    solar_system_scene.AddCube(device.Get(), deviceContext.Get(), texture.Get(), cb_vs_vertex_shader, DirectX::XMFLOAT3{ x, y, z });
}

void Graphics::DrawScene(Scene& scene, const XMMATRIX& viewProjectionMatrix) {

    Entity primary_camera = scene.GetPrimaryCamera();

    XMMATRIX vpm = XMMatrixIdentity();

    auto point_lights = scene.GetEntitysByComponent<PointLightComponent>();
    int j = 0;
    for (auto&& i : point_lights) {
        auto& point_light_comp = i.GetComponent<PointLightComponent>();

        auto& local_pos = i.GetComponent<TransformComponent>().local_position;
        auto& world_pos = i.GetComponent<TransformComponent>().world_position;

        XMVECTOR scale_v;
        XMVECTOR rot_v;
        XMVECTOR pos_v;

        XMMatrixDecompose(&scale_v, &rot_v, &pos_v, ComponentSystems::GetTransformMatrix(i));

        XMFLOAT3 pos;

        XMStoreFloat3(&pos, pos_v);

        cb_ps_light.data.size = test_entt_scene.GetEntitysByComponent<PointLightComponent>().size();
        cb_ps_light.data.lights[j].dynamicLightColor = point_light_comp.lightColor;
        cb_ps_light.data.lights[j].dynamicLightStrength = point_light_comp.lightStrength;
        cb_ps_light.data.lights[j].dynamicLightPosition = pos;
        cb_ps_light.data.lights[j].dynamicLightAttenuation_A = point_light_comp.attennuation_A;
        cb_ps_light.data.lights[j].dynamicLightAttenuation_B = point_light_comp.attennuation_B;
        cb_ps_light.data.lights[j].dynamicLightAttenuation_C = point_light_comp.attennuation_C;
        /*cb_ps_light.data.dynamicLightColor = point_light_comp.lightColor;
        cb_ps_light.data.dynamicLightStrength = point_light_comp.lightStrength;
        cb_ps_light.data.dynamicLightPosition = pos;
        cb_ps_light.data.dynamicLightAttenuation_A = point_light_comp.attennuation_A;
        cb_ps_light.data.dynamicLightAttenuation_B = point_light_comp.attennuation_B;
        cb_ps_light.data.dynamicLightAttenuation_C = point_light_comp.attennuation_C;*/
        j++;
    }
    cb_ps_light.ApplyChanges();
    deviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddressOf());

    auto renderableEntities = scene.GetEntitysByComponent<MeshComponent>();
    for (auto&& i : renderableEntities)
    {
        deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertex_shader.GetAddressOf());

        auto&& meshes = i.GetComponent<MeshComponent>().model.meshes;

        auto worldMatrix = ComponentSystems::GetTransformMatrix(i);

        DirectX::BoundingFrustum local_frustum;

        if (state == Simulate && primary_camera) 
        {
            auto transform = primary_camera.GetComponent<TransformComponent>().GetLocalTransformMatrix();
            XMVECTOR Det = XMMatrixDeterminant(transform);
            XMMATRIX view = XMMatrixInverse(&Det,transform);
            vpm = view * primary_camera.GetComponent<CameraComponent>().camera.GetProjection();
            local_frustum = primary_camera.GetComponent<CameraComponent>().camera.GetFrustum();
            m_effect->SetVertexColorEnabled(true);
            m_effect->SetView(view);
            m_effect->SetProjection(primary_camera.GetComponent<CameraComponent>().camera.GetProjection());
        } 
        else 
        {
            vpm = viewProjectionMatrix;
            local_frustum = cam_container.GetCameraById(0).GetLocalBoundingFrustum();
            m_effect->SetVertexColorEnabled(true);
            m_effect->SetView(cam_container.GetCurrentCamera().GetViewMatrix());
            m_effect->SetProjection(cam_container.GetCurrentCamera().GetProjectionMatrix());
        }

        
        if (local_frustum.Contains(i.GetComponent<MeshComponent>().transformed_bounding_box) != DirectX::DISJOINT) 
        {
            for (int i = 0; i < meshes.size(); i++) {
                cb_vs_vertex_shader.data.wvpMatrix = meshes[i].GetMeshTransform() * worldMatrix * vpm; 
                cb_vs_vertex_shader.data.worldMatrix = meshes[i].GetMeshTransform() * worldMatrix; //Calculate World
                cb_vs_vertex_shader.ApplyChanges();

                UINT offset = 0;

                auto textures = meshes[i].GetTextures();


                if (textures.size() != 0) 
                {
                    for (int j = 0; j < textures.size(); j++) {
                        if (textures[j].GetType() == aiTextureType::aiTextureType_DIFFUSE) {
                            deviceContext->PSSetShaderResources(0, 1, textures[j].GetTextureResourceViewAddress());
                            break;
                        }
                    }
                } else 
                {
                    this->deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf()); //Set Texture
                }
                

                auto vertexbuffer = meshes[i].GetVertexBuffer();
                auto indexbuffer = meshes[i].GetIndexBuffer();

                this->deviceContext->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), vertexbuffer.StridePtr(), &offset);
                this->deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
                this->deviceContext->DrawIndexed(indexbuffer.IndexCount(), 0, 0);
            }
        }
    }

    
}

void Graphics::DrawDebugScene(Scene& scene) 
{
    if (state == States::Editor) 
    {
        m_batch->Begin();
        auto renderableEntities = scene.GetEntitysByComponent<MeshComponent>();
        for (auto&& i : renderableEntities) {
            Draw(m_batch.get(), i.GetComponent<MeshComponent>().transformed_bounding_box, DirectX::Colors::Pink);
        }

        auto transformEntites = scene.GetEntitysByComponent<TransformComponent>();
        for (auto&& i : transformEntites) {
            auto& transform_comp = i.GetComponent<TransformComponent>();

            XMVECTOR scale;
            XMVECTOR rot;
            XMVECTOR pos;

            XMMatrixDecompose(&scale, &rot, &pos, ComponentSystems::GetTransformMatrix(i));

            DrawRay(m_batch.get(), pos, transform_comp.GetForwardVector(), true, DirectX::Colors::LightBlue);
            DrawRay(m_batch.get(), pos, transform_comp.GetRightVector(), true, DirectX::Colors::Red);
            DrawRay(m_batch.get(), pos, transform_comp.GetUpVector(), true, DirectX::Colors::LightGreen);
        }

        auto camerasEntities = scene.GetEntitysByComponent<CameraComponent>();
        for (auto&& i : camerasEntities) {
            Draw(m_batch.get(), i.GetComponent<CameraComponent>().camera.GetFrustum(), DirectX::Colors::Orange);
        }

        m_batch->End();
    }
}
//void Graphics::DrawModel(RenderableEntities renderable_entity)
//{
//
//}

void Graphics::UnloadProject_() {
    motor::utils::debug_write::info("Unloading project...\n");
    UnloadScene_();
    project_->CloseProject();
    showScenes_ = false;
    loadedProject_ = false;
}

void Graphics::LoadProject_() {
    debug_assert(project_->IsOpened());
    motor::utils::debug_write::info("Loading project...\n");
    assetViewerUI_->SetRoot(std::filesystem::current_path());
    showScenes_ = true;
    loadedProject_ = true;
}

void Graphics::UnloadScene_() {
    physics_.Clean();
    test_entt_scene.Reset();
    model_loader.Reset();
    cam_container.Reset();
    loadedScene_ = false;
}

void Graphics::UpdatePhysics_() {
    if (simulate_) {
        auto tnow = std::chrono::steady_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(tnow - prevTime_).count();
        float delta = double(us) * 0.000001;
        physics_.Update(delta);
        auto es = test_entt_scene.GetPhysicsEntities();
        for (auto& e : es) {
            auto& p = e.GetComponent<PhysicsComponent>();
            auto& t = e.GetComponent<TransformComponent>();
            btVector3 location, rotation;
            if (p.id != -1) {
                physics_.Get(p.id, location, rotation);
                t.world_position = XMFLOAT3{ location.x(), location.y(), location.z() };
                t.world_rotation = XMFLOAT3{ rotation.x(), rotation.y(), rotation.z() };
            }
        }
    } else {
        auto es = test_entt_scene.GetPhysicsEntities();
        for (auto& e : es) {
            auto& p = e.GetComponent<PhysicsComponent>();
            auto& t = e.GetComponent<TransformComponent>();
            if (p.id != -1) {
                physics_.SetLocRot(p.id,
                  btVector3{ t.world_position.x, t.world_position.y, t.world_position.z },
                  btVector3{ t.world_rotation.x, t.world_rotation.y, t.world_rotation.z });
            }
        }
    }
}