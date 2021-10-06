#pragma once

#include <motor/other/DirectXIncludes.h>
#include <motor/other/Logs.h>
#include <motor/graphics/Shaders.h>
#include <motor/graphics/Camera.h>
#include <motor/other/Timer.h>
#include <motor/other/StringConverter.h>
#include <motor/other/COMException.h>
#include <motor/graphics/Model.h>
#include <motor/graphics/RenderableGameObject.h>
#include <motor/graphics/Light.h>
#include <motor/graphics/CameraContainer.h>
#include <motor/graphics/scenes/SolarSystem.h>

// ImGui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
// 

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
    CameraContainer cam_container;

	RenderableGameObject gameObject;
	RenderableGameObject gameObject1;
	RenderableGameObject gameObject2;
	RenderableGameObject gameObject3;
	RenderableGameObject gameObject4;


	Light light;
	//Model test_mesh_model;
	SolarSystem solar_system_scene;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertex_shader;
	PixelShader pixel_shader;
	PixelShader pixel_shader_no_light;
	
	Timer fps_timer;
	ConstantBuffer<CB_VS_vertex_shader> cb_vs_vertex_shader;
	ConstantBuffer<CB_PS_light_shader> cb_ps_light;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;

	std::unique_ptr<DirectX::SpriteBatch> sprite_batch;
	std::unique_ptr<DirectX::SpriteFont> sprite_font;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;



	int windowWidth = 0;
	int windowHeight = 0;
};