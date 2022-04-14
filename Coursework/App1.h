// Application.h
#ifndef _APP1_H
#define _APP1_H
#include<Timer.h>
// Includes
#include "DXF.h"
#include"Light.h"
#include"AModel.h"

//sound 
#include"SoundClass.h"
//shadow shader
#include"ShadowShader.h"
#include"DepthShader.h"
#include"blackTextureShader.h"
//Geometry Shader
#include"OceanwaveShader.h"
#include"MoonShader.h"
#include"BeachShader.h"
//Bloom Shaders
#include"bloomShader.h"
#include"brightnessShader.h"
#include"horizontalBlurShader.h"
#include"verticalBlurShader.h"
#include"textureShader.h"




class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();
	void depthPass1(); //for directional light 
	void depthPass2(); // for spotlight 
	void bloomPass(); // for blooming everything that was in the sea pass 
	void renderPass(); // contains main render scene 
	void seaPass(); //things that are going to be bloomed 
	void finalPass(); // combines both main render scene and bloomed render scene 
private:
	//directional and spotlight 
	Light* light[2];

	//for inceasing or decreasing the downsample
	int downsampleAmount = 5;
	
	//ImGui UI stuff for both lights 
	XMFLOAT3 moonPosition = { 40.f,75.0f,50.0f };
	XMFLOAT3 lampPosition = { -31.250f,12.5f,52.9f };
	XMFLOAT3 moonLightColour[3];
	XMFLOAT3 lampLightColour[3];
	float specularPower[2];
	int spotCutoff = 30;
	bool spotLightToggle, moonLightToggle;
	//bloom RenderTexture  
	RenderTexture* seaTarget;  //Holds the mesh that is going to be bloomed 
	RenderTexture* downscale; //downscaling the texture for better bloom 
	
	//Post processing ortho mesh 
	OrthoMesh* postOrtho;

	//bloom Shaders 
	bloomShader* bloom_shader;
	brightnessShader* bright_shader;
	horizontalBlurShader* horizontal_blur_shader;
	verticalBlurShader* vertical_blur_shader;
	textureShader* texture_shader;  //basic texture shader used in bloom and shadows 

	// Hold the main scene render 
	RenderTexture* mainRenderScene;

	//timer variable for waves 
	Timer timer_;

	//sound variable for playing the song
	SoundClass* sound;

	//ocean shader, mesh and wave config variables
	OceanwaveShader* ocean_shader;
	PlaneMesh* ocean;
	float time; //for simulating waves 
	float amplitude; // height of the waves 
	float speed; // speed of the waves 
	float sharpness; //vertical sharpness of the waves 
	float wavelength; // lenght of the wave 
	XMFLOAT2 waveDir[2]; // direction of the two waves 
	float heightMapLevel = 1.f;
	//Moon shader and mesh 
	MoonShader* moon_shader;
	SphereMesh* moon;

	//skybox
	SphereMesh* sky;

	//beach shader and mesh 
	BeachShader* beach_shader;
	PlaneMesh* beach_model;

	//Shadow and depth shader 
	ShadowShader* shadow_shader;
	DepthShader* depth_shader;
	blackTextureShader* btexture_shader;
	ShadowMap* shadowMap[2];

	//lamp 
	AModel* lamp;
	SphereMesh* lamplight;
	//Models for shadows
	AModel* piano;
	AModel* tree;
	AModel* treeTrunk;
	AModel* treestem;
	AModel* leaves;
	AModel* body;
	SphereMesh* head;
	
	//for showing the normals on the ocean surface 
	bool oceanLightColour = false;
	bool showNormals = false;
	bool soundOption = true;
	int bheightMaplevel = 2;
	bool heightMaptoggle = false;
};

#endif