
#include "Texture.h"

const wchar_t ResourcePath[] = L"C:\\Users\\k9996\\Desktop\\Renderer\\HMRenderer\\Resource\\";

Texture::Texture(const wchar_t* fileName)
{
	_FileName = fileName;
	_Path = GetResourcePath(fileName);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wchar_t* path = GetResourcePath(fileName);

	Bitmap* image = Bitmap::FromFile(path);
	_Width = image->GetWidth();
	_Height = image->GetHeight();

	Gdiplus::Color* pColor = new Gdiplus::Color();
	for (UINT y = 0; y < _Height; y++)
	{
		for (UINT x = 0; x < _Width; x++)
		{
			image->GetPixel(x, _Height - 1 - y, pColor);
			_Buffer.push_back(LinearColor(pColor->GetR(), pColor->GetG(), pColor->GetB(), pColor->GetA()));
		}
	}
	delete pColor;
	delete image;
	int _BufferSize = _Buffer.size();

	LinearColor color = _Buffer[400];
	GdiplusShutdown(gdiplusToken);
}
void Texture::Release()
{
	_Width = 0;
	_Height = 0;
	_Buffer.clear();
	delete[] _FileName;
	delete[] _Path;
}
LinearColor Texture::GetPixel(const Vector2& uv) const
{
	int x = 0;
	int y = 0;
	if (uv.X == 1)
	{
		x = _Width - 1;
	}
	else
	{
		x = (int)(uv.X * _Width);
	}
	if (uv.Y == 1)
	{
		y = _Height - 1;
	}
	else
	{
		y = (int)(uv.Y * _Height);
	}
	return _Buffer[x + _Width * y];
}
wchar_t* Add(const wchar_t* str1, const wchar_t* str2)
{
	int size_1 = wcslen(str1);
	int size_2 = wcslen(str2) + 1;

	int size = size_1 + size_2;
	int i = 0;

	wchar_t* str = new wchar_t[size];
	for (i = 0; i < size_1; i++)
	{
		str[i] = str1[i];
	}

	for (int i = 0; i < size_2; i++)
	{
		str[i + size_1] = str2[i];
	}
	return str;
}
wchar_t* Texture::GetResourcePath(const wchar_t* fileName)
{
	return Add(ResourcePath, fileName);
}


