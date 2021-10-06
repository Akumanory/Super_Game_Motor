#include <motor/graphics/Graphics.h>

using namespace DirectX;

bool Graphics::Initialize(HWND hWnd, int width, int height)
{
	Logs::Debug("Initialize Graphics"); // Тестовый лог

	windowWidth = width;
	windowHeight = height;

	fps_timer.Start();

	if (!InitializeDirectX(hWnd))
	{
		Logs::Error("	InitializeDirectX is not initialized");
		return false;
	}

	if (!InitializeShaders())
	{
		Logs::Error("	InitializeShaders is not initialized");
		return false;
	}

	if (!InitializeScene())
	{
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

void Graphics::RenderFrame()
{
	// При Flip_Discard нужно при отображение кадра каждый раз устанавливать Render Targets
	deviceContext->OMSetRenderTargets(
		1,
		renderTargetView.GetAddressOf(),
		depth_stencil_view.Get()
	);



	float bgcolor[] = { 
		0.0f, // R
		0.0f, // G
		0.0f, // B
		1.0f  // A
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

	gameObject.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
	gameObject1.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
	gameObject2.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
	gameObject3.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
	gameObject4.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());

	solar_system_scene.DrawScene(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());
	
	deviceContext->PSSetShader(pixel_shader_no_light.GetShader(), NULL, 0);
	light.Draw(cam_container.GetCurrentCamera().GetViewMatrix() * cam_container.GetCurrentCamera().GetProjectionMatrix());

	// Draw Text and fps
	static int fps_counter = 0;
	static std::string fps_string = "FPS: 0";
	fps_counter += 1;
	if (fps_timer.GetMilisecondsElapsed() > 1000.0)
	{
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
		1.0f
	);

	ImGui::DragFloat(
		"Ambient Light strength",
		&cb_ps_light.data.ambientLightStrength,
		0.01f,
		0.0f,
		1.0f
	);
	ImGui::NewLine();
	ImGui::DragFloat3("Dynamic Light Color", &this->light.lightColor.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Strength", &this->light.lightStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation A", &this->light.attennuation_A, 0.01f, 0.1f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation B", &this->light.attennuation_B, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation C", &this->light.attennuation_C, 0.01f, 0.0f, 10.0f);

	ImGui::End();

	cam_container.ImGUIWindow();

	/*std::vector<std::string> test = { "A", "B" };

	ImGui::Begin;

	ImGui::BeginCombo("Active Camera", test[0].c_str());

	ImGui::End;*/


	// Assemble together Draw Data
	ImGui::Render();
	// Render draw data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hWnd)
{
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
			deviceContext.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain");

		// Creating backbuffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapchain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(backBuffer.GetAddressOf())
		);
		COM_ERROR_IF_FAILED(hr, "Falied GetBuffer");

		// render target view
		hr = device->CreateRenderTargetView(
			backBuffer.Get(), // backbuffer
			nullptr,
			renderTargetView.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Falied CreateRenderTargetView");

		// Depth stencil (z - buffer)
		CD3D11_TEXTURE2D_DESC depth_stencil_tex_desc(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			windowWidth,
			windowHeight
		);
		depth_stencil_tex_desc.MipLevels = 1;		
		depth_stencil_tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


		hr = device->CreateTexture2D(
			&depth_stencil_tex_desc,
			nullptr,
			depth_stencil_buffer.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = device->CreateDepthStencilView(
			depth_stencil_buffer.Get(),
			nullptr,
			depth_stencil_view.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view. ");

		deviceContext->OMSetRenderTargets(
			1,
			renderTargetView.GetAddressOf(),
			depth_stencil_view.Get()
		);

		// Create depth stencil state
		CD3D11_DEPTH_STENCIL_DESC depth_stencil_desc(D3D11_DEFAULT);
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = device->CreateDepthStencilState(
			&depth_stencil_desc,
			depth_stencil_state.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		// Create Viewport
		CD3D11_VIEWPORT viewport(
			0.0f,
			0.0f,
			static_cast<float>(windowWidth),
			static_cast<float>(windowHeight)
		);

		deviceContext->RSSetViewports(1, &viewport);

		// Rasterizer
		CD3D11_RASTERIZER_DESC rasterizer_desc(D3D11_DEFAULT);
		rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; // Wireframe(D3D11_FILL_WIREFRAME) is a possible considiration
		rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE; // Для того что бы не рендерилась задняя часть при риосвании против часовой трелки(короче если нормаль смотрит от нас)
		//rasterizer_desc.FrontCounterClockwise = TRUE // Для того что бы рисовать треугольники против часовой стрелки

		hr = device->CreateRasterizerState(
			&rasterizer_desc,
			rasterizer_state.GetAddressOf()
		);
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
			sampler_state.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
	}
	catch(COMException& ex)
	{
		Logs::Error(ex);
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders()
{
	Logs::Debug("	Initialize shaders");

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	UINT numElements = ARRAYSIZE(layoutDesc);

	if (!vertex_shader.Initialize(device, L"vertexshader.hlsl", layoutDesc, numElements))
	{
		Logs::Error("		Vertex shader not initialized");
		return false;
	}

	if (!pixel_shader.Initialize(device, L"pixelshader.hlsl"))
	{
		Logs::Error("		Pixel shader not initialized");
		return false;
	}

	if (!pixel_shader_no_light.Initialize(device, L"pixelshader_nolight.hlsl"))
	{
		Logs::Error("		Pixel shader not initialized");
		return false;
	}

	return true;
}

bool Graphics::InitializeScene()
{
	try {
		Logs::Debug("	Initialize scene");


		// Texture
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			device.Get(),
			L"Data\\Textures\\hylics_texture.jpg",
			nullptr,
			texture.GetAddressOf()
		);
		COM_ERROR_IF_FAILED(hr, "Failed to load wic texture from file.");

		// Constant buffer
		hr = cb_vs_vertex_shader.Initialize(device.Get(), deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = cb_ps_light.Initialize(device.Get(), deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cb_ps_light.data.ambientLightStrength = 0.5f;

		// Initialize  model

		/*for (size_t i = 0; i < 5; i++)
		{
			
			if (!models_solar_system[i].InitializeCube(device.Get(), deviceContext.Get(), texture.Get(), constant_buffer))
			{
				return false;
			}
			models_solar_system[i].SetPosition(0.0f, 0.0f, 0.0f);
		}*/

		/*if (!models[0].Initialize(device.Get(), deviceContext.Get(), texture.Get(), constant_buffer))
		{
			return false;
		}

		if (!model1.Initialize(device.Get(), deviceContext.Get(), texture.Get(), constant_buffer))
		{
			return false;
		}

		if (!model2.Initialize(device.Get(), deviceContext.Get(), texture.Get(), constant_buffer))
		{
			return false;
		}

		if (!model3.Initialize(device.Get(), deviceContext.Get(), texture.Get(), constant_buffer))
		{
			return false;
		}

		models[0].SetPosition(0.0f, 1.0f, 0.0f, false);

		model1.SetPosition(0.0f, 0.0f, 0.0f, false);*/

		
		/*if (!test_mesh_model.Initialize("Data\\Objects\\BOTTLE_v1.obj",device.Get(), deviceContext.Get(), texture.Get(), cb_vs_vertex_shader))
		{
			return false;
		}*/

		if (!gameObject.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader))
		{
			return false;
		}

		if (!gameObject1.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader))
		{
			return false;
		}

		if (!gameObject2.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader))
		{
			return false;
		}

		if (!gameObject3.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader))
		{
			return false;
		}

		if (!gameObject4.Initialize("Data\\Objects\\Cube\\Cube.obj", device.Get(), deviceContext.Get(), cb_vs_vertex_shader))
		{
			return false;
		}


		if (!light.Initialize(device.Get(), deviceContext.Get(), cb_vs_vertex_shader))
		{
			return false;
		}

		if (!solar_system_scene.Initialize(device.Get(), deviceContext.Get(), texture.Get(), cb_vs_vertex_shader))
		{
			return false;
		}

		gameObject1.SetPosition(4.0f, 0.0f, 0.0f);
		gameObject2.SetPosition(6.0f, 4.0f, 2.0f);
		gameObject3.SetPosition(-8.0f, -2.0f, -2.0f);
		gameObject4.SetPosition(3.0f, -4.0f, -6.0f);


		// Camera 1
		Camera camera;
		camera.SetCameraName("First camera");
		camera.SetPosition(0.0f, 0.0f, -2.0f);
        camera.SetProjectionValues(
			90.0f, // Fov 
			static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
			0.1f,
			1000.0f
		);

		// Camera 2
		Camera camera2;
        camera2.SetCameraName("Second camera");
        camera2.SetPosition(5.0f, 5.0f, -2.0f);
		camera2.SetProjectionValues(
			90.0f, // Fov 
			static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
			0.1f,
			1000.0f
		);

		// Add to camera container
		cam_container.AddCamera(camera);
		cam_container.AddCamera(camera2);


	}
	catch(COMException& ex)
	{
		Logs::Error(ex);
		return false;
	}
	return true;
}
