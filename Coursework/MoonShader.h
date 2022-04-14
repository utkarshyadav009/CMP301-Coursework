#pragma once
#include "BaseShader.h"

using namespace std;
using namespace DirectX;

//rendering and texturing the moon 
//is also getting used by the street lamp light and the revolving marble head 
class MoonShader : public BaseShader
{
public:
	MoonShader(ID3D11Device* device, HWND hwnd);
	~MoonShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);
private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};

