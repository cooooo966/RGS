#pragma once
#include"Window.h"
#include<chrono>
#include"Maths.h"
#include<string>
#include<vector>
#include"Shader/BlinnShader.h"
#include"Renderer.h"

namespace RGS {
	struct Camera
	{
		//Vec4 Pos = { 0.0f,0.0f,0.0f,1.0f };
		Vec4 Pos = { 0.0f,0.0f,2.0f,1.0f };
		Vec4 Right = { 1.0f,0.0f,0.0f,0.0f };
		Vec4 Up = { 0.0f,1.0f,0.0f,0.0f };
		Vec4 Dir = { 0.0f,0.0f,-1.0f,0.0f };
		//float Aspect = 4.0f / 4.0f;
		float Aspect = 4.0f / 3.0f;
	};

	class Application
	{
	public:
		Application(const char* name, const int width, const int height, const bool state);
		~Application();
		void Run();

		//为了展示新增
		void SetDisplayDepthTestAndBlend();
		void SetDisplayBlinnPhongModel();

	private:
		void Init();
		void Terminate();
		void OnUpdate(float time);
		void OnCameraUpdate(float time);

		void LoadMesh(const char* filePath);

	private:
		const char* m_Name;
		int m_Width, m_Height;
		std::chrono::steady_clock::time_point m_LastFrameTime;
		Window* m_Window;
		Camera m_Camera;
		std::vector<Triangle<BlinnVertex>> m_Mesh;
		BlinnUniforms m_Uniforms;

		//为了展示新增
		bool IsDisplayDepthTestAndBlend = false;
		bool IsDisplayBlinnPhongModel = false;
	};
}