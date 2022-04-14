#pragma once
#include "BaseShader.h"

using namespace std;
using namespace DirectX;

//basic texture shader
//sets the texture to the geometry 
//calculated normals and position 
class textureShader : public BaseShader
{
public:
	textureShader(ID3D11Device* device, HWND hwnd);
	~textureShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);

private:
	void initShader(const wchar_t*, const wchar_t*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};



