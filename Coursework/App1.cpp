// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"
#include<iostream>
App1::App1()
{
	ocean = nullptr;
	ocean_shader = nullptr;
	moon = nullptr;
	moon_shader = nullptr;
	bright_shader = nullptr;
	bloom_shader = nullptr;
	horizontal_blur_shader = nullptr;
	vertical_blur_shader = nullptr;
	texture_shader = nullptr;
	mainRenderScene = nullptr;
	postOrtho = nullptr;
	downscale = nullptr;
	beach_model = nullptr;
	beach_shader = nullptr;
	seaTarget = nullptr;
	shadowMap[0] = nullptr;
	shadowMap[1] = nullptr;
	shadow_shader = nullptr;
	depth_shader = nullptr;
	btexture_shader = nullptr;
	piano = nullptr;
	tree = nullptr;
	treestem = nullptr;
	treeTrunk = nullptr;
	leaves = nullptr;
	body = nullptr;
	head = nullptr;
} 

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//Loading textures 
	textureMgr->loadTexture(L"oceanTexture", L"res/red.png");
	textureMgr->loadTexture(L"beachTexture", L"res/sand.png");
	textureMgr->loadTexture(L"moonTexture", L"res/moontex.tif");
	textureMgr->loadTexture(L"nightSky", L"res/sky.png");
	textureMgr->loadTexture(L"sandHeightMap", L"res/sandHeightMap.png");
	textureMgr->loadTexture(L"piano", L"res/piano.png");
	textureMgr->loadTexture(L"tree", L"res/treeTexture.png");
	textureMgr->loadTexture(L"leaf", L"res/leafTexture.png");
	textureMgr->loadTexture(L"clothes", L"res/shinjiikarischoolclothesdiss_00.png");
	textureMgr->loadTexture(L"head", L"res/head.png");
	textureMgr->loadTexture(L"lamp", L"res/lamp.png");
	textureMgr->loadTexture(L"lamplight", L"res/lamplight.png");
	textureMgr->loadTexture(L"oceanHeightMap", L"res/oceanHeightMap.png");
	
	// initialising  ocean shader and mesh 
	ocean = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	ocean_shader = new OceanwaveShader(renderer->getDevice(), hwnd);
	waveDir[0] = { 1.0f,0.0f };
	waveDir[1] = { 1.0f,0.5f };

	//ocean WAVE speed and stuff
	speed = 3.0f;
	sharpness = 1.0f;
	amplitude = 1.0f;
	time = 1.f;
	wavelength = 10;

	// initialising  moon shader and mesh 
	moon = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(),false,20);
	moon_shader = new MoonShader(renderer->getDevice(), hwnd);

	//initialising  sky box mesh 
	sky = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), true, 100);

	// initialising  beach shader and mesh 
	beach_model = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	beach_shader = new BeachShader(renderer->getDevice(), hwnd);


	//models for shadows 
	piano = new AModel(renderer->getDevice(), "res/piano.obj");
	tree = new AModel(renderer->getDevice(), "res/treess.obj");
	treeTrunk = new AModel(renderer->getDevice(), "res/trees.obj");
	treestem = new AModel(renderer->getDevice(), "res/treeStem.obj");
	leaves = new AModel(renderer->getDevice(), "res/leaves.obj");
	body = new AModel(renderer->getDevice(), "res/headlessshinji.obj");
	head = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), true,20);
	lamp = new AModel(renderer->getDevice(), "res/lamp.obj");
	lamplight = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), false, 20);

	//initialising  Shadow shaders and depth shader 
	depth_shader  = new DepthShader(renderer->getDevice(), hwnd);
	shadow_shader = new ShadowShader(renderer->getDevice(), hwnd);
	btexture_shader = new blackTextureShader(renderer->getDevice(), hwnd);

	// Variables for defining shadow map
	int shadowmapWidth = 3840;
	int shadowmapHeight = 2160;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// shadow maps
	shadowMap[0] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	shadowMap[1] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	
	//post shaders initialisation 
	bright_shader = new brightnessShader(renderer->getDevice(), hwnd);
	bright_shader->setBrightnessThresholds(1.0f);
	bloom_shader = new bloomShader(renderer->getDevice(), hwnd);
	horizontal_blur_shader = new horizontalBlurShader(renderer->getDevice(), hwnd);
	vertical_blur_shader = new verticalBlurShader(renderer->getDevice(), hwnd);
	texture_shader = new textureShader(renderer->getDevice(), hwnd);


	//render to texture initialisation 
	postOrtho = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight, 0.0f, 0.0f);

	int dsWidth = sWidth / downsampleAmount;
	int dsHeight = sHeight / downsampleAmount;

	//initialising all  the Render to Texture  for bloom
	mainRenderScene = new RenderTexture(renderer->getDevice(), sWidth, sHeight, 0.1f, 100.0f);
	seaTarget = new RenderTexture(renderer->getDevice(), sWidth, sHeight, 0.1f, 100.0f);
	downscale = new RenderTexture(renderer->getDevice(), dsWidth, dsHeight, 0.1f, 100.0f);
	
	// Config directional light
	light[0] = new Light();
	light[0]->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light[0]->setDiffuseColour(0.413f, 0.507f, 0.625f, 1.0f);
	light[0]->setDirection(-1.0f, -1.0f, 0.0f);
	light[0]->setPosition(moonPosition.x, moonPosition.y, moonPosition.z);
	light[0]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light[0]->setSpecularPower(50.0f);
	light[0]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 200.f);
	moonLightColour[0] = { 0.413f, 0.507f, 0.625f };
	moonLightColour[1] = { 0.3f, 0.3f, 0.3f };
	moonLightColour[2] = { 1.0f, 1.0f, 1.0f };
	specularPower[0] = 50.0f;
	moonLightToggle = true;
	// Config spot light
	light[1] = new Light();
	light[1]->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	light[1]->setDiffuseColour(1.0f, 0.901f, 0.572f, 1.0f);
	light[1]->setDirection(0.0f, -1.0f, 0.0f);
	light[1]->setPosition(lampPosition.x, lampPosition.y, lampPosition.z);
	light[1]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light[1]->setSpecularPower(50.0f);
	light[1]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 200.f);
	lampLightColour[0] = { 1.0f, 0.901f, 0.572f };
	lampLightColour[1] = { 0.1f, 0.1f, 0.1f };
	lampLightColour[2] = { 1.0f, 1.0f, 1.0f };
	specularPower[1] = 50.f;
	spotLightToggle = true;
	//initializing the sound class 
	sound = new SoundClass;
	if (!sound)
	{
		cout << "Unable to load sound\n";
	}
	bool result = sound->Initialize(hwnd);
	if (!result)
	{
		cout << "Unable to load sound\n";
	}
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	
	if (ocean)
	{
		delete ocean;
		ocean = 0;
	}

	if (ocean_shader)
	{
		delete ocean_shader;
		ocean_shader = 0;
	}
	if (sound)
	{
		delete sound;
		sound = 0;
	}
	if (moon)
	{
		delete moon;
		moon = 0;
	}

	if (moon_shader)
	{
		delete moon_shader;
		moon_shader = 0;
	}
	if (bright_shader)
	{
		delete bright_shader;
		bright_shader = 0;
	}
	if (bloom_shader)
	{
		delete bloom_shader;
		bloom_shader = 0;
	}
	if (horizontal_blur_shader)
	{
		delete horizontal_blur_shader;
		horizontal_blur_shader = 0;
	}
	if (vertical_blur_shader)
	{
		delete vertical_blur_shader;
		vertical_blur_shader = 0;
	}
	if (texture_shader)
	{
		delete texture_shader;
		texture_shader = 0;
	}
	if (mainRenderScene)
	{
		delete mainRenderScene;
		mainRenderScene = 0;
	}
	if (postOrtho)
	{
		delete postOrtho;
		postOrtho = 0;
	}
	if (downscale)
	{
		delete downscale;
		downscale = 0;
	}
	if (beach_model)
	{
		delete beach_model;
		beach_model = 0;
	}
	if (beach_shader)
	{
		delete beach_shader;
		beach_shader = 0;
	}
	if (shadowMap)
	{
		delete shadowMap;
		shadowMap[0] = 0;
		shadowMap[1] = 0;
	}
	if (shadow_shader)
	{
		delete shadow_shader;
		shadow_shader = 0;
	}
	if (depth_shader)
	{
		delete depth_shader;
		depth_shader = 0;
	}
	if (btexture_shader)
	{
		delete btexture_shader;
		btexture_shader = 0;
	}
	if (piano)
	{
		delete piano;
		piano = 0;
	}
	if (tree)
	{
		delete tree;
		tree = 0;
	}
	if (treestem)
	{
		delete treestem;
		treestem = 0;
	}
	if (treeTrunk)
	{
		delete treeTrunk;
		treeTrunk = 0;
	}
	if (leaves)
	{
		delete leaves;
		leaves = 0;
	}
	if (body)
	{
		delete body;
		body = 0;
	}
	if (head)
	{
		delete head;
		head = 0;
	}
}


bool App1::frame()
{
	bool result;
	timer_.frame();
	
	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool App1::render()
{
	//setting the directional and spot lights positions for Light UI
	light[0]->setPosition(moonPosition.x, moonPosition.y, moonPosition.z);
	light[1]->setPosition(lampPosition.x, lampPosition.y+5, lampPosition.z);

	//setting the colour of directional light for light UI
	light[0]->setDiffuseColour(moonLightColour[0].x, moonLightColour[0].y, moonLightColour[0].z, 1.0f);
	light[0]->setAmbientColour(moonLightColour[1].x, moonLightColour[1].y, moonLightColour[1].z, 1.0f);
	light[0]->setSpecularColour(moonLightColour[2].x, moonLightColour[2].y, moonLightColour[2].z, 1.0f);
	light[0]->setSpecularPower(specularPower[0]);

	//setting the colour of spot light for light UI
	light[1]->setDiffuseColour(lampLightColour[0].x, lampLightColour[0].y, lampLightColour[0].z, 1.0f);
	light[1]->setAmbientColour(lampLightColour[1].x, lampLightColour[1].y, lampLightColour[1].z, 1.0f);
	light[1]->setSpecularColour(lampLightColour[2].x, lampLightColour[2].y, lampLightColour[2].z, 1.0f);
	light[1]->setSpecularPower(specularPower[1]);

	//for dynamic shadows
	// sets light direction with respect to a point in your scene
	XMVECTOR ov, lv, ldv; //ov is object position vector, lv is light position vector, ldv is light direction vector 
	XMFLOAT3 of, lf, ldf; //of is object position float3, lf is light position float3, ldf is light direction float3 
	lf = { light[0]->getPosition() }; // light position 
	of = { -50.f,3.f,50.f };// setting the object position to the center of the scene i.e the center of the beach 
	ov = XMLoadFloat3(&of); // loading float 3 into the vector for normalisation 
	lv = XMLoadFloat3(&lf); // loading float 3 into the vector for normalisation 
	ldv = XMVector3Normalize(ov - lv); // subtracting light position from object position and normalising it 
	XMStoreFloat3(&ldf, ldv); // storing light direction vector in a float3 
	light[0]->setDirection(ldf.x, ldf.y, ldf.z); // setting direction from that vector 


	camera->update();
	time += timer_.getTime();

	depthPass1(); // for directional light
	depthPass2(); // for spotlight 
	renderPass(); // main render pass 
	seaPass(); //everything here will getbloomed if it has a texture colour 
	bloomPass(); // blooms everything from seaPass
	finalPass(); // combines the main render scene with the bloomed scene 

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}
void App1::depthPass1()
{
	static float rotation = 0.f;
	rotation += (float)3.14 * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	// Set the render target to be the render to texture.
	shadowMap[0]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
	
	// get the view, and projection matrices from the light.
	light[0]->generateViewMatrix();
	XMMATRIX lightViewMatrix = light[0]->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light[0]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//render the base 
	beach_model->sendData(renderer->getDeviceContext());
	depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(-99.0f, 0.0f, 0.0f), lightViewMatrix, lightProjectionMatrix);
	depth_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

	beach_model->sendData(renderer->getDeviceContext());
	depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.1, 1, 1) * XMMatrixRotationZ(XMConvertToRadians(-15)), lightViewMatrix, lightProjectionMatrix);
	depth_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

	//render models which are suppose to have shadows 
	{
		piano->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.25f, 0.25f, 0.25f) * XMMatrixTranslation(-30, 7.1f, 30.0f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), piano->getIndexCount());

		tree->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), tree->getIndexCount());
		treeTrunk->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), treeTrunk->getIndexCount());
		treestem->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());
		leaves->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());

		lamp->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(lampPosition.x, 15.8f, lampPosition.z + 8), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), lamp->getIndexCount());

		lamplight->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(lampPosition.x, 25.8f, lampPosition.z + 6.5), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), lamplight->getIndexCount());

		body->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(-30.5, 0.0f, 50.0f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), body->getIndexCount());

		head->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(rotation) * XMMatrixTranslation(-29.5f, 3.8f, 50.0f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), head->getIndexCount());
	}
}
void App1::depthPass2()
{
	static float rotation = 0.f;
	rotation += (float)3.14 * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	// Set the render target to be the render to texture.
	shadowMap[1]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the view, and projection matrices from the spot light.
	light[1]->generateViewMatrix();
	light[1]->generateProjectionMatrix(0.1, 100);
	XMMATRIX lightViewMatrix = light[1]->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light[1]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	//render base 
	beach_model->sendData(renderer->getDeviceContext());
	depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(-99.0f, 0.0f, 0.0f), lightViewMatrix, lightProjectionMatrix);
	depth_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

	beach_model->sendData(renderer->getDeviceContext());
	depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.1, 1, 1) * XMMatrixRotationZ(XMConvertToRadians(-15)), lightViewMatrix, lightProjectionMatrix);
	depth_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());
	
	//render the objects which are suppose to have shadows 
	{
		piano->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.25f, 0.25f, 0.25f) * XMMatrixTranslation(-30, 7.1f, 30.0f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), piano->getIndexCount());

		tree->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), tree->getIndexCount());
		treeTrunk->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), treeTrunk->getIndexCount());
		treestem->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());
		leaves->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());

		body->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(-30.5, 0.0f, 50.0f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), body->getIndexCount());

		head->sendData(renderer->getDeviceContext());
		depth_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(rotation) * XMMatrixTranslation(-29.5f, 3.8f, 50.0f), lightViewMatrix, lightProjectionMatrix);
		depth_shader->render(renderer->getDeviceContext(), head->getIndexCount());
	}

}
void App1::renderPass()
{
	camera->update();
	
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	
	//for skybox rotation  
	static float rotation = 0.f;
	rotation += (float)3.14 * 0.0001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	//for head rotation 
	static float headrotation = 0.f;
	headrotation += (float)3.14 * 0.001f;
	if (headrotation > 360.0f)
	{
		headrotation -= 360.0f;
	}
	//Set the render target to the main render scene.
	mainRenderScene->setRenderTarget(renderer->getDeviceContext());
	mainRenderScene->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.f, 0.f, 1.0f);

	//setting the zbuffer to false to render the skybox 
	renderer->setZBuffer(false);
	sky->sendData(renderer->getDeviceContext());
	texture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationY(rotation) * XMMatrixScaling(10, 10, 10) * XMMatrixTranslation(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"nightSky"));
	texture_shader->render(renderer->getDeviceContext(), sky->getIndexCount());
	renderer->setZBuffer(true);

	//rendering models with proper lighting and textures 
	moon->sendData(renderer->getDeviceContext());
	moon_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10, 10, 10) * XMMatrixTranslation(moonPosition.x, moonPosition.y, moonPosition.z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"moonTexture"));
	moon_shader->render(renderer->getDeviceContext(), moon->getIndexCount());

	beach_model->sendData(renderer->getDeviceContext());
	beach_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(-99.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"beachTexture"), textureMgr->getTexture(L"sandHeightMap"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light,camera->getPosition(), spotCutoff,spotLightToggle,moonLightToggle, bheightMaplevel, heightMaptoggle);
	beach_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());
	
	//for checking if the ocean is being lit properly, light colour changes are not noticable due to the texture color 
	{
		if (!oceanLightColour){
			ocean->sendData(renderer->getDeviceContext());
			ocean_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix*XMMatrixTranslation(0.0,0.3,0.0), viewMatrix, projectionMatrix, textureMgr->getTexture(L"oceanTexture"), textureMgr->getTexture(L"oceanHeightMap"), light, camera->getPosition(), time, speed, sharpness, amplitude, wavelength, waveDir, heightMapLevel, spotCutoff, showNormals);
			ocean_shader->render(renderer->getDeviceContext(), ocean->getIndexCount());
		}
		else {
			ocean->sendData(renderer->getDeviceContext());
			ocean_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"blue"), textureMgr->getTexture(L"oceanHeightMap"), light, camera->getPosition(), time, speed, sharpness, amplitude, wavelength, waveDir, heightMapLevel, spotCutoff, showNormals);
			ocean_shader->render(renderer->getDeviceContext(), ocean->getIndexCount());
		}
	}
	//rendering the remaining models 
	beach_model->sendData(renderer->getDeviceContext());
	beach_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationZ(XMConvertToRadians(-10))*XMMatrixTranslation(-1.0,-0.2,0), viewMatrix, projectionMatrix, textureMgr->getTexture(L"beachTexture"), textureMgr->getTexture(L"sandHeightMap"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle, bheightMaplevel, heightMaptoggle);
	beach_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

	piano->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.25f, 0.25f, 0.25f) * XMMatrixTranslation(-30.5, 7.1f, 30.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"piano"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), piano->getIndexCount());

	tree->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), tree->getIndexCount());
	treeTrunk->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), treeTrunk->getIndexCount());
	treestem->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());
	leaves->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"leaf"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());

	body->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(5.0f, 5.0f, 5.0f)* XMMatrixRotationY(XMConvertToRadians(-90)) *XMMatrixTranslation(-31.0, 0.0f, 50.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"clothes"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), body->getIndexCount());

	head->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(headrotation) * XMMatrixTranslation(-30.0f, 3.8f, 50.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"head"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), head->getIndexCount());

	lamp->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix*XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(lampPosition.x, 15.8f, lampPosition.z+8), viewMatrix, projectionMatrix, textureMgr->getTexture(L"lamp"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), lamp->getIndexCount());

	lamplight->sendData(renderer->getDeviceContext());
	shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix  * XMMatrixTranslation(lampPosition.x, 25.8f, lampPosition.z+6.5), viewMatrix, projectionMatrix, textureMgr->getTexture(L"lamplight"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
	shadow_shader->render(renderer->getDeviceContext(), lamplight->getIndexCount());

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::seaPass()
{
	static float rotation = 0.f;
	rotation += (float)3.14 * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Set the render target to seaTarget.
	seaTarget->setRenderTarget(renderer->getDeviceContext());
	seaTarget->clearRenderTarget(renderer->getDeviceContext(), 0.f, 0.f, 0.f, 1.0f);

	//models to be bloomed 
	{
		if (moonLightToggle) {
			moon->sendData(renderer->getDeviceContext());
			moon_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10, 10, 10) * XMMatrixTranslation(moonPosition.x, moonPosition.y, moonPosition.z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"moonTexture"));
			moon_shader->render(renderer->getDeviceContext(), moon->getIndexCount());
		}

		if (spotLightToggle) {
			lamplight->sendData(renderer->getDeviceContext());
			moon_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(lampPosition.x, 25.8f, lampPosition.z + 6.5), viewMatrix, projectionMatrix, textureMgr->getTexture(L"lamplight"));
			moon_shader->render(renderer->getDeviceContext(), lamplight->getIndexCount());
		}


		head->sendData(renderer->getDeviceContext());
		moon_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(rotation) * XMMatrixTranslation(-30.0f, 3.8f, 50.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"head"));
		moon_shader->render(renderer->getDeviceContext(), head->getIndexCount());

		if (!oceanLightColour && !showNormals) {
			ocean->sendData(renderer->getDeviceContext());
			ocean_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"oceanTexture"), textureMgr->getTexture(L"oceanHeightMap"), light, camera->getPosition(), time, speed, sharpness, amplitude, wavelength, waveDir, heightMapLevel, spotCutoff, showNormals);
			ocean_shader->render(renderer->getDeviceContext(), ocean->getIndexCount());
		}
	}
	//models with black texture so bloom doesn't overlap on the main render scene 
	{
		beach_model->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(-99.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"beachTexture"));
		btexture_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

		piano->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.25f, 0.25f, 0.25f) * XMMatrixTranslation(-30, 7.1f, 30.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"piano"));
		btexture_shader->render(renderer->getDeviceContext(), piano->getIndexCount());

		tree->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"));
		btexture_shader->render(renderer->getDeviceContext(), tree->getIndexCount());
		treeTrunk->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"));
		btexture_shader->render(renderer->getDeviceContext(), treeTrunk->getIndexCount());
		treestem->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"));
		btexture_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());
		leaves->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"leaf"));
		btexture_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());

		body->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(-31.f, 0.0f, 50.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"clothes"));
		btexture_shader->render(renderer->getDeviceContext(), body->getIndexCount());

		lamp->sendData(renderer->getDeviceContext());
		btexture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(lampPosition.x, 15.8f, lampPosition.z + 8), viewMatrix, projectionMatrix, textureMgr->getTexture(L"lamp"));
		btexture_shader->render(renderer->getDeviceContext(), lamp->getIndexCount());

	}
	

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::finalPass()
{
	camera->update();

	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//combining both the main renderSene and the bloomed seaTarget render scene 
	if (!renderer->getWireframeState()) {
		postOrtho->sendData(renderer->getDeviceContext());
		bloom_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, mainRenderScene->getShaderResourceView(), seaTarget->getShaderResourceView());
		bloom_shader->render(renderer->getDeviceContext(), postOrtho->getIndexCount());
	}
	
	//for skybox rotation in wireframe mode 
	static float rotation = 0.f;
	rotation += (float)3.14 * 0.0001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	//for head rotation in wireframe mode 
	static float headrotation = 0.f;
	headrotation += (float)3.14 * 0.001f;
	if (headrotation > 360.0f)
	{
		headrotation -= 360.0f;
	}
	//for wireframe mode 
	if (renderer->getWireframeState())
	{
		renderer->setZBuffer(false);
		sky->sendData(renderer->getDeviceContext());
		texture_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationY(rotation) * XMMatrixScaling(10, 10, 10) * XMMatrixTranslation(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"nightSky"));
		texture_shader->render(renderer->getDeviceContext(), sky->getIndexCount());
		renderer->setZBuffer(true);

		beach_model->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(-99.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"beachTexture"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

		ocean->sendData(renderer->getDeviceContext());
		ocean_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"oceanTexture"), textureMgr->getTexture(L"oceanHeightMap"), light, camera->getPosition(), time, speed, sharpness, amplitude, wavelength,waveDir, heightMapLevel, spotCutoff, showNormals);
		ocean_shader->render(renderer->getDeviceContext(), ocean->getIndexCount());
		
		moon->sendData(renderer->getDeviceContext());
		moon_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10, 10, 10) * XMMatrixTranslation(moonPosition.x, moonPosition.y, moonPosition.z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"moonTexture"));
		moon_shader->render(renderer->getDeviceContext(), moon->getIndexCount());

		beach_model->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(-99.0f, 0.0f, 0.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"beachTexture"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), beach_model->getIndexCount());

		piano->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.25f, 0.25f, 0.25f) * XMMatrixTranslation(-30.5, 7.1f, 30.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"piano"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), piano->getIndexCount());

		tree->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), tree->getIndexCount());
		treeTrunk->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), treeTrunk->getIndexCount());
		treestem->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"tree"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());
		leaves->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-60.0f, 0.0f, 50.f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"leaf"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), treestem->getIndexCount());

		body->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(-31.0, 0.0f, 50.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"clothes"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), body->getIndexCount());

		head->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(headrotation) * XMMatrixTranslation(-30.0f, 3.8f, 50.0f), viewMatrix, projectionMatrix, textureMgr->getTexture(L"head"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), head->getIndexCount());

		lamp->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationY(XMConvertToRadians(-90)) * XMMatrixTranslation(lampPosition.x,15.8f, lampPosition.z + 8), viewMatrix, projectionMatrix, textureMgr->getTexture(L"lamp"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), lamp->getIndexCount());

		lamplight->sendData(renderer->getDeviceContext());
		shadow_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(lampPosition.x, 25.8f, lampPosition.z + 6.5), viewMatrix, projectionMatrix, textureMgr->getTexture(L"lamplight"), shadowMap[0]->getDepthMapSRV(), shadowMap[1]->getDepthMapSRV(), light, camera->getPosition(), spotCutoff, spotLightToggle, moonLightToggle);
		shadow_shader->render(renderer->getDeviceContext(), lamplight->getIndexCount());
	}
}

void App1::bloomPass()
{
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	XMFLOAT2 size = XMFLOAT2(seaTarget->getTextureWidth(), seaTarget->getTextureHeight());


	// Downscale and horizontal blur
	downscale->setRenderTarget(renderer->getDeviceContext());
	downscale->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	size = XMFLOAT2(downscale->getTextureWidth(), downscale->getTextureHeight());

	postOrtho->sendData(renderer->getDeviceContext());
	horizontal_blur_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, seaTarget->getShaderResourceView(), size);
	horizontal_blur_shader->render(renderer->getDeviceContext(), postOrtho->getIndexCount());

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();

	// vertical blur 
	seaTarget->setRenderTarget(renderer->getDeviceContext());
	seaTarget->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	size = XMFLOAT2(downscale->getTextureWidth(), downscale->getTextureHeight());

	postOrtho->sendData(renderer->getDeviceContext());
	vertical_blur_shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, downscale->getShaderResourceView(), size);
	vertical_blur_shader->render(renderer->getDeviceContext(), postOrtho->getIndexCount());

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);
	
	//wave and light UI 
	float waveDir1[2];
	waveDir1[0] = waveDir[0].x;
	waveDir1[1] = waveDir[0].y;
	float waveDir2[2];
	waveDir2[0] = waveDir[1].x;
	waveDir2[1] = waveDir[1].y;
	
	float moonPos[3];
	moonPos[0] = moonPosition.x;
	moonPos[1] = moonPosition.y;
	moonPos[2] = moonPosition.z;
	float moonlightc[3];
	moonlightc[0] = moonLightColour[0].x;
	moonlightc[1] = moonLightColour[0].y;
	moonlightc[2] = moonLightColour[0].z;

	float moonlightcA[3];
	moonlightcA[0] = moonLightColour[1].x;
	moonlightcA[1] = moonLightColour[1].y;
	moonlightcA[2] = moonLightColour[1].z;

	float moonlightcS[3];
	moonlightcS[0] = moonLightColour[2].x;
	moonlightcS[1] = moonLightColour[2].y;
	moonlightcS[2] = moonLightColour[2].z;

	float moonlightcDir[3];
	moonlightcDir[0] = moonLightColour[2].x;
	moonlightcDir[1] = moonLightColour[2].y;
	moonlightcDir[2] = moonLightColour[2].z;

	float lampPos[3];
	lampPos[0] = lampPosition.x;
	lampPos[1] = lampPosition.y;
	lampPos[2] = lampPosition.z;

	float lamplightc[3];
	lamplightc[0] = lampLightColour[0].x;
	lamplightc[1] = lampLightColour[0].y;
	lamplightc[2] = lampLightColour[0].z;

	float lamplightcA[3];	
	lamplightcA[0] = lampLightColour[1].x;
	lamplightcA[1] = lampLightColour[1].y;
	lamplightcA[2] = lampLightColour[1].z;

	float lamplightcS[3];
	lamplightcS[0] = lampLightColour[2].x;
	lamplightcS[1] = lampLightColour[2].y;
	lamplightcS[2] = lampLightColour[2].z;


	
	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	if(ImGui::Checkbox("Play/Pause music", &soundOption))
	{	if (!soundOption)
		{
			sound->pause();
		}
		else
			sound->play();
	}
	if (ImGui::CollapsingHeader("Moon Light Settings"))
	{
		ImGui::Checkbox("On/Off", &moonLightToggle);
		ImGui::SliderFloat3("Postion", moonPos, -120.f, 100.f, "%.3f");
		ImGui::ColorEdit3("Moon Light Diffuse Colour", moonlightc);
		ImGui::ColorEdit3("Moon Light Ambient Colour", moonlightcA);
		ImGui::ColorEdit3("Moon Light Specular Colour", moonlightcS);
		ImGui::SliderFloat("Moon Light Specular Power", &specularPower[0], 1.f, 100.f);
		if (ImGui::Button("Default Moon Light Colors"))
		{
			moonlightc[0] = 0.413f; moonlightc[1] = 0.507f;moonlightc[2] = 0.625f;
			moonlightcA[0] = 0.3f; moonlightcA[1] = 0.3f; moonlightcA[2] = 0.3f;
			moonlightcS[0] = 1.0f; moonlightcS[1] = 1.0f; moonlightcS[2] = 1.0f;
		}	
		ImGui::SameLine();
		if (ImGui::Button("Default Moon position"))
		{
			moonPos[0] = 40.f; moonPos[1] = 75.0f; moonPos[2] = 50.0f;
		}

	}
	if (ImGui::CollapsingHeader("Lamp Light Settings"))
	{
		ImGui::Checkbox("On/Off", &spotLightToggle);
		ImGui::SliderFloat3("Lamp Position", lampPos, -100.f, 100.f, "%.3f");
		ImGui::SliderInt("Lamp Diameter", &spotCutoff, 0, 100.f);
		ImGui::ColorEdit3("Lamp Light Diffuse Colour", lamplightc);
		ImGui::ColorEdit3("Lamp Light Ambient Colour", lamplightcA);
		ImGui::ColorEdit3("Lamp Light Specular Colour", lamplightcS);
		ImGui::SliderFloat("Lamp Light Specular Power", &specularPower[1], 1.f, 100.f);
		if (ImGui::Button("Default Lamp Light Colors"))
		{
			lamplightc[0] = 1.0f;lamplightc[1] = 0.901f;lamplightc[2] = 0.572f;
			lamplightcA[0] = 0.1f;lamplightcA[1] = 0.1f;lamplightcA[2] = 0.1f;
			lamplightcS[0] = 1.0f;lamplightcS[1] = 1.0f;lamplightcS[2] = 1.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Default Lamp position"))
		{
			lampPos[0] = -31.250f; lampPos[1] = 12.5f; lampPos[2] = 52.9f;
		}
	}
	if (ImGui::CollapsingHeader("Wave Settings"))
	{
		ImGui::SliderFloat("Speed", &speed, 0.0, 15.0f, "%.3f");
		ImGui::SliderFloat("Steepness", &sharpness, 0.0f, 1.0f, "%.3f");
		ImGui::SliderFloat("Amplitude", &amplitude, 0.0f, 15.0f, "%.3f");
		ImGui::SliderFloat("HeightMap Level", &heightMapLevel, 0.1, 5);
		ImGui::SliderFloat("Wavelength", &wavelength, 0.0f, 50.f, "%.3f");
		ImGui::SliderFloat2("Wave one Direction", waveDir1, 0, 1.f);
		ImGui::SliderFloat2("Wave two Direction", waveDir2, 0, 1.f);
		ImGui::Checkbox("Remove bloom texture to see light colour changes on the surface", &oceanLightColour);
		ImGui::Checkbox("See normals", &showNormals);

	}
	if (ImGui::CollapsingHeader("Beach Settings"))
	{
		ImGui::Checkbox("HeightMap Toggle", &heightMaptoggle);
		ImGui::SliderInt("Height Map", &bheightMaplevel, 1.0f, 10.f);
	}
	if (ImGui::CollapsingHeader("Bloom Settings"))
	{
		//CHANGE dOWNSAMPLE VALUE
		bool changed = ImGui::SliderInt("Down sample amount", &downsampleAmount, 1, 100);
		if (changed) {
			delete downscale;
			int dsWidth = sWidth / downsampleAmount;
			int dsHeight = sHeight / downsampleAmount;

			downscale = new RenderTexture(renderer->getDevice(), dsWidth, dsHeight, 0.1f, 100.0f);
		}
	}

	moonPosition.x = moonPos[0];
	moonPosition.y = moonPos[1];
	moonPosition.z = moonPos[2];

	moonLightColour[0].x = moonlightc[0];
	moonLightColour[0].y = moonlightc[1];
	moonLightColour[0].z = moonlightc[2];

	moonLightColour[1].x = moonlightcA[0];
	moonLightColour[1].y = moonlightcA[1];
	moonLightColour[1].z = moonlightcA[2];

	moonLightColour[2].x = moonlightcS[0];
	moonLightColour[2].y = moonlightcS[1];
	moonLightColour[2].z = moonlightcS[2];


	lampPosition.x = lampPos[0];
	lampPosition.y = lampPos[1];
	lampPosition.z = lampPos[2];

	lampLightColour[0].x = lamplightc[0];
	lampLightColour[0].y = lamplightc[1];
	lampLightColour[0].z = lamplightc[2];

	lampLightColour[1].x = lamplightcA[0];
	lampLightColour[1].y = lamplightcA[1];
	lampLightColour[1].z = lamplightcA[2];

	lampLightColour[2].x = lamplightcS[0];
	lampLightColour[2].y = lamplightcS[1];
	lampLightColour[2].z = lamplightcS[2];


	waveDir[0].x = waveDir1[0];
	waveDir[0].y = waveDir1[1];

	waveDir[1].x = waveDir2[0];
	waveDir[1].y = waveDir2[1];

	
	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}