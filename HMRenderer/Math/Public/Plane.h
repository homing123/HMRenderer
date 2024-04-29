#pragma once
#include "MathUtil.h"

namespace HM
{
	struct  Plane
	{
	public :
		Plane(const Vector3& normal, const Vector3& pos)
		{
			Normal = normal.GetNormal();
			if (pos == Vector3::Zero) 
			{
				D = 0;
			}
			else
			{
				D = -Normal.Dot(pos);
			}
		}
		Plane(const Vector4& vt4) 
		{
			Normal = vt4.GetVt3();
			D = vt4.W;
			Normalize();
		}

		

		Vector3 Normal;
		float D;

		void Normalize() 
		{
			float sizePow = Normal.SizePow();
			if (Math::CheckRangeSM(sizePow, 1)) {
				return;
			}
			//fast InvSqrt 알고리즘 알아보기
			float invsqrt = 1 / sqrt(sizePow);
			Normal *= invsqrt;
			D *= invsqrt;
		}
		FORCEINLINE constexpr float Distance(const Vector3& pos) const
		{
			return Normal.Dot(pos) + D;
		}
		FORCEINLINE constexpr float DistanceAbs(const Vector3& pos) const
		{
			return Math::Abs(Normal.Dot(pos) + D);
		}
		FORCEINLINE constexpr bool isUp(const Vector3& pos) const
		{
			return Distance(pos) > 0;
		}
	};
}