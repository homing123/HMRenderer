#pragma once
#include "MathUtil.h"

namespace HM
{
	enum class BoundCheckResult : UINT32
	{
		Outside = 0,
		Intersect,
		Inside
	};
	struct Frustum
	{
	public:

		std::array<Plane, 6> Planes; // +y,-y,+x,-x,+z,-z ¼ø¼­

		FORCEINLINE constexpr BoundCheckResult CheckBound(const Vector3& pos) const
		{
			for (const auto& p : Planes)
			{
				if (p.isUp(pos))
				{
					return BoundCheckResult::Outside;
				}
				else if (Math::CheckRangeSM(p.Distance(pos), 0.f))
				{
					return BoundCheckResult::Intersect;
				}
			}
			return BoundCheckResult::Inside;

		}
		
	};
}