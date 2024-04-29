#pragma once

#include "MathUtil.h"

namespace HM
{
	struct Matrix4x4
	{
	public:
		Matrix4x4() = default;
		Matrix4x4(float _00, float _01, float _02, float _03, float _10, float _11, float _12, float _13, float _20, float _21, float _22, float _23, float _30, float _31, float _32, float _33)
		{
			_Array[0][0] = _00;
			_Array[0][1] = _01;
			_Array[0][2] = _02;
			_Array[0][3] = _03;
			_Array[1][0] = _10;
			_Array[1][1] = _11;
			_Array[1][2] = _12;
			_Array[1][3] = _13;
			_Array[2][0] = _20;
			_Array[2][1] = _21;
			_Array[2][2] = _22;
			_Array[2][3] = _23;
			_Array[3][0] = _30;
			_Array[3][1] = _31;
			_Array[3][2] = _32;
			_Array[3][3] = _33;
		}
		Matrix4x4(float _array[4][4])
		{
			_Array[0][0] = _array[0][0];
			_Array[0][1] = _array[0][1];
			_Array[0][2] = _array[0][2];
			_Array[0][3] = _array[0][3];
			_Array[1][0] = _array[1][0];
			_Array[1][1] = _array[1][1];
			_Array[1][2] = _array[1][2];
			_Array[1][3] = _array[1][3];
			_Array[2][0] = _array[2][0];
			_Array[2][1] = _array[2][1];
			_Array[2][2] = _array[2][2];
			_Array[2][3] = _array[2][3];
			_Array[3][0] = _array[3][0];
			_Array[3][1] = _array[3][1];
			_Array[3][2] = _array[3][2];
			_Array[3][3] = _array[3][3];
		}

		Matrix4x4(const Vector4& vt4_0, const Vector4& vt4_1, const Vector4& vt4_2, const Vector4& vt4_3)
		{
			_Array[0][0] = vt4_0.X;
			_Array[0][1] = vt4_1.X;
			_Array[0][2] = vt4_2.X;
			_Array[0][3] = vt4_3.X;
			_Array[1][0] = vt4_0.Y;
			_Array[1][1] = vt4_1.Y;
			_Array[1][2] = vt4_2.Y;
			_Array[1][3] = vt4_3.Y;
			_Array[2][0] = vt4_0.Z;
			_Array[2][1] = vt4_1.Z;
			_Array[2][2] = vt4_2.Z;
			_Array[2][3] = vt4_3.Z;
			_Array[3][0] = vt4_0.W;
			_Array[3][1] = vt4_1.W;
			_Array[3][2] = vt4_2.W;
			_Array[3][3] = vt4_3.W;
		}

		float _Array[4][4];

		FORCEINLINE void operator+(Matrix4x4& mat)
		{
			_Array[0][0] += mat._Array[0][0];
			_Array[0][1] += mat._Array[0][1];
			_Array[0][2] += mat._Array[0][2];
			_Array[0][3] += mat._Array[0][3];
			_Array[1][0] += mat._Array[1][0];
			_Array[1][1] += mat._Array[1][1];
			_Array[1][2] += mat._Array[1][2];
			_Array[1][3] += mat._Array[1][3];
			_Array[2][0] += mat._Array[2][0];
			_Array[2][1] += mat._Array[2][1];
			_Array[2][2] += mat._Array[2][2];
			_Array[2][3] += mat._Array[2][3];
			_Array[3][0] += mat._Array[3][0];
			_Array[3][1] += mat._Array[3][1];
			_Array[3][2] += mat._Array[3][2];
			_Array[3][3] += mat._Array[3][3];
		}

		FORCEINLINE void operator*(float scalar)
		{
			_Array[0][0] *= scalar;
			_Array[0][1] *= scalar;
			_Array[0][2] *= scalar;
			_Array[0][3] *= scalar;
			_Array[1][0] *= scalar;
			_Array[1][1] *= scalar;
			_Array[1][2] *= scalar;
			_Array[1][3] *= scalar;
			_Array[2][0] *= scalar;
			_Array[2][1] *= scalar;
			_Array[2][2] *= scalar;
			_Array[2][3] *= scalar;
			_Array[3][0] *= scalar;
			_Array[3][1] *= scalar;
			_Array[3][2] *= scalar;
			_Array[3][3] *= scalar;
		}

		FORCEINLINE Matrix4x4 operator*(Matrix4x4& mat)
		{
			float _array[4][4];
			_array[0][0] = _Array[0][0] * mat._Array[0][0] + _Array[0][1] * mat._Array[1][0] + _Array[0][2] * mat._Array[2][0] + _Array[0][3] * mat._Array[3][0];
			_array[0][1] = _Array[0][0] * mat._Array[0][1] + _Array[0][1] * mat._Array[1][1] + _Array[0][2] * mat._Array[2][1] + _Array[0][3] * mat._Array[3][1];
			_array[0][2] = _Array[0][0] * mat._Array[0][2] + _Array[0][1] * mat._Array[1][2] + _Array[0][2] * mat._Array[2][2] + _Array[0][3] * mat._Array[3][2];
			_array[0][3] = _Array[0][0] * mat._Array[0][3] + _Array[0][1] * mat._Array[1][3] + _Array[0][2] * mat._Array[2][3] + _Array[0][3] * mat._Array[3][3];

			_array[1][0] = _Array[1][0] * mat._Array[0][0] + _Array[1][1] * mat._Array[1][0] + _Array[1][2] * mat._Array[2][0] + _Array[1][3] * mat._Array[3][0];
			_array[1][1] = _Array[1][0] * mat._Array[0][1] + _Array[1][1] * mat._Array[1][1] + _Array[1][2] * mat._Array[2][1] + _Array[1][3] * mat._Array[3][1];
			_array[1][2] = _Array[1][0] * mat._Array[0][2] + _Array[1][1] * mat._Array[1][2] + _Array[1][2] * mat._Array[2][2] + _Array[1][3] * mat._Array[3][2];
			_array[1][3] = _Array[1][0] * mat._Array[0][3] + _Array[1][1] * mat._Array[1][3] + _Array[1][2] * mat._Array[2][3] + _Array[1][3] * mat._Array[3][3];

			_array[2][0] = _Array[2][0] * mat._Array[0][0] + _Array[2][1] * mat._Array[1][0] + _Array[2][2] * mat._Array[2][0] + _Array[2][3] * mat._Array[3][0];
			_array[2][1] = _Array[2][0] * mat._Array[0][1] + _Array[2][1] * mat._Array[1][1] + _Array[2][2] * mat._Array[2][1] + _Array[2][3] * mat._Array[3][1];
			_array[2][2] = _Array[2][0] * mat._Array[0][2] + _Array[2][1] * mat._Array[1][2] + _Array[2][2] * mat._Array[2][2] + _Array[2][3] * mat._Array[3][2];
			_array[2][3] = _Array[2][0] * mat._Array[0][3] + _Array[2][1] * mat._Array[1][3] + _Array[2][2] * mat._Array[2][3] + _Array[2][3] * mat._Array[3][3];

			_array[3][0] = _Array[3][0] * mat._Array[0][0] + _Array[3][1] * mat._Array[1][0] + _Array[3][2] * mat._Array[2][0] + _Array[3][3] * mat._Array[3][0];
			_array[3][1] = _Array[3][0] * mat._Array[0][1] + _Array[3][1] * mat._Array[1][1] + _Array[3][2] * mat._Array[2][1] + _Array[3][3] * mat._Array[3][1];
			_array[3][2] = _Array[3][0] * mat._Array[0][2] + _Array[3][1] * mat._Array[1][2] + _Array[3][2] * mat._Array[2][2] + _Array[3][3] * mat._Array[3][2];
			_array[3][3] = _Array[3][0] * mat._Array[0][3] + _Array[3][1] * mat._Array[1][3] + _Array[3][2] * mat._Array[2][3] + _Array[3][3] * mat._Array[3][3];

			return Matrix4x4(_array);
		}


		FORCEINLINE Vector4 operator*(Vector4& vt4)
		{
			return Vector4(_Array[0][0] * vt4.X + _Array[0][1] * vt4.Y + _Array[0][2] * vt4.Z + _Array[0][3] * vt4.W,
				_Array[1][0] * vt4.X + _Array[1][1] * vt4.Y + _Array[1][2] * vt4.Z + _Array[1][3] * vt4.W,
				_Array[2][0] * vt4.X + _Array[2][1] * vt4.Y + _Array[2][2] * vt4.Z + _Array[2][3] * vt4.W,
				_Array[3][0] * vt4.X + _Array[3][1] * vt4.Y + _Array[3][2] * vt4.Z + _Array[3][3] * vt4.W);
		}

		FORCEINLINE Vector4 operator*(const Vector4& vt4) const
		{
			return Vector4(_Array[0][0] * vt4.X + _Array[0][1] * vt4.Y + _Array[0][2] * vt4.Z + _Array[0][3] * vt4.W,
				_Array[1][0] * vt4.X + _Array[1][1] * vt4.Y + _Array[1][2] * vt4.Z + _Array[1][3] * vt4.W,
				_Array[2][0] * vt4.X + _Array[2][1] * vt4.Y + _Array[2][2] * vt4.Z + _Array[2][3] * vt4.W,
				_Array[3][0] * vt4.X + _Array[3][1] * vt4.Y + _Array[3][2] * vt4.Z + _Array[3][3] * vt4.W);
		}

		FORCEINLINE Vector3 operator*(Vector3& vt3)
		{
			return Vector3(_Array[0][0] * vt3.X + _Array[0][1] * vt3.Y + _Array[0][2] * vt3.Z + _Array[0][3],
				_Array[1][0] * vt3.X + _Array[1][1] * vt3.Y + _Array[1][2] * vt3.Z + _Array[1][3], 
				_Array[2][0] * vt3.X + _Array[2][1] * vt3.Y + _Array[2][2] * vt3.Z + _Array[2][3] );
		}
		FORCEINLINE Matrix4x4 GetTranspose()
		{
			return Matrix4x4(_Array[0][0], _Array[1][0], _Array[2][0], _Array[3][0],
				_Array[0][1], _Array[1][1], _Array[2][1], _Array[3][1],
				_Array[0][2], _Array[1][2], _Array[2][2], _Array[3][2], 
				_Array[0][3], _Array[1][3], _Array[2][3], _Array[3][3]);
		}

		/*FORCEINLINE float Det3D() {
			return _Array[0][0] * (_Array[1][1] * _Array[2][2] - _Array[1][2] * _Array[2][1]) - _Array[0][1] * (_Array[1][0] * _Array[2][2] - _Array[1][2] * _Array[2][0]) + _Array[0][2] * (_Array[1][0] * _Array[2][1] - _Array[1][1] * _Array[2][0]);
		}
		FORCEINLINE bool GetReverse3D(Matrix3x3& outMat) {
			float det = Det3D();
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
		}*/

	};
}