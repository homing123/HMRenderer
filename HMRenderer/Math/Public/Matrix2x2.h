#pragma once

#include "MathUtil.h"
#include "Vector2.h"

namespace HM
{
	struct Matrix2x2
	{
	public:
		Matrix2x2() = default;
		Matrix2x2(float _00, float _01, float _10, float _11)
		{
			_Array[0][0] = _00;
			_Array[0][1] = _01;
			_Array[1][0] = _10;
			_Array[1][1] = _11;
		}
		Matrix2x2(float _array[2][2])
		{
			_Array[0][0] = _array[0][0];
			_Array[0][1] = _array[0][1];
			_Array[1][0] = _array[1][0];
			_Array[1][1] = _array[1][1];
		}
		Matrix2x2(const Vector2& v1, const Vector2& v2)
		{
			_Array[0][0] = v1.X;
			_Array[0][1] = v2.X;
			_Array[1][0] = v1.Y;
			_Array[1][1] = v2.Y;
		}
		float _Array[2][2];

		FORCEINLINE void operator+(Matrix2x2& mat)
		{
			_Array[0][0] += mat._Array[0][0];
			_Array[0][1] += mat._Array[0][1];
			_Array[1][0] += mat._Array[1][0];
			_Array[1][0] += mat._Array[1][1];
		}

		FORCEINLINE void operator*(float scalar)
		{
			_Array[0][0] *= scalar;
			_Array[0][1] *= scalar;
			_Array[1][0] *= scalar;
			_Array[1][0] *= scalar;
		}

		FORCEINLINE Matrix2x2 operator*(Matrix2x2& mat)
		{
			float _array[2][2];
			_array[0][0] = _Array[0][0] * mat._Array[0][0] + _Array[0][1] * mat._Array[1][0];
			_array[0][1] = _Array[0][0] * mat._Array[0][1] + _Array[0][1] * mat._Array[1][1];
			_array[1][0] = _Array[1][0] * mat._Array[0][0] + _Array[1][1] * mat._Array[1][0];
			_array[1][1] = _Array[1][0] * mat._Array[0][1] + _Array[1][1] * mat._Array[1][1];

			return Matrix2x2(_array);
		}


		FORCEINLINE Vector2 operator*(Vector2& vt2)
		{
			return Vector2(_Array[0][0] * vt2.X + _Array[0][1] * vt2.Y, _Array[1][0] * vt2.X + _Array[1][1] * vt2.Y);
		}

		FORCEINLINE Matrix2x2 GetTranspose()
		{
			return Matrix2x2(_Array[0][0], _Array[1][0], _Array[0][1], _Array[1][1]);
		}


		FORCEINLINE float Det() 
		{
			return _Array[0][0] * _Array[1][1] - _Array[0][1] * _Array[1][0];
		}

		FORCEINLINE bool GetReverse(Matrix2x2& outMat)
		{
			float det = Det();
			if (det == 0)
			{
				outMat._Array[0][0] = _Array[0][0];
				outMat._Array[0][1] = _Array[0][1];
				outMat._Array[1][0] = _Array[1][0];
				outMat._Array[1][1] = _Array[1][1];
				return false;
			}
			else
			{
				float indet = 1 / det;

				outMat._Array[0][0] = _Array[1][1] * indet;
				outMat._Array[0][1] = -_Array[0][1] * indet;
				outMat._Array[1][0] = -_Array[1][0] * indet;
				outMat._Array[1][1] = _Array[0][0] * indet;
				return true;
			}
		}


	};
}