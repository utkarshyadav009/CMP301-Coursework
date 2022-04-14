#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

//for rendring and texturing the ocean
//also for sending the gerstner wave data to the vertex shader for wave simulation
class OceanwaveShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 direction[2];
		XMFLOAT4 position[2];
		XMFLOAT4 cutoff;
		XMFLOAT4 specularPower;
		XMFLOAT4 specular;
		bool showNormals;
		XMFLOAT3 padding;
	};

	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

	struct TimeBufferType
	{
		float time;
		float speed;
		float amplitude;
		float steepness;
		XMFLOAT2 wavelength;
		XMFLOAT2 heightMap;
		XMFLOAT4 direction[2];
	};			 


public:
	OceanwaveShader(ID3D11Device* device, HWND hwnd);
	~OceanwaveShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* oceanTexture, ID3D11ShaderResourceView* oceanHeightMap, Light* light[2], XMFLOAT3 cameraPos, float time, float speed, float steepness, float amplitude, float wavelength,XMFLOAT2 directions[2],float heightMapLevel, int spotCutoff,bool shownormals);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* cameraBuffer;


};

