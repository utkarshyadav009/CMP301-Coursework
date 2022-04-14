#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;
class ReflectionShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ReflectionBufferType
	{
		XMMATRIX reflectionMatrix;
	};
public:
	ReflectionShader(ID3D11Device* device, HWND hwnd);
	~ReflectionShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* reflectionTexture, const XMMATRIX& reflection);
private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* reflectionBuffer;

};

