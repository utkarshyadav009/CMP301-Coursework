#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

//for rendering and the beach
//only getting called once for wireframe mode 
//the beach surface is now using the shadow shader
class BeachShader : public BaseShader
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
		float heightMap;
		bool useHeightMap;
		XMFLOAT3 padding;
	};

public:

	BeachShader(ID3D11Device* device, HWND hwnd);
	~BeachShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heighttexture,ID3D11ShaderResourceView* depthMap1, ID3D11ShaderResourceView* depthMap2, Light* light[2], XMFLOAT3 cameraPosition, int spotCutoff, bool spotlightToggle, bool moonlightToggle,int heightMaplevel,bool heightMaptoggle);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* cameraBuffer;
};

