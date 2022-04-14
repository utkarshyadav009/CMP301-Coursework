#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "DXF.h"

using namespace std;
using namespace DirectX;

//sends the data for shadows and lighting 
//getting used by most geometry except the ocean, moon and lamp light
class ShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX pointlightView;
		XMMATRIX pointlightProjection;
		XMMATRIX spotlightView;
		XMMATRIX spotlightProjection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 direction[2];
		XMFLOAT4 position[2];
		XMFLOAT4 cutoff;
		XMFLOAT4 specularPower;
		XMFLOAT4 specular;
		bool spotlightToggle;
		XMFLOAT3 padding;
		bool moonlightToggle;
		XMFLOAT3 padding1;
	};
	struct CameraBufferType
	{
		XMFLOAT3 position;
		float padding;
	};

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap1, ID3D11ShaderResourceView* depthMap2, Light* light[2], XMFLOAT3 cameraPosition, int spotCutoff,bool spotlightToggle,bool moonlightToggle);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;	
	ID3D11Buffer* cameraBuffer;
};

#endif