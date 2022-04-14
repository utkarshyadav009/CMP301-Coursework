#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

//for rendering and texturing the geometry to a black texure 
//used in the bloom pass to stop the bloom overlapping the geometry 
class blackTextureShader : public BaseShader
{
public:
	blackTextureShader(ID3D11Device* device, HWND hwnd);
	~blackTextureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};
