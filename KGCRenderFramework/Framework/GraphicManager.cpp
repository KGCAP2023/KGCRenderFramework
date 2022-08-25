#include "GraphicManager.h"



//imgui / 타이머
bool GraphicManager::Initialize(HWND hwnd, int width, int height)
{
	/*
	
	initial 

	*/
	fps.Start();

	this->width = width;
	this->height = height;

	//DIRECTX 초기화
	if (!InitializeDirectX(hwnd)) return false;

	//쉐이더 초기화 
	if (!InitializeShaders()) return false;

	//씬 초기화
	if (!InitializeScene()) return false;

	/*
	
	IMGUI SETTING
	
	*/

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

	float bgcolor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->RSSetState(rasterizerState.Get()); //레스터라이저 설정한 옵션을 가져온다
	deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0); //뎁스 스텐실 뷰
	deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF); //블렌딩
	deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf()); //텍스쳐 렌더링

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
	deviceContext->IASetInputLayout(vs_1.GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(vs_1.GetShader(), NULL, 0);
	deviceContext->PSSetShader(ps_1.GetShader(), NULL, 0);

	

	XMMATRIX vp = cameraComponent->GetViewMatrix() * cameraComponent->GetProjectionMatrix();

	//{
	//	this->obj->Draw(vp);
	//}

	//{
	//	this->obj3->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	//}

	deviceContext->IASetInputLayout(vs_2.GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(vs_2.GetShader(), NULL, 0);
	deviceContext->PSSetShader(ps_2.GetShader(), NULL, 0);

	{
		for (const auto& kv : GameObject::gameObjects) {
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

	const wchar_t* output = L"렌더프레임워크";

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), output, XMFLOAT2(0, 0), Colors::Black, 0.0f, XMFLOAT2(0, 0),1.0f);
	spriteBatch->End();

	//TEXT 출력


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

    sd.SampleDesc.Count = 1;
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
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
    if (FAILED(hr)) return false;

	

	//뎁스/스텐실 버퍼 디스크립터
	CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->width, this->height);
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	

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
	hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
	if (FAILED(hr)) return false;

	//Create Rasterizer State for culling front
	CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
	rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
	if (FAILED(hr)) return false;

	////Create Blend State
	//D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
	//rtbd.BlendEnable = true;
	//rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	//rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	//rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	//rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	//rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	//D3D11_BLEND_DESC blendDesc = { 0 };
	//blendDesc.RenderTarget[0] = rtbd;

	//hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
	//COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Resource\\Fonts\\taebak.spritefont");

	//샘플러 스테이트를 생성합니다.
	CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
	if (FAILED(hr)) return false;


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

	UINT numElements2D = ARRAYSIZE(layout);

	if (!vs_1.Initialize(L"Shader\\VertexShader.hlsl","main", "vs_4_0_level_9_3", layout, numElements2D, device))
	{
		Logger::Log(L"버텍스쉐이더 로드 실패");
		return false;
	}

	if (!ps_1.Initialize(L"Shader\\PixelShader.hlsl", "main", "ps_4_0_level_9_3", device))
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

	if (!vs_2.Initialize(L"Shader\\VertexShader2.hlsl", "main", "vs_4_0_level_9_3", layout3D, numElements3D, device))
	{
		Logger::Log(L"버텍스쉐이더 로드 실패");
		return false;
	}
		

	if (!ps_2.Initialize(L"Shader\\PixelShader2.hlsl", "main", "ps_4_0_level_9_3", device))
	{
		Logger::Log(L"픽셀쉐이더 로드 실패");
		return false;
	}

	std::cout << "[O] Successfully Completed Shaders Initialize!" << std::endl;
	return true;
}

bool GraphicManager::InitializeScene()
{

	//텍스쳐를 파일에서 읽어옵니다.
	// DIRECTX TEX 깃허브 참고 
	//-> DirectX::CreateWICTextureFromFile
	HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Resource\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
	if (FAILED(hr)) Logger::Log(hr,L"흙 텍스쳐 로드 실패");

	//상수버퍼 초기화 & 생성
	//-> 버텍스쉐이더
	//-> 픽셀 쉐이더 
	hr = this->cb1.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"상수버퍼1 로드 실패");

	hr = this->cb2.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"상수버퍼2 로드 실패");

	hr = this->cb_light.Init(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr)) Logger::Log(hr, L"상수버퍼3 로드 실패");

	this->cb_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->cb_light.data.ambientLightStrength = 1.0f;


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
	this->camera = new GameObject();
	this->cameraComponent = new Camera3D(camera);
	camera->AddComponent(cameraComponent);

	GameObject* obj1 = new GameObject();
	ModelRenderer* render1 = new ModelRenderer(obj1);
	render1->Init("Resource\\Objects\\ying\\ying.pmx", this->device.Get(), this->deviceContext.Get(), this->cb2);
	obj1->AddComponent(render1);

	obj1->transform.SetPosition(5,0,5);

	GameObject* obj2 = new GameObject();
	ModelRenderer* render2 = new ModelRenderer(obj2);
	render2->Init("Resource\\Objects\\Nanosuit\\Nanosuit.obj", this->device.Get(), this->deviceContext.Get(), this->cb2);
	obj2->AddComponent(render2);

	GameObject* obj3 = new GameObject();
	ModelRenderer* render3 = new ModelRenderer(obj3);
	render3->Init("Resource\\Objects\\Sponza\\sponza.obj", this->device.Get(), this->deviceContext.Get(), this->cb2);
	obj3->AddComponent(render3);
	

	GameObject::gameObjects.insert(std::make_pair("hello1", obj1));
	GameObject::gameObjects.insert(std::make_pair("hello2", obj2));
	GameObject::gameObjects.insert(std::make_pair("hello4", obj3));


	//카메라 
	camera->transform.SetPosition(0.0f, 0.0f, -10.0f);
	this->cameraComponent->SetProjectionValues(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 3000.0f);

	/**********************************************/

	std::cout << "[O] Successfully Completed Scene Initialize!" << std::endl;
	return true;
}
