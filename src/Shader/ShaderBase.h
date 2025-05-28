#pragma once
#include"../Maths.h"

namespace RGS {
	struct VertexBase {
		Vec4 ModelPos = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

	struct VaryingsBase
	{
		Vec4 ClipPos = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vec4 NdcPos = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vec4 FragPos = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	struct UniformsBase
	{
		Mat4 MVP;
	};
}