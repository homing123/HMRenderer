#pragma once

#include "MathUtil.h"

namespace HM
{
	struct Matrix3x3
	{
	public:
		Matrix3x3() = default;
		Matrix3x3(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22)
		{
			_Array[0][0] = _00;
			_Array[0][1] = _01;
			_Array[0][2] = _02;
			_Array[1][0] = _10;
			_Array[1][1] = _11;
			_Array[1][2] = _12;
			_Array[2][0] = _20;
			_Array[2][1] = _21;
			_Array[2][2] = _22;
		}
		Matrix3x3(float _array[3][3])
		{
			_Array[0][0] = _array[0][0];
			_Array[0][1] = _array[0][1];
			_Array[0][2] = _array[0][2];
			_Array[1][0] = _array[1][0];
			_Array[1][1] = _array[1][1];
			_Array[1][2] = _array[1][2];
			_Array[2][0] = _array[2][0];
			_Array[2][1] = _array[2][1];
			_Array[2][2] = _array[2][2];
		}

		Matrix3x3(Vector3& vt3_0, Vector3& vt3_1, Vector3& vt3_2)
		{
			_Array[0][0] = vt3_0.X;
			_Array[0][1] = vt3_1.X;
			_Array[0][2] = vt3_2.X;
			_Array[1][0] = vt3_0.Y;
			_Array[1][1] = vt3_1.Y;
			_Array[1][2] = vt3_2.Y;			
			_Array[2][0] = vt3_0.Z;
			_Array[2][1] = vt3_1.Z;
			_Array[2][2] = vt3_2.Z;
		}

		float _Array[3][3];

		FORCEINLINE void operator+(Matrix3x3& mat)
		{
			_Array[0][0] += mat._Array[0][0];
			_Array[0][1] += mat._Array[0][1];
			_Array[0][2] += mat._Array[0][2];
			_Array[1][0] += mat._Array[1][0];
			_Array[1][1] += mat._Array[1][1];
			_Array[1][2] += mat._Array[1][2];
			_Array[2][0] += mat._Array[2][0];
			_Array[2][1] += mat._Array[2][1];
			_Array[2][2] += mat._Array[2][2];
		}

		FORCEINLINE void operator*(float scalar)
		{
			_Array[0][0] *= scalar;
			_Array[0][1] *= scalar;
			_Array[0][2] *= scalar;
			_Array[1][0] *= scalar;
			_Array[1][1] *= scalar;
			_Array[1][2] *= scalar;
			_Array[2][0] *= scalar;
			_Array[2][1] *= scalar;
			_Array[2][2] *= scalar;

		}

		FORCEINLINE Matrix3x3 operator*(Matrix3x3& mat)
		{
			float _array[3][3];
			_array[0][0] = _Array[0][0] * mat._Array[0][0] + _Array[0][1] * mat._Array[1][0] + _Array[0][2] * mat._Array[2][0];
			_array[0][1] = _Array[0][0] * mat._Array[0][1] + _Array[0][1] * mat._Array[1][1] + _Array[0][2] * mat._Array[2][1];
			_array[0][2] = _Array[0][0] * mat._Array[0][2] + _Array[0][1] * mat._Array[1][2] + _Array[0][2] * mat._Array[2][2];

			_array[1][0] = _Array[1][0] * mat._Array[0][0] + _Array[1][1] * mat._Array[1][0] + _Array[1][2] * mat._Array[2][0];
			_array[1][1] = _Array[1][0] * mat._Array[0][1] + _Array[1][1] * mat._Array[1][1] + _Array[1][2] * mat._Array[2][1];
			_array[1][2] = _Array[1][0] * mat._Array[0][2] + _Array[1][1] * mat._Array[1][2] + _Array[1][2] * mat._Array[2][2];

			_array[2][0] = _Array[2][0] * mat._Array[0][0] + _Array[2][1] * mat._Array[1][0] + _Array[2][2] * mat._Array[2][0];
			_array[2][1] = _Array[2][0] * mat._Array[0][1] + _Array[2][1] * mat._Array[1][1] + _Array[2][2] * mat._Array[2][1];
			_array[2][2] = _Array[2][0] * mat._Array[0][2] + _Array[2][1] * mat._Array[1][2] + _Array[2][2] * mat._Array[2][2];


			return Matrix3x3(_array);
		}


		FORCEINLINE Vector3 operator*(Vector3& vt3)
		{
			return Vector3(_Array[0][0] * vt3.X + _Array[0][1] * vt3.Y + _Array[0][2] * vt3.Z,
				_Array[1][0] * vt3.X + _Array[1][1] * vt3.Y + _Array[1][2] * vt3.Z,
				_Array[2][0] * vt3.X + _Array[2][1] * vt3.Y + _Array[2][2] * vt3.Z);
		}

		FORCEINLINE Vector2 operator*(Vector2& vt2)
		{
			return Vector2(_Array[0][0] * vt2.X + _Array[0][1] * vt2.Y + _Array[0][2],
				_Array[1][0] * vt2.X + _Array[1][1] * vt2.Y + _Array[1][2]);
		}
		FORCEINLINE Matrix3x3 GetTranspose()
		{
			return Matrix3x3(_Array[0][0], _Array[1][0], _Array[2][0], _Array[0][1], _Array[1][1], _Array[2][1], _Array[0][2], _Array[1][2], _Array[2][2]);
		}

		FORCEINLINE float Det() {
			return _Array[0][0] * (_Array[1][1] * _Array[2][2] - _Array[1][2] * _Array[2][1]) - _Array[0][1] * (_Array[1][0] * _Array[2][2] - _Array[1][2] * _Array[2][0]) + _Array[0][2] * (_Array[1][0] * _Array[2][1] - _Array[1][1] * _Array[2][0]);
		}
		FORCEINLINE bool GetReverse(Matrix3x3& outMat) {
			float det = Det();
			if (det == 0)
			{
				outMat._Array[0][0] = _Array[0][0];
				outMat._Array[0][1] = _Array[0][1];
				outMat._Array[0][2] = _Array[0][2];
				outMat._Array[1][0] = _Array[1][0];
				outMat._Array[1][1] = _Array[1][1];
				outMat._Array[1][2] = _Array[1][2];
				outMat._Array[2][0] = _Array[2][0];
				outMat._Array[2][1] = _Array[2][1];
				outMat._Array[2][2] = _Array[2][2];
				return false;
			}
			else
			{
				float indet = 1 / det;
				outMat._Array[0][0] = (_Array[1][1] * _Array[2][2] - _Array[1][2] * _Array[2][1]) * indet;
				outMat._Array[0][1] = (_Array[0][2] * _Array[2][1] - _Array[0][1] * _Array[2][2]) * indet;
				outMat._Array[0][2] = (_Array[0][1] * _Array[1][2] - _Array[0][2] * _Array[1][1]) * indet;
				outMat._Array[1][0] = (_Array[1][2] * _Array[2][0] - _Array[1][0] * _Array[2][2]) * indet;
				outMat._Array[1][1] = (_Array[0][0] * _Array[2][2] - _Array[0][2] * _Array[2][0]) * indet;
				outMat._Array[1][2] = (_Array[0][2] * _Array[1][0] - _Array[0][0] * _Array[1][2]) * indet;
				outMat._Array[2][0] = (_Array[1][0] * _Array[2][1] - _Array[1][1] * _Array[2][0]) * indet;
				outMat._Array[2][1] = (_Array[0][1] * _Array[2][0] - _Array[0][0] * _Array[2][1]) * indet;
				outMat._Array[2][2] = (_Array[0][0] * _Array[1][1] - _Array[0][1] * _Array[1][0]) * indet;
				return true;
			}
		}

	};
}