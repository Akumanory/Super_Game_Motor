#include <motor/graphics/Graphics.h>
#include <motor/task_system.hpp>

#include "ImGuizmo.h"

using namespace DirectX;

int gizmoCount = 1;
float camDistance = 8.f;
static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);

float objectMatrix[4][16] = {
    { 1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f },

    { 1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      2.f, 0.f, 0.f, 1.f },

    { 1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      2.f, 0.f, 2.f, 1.f },

    { 1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 2.f, 1.f }
};

static const float identityMatrix[16] = { 1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f };

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16) {
    float temp, temp2, temp3, temp4;
    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    m16[0] = temp / temp2;
    m16[1] = 0.0;
    m16[2] = 0.0;
    m16[3] = 0.0;
    m16[4] = 0.0;
    m16[5] = temp / temp3;
    m16[6] = 0.0;
    m16[7] = 0.0;
    m16[8] = (right + left) / temp2;
    m16[9] = (top + bottom) / temp3;
    m16[10] = (-zfar - znear) / temp4;
    m16[11] = -1.0f;
    m16[12] = 0.0;
    m16[13] = 0.0;
    m16[14] = (-temp * zfar) / temp4;
    m16[15] = 0.0;
}

void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16) {
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
    xmax = ymax * aspectRatio;
    Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}

void Cross(const float* a, const float* b, float* r) {
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
}

float Dot(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void Normalize(const float* a, float* r) {
    float il = 1.f / (sqrtf(Dot(a, a)) + FLT_EPSILON);
    r[0] = a[0] * il;
    r[1] = a[1] * il;
    r[2] = a[2] * il;
}

void LookAt(const float* eye, const float* at, const float* up, float* m16) {
    float X[3], Y[3], Z[3], tmp[3];

    tmp[0] = eye[0] - at[0];
    tmp[1] = eye[1] - at[1];
    tmp[2] = eye[2] - at[2];
    Normalize(tmp, Z);
    Normalize(up, Y);

    Cross(Y, Z, tmp);
    Normalize(tmp, X);

    Cross(Z, X, tmp);
    Normalize(tmp, Y);

    m16[0] = X[0];
    m16[1] = Y[0];
    m16[2] = Z[0];
    m16[3] = 0.0f;
    m16[4] = X[1];
    m16[5] = Y[1];
    m16[6] = Z[1];
    m16[7] = 0.0f;
    m16[8] = X[2];
    m16[9] = Y[2];
    m16[10] = Z[2];
    m16[11] = 0.0f;
    m16[12] = -Dot(X, eye);
    m16[13] = -Dot(Y, eye);
    m16[14] = -Dot(Z, eye);
    m16[15] = 1.0f;
}

void OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16) {
    m16[0] = 2 / (r - l);
    m16[1] = 0.0f;
    m16[2] = 0.0f;
    m16[3] = 0.0f;
    m16[4] = 0.0f;
    m16[5] = 2 / (t - b);
    m16[6] = 0.0f;
    m16[7] = 0.0f;
    m16[8] = 0.0f;
    m16[9] = 0.0f;
    m16[10] = 1.0f / (zf - zn);
    m16[11] = 0.0f;
    m16[12] = (l + r) / (l - r);
    m16[13] = (t + b) / (b - t);
    m16[14] = zn / (zn - zf);
    m16[15] = 1.0f;
}

inline void rotationY(const float angle, float* m16) {
    float c = cosf(angle);
    float s = sinf(angle);

    m16[0] = c;
    m16[1] = 0.0f;
    m16[2] = -s;
    m16[3] = 0.0f;
    m16[4] = 0.0f;
    m16[5] = 1.f;
    m16[6] = 0.0f;
    m16[7] = 0.0f;
    m16[8] = s;
    m16[9] = 0.0f;
    m16[10] = c;
    m16[11] = 0.0f;
    m16[12] = 0.f;
    m16[13] = 0.f;
    m16[14] = 0.f;
    m16[15] = 1.0f;
}

void EditTransform(const float* cameraView, const float* cameraProjection, float* matrix, bool editTransformDecomposition) {
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::MODE::LOCAL);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    if (editTransformDecomposition) {
        if (ImGui::IsKeyPressed(90))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(69))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(82)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
            mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        if (mCurrentGizmoOperation != ImGuizmo::SCALE) {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(83))
            useSnap = !useSnap;
        ImGui::Checkbox("", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation) {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing) {
            ImGui::PushID(3);
            ImGui::Checkbox("", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;

    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
    ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    //mGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
}

bool Graphics::Initialize(HWND hWnd, int width, int height) {
    Logs::Debug("Initialize Graphics"); // Тестовый лог

    windowWidth = width;
    windowHeight = height;

    fps_timer.Start();

    if (!InitializeDirectX(hWnd)) {
        Logs::Error("	InitializeDirectX is not initialized");
        return false;
    }

    if (!InitializeShaders()) {
        Logs::Error("	InitializeShaders is not initialized");
        return false;
    }

    if (!InitializeScene()) {
        Logs::Error("	InitializeScene is not initialized");
        return false;
    }

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

void ToImGuizmo(float* dest, const float* src) {
    for (auto row = 0; row < 4; row++) {
        for (auto col = 0; col < 4; col++)
            dest[row * 4 + col] = src[col * 4 + row];
    }
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
    deviceContext->IASetInputLayout(vertex_shader.GetInputLayout());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TrianleList

    deviceContext->RSSetState(rasterizer_state.Get());
    deviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 0);

    deviceContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
    deviceContext->VSSetShader(vertex_shader.GetShader(), nullptr, 0);
    deviceContext->PSSetShader(pixel_shader.GetShader(), nullptr, 0);

    // ------------------------------------------------------------------
    UINT offset = 0;

    cb_ps_light.data.dynamicLightColor = light.lightColor;
    cb_ps_light.data.dynamicLightStrength = light.lightStrength;
    cb_ps_light.data.dynamicLightPosition = light.GetPositionFloat3();
    cb_ps_light.data.dynamicLightAttenuation_A = light.attennuation_A;
    cb_ps_light.data.dynamicLightAttenuation_B = light.attennuation_B;
    cb_ps_light.data.dynamicLightAttenuation_C = light.attennuation_C;
    cb_ps_light.ApplyChanges();
    deviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddressOf());

    /*model1.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	model2.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	model3.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());*/
    //test_mesh_model.DrawMeshes(camera.GetViewMatrix() * camera.GetProjectionMatrix());

    DrawObjects(true);

    /// -------------------------------------------------------------------

    //test_entt_scene.DrawSceneEntt(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix(), localSpaceFrustum);
    /// -------------------------------------------------------------------
    DrawScene(test_entt_scene, cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
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

    //solar_system_scene.DrawScene(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix(), cam_container.GetCameraById(0).GetLocalBoundingFrustum());

    deviceContext->PSSetShader(pixel_shader_no_light.GetShader(), NULL, 0);
    light.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());

    // Simple batch
    m_effect->SetVertexColorEnabled(true);
    m_effect->SetView(cam_container.GetCurrentCamera().GetViewMatrix());
    m_effect->SetProjection(cam_container.GetCurrentCamera().GetProjectionMatrix());

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

    m_batch->Begin();

    DrawDebugScene(test_entt_scene);
    DrawRay(m_batch.get(), renderable_objects[1].GetPositionVector(), renderable_objects[1].GetForwardVector(), true, DirectX::Colors::LightGreen);
    DrawRay(m_batch.get(), renderable_objects[1].GetPositionVector(), renderable_objects[1].GetRightVector(), true, DirectX::Colors::Red);
    DrawRay(m_batch.get(), renderable_objects[1].GetPositionVector(), renderable_objects[1].GetUpVector(), true, DirectX::Colors::LightBlue);
    Draw(m_batch.get(), renderable_objects[0].GetBoundingBox(), DirectX::Colors::Green);
    Draw(m_batch.get(), renderable_objects[1].GetBoundingBox(), DirectX::Colors::Blue);
    Draw(m_batch.get(), renderable_objects[2].GetBoundingBox(), DirectX::Colors::Olive);
    Draw(m_batch.get(), renderable_objects[3].GetBoundingBox(), DirectX::Colors::DarkSeaGreen);
    Draw(m_batch.get(), renderable_objects[4].GetBoundingBox(), DirectX::Colors::Yellow);

    m_batch->End();

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
    // Creater ImGui test window
    ImGui::Begin("Light Controls");

    ImGui::DragFloat3(
      "Ambient Light Color",
      &cb_ps_light.data.ambientLightColor.x,
      0.01f,
      0.0f,
      1.0f);

    ImGui::DragFloat(
      "Ambient Light strength",
      &cb_ps_light.data.ambientLightStrength,
      0.01f,
      0.0f,
      1.0f);
    ImGui::NewLine();
    ImGui::DragFloat3("Dynamic Light Color", &this->light.lightColor.x, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Dynamic Light Strength", &this->light.lightStrength, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Dynamic Light Attenuation A", &this->light.attennuation_A, 0.01f, 0.1f, 10.0f);
    ImGui::DragFloat("Dynamic Light Attenuation B", &this->light.attennuation_B, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Dynamic Light Attenuation C", &this->light.attennuation_C, 0.01f, 0.0f, 10.0f);

    ImGui::End();

    cam_container.ImGUIWindow();

    scene_hierachy.OnImguiRender();

    /*std::vector<std::string> test = { "A", "B" };

	ImGui::Begin;

	ImGui::BeginCombo("Active Camera", test[0].c_str());

	ImGui::End;*/

    if (showConsole_ != nullptr && consoleUI_ != nullptr) {
        if (*showConsole_) {
            consoleUI_->Draw("Lua Console", showConsole_);
        }
    }

    XMFLOAT4X4 camView, camProjection;
    XMStoreFloat4x4(&camView, cam_container.GetCurrentCamera().GetViewMatrix());
    XMStoreFloat4x4(&camProjection, cam_container.GetCurrentCamera().GetProjectionMatrix());
    float cameraProjection[16], cameraView[16];
    ToImGuizmo(cameraProjection, (float*)camProjection.m);
    ToImGuizmo(cameraView, (float*)camView.m);

    int lastUsing = 0;
    bool isPerspective = true;
    float fov = 27.f;
    float viewWidth = 10.f; // for orthographic
    float camYAngle = 165.f / 180.f * 3.14159f;
    float camXAngle = 32.f / 180.f * 3.14159f;
    bool useWindow = false;
    static bool firstFrame = true;

    ImGuiIO& io = ImGui::GetIO();
    if (isPerspective) {
        Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, cameraProjection);
    } else {
        float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
        OrthoGraphic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, cameraProjection);
    }
    ImGuizmo::SetOrthographic(!isPerspective);
    ImGuizmo::BeginFrame();

    ImGui::SetNextWindowPos(ImVec2(1024, 100), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Appearing);

    // create a window and insert the inspector
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);
    ImGui::Begin("Editor");
    if (ImGui::RadioButton("Full view", !useWindow))
        useWindow = false;
    ImGui::SameLine();
    if (ImGui::RadioButton("Window", useWindow))
        useWindow = true;

    ImGui::Text("Camera");
    bool viewDirty = false;
    if (ImGui::RadioButton("Perspective", isPerspective))
        isPerspective = true;
    ImGui::SameLine();
    if (ImGui::RadioButton("Orthographic", !isPerspective))
        isPerspective = false;
    if (isPerspective) {
        ImGui::SliderFloat("Fov", &fov, 20.f, 110.f);
    } else {
        ImGui::SliderFloat("Ortho width", &viewWidth, 1, 20);
    }
    viewDirty |= ImGui::SliderFloat("Distance", &camDistance, 1.f, 10.f);
    ImGui::SliderInt("Gizmo count", &gizmoCount, 1, 4);

    if (viewDirty || firstFrame) {
        float eye[] = { cosf(camYAngle) * cosf(camXAngle) * camDistance, sinf(camXAngle) * camDistance, sinf(camYAngle) * cosf(camXAngle) * camDistance };
        float at[] = { 0.f, 0.f, 0.f };
        float up[] = { 0.f, 1.f, 0.f };
        LookAt(eye, at, up, cameraView);
        firstFrame = false;
    }

    ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
    if (ImGuizmo::IsUsing()) {
        ImGui::Text("Using gizmo");
    } else {
        ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
        ImGui::SameLine();
        ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
        ImGui::SameLine();
        ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
        ImGui::SameLine();
        ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
    }
    ImGui::Separator();
    for (int matId = 0; matId < gizmoCount; matId++) {
        ImGuizmo::SetID(matId);

        //EditTransform(cameraView, cameraProjection, objectMatrix[matId], lastUsing == matId);
        EditTransform(
          (float*)camView.m,
          (float*)camProjection.m,
          objectMatrix[matId],
          lastUsing == matId);
        if (ImGuizmo::IsUsing()) {
            lastUsing = matId;
        }
    }

    ImGui::End();

    

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

    if (!vertex_shader.Initialize(device, L"vertexshader.hlsl", layoutDesc, numElements)) {
        Logs::Error("		Vertex shader not initialized");
        return false;
    }

    if (!pixel_shader.Initialize(device, L"pixelshader.hlsl")) {
        Logs::Error("		Pixel shader not initialized");
        return false;
    }

    if (!pixel_shader_no_light.Initialize(device, L"pixelshader_nolight.hlsl")) {
        Logs::Error("		Pixel shader not initialized");
        return false;
    }

    return true;
}

bool Graphics::InitializeScene() {
    try {
        std::lock_guard guard{ renderable_objects_mtx };
        Logs::Debug("	Initialize scene");

        // Texture
        HRESULT hr = DirectX::CreateWICTextureFromFile(
          device.Get(),
          L"Data\\Textures\\hylics_texture.jpg",
          nullptr,
          texture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to load wic texture from file.");

        // Constant buffer
        hr = cb_vs_vertex_shader.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        hr = cb_ps_light.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

        cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
        cb_ps_light.data.ambientLightStrength = 0.5f;

        // LoadModels test
        // ---------------
        model_loader.Initialize(device.Get());

        model_loader.LoadModel("Data\\Objects\\Cube\\Cube.obj", "GrayCube");
        model_loader.LoadModel("Data\\Objects\\Wayne_pog_v2\\wayne_pog_v2.obj", "WayneCube");
        model_loader.LoadModel("Data\\Objects\\BOTTLE_V1.fbx", "Bottle");
        model_loader.LoadModel("Data\\Objects\\RubikCube.fbx", "RubikCube");
        
        
        // ---------------

        // Draw debug
        m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(deviceContext.Get());
        m_states = std::make_unique<CommonStates>(device.Get());
        m_effect = std::make_unique<BasicEffect>(device.Get());

        RenderableGameObject gameObject;

        if (!gameObject.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        renderable_objects.push_back(gameObject);

        RenderableGameObject gameObject1;

        if (!gameObject1.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        renderable_objects.push_back(gameObject1);

        RenderableGameObject gameObject2;

        if (!gameObject2.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        renderable_objects.push_back(gameObject2);

        RenderableGameObject gameObject3;

        if (!gameObject3.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        renderable_objects.push_back(gameObject3);

        RenderableGameObject gameObject4;

        if (!gameObject4.Initialize("Data\\Objects\\Wayne_pog_v2\\wayne_pog_v2.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        renderable_objects.push_back(gameObject4);

        if (!light.Initialize(device.Get(), deviceContext.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        if (!solar_system_scene.Initialize(device.Get(), deviceContext.Get(), texture.Get(), cb_vs_vertex_shader)) {
            return false;
        }

        renderable_objects[1].SetPosition(4.0f, 0.0f, 0.0f);
        //----------------------
        renderable_objects[1].SetRotation(0.0f, 0.5f, 0.0f);
        // ---------------------
        renderable_objects[2].SetPosition(6.0f, 4.0f, 2.0f);
        renderable_objects[3].SetPosition(-8.0f, -2.0f, -2.0f);
        renderable_objects[4].SetPosition(3.0f, -4.0f, -6.0f);

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

        entity1 = test_entt_scene.CreateEntity("First Entity");
        ComponentSystems::SetPosition(entity1, DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f));
        ComponentSystems::SetRotation(entity1, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        entity1.AddComponent<MeshComponent>();
        //ComponentSystems::SetModel(entity1, model_loader.GetModelById(0));

        entity2 = test_entt_scene.CreateEntity("Second Entity");
        ComponentSystems::SetPosition(entity2, DirectX::XMFLOAT3(0.0f, 6.0f, 4.0f));
        ComponentSystems::SetRotation(entity2, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        entity2.AddComponent<MeshComponent>();
        //ComponentSystems::SetModel(entity2, model_loader.GetModelById(0));

        entity3 = test_entt_scene.CreateEntity("Third Entity");
        ComponentSystems::SetPosition(entity3, DirectX::XMFLOAT3(0.0f, -4.0f, 3.0f));
        ComponentSystems::SetRotation(entity3, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        //entity3.AddComponent<MeshComponent>();
        //ComponentSystems::SetModel(entity3, model_loader.GetModelById(0));


        entity3.AddComponent<ParentComponent>(entity1);

        //ComponentSystems::SetChildEntity(entity1, entity2);
        //ComponentSystems::SetChildEntity(entity1, entity3);

        scene_hierachy.SetContext(&test_entt_scene);


    } catch (COMException& ex) {
        Logs::Error(ex);
        return false;
    }
    return true;
}

void Graphics::DrawObjects(bool f_culling_enabled) {
    // Check Frustrum culling
    viewMatrix = cam_container.GetCameraById(0).GetViewMatrix();
    XMVECTOR Det = XMMatrixDeterminant(viewMatrix);
    XMMATRIX invView = XMMatrixInverse(&Det, viewMatrix);

    BoundingFrustum localSpaceFrustum = cam_container.GetCameraById(0).GetLocalBoundingFrustum();

    for (size_t i = 0; i < renderable_objects.size(); i++) {

        if (f_culling_enabled) { /*
            XMMATRIX world = renderable_objects[i].GetWorldMatrix();
            XMVECTOR Det2 = XMMatrixDeterminant(world);
            XMMATRIX invWorld = XMMatrixInverse(&Det2, world);
            XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

            BoundingFrustum localSpaceFrustum;
            f_culling.Transform(localSpaceFrustum, viewToLocal);*/

            if (localSpaceFrustum.Contains(renderable_objects[i].GetBoundingBox()) != DirectX::DISJOINT) {
                renderable_objects[i].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
            } else {
                //Logs::Debug("Object not rendered");
            }
        } else {
            renderable_objects[i].Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
        }
    }
}

void Graphics::setConsole(motor::ui_system::ConsoleUI* console, bool* showConsole) {
    consoleUI_ = console;
    showConsole_ = showConsole;
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

// TODO: нужно не удалять
void Graphics::DrawScene(Scene& scene, const XMMATRIX& viewProjectionMatrix) {

    deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertex_shader.GetAddressOf());

    auto renderableEntities = scene.GetRenderableEntities();
    for (auto&& i : renderableEntities)
    {
        auto&& meshes = i.GetComponent<MeshComponent>().model.meshes;

        auto worldMatrix = ComponentSystems::GetTransformMatrix(i);

        DirectX::BoundingFrustum local_frustum = cam_container.GetCameraById(0).GetLocalBoundingFrustum();
        if (local_frustum.Contains(i.GetComponent<MeshComponent>().transformed_bounding_box) != DirectX::DISJOINT) 
        {
            for (int i = 0; i < meshes.size(); i++) {
                cb_vs_vertex_shader.data.wvpMatrix = meshes[i].GetMeshTransform() * worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
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

        ComponentSystems::UpdateBoundingBox(i);
    }
}

void Graphics::DrawDebugScene(Scene& scene) 
{
    auto renderableEntities = scene.GetRenderableEntities();
    for (auto&& i : renderableEntities) {
        Draw(m_batch.get(), i.GetComponent<MeshComponent>().transformed_bounding_box, DirectX::Colors::Pink);
    }
}

//void Graphics::DrawModel(RenderableEntities renderable_entity)
//{
//
//}