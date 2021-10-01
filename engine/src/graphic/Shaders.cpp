#include "Shaders.h"

#pragma region VertexShader
bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	Logs::Debug("		Vertex shader initialize");

	ID3DBlob* errorVertexCode = nullptr;
	HRESULT hr = D3DCompileFromFile(shaderpath.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shader_buffer.GetAddressOf(),
		&errorVertexCode);

	if (FAILED(hr)) 
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode != nullptr) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			Logs::Error(hr, compileErrors);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::wstring errorMsg = L"Missing Shader File: ";
			errorMsg += shaderpath;
			Logs::Error(hr, errorMsg);
		}
		errorVertexCode->Release();
		return false;
	}

	hr = device->CreateVertexShader(
		shader_buffer->GetBufferPointer(),
		shader_buffer->GetBufferSize(),
		nullptr,
		shader.GetAddressOf()
	);

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create vertex shader: ";
		errorMsg += shaderpath;
		Logs::Error(hr, errorMsg);
		return false;
	}

	hr = device->CreateInputLayout(
		layoutDesc,
		numElements,
		shader_buffer->GetBufferPointer(),
		shader_buffer->GetBufferSize(),
		inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		Logs::Error(hr, "Falied CreateInputLayout");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return shader_buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return inputLayout.Get();
}
#pragma endregion

#pragma region PixelShader
bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{
	Logs::Debug("		Pixel shader initialize");

	ID3DBlob* errorPixelCode = nullptr;
	HRESULT hr = D3DCompileFromFile(shaderpath.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shader_buffer.GetAddressOf(),
		&errorPixelCode);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode != nullptr) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			Logs::Error(hr, compileErrors);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::wstring errorMsg = L"Missing Shader File: ";
			errorMsg += shaderpath;
			Logs::Error(hr, errorMsg);
		}

		errorPixelCode->Release();
		return false;
	}

	hr = device->CreatePixelShader(
		shader_buffer->GetBufferPointer(),
		shader_buffer->GetBufferSize(),
		nullptr,
		shader.GetAddressOf()
	);

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create pixel shader: ";
		errorMsg += shaderpath;
		Logs::Error(hr, errorMsg);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return shader_buffer.Get();
}
#pragma endregion
