#pragma once

#include "EngineUtil.h"
#include "GameObject.h"
#include "Engine.h"

enum class DrawMode
{
	Basic,
	Wireframe,
	DepthBuffer,
};


class SoftRenderer
{
public:
	SoftRenderer() = default;
	SoftRenderer(const ScreenPoint& psize, const HWND& hwnd);

	void ChangeBGColor(const Color32& color);
	void DrawPoint(const ScreenPoint& screenPos, const Color32& color);
	void DrawPoint(const Vector2& worldPos, const Color32& color);

	void Render();
	void SetScreenSize(const ScreenPoint& screensize) {
		_ScreenSize = screensize;
	}
private:
	void Init();

	FORCEINLINE int GetScreenBufferIdx(const ScreenPoint& screenPos)const;
	FORCEINLINE bool IsInScreen(const ScreenPoint& screenPos)const;
	void SetPixel(const ScreenPoint& screenPos, const Color32& color);
	void SetPixel(const Vector2& worldPos, const Color32& color);
	void SetPixelDepth(const ScreenPoint& screenPos, const Color32& color, const float depth);

	void DrawBG();
	void DepthInit();
	void DrawSegment(const Segment2D& segment, const Color32& color);
	bool SegmentClip(Segment2D& segment, const Vector2& min, const Vector2& max);
	void DrawTriangle2D(Vertex2D& p1, Vertex2D& p2, Vertex2D& p3);
	void DrawTriangle2D(Vertex2D& p1, Vertex2D& p2, Vertex2D& p3, const Texture& texture);
	void DrawMesh2D(const GameObject& obj);
	void DrawTriangle(Vertex& p1, Vertex& p2, Vertex& p3);
	void DrawTriangle(Vertex& p1, Vertex& p2, Vertex& p3, const Texture& texture);
	void DrawMesh(const GameObject& obj);

	DrawMode _DrawMode = DrawMode::DepthBuffer;
	bool _BackSpaceCulling = true;
	Color32* _ColorBuffer;
	Color32 _BGColor;
	float* _DepthBuffer;

	ScreenPoint _ScreenSize;
	HWND _hWnd;

	HBITMAP _Bitmap, _BitmapOld;

	HDC _ScreenDC, _MemoryDC;
};

FORCEINLINE int SoftRenderer::GetScreenBufferIdx(const ScreenPoint& screenPos) const
{
	return screenPos.Y * _ScreenSize.X + screenPos.X;
}
FORCEINLINE bool SoftRenderer::IsInScreen(const ScreenPoint& screenPos) const
{
	return !(screenPos.X < 0 || screenPos.X >= _ScreenSize.X || screenPos.Y < 0 || screenPos.Y >= _ScreenSize.Y);
}

//오른쪽 위 픽셀을 그림