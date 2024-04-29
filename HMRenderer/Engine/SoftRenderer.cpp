#include "SoftRenderer.h"


const Color32 Color32::Error(255, 0, 255, 255);
const Color32 Color32::White(255, 255, 255, 255);
const Color32 Color32::Black(0, 0, 0, 255);
const Color32 Color32::Gray(128, 128, 128, 255);
const Color32 Color32::Red(255, 0, 0, 255);
const Color32 Color32::Green(0, 255, 0, 255);
const Color32 Color32::Blue(0, 0, 255, 255);
const Color32 Color32::Yellow(255, 255, 0, 255);
const Color32 Color32::Cyan(0, 255, 255, 255);
const Color32 Color32::Magenta(255, 0, 255, 255);

const Vector2 Vector2::Zero(0, 0);
const Vector2 Vector2::One(1, 1);
const Vector3 Vector3::Zero(0, 0, 0);
const Vector3 Vector3::One(1, 1, 1);

//std::mt19937 Math::Generator((unsigned int)time(NULL));
std::mt19937 Math::Generator(0);

SoftRenderer::SoftRenderer(const ScreenPoint& size, const HWND& hwnd)
{

	_ScreenSize = size;
	_hWnd = hwnd;

	_ColorBuffer = new Color32[_ScreenSize.X * _ScreenSize.Y];
	_DepthBuffer = new float[_ScreenSize.X * _ScreenSize.Y];

	Init();
}

void SoftRenderer::Init()
{
	_ScreenDC = GetDC(_hWnd);

	//ScreenDC와 호환지만 화면에 그려지지않는 다른 DC를 만드는 함수
	_MemoryDC = CreateCompatibleDC(_ScreenDC);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = _ScreenSize.X;
	bmi.bmiHeader.biHeight = -_ScreenSize.Y; // 음수로 설정하여 픽셀 배열이 상단에서 시작하도록 함
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // 32비트 컬러 사용
	bmi.bmiHeader.biCompression = BI_RGB;


	_Bitmap = CreateDIBSection(_MemoryDC, &bmi, DIB_RGB_COLORS, (void**)&_ColorBuffer, NULL, 0);
	if (_Bitmap == NULL) {
		// 생성 실패 처리
		return;
	}

	_BitmapOld = (HBITMAP)SelectObject(_MemoryDC, _Bitmap);
	ChangeBGColor(Color32(255, 255, 255, 255));

}

void SoftRenderer::ChangeBGColor(const Color32& color)
{
	_BGColor = color;
}

void SoftRenderer::DrawPoint(const ScreenPoint& screenPos, const Color32& color)
{
	SetPixel(screenPos, color);
}
void SoftRenderer::DrawPoint(const Vector2& wolrdPos, const Color32& color)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(_ScreenSize, wolrdPos), color);
}


void SoftRenderer::Render()
{
	DrawBG();
	DepthInit();
	const auto& objs = Engine::PInstance->GetObjs();
	int size = objs.size();
	for (int i = 0; i < size; i++)
	{
		DrawMesh(*objs[i].get());
	}

	// 그리기 작업 수행
	BitBlt(_ScreenDC, 0, 0, _ScreenSize.X, _ScreenSize.Y, _MemoryDC, 0, 0, SRCCOPY); //복사 및 출력

}

void SoftRenderer::DrawBG()
{
	for (int i = 0; i < _ScreenSize.X * _ScreenSize.Y; i++)
	{
		_ColorBuffer[i] = _BGColor;
	}
}

void SoftRenderer::DepthInit()
{
	for (int i = 0; i < _ScreenSize.X * _ScreenSize.Y; i++)
	{
		_DepthBuffer[i] = 1.f;
	}
}
void SoftRenderer::SetPixelDepth(const ScreenPoint& screenPos, const Color32& color, const float depth)
{

	if (IsInScreen(screenPos))
	{
		int idx = GetScreenBufferIdx(screenPos);
		float buffervalue = _DepthBuffer[idx];
		int a = 0;
		if (_DepthBuffer[idx] > depth) 
		{
			_DepthBuffer[idx] = depth;
			_ColorBuffer[idx] = color;
		}
	}
}

void SoftRenderer::SetPixel(const ScreenPoint& screenPos, const Color32& color)
{
	if (IsInScreen(screenPos))
	{
		_ColorBuffer[GetScreenBufferIdx(screenPos)] = color;
	}
}

void SoftRenderer::SetPixel(const Vector2& worldPos, const Color32& color)
{
	ScreenPoint screenPos = ScreenPoint::ToScreenCoordinate(_ScreenSize, worldPos);
	if (IsInScreen(screenPos))
	{
		_ColorBuffer[GetScreenBufferIdx(screenPos)] = color;
	}
}
bool SoftRenderer::SegmentClip(Segment2D& segment, const Vector2& min, const Vector2& max)
{
	if ((segment._Pos.X < min.X && segment._Pos2.X < min.X)
		|| (segment._Pos.X >= max.X && segment._Pos2.X >= max.X)
		|| (segment._Pos.Y < min.Y && segment._Pos2.Y < min.Y)
		|| (segment._Pos.Y >= max.Y && segment._Pos2.Y >= max.Y))
	{
		return false;
	}

	if (segment._Pos.X < min.X)
	{
		segment.GetVt2_X(min.X, segment._Pos);
	}
	else if (segment._Pos.X > max.X)
	{
		segment.GetVt2_X(max.X, segment._Pos);
	}

	if (segment._Pos.Y < min.Y)
	{
		segment.GetVt2_Y(min.Y, segment._Pos);
	}
	else if (segment._Pos.Y > max.Y)
	{
		segment.GetVt2_Y(max.Y, segment._Pos);
	}

	if (segment._Pos2.X < min.X)
	{
		segment.GetVt2_X(min.X, segment._Pos2);
	}
	else if (segment._Pos2.X > max.X)
	{
		segment.GetVt2_X(max.X, segment._Pos2);
	}

	if (segment._Pos2.Y < min.Y)
	{
		segment.GetVt2_Y(min.Y, segment._Pos2);
	}
	else if (segment._Pos2.Y > max.Y)
	{
		segment.GetVt2_Y(max.Y, segment._Pos2);
	}

	return !(segment._Pos.X < min.X || segment._Pos2.X < min.X
		|| segment._Pos.X > max.X || segment._Pos2.X > max.X
		|| segment._Pos.Y < min.Y || segment._Pos2.Y < min.Y
		|| segment._Pos.Y > max.Y || segment._Pos2.Y > max.Y);
}

void SoftRenderer::DrawSegment(const Segment2D& segment, const Color32& color)
{
	Segment2D clipSegment = Segment2D();
	if (segment._Pos.X > segment._Pos2.X) 
	{
		clipSegment = Segment2D(segment._Pos2, segment._Pos);
	}
	else  
	{
		clipSegment = Segment2D(segment._Pos, segment._Pos2);
	}

	Vector2 screenMax(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);
	Vector2 screenMin(-screenMax.X, -screenMax.Y);

	if (SegmentClip(clipSegment, screenMin, screenMax) == false)
	{
		return;
	}
	Vector2 clipvt2 = clipSegment._Pos;
	Vector2 originvt2 = segment._Pos;

	Vector2 curPos(clipSegment._Pos.X, clipSegment._Pos.Y);
	int dx = clipSegment._Pos.X > clipSegment._Pos2.X ? -1 : 1;
	if ((int)clipSegment._Pos.X == (int)clipSegment._Pos2.X)
	{
		dx = 0;
	}

	int dy = clipSegment._Pos.Y > clipSegment._Pos2.Y ? -1 : 1;
	if ((int)clipSegment._Pos.Y == (int)clipSegment._Pos2.Y)
	{
		dy = 0;
	}

	int limit = 0;

	if (dx == 0)
	{
		if (dy == 0)
		{
			SetPixel(curPos, color);
		}
		else
		{
			while (true)
			{
				SetPixel(curPos, color);
				curPos.Y += dy;
				if ((clipSegment._Pos2.Y - curPos.Y) * dy < 0)
				{
					break;
				}
				limit++;
				if (limit > 10000) {
					break;
				}
			}
		}
	}
	else
	{
		if (dy == 0)
		{
			while (true)
			{
				SetPixel(curPos, color);
				curPos.X += dx;
				if ((clipSegment._Pos2.X - curPos.X) * dx < 0)
				{
					break;
				}

				limit++;
				if (limit > 10000) {
					break;
				}
			}
		}
		else
		{
			float disx = abs(clipSegment._Pos.X - clipSegment._Pos2.X);
			float disy = abs(clipSegment._Pos.Y - clipSegment._Pos2.Y);
			int curw = 0;
			int curh = 0;
			//curh < curw 면 x만증가 else 면 xy둘다 증가
			if (disx >= disy)
			{
				//x 한칸에 y두칸이상 가는경우 없음
				//x기준 계산
				curw = 2;
				curh = 1;

				while (true)
				{
					SetPixel(curPos, color);
					if (curh * disy >= curw * disx)
					{
						curw += 2;
						curPos.Y += dy;
					}
					curh += 2;
					curPos.X += dx;

					if ((clipSegment._Pos2.X - curPos.X) * dx < 0)
					{
						break;
					}

					limit++;
					if (limit > 10000) {
						break;
					}
				}
			}
			else
			{
				curw = 1;
				curh = 2;

				while (true)
				{
					SetPixel(curPos, color);
					if (curw * disx >= curh * disy)
					{
						curh += 2;
						curPos.X += dx;
					}
					curw += 2;
					curPos.Y += dy;

					if ((clipSegment._Pos2.Y - curPos.Y) * dy < 0)
					{
						break;
					}

					limit++;
					if (limit > 10000) {
						break;
					}
				}
			}
		}
	}
}

#pragma region 3D
void SoftRenderer::DrawMesh(const GameObject& obj)
{
	if (obj.isVisible() == false)
	{
		return;
	}

	auto meshKey = obj.GetMeshKey();
	auto textureKey = obj.GetTextureKey();

	const Mesh& mesh = Engine::PInstance->GetMesh(obj.GetMeshKey());
	const Mesh* pMesh = &Engine::PInstance->GetMesh(obj.GetMeshKey());;
	const Transform& transform = obj.GetTransform();

	Texture texture;
	bool isTexture = Engine::PInstance->GetTexture(obj.GetTextureKey(), texture);

	std::vector<Vector3> vertices = mesh.GetVertices();
	std::vector<size_t> indices = mesh.GetIndices();
	std::vector<LinearColor> colors = mesh.GetColors();
	std::vector<Vector2> uvs = mesh.GetUVs();

	Matrix4x4 WorldMat = obj.GetTransform().GetModelingMatrix();
	Matrix4x4 FinalMat = Engine::PInstance->GetMainCam().GetPerspectiveViewMatrix() * WorldMat;

	float camNear, camFar;
	Engine::PInstance->GetMainCam().GetNearFar(camNear, camFar);
	float inv_CamDis = 1 / (camFar - camNear);

	Transform& T_Cam = Engine::PInstance->GetMainCam().GetTransform();
	Vector3 camPos = T_Cam.GetPos();
	Vector3 camFront = T_Cam.GetFront();
	int indexSize = indices.size();
	switch (_DrawMode) 
	{
		case DrawMode::Wireframe:
			for (int i = 0; i < indexSize; i += 3)
			{		
				int idx_1 = indices[i];
				int idx_2 = indices[i + 1];
				int idx_3 = indices[i + 2];

				Vector4 vt4_1 = FinalMat * Vector4(vertices[idx_1], true);
				Vector4 vt4_2 = FinalMat * Vector4(vertices[idx_2], true);
				Vector4 vt4_3 = FinalMat * Vector4(vertices[idx_3], true);

				vt4_1.MulXYZ(1 / vt4_1.W);
				vt4_2.MulXYZ(1 / vt4_2.W);
				vt4_3.MulXYZ(1 / vt4_3.W);

				if (_BackSpaceCulling)
				{
					Vector3 vt3_1 = vt4_1.GetVt3();
					Vector3 vt3_2 = vt4_2.GetVt3();
					Vector3 vt3_3 = vt4_3.GetVt3();

					Vector3 vt3_12 = vt3_2 - vt3_1;
					Vector3 vt3_13 = vt3_3 - vt3_1;
					Vector3 z = Vector3(0, 0, 1);
					float dotValue = z.Dot(vt3_12.CrossProduct(vt3_13));
					if (z.Dot(vt3_12.CrossProduct(vt3_13)) >= 0)
					{
						continue;
					}
				}
				vt4_1.X *= _ScreenSize.X * 0.5f;
				vt4_1.Y *= _ScreenSize.Y * 0.5f;
				vt4_2.X *= _ScreenSize.X * 0.5f;
				vt4_2.Y *= _ScreenSize.Y * 0.5f;
				vt4_3.X *= _ScreenSize.X * 0.5f;
				vt4_3.Y *= _ScreenSize.Y * 0.5f;

				Vector2 vt2_1 = vt4_1.GetVt2();
				Vector2 vt2_2 = vt4_2.GetVt2();
				Vector2 vt2_3 = vt4_3.GetVt2();

				DrawSegment(Segment2D(vt2_1, vt2_2), LinearColor::Gray.ToColor32());
				DrawSegment(Segment2D(vt2_2, vt2_3), LinearColor::Gray.ToColor32());
				DrawSegment(Segment2D(vt2_1, vt2_3), LinearColor::Gray.ToColor32());
				//선이 두갈래로 나오는현상 = 같은두점인데 긋는 방향이 달라서 그럼 
			}
			break;
		case DrawMode::Basic:
			for (int i = 0; i < indexSize; i += 3)
			{
				int idx_1 = indices[i];
				int idx_2 = indices[i + 1];
				int idx_3 = indices[i + 2];

				Vector4 vt4_1 = FinalMat * Vector4(vertices[idx_1], true);
				Vector4 vt4_2 = FinalMat * Vector4(vertices[idx_2], true);
				Vector4 vt4_3 = FinalMat * Vector4(vertices[idx_3], true);

				vt4_1.MulXYZ(1 / vt4_1.W);
				vt4_2.MulXYZ(1 / vt4_2.W);
				vt4_3.MulXYZ(1 / vt4_3.W);

				if (_BackSpaceCulling)
				{
					Vector3 vt3_1 = vt4_1.GetVt3();
					Vector3 vt3_2 = vt4_2.GetVt3();
					Vector3 vt3_3 = vt4_3.GetVt3();

					Vector3 vt3_12 = vt3_2 - vt3_1;
					Vector3 vt3_13 = vt3_3 - vt3_1;
					Vector3 z = Vector3(0, 0, 1);
					float dotValue = z.Dot(vt3_12.CrossProduct(vt3_13));
					if (z.Dot(vt3_12.CrossProduct(vt3_13)) >= 0)
					{
						continue;
					}
				}
				vt4_1.X *= _ScreenSize.X * 0.5f;
				vt4_1.Y *= _ScreenSize.Y * 0.5f;
				vt4_2.X *= _ScreenSize.X * 0.5f;
				vt4_2.Y *= _ScreenSize.Y * 0.5f;
				vt4_3.X *= _ScreenSize.X * 0.5f;
				vt4_3.Y *= _ScreenSize.Y * 0.5f;

				vt4_1.Z = (((vt4_1.W - camNear) * inv_CamDis) * 2 - 1);
				vt4_2.Z = (((vt4_2.W - camNear) * inv_CamDis) * 2 - 1);
				vt4_3.Z = (((vt4_3.W - camNear) * inv_CamDis) * 2 - 1);

				Vertex v1(vt4_1, colors[idx_1], uvs[idx_1]);
				Vertex v2(vt4_2, colors[idx_2], uvs[idx_2]);
				Vertex v3(vt4_3, colors[idx_3], uvs[idx_3]);

				if (isTexture) 
				{
					DrawTriangle(v1, v2, v3, texture);
				}
				else
				{
					DrawTriangle(v1, v2, v3);
				}
				//선이 두갈래로 나오는현상 = 같은두점인데 긋는 방향이 달라서 그럼 
			}
			break;
		case DrawMode::DepthBuffer:

			for (int i = 0; i < indexSize; i += 3)
			{
				int idx_1 = indices[i];
				int idx_2 = indices[i + 1];
				int idx_3 = indices[i + 2];

				Vector4 vt4_1 = FinalMat * Vector4(vertices[idx_1], true);
				Vector4 vt4_2 = FinalMat * Vector4(vertices[idx_2], true);
				Vector4 vt4_3 = FinalMat * Vector4(vertices[idx_3], true);

				vt4_1.MulXYZ(1 / vt4_1.W);
				vt4_2.MulXYZ(1 / vt4_2.W);
				vt4_3.MulXYZ(1 / vt4_3.W);

				if (_BackSpaceCulling)
				{
					Vector3 vt3_1 = vt4_1.GetVt3();
					Vector3 vt3_2 = vt4_2.GetVt3();
					Vector3 vt3_3 = vt4_3.GetVt3();

					Vector3 vt3_12 = vt3_2 - vt3_1;
					Vector3 vt3_13 = vt3_3 - vt3_1;
					Vector3 z = Vector3(0, 0, 1);
					float dotValue = z.Dot(vt3_12.CrossProduct(vt3_13));
					if (z.Dot(vt3_12.CrossProduct(vt3_13)) >= 0)
					{
						continue;
					}
				}
				vt4_1.X *= _ScreenSize.X * 0.5f;
				vt4_1.Y *= _ScreenSize.Y * 0.5f;
				vt4_2.X *= _ScreenSize.X * 0.5f;
				vt4_2.Y *= _ScreenSize.Y * 0.5f;
				vt4_3.X *= _ScreenSize.X * 0.5f;
				vt4_3.Y *= _ScreenSize.Y * 0.5f;

				vt4_1.Z = (vt4_1.W - camNear) * inv_CamDis;
				vt4_2.Z = (vt4_2.W - camNear) * inv_CamDis;
				vt4_3.Z = (vt4_3.W - camNear) * inv_CamDis;

				Vertex v1(vt4_1, LinearColor::White * vt4_1.Z, uvs[idx_1]);
				Vertex v2(vt4_2, LinearColor::White * vt4_2.Z, uvs[idx_2]);
				Vertex v3(vt4_3, LinearColor::White * vt4_3.Z, uvs[idx_3]);
				DrawTriangle(v1, v2, v3);			
				//선이 두갈래로 나오는현상 = 같은두점인데 긋는 방향이 달라서 그럼 
			}
			break;
	}
}

/// oneminust가 계산으로는 0인데 s와 t의 float값 한계로인해 0이안되고 남는경우가 존재 이로인해 일직선이 제대로 안그려지는 경우 있음 => scanline 으로 해보기
void SoftRenderer::DrawTriangle(Vertex& p1, Vertex& p2, Vertex& p3)
{
	Vector2 vt2_1 = Vector2(p1.Pos.X, p1.Pos.Y);
	Vector2 vt2_2 = Vector2(p2.Pos.X, p2.Pos.Y);
	Vector2 vt2_3 = Vector2(p3.Pos.X, p3.Pos.Y);

	Vector2 min = Vector2(Math::Min3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Min3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));
	Vector2 max = Vector2(Math::Max3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Max3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));

	ScreenPoint minPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, min);
	ScreenPoint maxPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, max);

	Vector2 u(vt2_1 - vt2_3);
	Vector2 v(vt2_2 - vt2_3);
	float uDotv = u.Dot(v);
	float vDotv = v.Dot(v);
	float uDotu = u.Dot(u);
	float denominator = uDotv * uDotv - vDotv * uDotu;

	float inv_w1 = 1 / p1.Pos.W;
	float inv_w2 = 1 / p2.Pos.W;
	float inv_w3 = 1 / p3.Pos.W;

	if (denominator == 0)
	{
		return;
	}
	float indenominator = 1 / denominator;

	for (int y = maxPoint.Y; y <= minPoint.Y; y++)
	{
		for (int x = minPoint.X; x <= maxPoint.X; x++)
		{
			ScreenPoint fragment = ScreenPoint(x, y);
			Vector2 w = fragment.ToCartesianCoordinate(_ScreenSize) - vt2_3;

			float wDotu = w.Dot(u);
			float wDotv = w.Dot(v);

			float s = (wDotv * uDotv - wDotu * vDotv) * indenominator;
			float t = (wDotu * uDotv - wDotv * uDotu) * indenominator;

			float oneMinusST = 1 - s - t;
			if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && oneMinusST >= 0 && oneMinusST <= 1) 
			{
				float inv_total = 1 / (s * inv_w1 + t * inv_w2 + oneMinusST * inv_w3);
				float depth = p1.Pos.Z * s + p2.Pos.Z * t + p3.Pos.Z * oneMinusST;
				LinearColor color = (p1.Color * s * inv_w1 + p2.Color * t * inv_w2 + p3.Color * oneMinusST * inv_w3) * inv_total;
				SetPixelDepth(fragment, color.ToColor32(), depth);
			}
		}
	}
}
void SoftRenderer::DrawTriangle(Vertex& p1, Vertex& p2, Vertex& p3, const Texture& texture)
{
	Vector2 vt2_1 = Vector2(p1.Pos.X, p1.Pos.Y);
	Vector2 vt2_2 = Vector2(p2.Pos.X, p2.Pos.Y);
	Vector2 vt2_3 = Vector2(p3.Pos.X, p3.Pos.Y);

	Vector2 min = Vector2(Math::Min3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Min3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));
	Vector2 max = Vector2(Math::Max3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Max3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));

	ScreenPoint minPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, min);
	ScreenPoint maxPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, max);

	Vector2 u(vt2_1 - vt2_3);
	Vector2 v(vt2_2 - vt2_3);
	float uDotv = u.Dot(v);
	float vDotv = v.Dot(v);
	float uDotu = u.Dot(u);
	float denominator = uDotv * uDotv - vDotv * uDotu;
	if (denominator == 0)
	{
		return;
	}
	float indenominator = 1 / denominator;

	float inv_w1 = 1 / p1.Pos.W;
	float inv_w2 = 1 / p2.Pos.W; 
	float inv_w3 = 1 / p3.Pos.W;

	for (int y = maxPoint.Y; y <= minPoint.Y; y++)
	{
		for (int x = minPoint.X; x <= maxPoint.X; x++)
		{
			ScreenPoint fragment = ScreenPoint(x, y);
			Vector2 w = fragment.ToCartesianCoordinate(_ScreenSize) - vt2_3;

			float wDotu = w.Dot(u);
			float wDotv = w.Dot(v);

			float s = (wDotv * uDotv - wDotu * vDotv) * indenominator;
			float t = (wDotu * uDotv - wDotv * uDotu) * indenominator;
			float oneMinusST = 1 - s - t;
			if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && oneMinusST >= 0 && oneMinusST <= 1)
			{
				float inv_total = 1 / (s * inv_w1 + t * inv_w2 + oneMinusST * inv_w3);
				float depth = p1.Pos.Z * s + p2.Pos.Z * t + p3.Pos.Z * oneMinusST;
				Vector2 uv = (p1.UV * s * inv_w1 + p2.UV * t * inv_w2 + p3.UV * oneMinusST * inv_w3) * inv_total;
				SetPixelDepth(fragment, texture.GetPixel(uv).ToColor32(), depth);
			}
		}
	}
	int b = 0;
}
#pragma endregion
#pragma region 2D

void SoftRenderer::DrawMesh2D(const GameObject& obj)
{
	if (obj.isVisible() == false)
	{
		return;
	}

	auto meshKey = obj.GetMeshKey();
	auto textureKey = obj.GetTextureKey();

	const Mesh& mesh = Engine::PInstance->GetMesh(obj.GetMeshKey());
	const Mesh* pMesh = &Engine::PInstance->GetMesh(obj.GetMeshKey());;
	const Transform& transform = obj.GetTransform();

	Texture texture;
	bool isTexture = Engine::PInstance->GetTexture(obj.GetTextureKey(), texture);

	//2D 수정 귀찮아서 패스
	std::vector<Vector2> vertices;//= mesh.GetVertices();
	std::vector<size_t> indices = mesh.GetIndices();
	std::vector<Vector2> uvs = mesh.GetUVs();
	std::vector<LinearColor> colors = mesh.GetColors();
	Matrix3x3 ViewMat = Engine::PInstance->GetMainCam().GetViewMat2D();
	//2D 수정 귀찮아서 패스
	Matrix3x3 WorldMat;// = obj.GetTransform().GetModelingMatrix();
	Matrix3x3 mat2D = ViewMat * WorldMat;
	int indexSize = indices.size();
	if (isTexture) {

		for (int i = 0; i < indexSize; i += 3)
		{
			Vector2 vt2_1 = mat2D * vertices[indices[i]];
			Vector2 vt2_2 = mat2D * vertices[indices[i + 1]];
			Vector2 vt2_3 = mat2D * vertices[indices[i + 2]];

			Vertex2D v1(vt2_1, colors[indices[i]], uvs[indices[i]]);
			Vertex2D v2(vt2_2, colors[indices[i + 1]], uvs[indices[i + 1]]);
			Vertex2D v3(vt2_3, colors[indices[i + 2]], uvs[indices[i + 2]]);
			DrawTriangle2D(v1, v2, v3, texture);
		}
	}
	else {
		for (int i = 0; i < indexSize; i += 3)
		{
			Vector2 vt2_1 = mat2D * vertices[indices[i]];
			Vector2 vt2_2 = mat2D * vertices[indices[i + 1]];
			Vector2 vt2_3 = mat2D * vertices[indices[i + 2]];

			Vertex2D v1(vt2_1, colors[indices[i]], uvs[indices[i]]);
			Vertex2D v2(vt2_2, colors[indices[i + 1]], uvs[indices[i + 1]]);
			Vertex2D v3(vt2_3, colors[indices[i + 2]], uvs[indices[i + 2]]);
			DrawTriangle2D(v1, v2, v3);
		}
	}

}

/// oneminust가 계산으로는 0인데 s와 t의 float값 한계로인해 0이안되고 남는경우가 존재 이로인해 일직선이 제대로 안그려지는 경우 있음
void SoftRenderer::DrawTriangle2D(Vertex2D& p1, Vertex2D& p2, Vertex2D& p3)
{
	Vector2 min = Vector2(Math::Min3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Min3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));
	Vector2 max = Vector2(Math::Max3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Max3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));

	ScreenPoint minPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, min);
	ScreenPoint maxPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, max);

	Vector2 u(p1.Pos - p3.Pos);
	Vector2 v(p2.Pos - p3.Pos);
	float uDotv = u.Dot(v);
	float vDotv = v.Dot(v);
	float uDotu = u.Dot(u);
	float denominator = uDotv * uDotv - vDotv * uDotu;
	if (denominator == 0)
	{
		return;
	}
	float indenominator = 1 / denominator;

	for (int y = maxPoint.Y; y <= minPoint.Y; y++)
	{
		for (int x = minPoint.X; x <= maxPoint.X; x++)
		{
			ScreenPoint fragment = ScreenPoint(x, y);
			Vector2 w = fragment.ToCartesianCoordinate(_ScreenSize) - p3.Pos;

			float wDotu = w.Dot(u);
			float wDotv = w.Dot(v);

			float s = (wDotv * uDotv - wDotu * vDotv) * indenominator;
			float t = (wDotu * uDotv - wDotv * uDotu) * indenominator;

			float oneMinusST = 1 - s - t;
			if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && oneMinusST >= 0 && oneMinusST <= 1) {
				LinearColor color = p1.Color * s + p2.Color * t + p3.Color * oneMinusST;
				SetPixel(fragment, color.ToColor32());
			}
		}
	}
}
void SoftRenderer::DrawTriangle2D(Vertex2D& p1, Vertex2D& p2, Vertex2D& p3, const Texture& texture)
{
	Vector2 min = Vector2(Math::Min3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Min3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));
	Vector2 max = Vector2(Math::Max3(p1.Pos.X, p2.Pos.X, p3.Pos.X), Math::Max3(p1.Pos.Y, p2.Pos.Y, p3.Pos.Y));

	ScreenPoint minPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, min);
	ScreenPoint maxPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, max);

	Vector2 u(p1.Pos - p3.Pos);
	Vector2 v(p2.Pos - p3.Pos);
	float uDotv = u.Dot(v);
	float vDotv = v.Dot(v);
	float uDotu = u.Dot(u);
	float denominator = uDotv * uDotv - vDotv * uDotu;
	if (denominator == 0)
	{
		return;
	}
	float indenominator = 1 / denominator;

	for (int y = maxPoint.Y; y <= minPoint.Y; y++)
	{
		for (int x = minPoint.X; x <= maxPoint.X; x++)
		{
			ScreenPoint fragment = ScreenPoint(x, y);
			Vector2 w = fragment.ToCartesianCoordinate(_ScreenSize) - p3.Pos;

			float wDotu = w.Dot(u);
			float wDotv = w.Dot(v);

			float s = (wDotv * uDotv - wDotu * vDotv) * indenominator;
			float t = (wDotu * uDotv - wDotv * uDotu) * indenominator;

			float oneMinusST = 1 - s - t;
			if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && oneMinusST >= 0 && oneMinusST <= 1) {
				Vector2 uv = p1.UV * s + p2.UV * t + p3.UV * oneMinusST;
				SetPixel(fragment, texture.GetPixel(uv).ToColor32());
			}
		}
	}
}

#pragma endregion