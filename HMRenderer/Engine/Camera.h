#pragma once
#include "GameObject.h"
namespace HM
{
	class Camera {
	public:
		Camera() = default;
		~Camera() {}

		Transform& GetTransform() { return _Transform; }
		const Transform& GetTransform() const { return _Transform; }

		const ScreenPoint& GetViewPortSize() const { return _ViewPortSize; }

		void SetViewPortSize(const ScreenPoint& viewportSize) {
			_ViewPortSize = viewportSize;
		}

		FORCEINLINE Matrix3x3 GetViewMat2D() const;
		FORCEINLINE Matrix4x4 GetViewMat() const;

		FORCEINLINE Matrix4x4 GetPerspectiveMatrix() const;
		FORCEINLINE Matrix4x4 GetPerspectiveViewMatrix() const;


		void SetNearDis(const float neardis) 
		{
			_NearDis = neardis;
		}
		void SetFarDis(const float fardis) {
			_FarDis = fardis;
		}
		void SetFOV(const float fov) {
			_FOV = fov;
		}

		void GetNearFar(float& outNear, float& outFar) {
			outNear = _NearDis;
			outFar = _FarDis;
		}

	private:
		Transform _Transform;
		ScreenPoint _ViewPortSize;
		float _NearDis = 1.f;
		float _FarDis = 100.f;
		float _FOV = 90.f ;
	};

	FORCEINLINE Matrix3x3 Camera::GetViewMat2D() const
	{
		//카메라 회전 고려 x
		return Matrix3x3(1, 0, -_Transform.GetPos().X, 0, 1, -_Transform.GetPos().Y, 0, 0, 1);
	}
	FORCEINLINE Matrix4x4 Camera::GetViewMat() const
	{
		//스케일,회전,이동
		//TRS 인데 S는 항듣원
		//(TR)-1 = R-1 T-1
		//R역함수는 각 기저벡터의 내적이 0, 길이가 1인점을 이용해서 전치행렬이 역행렬인것을 검증가능
		//T는 이동의 역행렬은 반대방향으로 이동이라 이동좌표에 -곱해주면 됨

		//아래 카메라가 바라보는방향의 기준은 모든축의 각도값이 0도일때 기준
		//책 = 오른손좌표계 오른손좌표계에선 카메라가 -Z방향을 봐야 X,Y축의 방향이 맞음
		//내코드 = 왼손좌표계 카메라가 +Z방향을 봐야 X,Y축의 방향이 맞음
		//책에선 Y축을 180도 돌리지만 난 그럴필요없음

		Vector3 pos = _Transform.GetPos();
		Vector3 front = _Transform.GetFront();
		Vector3 right = _Transform.GetRight();
		Vector3 up = _Transform.GetUp();

		return Matrix4x4(right.X, right.Y, right.Z, -right.Dot(pos),
			up.X, up.Y, up.Z, -up.Dot(pos),
			front.X, front.Y, front.Z, -front.Dot(pos),
			0,0,0,1);
	}
	FORCEINLINE Matrix4x4 Camera::GetPerspectiveMatrix() const
	{	
		float InvFN = 1 / (_FarDis - _NearDis);
		float viewportRatio = _ViewPortSize.Y / _ViewPortSize.X;
		float Inv_fov = 1 / Math::Tan(_FOV * 0.5f);
		float k = (_FarDis + _NearDis) * InvFN;
		float l = (-2 * _FarDis * _NearDis) * InvFN;

		return Matrix4x4(
			Vector4(viewportRatio * Inv_fov, 0.f, 0.f, 0.f),
			Vector4(0.f, Inv_fov, 0.f, 0.f),
			Vector4(0.f,0.f,k,l),
			Vector4(0.f,0.f,1.f,0.f));
	}

	FORCEINLINE Matrix4x4 Camera::GetPerspectiveViewMatrix() const
	{
		Vector3 pos = _Transform.GetPos();
		Vector3 front = _Transform.GetFront();
		Vector3 right = _Transform.GetRight();
		Vector3 up = _Transform.GetUp();

		float InvFN = 1 / (_FarDis - _NearDis);
		float a = (float)_ViewPortSize.Y / _ViewPortSize.X;
		float d = 1 / Math::Tan(_FOV * 0.5f);
		float k = (_FarDis + _NearDis) * InvFN;
		float l = (-2 * _FarDis * _NearDis) * InvFN;
		float ad = a * d;
		
		return Matrix4x4(
			Vector4(right.X * ad, up.X * d, front.X * k, front.X),
			Vector4(right.Y * ad, up.Y * d, front.Y * k, front.Y),
			Vector4(right.Z * ad, up.Z * d, front.Z * k, front.Z),
			Vector4(-right.Dot(pos) * ad, -up.Dot(pos) * d, -front.Dot(pos) * k + l, -front.Dot(pos)));
	}
}