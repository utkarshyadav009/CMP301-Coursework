#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

//vertical blur for bloom 
//gets the height of the texture and blurs vertically 
class verticalBlurShader : public BaseShader
{

private:

	struct VblurBufferType
	{
		float height;
		XMFLOAT3 padding;
	};

public:


	verticalBlurShader(ID3D11Device* device, HWND hwnd);
	~verticalBlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, XMFLOAT2 resolution);

private:

	void initShader(const wchar_t* vs, const wchar_t* ps);

private:

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* configBuffer;
	ID3D11SamplerState* sampleState;

};
