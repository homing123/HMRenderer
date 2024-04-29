#include "Engine.h"

Engine* Engine::PInstance = NULL;


Engine::Engine(const ScreenPoint& screenSize, const HWND& hWnd) 
{
	pInput = new InputManager();
	pRenderer = new SoftRenderer(screenSize, hWnd);
	_MainCam.SetViewPortSize(screenSize);
}

GameObject& Engine::CreateGameObject(const std::string& name) 
{
	GameObject* obj = new GameObject(std::hash<std::string>()(name));
	_GameObjectes.push_back(std::unique_ptr<GameObject>(obj));
	return *_GameObjectes[_GameObjectes.size() - 1].get();
}
Mesh& Engine::CreateMesh(const std::size_t& meshKey)
{
	Mesh* mesh = new Mesh();
	_Meshes[meshKey] = std::unique_ptr<Mesh>(mesh);
	return *mesh;
}
void Engine::LoadTexture(const std::size_t textureKey, const std::string& fileName) {
	wstring widestr = wstring(fileName.begin(), fileName.end());
	const wchar_t* wcharName = widestr.c_str();

	Texture* texture = new Texture(wcharName);
	std::unique_ptr<Texture> unique(texture);

	_Textures[BaseTexture] = std::move(unique);
}

const std::size_t Engine::QuadMesh = std::hash<std::string>()("SM_Quad");
const std::size_t Engine::BaseTexture = std::hash<std::string>()("Base");
const std::string Engine::TextureFileName("Face_Cube.png");
bool Engine::LoadResource() 
{
	Mesh& quadMesh = CreateMesh(QuadMesh);

	constexpr float squareHalfSize = 0.5f;
	constexpr int VertexCount = 4;
	constexpr int TriangleCount = 2;
	constexpr int IndexCount = TriangleCount * 3;

	quadMesh.GetVertices() =
	{
		//-y
		Vector3(-squareHalfSize,-squareHalfSize,-squareHalfSize),
		Vector3(squareHalfSize,-squareHalfSize,-squareHalfSize),
		Vector3(squareHalfSize,-squareHalfSize,squareHalfSize),
		Vector3(-squareHalfSize,-squareHalfSize,squareHalfSize),
		//+y	
		Vector3(-squareHalfSize,squareHalfSize,-squareHalfSize),
		Vector3(-squareHalfSize,squareHalfSize,squareHalfSize),
		Vector3(squareHalfSize,squareHalfSize,squareHalfSize),
		Vector3(squareHalfSize,squareHalfSize,-squareHalfSize),
		//+z
		Vector3(squareHalfSize,-squareHalfSize,squareHalfSize),
		Vector3(squareHalfSize,squareHalfSize,squareHalfSize),
		Vector3(-squareHalfSize,squareHalfSize,squareHalfSize),
		Vector3(-squareHalfSize,-squareHalfSize,squareHalfSize),
		//+x
		Vector3(squareHalfSize,-squareHalfSize,-squareHalfSize),
		Vector3(squareHalfSize,squareHalfSize,-squareHalfSize),
		Vector3(squareHalfSize,squareHalfSize,squareHalfSize),
		Vector3(squareHalfSize,-squareHalfSize,squareHalfSize),
		//-z
		Vector3(-squareHalfSize,-squareHalfSize,-squareHalfSize),
		Vector3(-squareHalfSize,squareHalfSize,-squareHalfSize),
		Vector3(squareHalfSize,squareHalfSize,-squareHalfSize),
		Vector3(squareHalfSize,-squareHalfSize,-squareHalfSize),
		//-x
		Vector3(-squareHalfSize,-squareHalfSize,squareHalfSize),
		Vector3(-squareHalfSize,squareHalfSize,squareHalfSize),
		Vector3(-squareHalfSize,squareHalfSize,-squareHalfSize),
		Vector3(-squareHalfSize,-squareHalfSize,-squareHalfSize),
	};

	quadMesh.GetUVs() =
	{
		//-y
		Vector2(0.5f,0.f),
		Vector2(0.25f,0.f),	
		Vector2(0.25f,0.25f),	
		Vector2(0.5f,0.25f),
		//+y
		Vector2(0.5f,0.75f),
		Vector2(0.5f,0.5f),
		Vector2(0.25f,0.5f),
		Vector2(0.25f,0.75f),
		//+z
		Vector2(0.25f,0.25f),
		Vector2(0.25f,0.5f),
		Vector2(0.5f,0.5f),
		Vector2(0.5f,0.25f),
		//+x
		Vector2(0.f,0.25f),
		Vector2(0.f,0.5f),
		Vector2(0.25f,0.5f),
		Vector2(0.25f,0.25f),
		//-z
		Vector2(0.75f,0.25f),
		Vector2(0.75f,0.5f),
		Vector2(1.f,0.5f),
		Vector2(1.f,0.25f),
		//-x
		Vector2(0.5f,0.25f),
		Vector2(0.5f,0.5f),
		Vector2(0.75f,0.5f),
		Vector2(0.75f,0.25f),
	};

	quadMesh.GetIndices() =
	{
		0,1,2,0,2,3, //-y
		4,5,6,4,6,7, //+y
		8,9,10,8,10,11, //-z
		12,13,14,12,14,15, //+x
		16,17,18,16,18,19, //+z
		20,21,22,20,22,23 //-x
	};

	quadMesh.GetColors() =
	{
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,	
		LinearColor::Error,	
		LinearColor::Error,	
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
		LinearColor::Error,
	};

	LoadTexture(BaseTexture, TextureFileName);

	return true;
}

static const std::string PlayerGo("Player");

void Engine::LoadScene()
{
	for (int i = 0; i < 2; i++) 
	{
		string name = PlayerGo + to_string(i);
		GameObject& obj = CreateGameObject(name);
		obj.SetMesh(QuadMesh);
		obj.SetTexture(BaseTexture);
	}
}
void Engine::Start()
{
	Transform& T_Cam = _MainCam.GetTransform();
	Vector3 CamPos(0.f,0.f,-20.f);
	Vector3 CamLookPos(0.f, 0.f, 0.f);
	T_Cam.SetPosition(CamPos);
	T_Cam.LookAt(CamLookPos);

	GameObject* pobj;
	if (GetGameObject(PlayerGo + to_string(0), pobj))
	{
		Transform& t_obj = pobj->GetTransform();
		t_obj.SetPosition(Vector3(0.f, 0.f, 0.f));
		t_obj.SetScale(Vector3(10.f, 10.f, 10.f));
	}

	if (GetGameObject(PlayerGo + to_string(1), pobj))
	{
		Transform& t_obj = pobj->GetTransform();
		t_obj.SetPosition(Vector3(0.f,0.f,0.f));
		t_obj.SetScale(Vector3(10.f,10.f,10.f));
		t_obj.AddRotationY(45);
	}
}

float curDis = 20.f;
float moveSpeed = 5.f;
float curAngle = 270.f;
float CamRotYSpeed = 100.f;
void Engine::Update() 
{
	Transform& T_Cam = _MainCam.GetTransform();
	float rot = CamRotYSpeed * _DeltaTime;
	if (pInput->isKey(Key::Arrow_Right))
	{
		curAngle += CamRotYSpeed * _DeltaTime;
	}
	if (pInput->isKey(Key::Arrow_Left))
	{
		curAngle -= CamRotYSpeed * _DeltaTime;
	}
	if (pInput->isKey(Key::Arrow_Up))
	{
		curDis -= moveSpeed * _DeltaTime;
	}
	if (pInput->isKey(Key::Arrow_Down))
	{
		curDis += moveSpeed * _DeltaTime;
	}

	if (curDis > 105)
	{
		int a = 0;
	}
	Vector3 camPos(Math::Cos(curAngle), 0.f, Math::Sin(curAngle));
	T_Cam.SetPosition(camPos * curDis);
	T_Cam.LookAt(Vector3::Zero);
}
void Engine::Cycle()
{
	_DeltaTime = TimeUtil::GetDeltaTime();
	_ElapsedTime += _DeltaTime;
	Update();
	pRenderer->Render();

	pInput->Loging();
}
