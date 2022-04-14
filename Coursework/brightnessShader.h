#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

//brightnessShader gets a brightness threshold value anything above that threshold is going to get bloomed 
//not really utilising this in the project at this stage 
//as the ocean texture is a plain red texture which wouldn't get bloomed if passed through threshold 
class brightnessShader : public BaseShader
{
private:

	struct thresholdBufferType
	{
		XMFLOAT4 brightnessThreshhold;
	};

public:


	brightnessShader(ID3D11Device* device, HWND hwnd);
	~brightnessShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);
	void setBrightnessThresholds(XMFLOAT4 levels);
	void setBrightnessThresholds(float level);
private:

	void initShader(const wchar_t* vs, const wchar_t* ps);

private:

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* configBuffer;
	ID3D11SamplerState* sampleState;
	XMFLOAT4 thresholds;

};

