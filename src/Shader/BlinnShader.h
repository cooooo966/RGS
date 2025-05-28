#pragma once
#include"ShaderBase.h"
#include"Texture.h"

namespace RGS {
	struct BlinnVertex:public VertexBase
	{
		Vec3 ModelNormal;
		Vec2 TexCoord = { 0.0f,0.0f };
	};

	struct BlinnVaryings:public VaryingsBase
	{
		Vec3 WorldPos;
		Vec3 WorldNormal;
		Vec2 TexCoord;
	};

	struct BlinnUniforms :public UniformsBase
	{
		Mat4 Model;
		Mat4 ModelNormalToWorld;
		Vec3 LightPos{ 0.0f, 1.0f, 2.0f };
		Vec3 LightAmbient{ 0.2f, 0.2f, 0.2f };
		Vec3 LightDiffuse{ 0.5f, 0.5f, 0.5f };
		Vec3 LightSpecular{ 1.0f, 1.0f, 1.0f };
		Vec3 ObjectColor{ 1.0f, 1.0f, 1.0f };
		Vec3 CameraPos;
		float Shininess = 32.0f;

		Texture* Diffuse;
		Texture* Specular;

		//Ϊ��չʾ���
		bool IsAnother = false;
		bool IsDisplayBlinnPhong = false;
	};

	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);
	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);
}