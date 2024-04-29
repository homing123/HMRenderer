#pragma once
#include "MathUtil.h"
namespace HM
{
	struct Vertex {
	public:
		Vertex() = default;
		Vertex(const Vector4& pos, const LinearColor& color, const Vector2& uv)
		{
			Pos = pos;
			Color = color;
			UV = uv;
		}
		Vertex(const Vector4& pos, const LinearColor& color)
		{
			Pos = pos;
			Color = color;
			UV = Vector2::Zero;
		}
		Vector4 Pos;
		LinearColor Color;
		Vector2 UV;

	};
}