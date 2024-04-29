#pragma once
#include "EngineUtil.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Texture.h"
#include "SoftRenderer.h"
#include "InputManager.h"
#include "Camera.h"

class SoftRenderer;

namespace HM {
	class Engine
	{
	public:

		static constexpr char InvalidHashName[] = "InvalidHash";
		const std::size_t InvaildHash = std::hash<std::string>()(InvalidHashName);
		GameObject InvaildGO = GameObject(InvaildHash);
		Engine(const ScreenPoint& screenSize, const HWND& hWnd);
		GameObject& CreateGameObject(const std::string& name);
		bool GetGameObject(const std::string& name, GameObject*& outObj)
		{
			std::size_t hash = std::hash<std::string>()(name);
			int size = _GameObjectes.size();
			for (int i = 0; i < size; i++) {
				GameObject* pobj = _GameObjectes[i].get();
				if (pobj->GetName() == hash)
				{
					outObj = pobj;
					return true;
				}
			}
			return false;
		}

		Mesh& CreateMesh(const std::size_t& meshKey);
		Mesh& GetMesh(const std::size_t& meshKey)
		{
			return *_Meshes.at(meshKey).get();
		}

		void LoadTexture(const std::size_t textureKey, const std::string& fileName);
		bool GetTexture(const std::size_t& textureKey, Texture& outTex)
		{
			if (_Textures.find(textureKey) == _Textures.end()) {
				return false;
			}
			else {
				outTex = *_Textures.at(textureKey).get();
				return true;
			}
		}

		static Engine* PInstance;
		bool LoadResource();
		void LoadScene();
		void Start();
		void Cycle();

		InputManager& GetInput() const
		{
			return *pInput;
		}
		const std::vector<std::unique_ptr<GameObject>>& GetObjs() const
		{
			return _GameObjectes;
		}

		const static std::size_t QuadMesh;

		FORCEINLINE Camera& GetMainCam() { return _MainCam; }
		FORCEINLINE const Camera& GetMainCam() const { return _MainCam; }
	private:
		//size_t�� ������ ���, 0�� ��Ÿ���� ���� ũ�⸦ ��Ÿ���� ����Ѵٰ� �Ѵ�
		//unique_ptr�� ��ü�������� �ϳ��� �������� �Ҷ� ����Ѵ�. �ߺ� delete ����
		//unordered_map ������ ���� ���������� map���� ����, Ű���� ����Ҽ��� �ӵ��� ������, c# �� dictionary

		std::vector<std::unique_ptr<GameObject>> _GameObjectes;
		std::unordered_map<std::size_t, std::unique_ptr<Mesh>> _Meshes;
		std::unordered_map<std::size_t, std::unique_ptr<Texture>> _Textures;


		int _Frame = 60;
		float _DeltaTime = 0.f;
		float _ElapsedTime = 0.f;

		void Update();
		void ChangeScreenSize(const ScreenPoint& size) 
		{

		}

		SoftRenderer* pRenderer;
		InputManager* pInput;

		const static std::size_t BaseTexture;
		const static std::string TextureFileName;

		Camera _MainCam;


	};
}