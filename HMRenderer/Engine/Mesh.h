#pragma once
#include "EngineUtil.h"
namespace HM
{
	class Mesh {

	public:
		std::vector<Vector3>& GetVertices() { return _Vertices; }
		const std::vector<Vector3>& GetVertices() const { return _Vertices; }
		std::vector<Vector2>& GetUVs() { return _UVs; }
		const std::vector<Vector2>& GetUVs() const { return _UVs; }
		std::vector<LinearColor>& GetColors() { return _Colors; }
		const std::vector<LinearColor>& GetColors() const { return _Colors; }
		std::vector<size_t>& GetIndices() { return _Indices; }
		const std::vector<size_t>& GetIndices() const { return _Indices; }

	private:
		std::vector<Vector3> _Vertices;
		std::vector<Vector2> _UVs;
		std::vector<LinearColor> _Colors;
		std::vector<size_t> _Indices;
	};
}