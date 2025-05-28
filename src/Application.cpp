#include"Application.h"
#include<iostream>
#include"FrameBuffer.h"
#include<fstream>

namespace RGS {

	Application::Application(const char* name, const int width, const int height, const bool state) 
		:m_Name(name), m_Width(width), m_Height(height)
	{
		Init();
		if (state) {
			SetDisplayBlinnPhongModel();
		}
		else {
			SetDisplayDepthTestAndBlend();
		}
	}

	Application::~Application()
	{
		Terminate();
	}

	void Application::Init() {

		if (IsDisplayDepthTestAndBlend) {
			m_Camera.Pos = { 0.0f,0.0f,0.0f,1.0f };
		}
		if (IsDisplayBlinnPhongModel) {
			m_Camera.Pos = { 0.0f,0.0f,2.0f,1.0f };
		}

		Window::Init();
		m_LastFrameTime = std::chrono::steady_clock::now();
		m_Window = Window::Create(m_Name, m_Width, m_Height);

		LoadMesh("assets\\box.obj");

		m_Uniforms.Diffuse = new Texture("assets\\container2.png");
		m_Uniforms.Specular = new Texture("assets\\container2_specular.png");
		ASSERT(m_Uniforms.Diffuse && m_Uniforms.Specular);
	}

	void Application::Terminate() {
		delete m_Uniforms.Diffuse;
		delete m_Uniforms.Specular;

		delete m_Window;
		Window::Terminate();
	}

	void Application::LoadMesh(const char* filePath) {
		std::ifstream file(filePath);
		ASSERT(file);

		std::vector<Vec3> positions;
		std::vector<Vec2> texCoords;
		std::vector<Vec3> normals;
		std::vector<int> posIndices;
		std::vector<int> texIndices;
		std::vector<int> normalIndices;

		std::string line;
		while (std::getline(file, line)) {
			int item = -1;
			if (line.find("v ") == 0) {
				Vec3 position;
				item = sscanf(line.c_str(), "v %f %f %f",
					&position.X, &position.Y, &position.Z);
				ASSERT(item == 3);
				positions.push_back(position);
			}
			else if (line.find("vt ") == 0) {
				Vec2 texCoord;
				item = sscanf(line.c_str(), "vt %f %f",
					&texCoord.X, &texCoord.Y);
				ASSERT(item == 2);
				texCoords.push_back(texCoord);
			}
			else if (line.find("vn ") == 0) {
				Vec3 normal;
				item = sscanf(line.c_str(), "vn %f %f %f",
					&normal.X, &normal.Y, &normal.Z);
				ASSERT(item == 3);
				normals.push_back(normal);
			}
			else if (line.find("f ") == 0) {
				int pIndices[3], uvIndices[3], nIndices[3];
				item = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
					&pIndices[0], &uvIndices[0], &nIndices[0],
					&pIndices[1], &uvIndices[1], &nIndices[1],
					&pIndices[2], &uvIndices[2], &nIndices[2]);
				ASSERT(item == 9);
				for (int i = 0; i < 3; i++) {
					posIndices.push_back(pIndices[i] - 1);
					texIndices.push_back(uvIndices[i] - 1);
					normalIndices.push_back(nIndices[i] - 1);
				}
			}
		}
		file.close();

		int triNum = posIndices.size() / 3;
		for (int i = 0; i < triNum; i++) {
			Triangle<BlinnVertex> tri;
			for (int j = 0; j < 3; j++) {
				int index = i * 3 + j;
				int pIndex = posIndices[index];
				int tIndex = texIndices[index];
				int nIndex = normalIndices[index];
			
				tri[j].ModelPos = { positions[pIndex],1.0f };
				tri[j].TexCoord = texCoords[tIndex];
				tri[j].ModelNormal = normals[nIndex];
			}
			m_Mesh.emplace_back(tri);
		}
	}

	void Application::OnCameraUpdate(float time) {
		constexpr float speed = 1.0f;
		//向上
		if (m_Window->GetKeyState(RGS_KEY_SPACE) == RGS_PRESS) {
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Up;
		}
		//向下
		if (m_Window->GetKeyState(RGS_KEY_LEFT_SHIFT) == RGS_PRESS) {
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Up;
		}
		//向右
		if (m_Window->GetKeyState(RGS_KEY_D) == RGS_PRESS) {
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Right;
		}
		//向左
		if (m_Window->GetKeyState(RGS_KEY_A) == RGS_PRESS) {
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Right;
		}
		//向前
		if (m_Window->GetKeyState(RGS_KEY_W) == RGS_PRESS) {
			m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Dir;
		}
		//向后
		if (m_Window->GetKeyState(RGS_KEY_S) == RGS_PRESS) {
			m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Dir;
		}

		constexpr float rotateSpeed = 1.0f;
		Mat4 rotation = Mat4Identity();
		//绕y轴(即up方向)逆时针旋转
		if (m_Window->GetKeyState(RGS_KEY_Q) == RGS_PRESS) {
			rotation = Mat4RotateY(time * rotateSpeed);
		}
		if (m_Window->GetKeyState(RGS_KEY_E) == RGS_PRESS) {
			rotation = Mat4RotateY(-time * rotateSpeed);
		}
		m_Camera.Dir = rotation * m_Camera.Dir;
		m_Camera.Dir = { Normalize(m_Camera.Dir),0.0f };
		m_Camera.Right = rotation * m_Camera.Right;
		m_Camera.Right = { Normalize(m_Camera.Right),0.0f };
	}

	void Application::SetDisplayDepthTestAndBlend() {
		IsDisplayBlinnPhongModel = false;
		IsDisplayDepthTestAndBlend = true;
	}

	void Application::SetDisplayBlinnPhongModel() {
		IsDisplayBlinnPhongModel = true;
		IsDisplayDepthTestAndBlend = false;
	}

	void Application::Run() {
		while (!m_Window->IsClosed()) {
			auto nowFrameTime = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(nowFrameTime - m_LastFrameTime);
			float deltaTime = duration.count() * 0.001f * 0.001f;
			m_LastFrameTime = nowFrameTime;
			OnUpdate(deltaTime);
			Window::PollInputEvent();
		}
	}

	void Application::OnUpdate(float time) {
		/*if (m_Window->GetKeyState(RGS_KEY_0) == RGS_PRESS)
			std::cout << "0被按下" << std::endl;
		if (m_Window->GetKeyState(RGS_KEY_A) == RGS_PRESS)
			std::cout << "A被按下" << std::endl;
		if (m_Window->GetKeyState(RGS_KEY_S) == RGS_PRESS)
			std::cout << "S被按下" << std::endl;
		if (m_Window->GetKeyState(RGS_KEY_D) == RGS_PRESS)
			std::cout << "D被按下" << std::endl;
		if (m_Window->GetKeyState(RGS_KEY_Q) == RGS_PRESS)
			std::cout << "Q被按下" << std::endl;
		if (m_Window->GetKeyState(RGS_KEY_W) == RGS_PRESS)
			std::cout << "W被按下" << std::endl;
		if (m_Window->GetKeyState(RGS_KEY_E) == RGS_PRESS)
			std::cout << "E被按下" << std::endl;*/

		//OnCameraUpdate(time);

		//Framebuffer framebuffer(m_Width, m_Height);
		////framebuffer.Clear({ 0.0f, 0.0f, 0.0f });

		//BlinnUniforms uniforms;
		//Mat4 view = Mat4LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, m_Camera.Up);
		//Mat4 proj = Mat4Persepective(90.0f / 180.0f * PI, m_Camera.Aspect, 0.1f, 10.0f);
		//uniforms.MVP = proj * view;
		////uniforms.MVP = Mat4Persepective(90.0f / 180.0f * PI, 1.0f, 1.0f, 10.0f);

		//Program program(BlinnVertexShader, BlinnFragmentShader);
		//program.EnableDoubleSide = true;
		//Triangle<BlinnVertex> tri;
		////tri[0].ModelPos = { 0.0f,0.0f,-8.0f,1.0f };
		////tri[2].ModelPos = { 30.0f,-10.0f,-10.0f,1.0f };
		////tri[2].ModelPos = { 1.0f,1.0f,-1.0f,1.0f };

		//uniforms.IsAnother = true;
		//tri[0].ModelPos = { 10.0f,10.0f,-10.0f,1.0f };
		//tri[1].ModelPos = { -1.0f,-1.0f,-1.0f,1.0f };
		//tri[2].ModelPos = { 10.0f,-10.0f,-10.0f,1.0f };
		//Renderer::Draw(framebuffer, program, tri, uniforms);

		//program.EnableBlend = true;
		//uniforms.IsAnother = false;
		//tri[0].ModelPos = { -10.0f,10.0f,-10.0f,1.0f };
		//tri[1].ModelPos = { -10.0f,-10.0f,-10.0f,1.0f };
		//tri[2].ModelPos = { 1.0f,-1.0f,-1.0f,1.0f };
		//Renderer::Draw(framebuffer, program, tri, uniforms);

		OnCameraUpdate(time);
		Framebuffer framebuffer(m_Width, m_Height);
		Program program(BlinnVertexShader, BlinnFragmentShader);

		if (IsDisplayDepthTestAndBlend) {
			program.EnableDoubleSide = true;
			Triangle<BlinnVertex> tri;
			BlinnUniforms uniforms;
			//m_Camera.Pos = { 0.0f,0.0f,0.0f,1.0f };
			Mat4 view = Mat4LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, m_Camera.Up);
			Mat4 proj = Mat4Persepective(90.0f / 180.0f * PI, m_Camera.Aspect, 0.1f, 10.0f);
			uniforms.MVP = proj * view;
			uniforms.IsDisplayBlinnPhong = false;
			//tri[0].ModelPos = { 0.0f,0.0f,-8.0f,1.0f };
			//tri[2].ModelPos = { 30.0f,-10.0f,-10.0f,1.0f };
			//tri[2].ModelPos = { 1.0f,1.0f,-1.0f,1.0f };

			uniforms.IsAnother = true;
			tri[0].ModelPos = { 10.0f,10.0f,-10.0f,1.0f };
			tri[1].ModelPos = { -1.0f,-1.0f,-1.0f,1.0f };
			tri[2].ModelPos = { 10.0f,-10.0f,-10.0f,1.0f };
			Renderer::Draw(framebuffer, program, tri, uniforms);

			program.EnableBlend = true;
			uniforms.IsAnother = false;
			tri[0].ModelPos = { -10.0f,10.0f,-10.0f,1.0f };
			tri[1].ModelPos = { -10.0f,-10.0f,-10.0f,1.0f };
			tri[2].ModelPos = { 1.0f,-1.0f,-1.0f,1.0f };
			Renderer::Draw(framebuffer, program, tri, uniforms);
		}

		if (IsDisplayBlinnPhongModel) {
			//program.EnableDoubleSide = true;
			//m_Camera.Pos = { 0.0f,0.0f,2.0f,1.0f };

			Mat4 view = Mat4LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, m_Camera.Up);
			Mat4 proj = Mat4Persepective(90.0f / 180.0f * PI, m_Camera.Aspect, 0.1f, 10.0f);
			Mat4 model = Mat4Identity();
			m_Uniforms.MVP = proj * view * model;
			m_Uniforms.CameraPos = m_Camera.Pos;
			m_Uniforms.Model = model;
			m_Uniforms.ModelNormalToWorld = Mat4Identity();
			m_Uniforms.IsDisplayBlinnPhong = true;

			/*m_Uniforms.Shininess *= pow(2, time * 2.0f);
			if (m_Uniforms.Shininess > 256.0f) {
				m_Uniforms.Shininess -= 256.0f;
			}*/
		
			for (auto tri : m_Mesh) {
				Renderer::Draw(framebuffer, program, tri, m_Uniforms);
			}
		}
		m_Window->DrawFrameBuffer(framebuffer);
	}
}