#pragma once

#include "MathUtil.h"
namespace HM 
{
	struct  ScreenPoint
	{
	public:
		FORCEINLINE constexpr ScreenPoint() = default;
		FORCEINLINE explicit constexpr ScreenPoint(int x, int y) : X(x), Y(y) {}
		FORCEINLINE explicit constexpr ScreenPoint(float x, float y) : X((int)x), Y((int)y) {}

		FORCEINLINE constexpr int GetIndex(const int width) const
		{
			return Y * width + X;
		}
		int X = 0;
		int Y = 0;

		FORCEINLINE constexpr void Move(const int x, const int y) {
			X += x;
			Y += y;
		}
		FORCEINLINE constexpr void MoveTo(const int x, const int y) {
			X = x;
			Y = y;
		}
		FORCEINLINE static constexpr ScreenPoint ToScreenCoordinate(const ScreenPoint& screenSize, const Vector2 vt2) 
		{
			return ScreenPoint(vt2.X + screenSize.X * 0.5f, -vt2.Y + screenSize.Y * 0.5f);
		}
		FORCEINLINE constexpr Vector2 ToCartesianCoordinate(const ScreenPoint& screenSize)
		{
			return Vector2(X - screenSize.X * 0.5f, screenSize.Y * 0.5f - Y);
		}
	};
}