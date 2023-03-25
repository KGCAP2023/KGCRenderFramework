#include "pch.h"
#include "GraphicManager.h"
#include "Framework.h"
#include "GameObjectManager.h"
//imgui / 타이머
bool GraphicManager::Initialize(Framework* framework,HWND hwnd, int width, int height)
{
	/*
	
	initial 

	*/
	fps.Start();

	this->width = width;
	this->height = height;

	this->hwnd = hwnd;
	this->framework = framework;
	this->res = &framework->resourceManager;

	//오브젝트 매니져 생성
	gameObjectManager = this->framework->GetGameObjectManagerInstance();

	//DIRECTX 초기화
	if (!InitializeDirectX(hwnd)) return false;

	//쉐이더 초기화 
	if (!InitializeShaders()) return false;

	//씬 초기화
	if (!InitializeScene()) return false;




	/*
	
	IMGUI SETTING
	
	*/

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	this->io_ = &io;

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());

	return true;

}

void GraphicManager::RenderFrame()
{

	/*
	렌더 초기 설정
	ClearRenderTargetView 기존 렌더타켓뷰 정리		deviceContext
	ClearDepthStencilView 기존 뎁스스텐실뷰 정리		deviceContext
	RSSetState 레스터라이저 셋팅						deviceContext
	OMSetDepthStencilState 뎁스스텐실뷰 셋팅			deviceContext
	OMSetBlendState 블렌딩 셋팅						deviceContext
	*/

	float bgcolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->RSSetState(rasterizerState.Get()); //레스터라이저 설정한 옵션을 가져온다
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0); //뎁스 스텐실 뷰
	deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF); //블렌딩
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf()); //텍스쳐 렌더링

	
	if(gameObjectManager->FindGameObject("Nanosuit")==nullptr)
	{
		GameObject* obj1 = gameObjectManager->CreateGameObject("Nanosuit", "..\\Resource\\Objects\\Nanosuit\\Nanosuit.obj");
		obj1->transform.SetPosition(0, 5, 0);
	}

	


	/*
	//DRAW CALL (3D 도형 그리기)
	IASetInputLayout 버텍스 인풋 레이아웃 세트		deviceContext
	IASetPrimitiveTopology 삼각형 그리는 방식 세트	deviceContext
	VSSetShader 버텍스 쉐이더 세트					deviceContext
	PSSetShader 픽셀 쉐이더 세트						deviceContext

	PSSetShaderResources 텍스쳐 세트					deviceContext
	PSSetSamplers 텍스쳐 셈플러 스테이트 세트			deviceContext

	-------------------------------

	VSSetConstantBuffers 상수버퍼 세트				deviceContext
	deviceContext->Map() 상수버퍼에 내용물 탑재		deviceContext


	IASetVertexBuffers 버텍스 버퍼 세트				deviceContext
	IASetIndexBuffer   인덱스 버퍼 세트				deviceContext

	DrawIndexed 호출									deviceContext

	swapchain->Present() 스왑체인 호출				swapchain

	*/


	VertexShader* vs_3 = res->FindVertexShader("vs_1");
	deviceContext->IASetInputLayout(vs_3->GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX vp = cameraComponent->GetViewMatrix() * cameraComponent->GetProjectionMatrix();
	cameraComponent->SetProjectionValues(90.0f, static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 3000.0f);

	{
		for (const auto& kv : gameObjectManager->gameObjects) {
			(kv.second)->Draw(vp);
		}
	}


	static int fpsCounter = 0;
	fpsCounter += 1;

	if (fps.getDeltaTime() > 1000.0)
	{
		std::cout << "FPS:" << fpsCounter << std::endl;
		fpsCounter = 0;
		fps.ReStart();
	}

	const wchar_t* output = L"캡스톤_프로젝트";
	res->spriteBatch->Begin();
	res->spriteFont->DrawString(res->spriteBatch.get(), output, XMFLOAT2(0, 30), Colors::Black, 0.0f, XMFLOAT2(0, 0), 1.0f);
	res->spriteBatch->End();

	//Sprite* sp = res->FindSprite("ani");
	//sp->Draw(DirectX::SimpleMath::Vector2(0, 0), res->spriteBatch.get());

	deviceContext->CopyResource(refTex, backBuffer.Get());

	/**

	 IMGUI

	**/
	static bool inspector = true;
	static bool ppp = true;
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	framework->layerManager.DockingSpace();
	framework->layerManager.Render();
	

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io_->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	//V-Sync on = 1   // 0.3
	//V-Sync off = 0  // 1.8 0.8
	this->swapchain->Present(0, NULL);

}

bool GraphicManager::InitializeDirectX(HWND hwnd)
{
    //드라이버 타입 
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //D3D호환 레벨
    D3D_FEATURE_LEVEL featureLevels[] =
    {
		D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    //스왑체인(백버퍼) 생성
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferDesc.Width = this->width;
    sd.BufferDesc.Height = this->height;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    sd.SampleDesc.Count = 1; //MSAA 4배 샘플링
    sd.SampleDesc.Quality = 0;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hwnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        D3D_DRIVER_TYPE  driverType = driverTypes[driverTypeIndex];

        hr = D3D11CreateDeviceAndSwapChain(NULL, //IDXGI Adapter
            driverType,
            NULL, //FOR SOFTWARE DRIVER TYPE
            NULL, //FLAGS FOR RUNTIME LAYERS
            featureLevels, //FEATURE LEVELS 배열
            numFeatureLevels , //FEATURE LEVELS 배열 크기
            D3D11_SDK_VERSION,
            &sd, //스왑체인 디스크립터 
            this->swapchain.GetAddressOf(), //스왑체인 포인터 변수의 주소
            this->device.GetAddressOf(), //디바이스 포인터 변수의 주소
            NULL, //Supported feature level
            this->deviceContext.GetAddressOf()); //Device Context Address

        if (SUCCEEDED(hr)) break;
    }

    if (FAILED(hr))
    {
		Logger::Log(hr, "[X] Failed DeviceAndSwapChain!");
        return false;
    }

    //ID3D11Texture2D* pBackBuffer = NULL;
    
    hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
    if (FAILED(hr)) return false;

	

	//뎁스/스텐실 버퍼 디스크립터
	CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->width, this->height);
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.SampleDesc.Count = 1; //MSAA 4배 샘플링
	depthStencilTextureDesc.SampleDesc.Quality = 0;

	hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.GetAddressOf());


	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
	if (FAILED(hr)) return false;

	
	//OMrenderTargetView에 렌더타켓뷰와 뎁스스텐실뷰를 넘겨준다
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());


	//depth stencil state 생성 
	CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr)) return false;

	
	//Viewport 생성과 셋팅 
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->width), static_cast<float>(this->height));;
	this->deviceContext->RSSetViewports(1, &viewport);

	//Rasterizer State 생성
	CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
	//rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	//rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; 
	//rasterizerDesc.FrontCounterClockwise = TRUE; 이거하면 반시계로 버텍스 그려야됨 

	//rasterizerDesc.MultisampleEnable = true; //MSAA 4배 샘플링

	hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
	if (FAILED(hr)) return false;

	//Create Rasterizer State for culling front
	CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
	rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
	if (FAILED(hr)) return false;

	////Create Blend State
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0] = rtbd;

	hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());

	if (FAILED(hr)) return false;

	//COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

	this->res->Init(this->framework);
	this->res->InitFont(L"..\\Resource\\Fonts\\nanum.spritefont");

	//샘플러 스테이트를 생성합니다.
	CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
	if (FAILED(hr)) return false;

	D3D11_TEXTURE2D_DESC descTex;
	ZeroMemory(&descTex, sizeof(descTex));
	descTex.Width = 1600;
	descTex.Height = 900;
	descTex.MipLevels = 1;
	descTex.ArraySize = 1;
	descTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descTex.SampleDesc.Count = 1;
	descTex.Usage = D3D11_USAGE_DEFAULT;
	descTex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descTex.CPUAccessFlags = 0;

	device->CreateTexture2D(&descTex, nullptr, &refTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	descSRV.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(refTex, &descSRV, &refRes);

	std::cout << "[O] Successfully Completed DirectX Initialize!" << std::endl;
	return true;
}

bool GraphicManager::InitializeShaders()
{
	// 쉐이더를 파일에서 읽어옵니다. 
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	//};

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!res->LoadVertexShader("vs_1", L"..\\Shader\\VertexShader.hlsl", layout, numElements))
	{
		Logger::Log(L"버텍스쉐이더 로드 실패");
		return false;
	}

	if (!res->LoadPixelShader("ps_1", L"..\\Shader\\PixelShader.hlsl"))
	{
		Logger::Log(L"픽셀쉐이더 로드 실패");
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout3D[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements3D = ARRAYSIZE(layout3D);

	if (!res->LoadVertexShader("vs_2", L"..\\Shader\\VertexShader2.hlsl", layout3D, numElements3D))
	{
		Logger::Log(L"버텍스쉐이더 로드 실패");
		return false;
	}

	if (!res->LoadPixelShader("ps_2", L"..\\Shader\\PixelShader2.hlsl"))
	{
		Logger::Log(L"픽셀쉐이더 로드 실패");
		return false;
	}

	//struct VS_INPUT
	//{
	//	float3 inPos : POSITION;
	//	float2 inTexCoord : TEXCOORD;
	//	float3 inNormal : NORMAL;
	//	float4 boneIndices : BONEINDEX;
	//	float4 boneWeights : BONEWEIGHT;
	//};


	D3D11_INPUT_ELEMENT_DESC layoutw[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"BLENDINDICES", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"BLENDWEIGHT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
	};

	UINT numElementsw = ARRAYSIZE(layoutw);

	if (!res->LoadVertexShader("vs_3", L"..\\Shader\\VertexShader3.hlsl", layoutw, numElementsw))
	{
		Logger::Log(L"버텍스쉐이더 로드 실패");
		return false;
	}

	std::cout << "[O] Successfully Completed Shaders Initialize!" << std::endl;
	return true;
}

bool GraphicManager::InitializeScene()
{
	std::cout << "[=] Starting Scene Initialize....." << std::endl;

	//텍스쳐를 파일에서 읽어옵니다.
	// DIRECTX TEX 깃허브 참고 
	//-> DirectX::CreateWICTextureFromFile



	HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"..\\Resource\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
	if (FAILED(hr)) Logger::Log(hr,L"흙 텍스쳐 로드 실패");

	//상수버퍼 초기화 & 생성
	//-> 버텍스쉐이더
	//-> 픽셀 쉐이더 
	hr = res->cb1.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"상수버퍼1 로드 실패");

	hr = res->cb2.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"상수버퍼2 로드 실패");

	hr = res->cb_skinning_1.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"cb_skinning_1 로드 실패");

	hr = res->cb_skinning_2.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"cb_skinning_2 로드 실패");

	hr = res->cb_light.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"상수버퍼3 로드 실패");

	res->cb_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	res->cb_light.data.ambientLightStrength = 1.0f;

	res->LoadSprite("ani", "..\\Resource\\a.jpg");


	
	// 오브젝트를 파일에서 읽기 & 초기화
	// -> fbx (게임오브젝트 빛 스프라이트)

	//obj = new SimpleRenderableObject();
	//obj->Init(this->device.Get(), this->deviceContext.Get(), this->cb1);
	//obj->SetPosition(1, 1, 1);


	//SimpleRenderableObject* obj1 = new SimpleRenderableObject();
	//obj1->Init(this->device.Get(), this->deviceContext.Get(), this->cb1);
	//obj1->SetPosition(4, 0, 4);

	//SimpleRenderableObject* obj2 = new SimpleRenderableObject();
	//obj2->Init(this->device.Get(), this->deviceContext.Get(), this->cb1);
	//obj2->SetPosition(10, 0, 10);


	//RenderableObject* obj4 = new RenderableObject();
	//
	//obj4->SetPosition(2,0,0);

	

	//GameObject* obj2 = CreateGameObject_2("boblampclean", "Resource\\Objects\\boblamp\\boblampclean.md5mesh");
	//obj2->transform.SetPosition(0, 5, 0);
	//obj2->transform.SetScale(0.2f, 0.2f, 0.2f);
	//obj2->SetActive(true);

	//assimp 자체에 fbx 애니메이션 버그가 있다.
	//GameObject* obj2 = CreateGameObject_2("spin", "Resource\\Objects\\spin\\spin.dae");

	//GameObject* obj2 = CreateGameObject_2("walk", "Resource\\Objects\\walking\\walk.fbx");
	//obj2->transform.SetPosition(0, 5, 0);
	//obj2->transform.SetScale(0.05f, 0.05f, 0.05f);
	//obj2->SetActive(true);

	//GameObject* obj5 = CreateGameObject_2("walk2", "Resource\\Objects\\walking\\walk.fbx");
	//obj5->transform.SetPosition(0, 5, 0);
	//obj5->transform.SetScale(0.05f, 0.05f, 0.05f);
	//obj5->SetActive(true);

	//GameObject* obj6 = CreateGameObject_2("walk3", "Resource\\Objects\\walking\\walk.fbx");
	//obj5->transform.SetPosition(0, 5, 0);
	//obj5->transform.SetScale(0.05f, 0.05f, 0.05f);
	//obj5->SetActive(true);

	//GameObject* obj7 = CreateGameObject_2("walk4", "Resource\\Objects\\walking\\walk.fbx");
	//obj5->transform.SetPosition(0, 5, 0);
	//obj5->transform.SetScale(0.05f, 0.05f, 0.05f);
	//obj5->SetActive(true);

	//GameObject* obj3 = CreateGameObject_1("Nanosuit", "Resource\\Objects\\Nanosuit\\Nanosuit.obj");
	//obj3->transform.SetPosition(0, 5, 0);


	////GameObject* obj8 = CreateGameObject_1("steve", "Resource\\Objects\\steve\\source\\steve.fbx");
	////obj8->transform.SetPosition(0, 5, 0);

	//GameObject* obj8 = CreateGameObject_1("grass", "Resource\\Objects\\grass\\Grass_Block.obj");
	//obj8->transform.SetPosition(0, 5, 0);
	//obj8->transform.SetScale(0.5,0.5,0.5);

	//GameObject* obj9 = CreateGameObject_1("grass2", "Resource\\Objects\\grass\\Grass_Block.obj");
	//obj9->transform.SetPosition(1, 5, 0);
	//obj9->transform.SetScale(0.5, 0.5, 0.5);
	//

	//GameObject* obj3 = CreateGameObject_1("sponza", "Resource\\Objects\\Sponza\\sponza.obj");
	//obj2->transform.SetPosition(0, 5, 0);

	//카메라 
	this->camera = new GameObject("camera");
	this->cameraComponent = new Camera3D(camera);
	camera->AddComponent(cameraComponent);
	camera->transform.SetPosition(0.0f, 12.0f, -10.0f);
	this->cameraComponent->SetProjectionValues(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 3000.0f);

	/**********************************************/

	std::cout << "[O] Successfully Completed Scene Initialize!" << std::endl;
	return true;
}



/*
 스키닝 메시 테스트 함수
*/
GameObject* GraphicManager::CreateGameObject_2(const std::string& name, const std::string& path)
{
	PixelShader* ps_2 = res->FindPixelShader("ps_2");
	VertexShader* vs_3 = res->FindVertexShader("vs_3");

	GameObject* obj = new GameObject(name);
	obj->SetActive(false);
	SkinnedMeshRenderer* render1 = new SkinnedMeshRenderer(obj);
	render1->Init(path, this->device.Get(), this->deviceContext.Get(), res->cb_skinning_1, res->cb_skinning_2, vs_3, ps_2);
	obj->AddComponent(render1);
	//obj->AddComponent(new BoundingBoxRenderer(obj, this->device.Get(), this->deviceContext.Get(), &ps_1));

	gameObjectManager->gameObjects.insert(std::make_pair(name, obj));

	return obj;
}

bool GraphicManager::doTreeNode(GameObject* obj, int index) {

	ImGui::PushID(index);
	
	bool treeNodeOpen = ImGui::TreeNodeEx(
		obj->ObjectName.c_str(),
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_FramePadding |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth
	);

	if (ImGui::IsItemClicked())
	{
		std::cout << "SceneHierarchy: " << obj->ObjectName << "Clicked!" << std::endl;

		GameObject* before = gameObjectManager->FindGameObject(this->target);

		if (before != nullptr)
		{
			this->target = obj->ObjectName;
			//obj->SetActive(true);
		}
		else
		{
			//obj->SetActive(true);
			this->target = obj->ObjectName;
		}
		

	}

	ImGui::PopID();

	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("SceneHierarchy",NULL,0);
		ImGui::Text(obj->ObjectName.c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneHierarchy");
		ImGui::EndDragDropTarget();
	}

	return treeNodeOpen;
}

void GraphicManager::SceneHierarchyWindow()
{

	ImGui::Begin("Scene Hierarchy");

	int index = 0;

	for (const auto& kv : gameObjectManager->gameObjects) {


		bool treeNodeOpen = doTreeNode(kv.second, index);

		if (treeNodeOpen) {
			Renderer* render = dynamic_cast<Renderer*>(kv.second->GetComponent(Component::Type::RENDERER_MODEL));
			std::string path = "path: " + render->GetPath();
			ImGui::Text(path.c_str());
			std::string isActive = "isActive:	";
			std::string isDestroy = "isDestory:  ";
			
			isActive += kv.second->isActive ? "TRUE" : "FALSE";
			isDestroy += kv.second->isDestroy ? "TRUE" : "FALSE";

			ImGui::Text(isActive.c_str());
			ImGui::Text(isDestroy.c_str());

			ImGui::TreePop();
		}
		index++;

	}

	ImGui::End();

}

bool GraphicManager::openFile2()
{
	OPENFILENAME OFN;
	TCHAR filePathName[100] = L"";
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = this->hwnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		MessageBox(this->hwnd, filePathName, L"열기 선택", MB_OK);

		std::wstring value = OFN.lpstrFile;

		std::string t = t.assign(value.begin(), value.end());
		this->sFilePath = t;
		int a = 0;
	}
	return true;
}


bool GraphicManager::openFile()
{
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return FALSE;

	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	//  SHOW OPEN FILE DIALOG WINDOW
	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	sFilePath = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = sFilePath.find_last_of("/\\");
	sSelectedFile = sFilePath.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}