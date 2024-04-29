#pragma once
#include "EngineUtil.h"
#include <ole2.h>
#include <gdiplus.h>
#include <wchar.h>
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;

namespace HM
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const wchar_t* fileName);
		~Texture() {	}

		void Release();
		bool isInitialized() {
			return _Buffer.size() > 0;
		}
		UINT32 GetWidth() const { return _Width; }
		UINT32 GetHeight() const { return _Height; }
		UINT32 GetSize() const { return _Width * _Height; }
		LinearColor GetPixel(const Vector2& uv) const;


		const wchar_t* _FileName;
		const wchar_t* _Path;
	private:
		static constexpr BYTE _Channel = 4;
		std::vector<LinearColor> _Buffer;
		UINT32 _Width = 0;
		UINT32 _Height = 0;
		static wchar_t* GetResourcePath(const wchar_t* fileName);


	};
}