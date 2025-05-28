#pragma once
#include"Maths.h"
#include<string>
#include<stb_image.h>

namespace RGS {
	class Texture
	{
	public:
		Texture(const std::string path);
		~Texture();

		Vec4 Sample(const Vec2& texCoord) const;

	private:
		void Init();

	private:
		int m_Width, m_Height, m_Channels;
		std::string m_Path;
		Vec4* m_Data = nullptr;
	};
}