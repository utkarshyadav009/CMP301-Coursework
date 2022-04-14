#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

//bloom shader gets the main scene render texture and the bloom scene render texture and adds them  up 
class bloomShader : public BaseShader
{
public:

	bloomShader(ID3D11Device* device, HWND hwnd);
	~bloomShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* main, ID3D11ShaderResourceView* bloomTexture);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};

