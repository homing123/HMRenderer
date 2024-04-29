#pragma once

#include "MathUtil.h"

namespace HM
{
	struct Vector3
	{
		FORCEINLINE constexpr Vector3() = default;
		FORCEINLINE explicit constexpr Vector3(int x, int y, int z) :X((float)x), Y((float)y), Z((float)z) {}
		FORCEINLINE explicit constexpr Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
	public:
		static constexpr BYTE Dimension = 3;

		union
		{
			struct {
				float X, Y, Z;
			};

			std::array<float, Dimension> Scalars = { 0.f,0.f,0.f };
		};

		FORCEINLINE constexpr bool operator==(const Vector3& vt3) const
		{
			return X == vt3.X && Y == vt3.Y && Z == vt3.Z;
		}
		FORCEINLINE constexpr Vector3 operator*(float scalar) const
		{
			return Vector3(X * scalar, Y * scalar, Z * scalar);
		}
		FORCEINLINE constexpr Vector3 operator/(float scalar) const
		{
			return Vector3(X / scalar, Y / scalar, Z / scalar);
		}
		FORCEINLINE constexpr void operator*=(float scalar)
		{
			X *= scalar;
			Y *= scalar;
			Z *= scalar;
		}
		FORCEINLINE constexpr Vector3 operator+(const Vector3& vt3) const
		{
			return Vector3(X + vt3.X, Y + vt3.Y, Z + vt3.Z);
		}
		FORCEINLINE constexpr void operator+=(const Vector3& vt3)
		{
			X += vt3.X;
			Y += vt3.Y;
			Z += vt3.Z;
		}
		FORCEINLINE constexpr Vector3 operator-(const Vector3& vt3) const
		{
			return Vector3(X - vt3.X, Y - vt3.Y, Z - vt3.Z);
		}
		FORCEINLINE constexpr void operator-=(const Vector3& vt3)
		{
			X -= vt3.X;
			Y -= vt3.Y;
			Z -= vt3.Z;
		}
		
		FORCEINLINE float SizePow() const
		{
			return X * X + Y * Y + Z * Z;
		}

		FORCEINLINE float Size() const
		{
			return sqrtf(X * X + Y * Y + Z * Z);
		}

		FORCEINLINE constexpr float Dot(const Vector3& vt3) const
		{
			return X * vt3.X + Y * vt3.Y + Z * vt3.Z;
		}
		FORCEINLINE constexpr Vector3 CrossProduct(const Vector3& vt3) const {
			return Vector3(Y * vt3.Z - Z * vt3.Y, Z * vt3.X - X * vt3.Z, X * vt3.Y - Y * vt3.X);
		}

		//vt3_1 * (vt3_2 * vt3_3)
		static Vector3 VTripleProduct(const Vector3& vt3_1, const Vector3& vt3_2, const Vector3& vt3_3) 
		{
			return(vt3_2 * vt3_1.Dot(vt3_3) - vt3_3 * vt3_1.Dot(vt3_2));
		}

		FORCEINLINE Vector3 GetNormal() const
		{
			float size = Size();
			return Vector3(X / size, Y / size, Z / size);
		}
		FORCEINLINE void Normalize()
		{
			float size = Size();
			X = X / size;
			Y = Y / size;
			Z = Z / size;
		}

		const static Vector3 Zero;
		const static Vector3 One;
	};




}

