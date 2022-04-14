#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

//horizontal blur for bloom 
//gets the width of the texture and blurs horizontally 
class horizontalBlurShader : public BaseShader

{
private:

	struct HblurBufferType
	{
		float width;
		XMFLOAT3 padding;
	};

public:


	horizontalBlurShader(ID3D11Device* device, HWND hwnd);
	~horizontalBlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, XMFLOAT2 resolution);

private:

	void initShader(const wchar_t* vs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* configBuffer;
	ID3D11SamplerState* sampleState;

};

