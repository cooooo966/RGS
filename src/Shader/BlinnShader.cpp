#include"Shader/BlinnShader.h"
#include<algorithm>

namespace RGS {
	void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms) {
		varyings.ClipPos = uniforms.MVP * vertex.ModelPos;
		varyings.TexCoord = vertex.TexCoord;
		varyings.WorldPos = uniforms.Model * vertex.ModelPos;
		varyings.WorldNormal = uniforms.ModelNormalToWorld * Vec4{ vertex.ModelNormal,0.0f };
	}

	Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms) {
		if (uniforms.IsDisplayBlinnPhong) {
			//漫反射光照计算
			Vec3 worldNomal = Normalize(varyings.WorldNormal);
			const Vec3& lightPos = uniforms.LightPos;
			const Vec3& worldPos = varyings.WorldPos;
			Vec3 lightDir = Normalize(lightPos - worldPos);
			Vec3 diffuse = std::max(Dot(worldNomal, lightDir), 0.0f) * uniforms.LightDiffuse;

			//镜面反射（高光项）光照计算
			const Vec3& cameraPos = uniforms.CameraPos;
			Vec3 viewDir = Normalize(cameraPos - worldPos);
			Vec3 halfWayDir = Normalize(lightDir + viewDir);
			Vec3 specular = (float)pow(std::max(Dot(worldNomal, halfWayDir), 0.0f), uniforms.Shininess) * uniforms.LightSpecular;

			//环境光照计算
			Vec3 ambient = uniforms.LightAmbient;

			//如果使用漫反射贴图与镜面贴图
			if (uniforms.Diffuse && uniforms.Specular) {
				const Vec2& texCoord = varyings.TexCoord;
				Vec3 diffColor = uniforms.Diffuse->Sample(texCoord);
				Vec3 specularStrength = uniforms.Specular->Sample(texCoord);

				ambient = ambient * diffColor;
				diffuse = diffuse * diffColor;
				specular = specular * specularStrength;

				Vec3 result = ambient + diffuse + specular;
				return { result,1.0f };
			}

			Vec3 result = (ambient + diffuse + specular) * uniforms.ObjectColor;

			return { result,1.0f };
		}
		else {
			if (uniforms.IsAnother) {
				Vec4 color = varyings.NdcPos * 0.5f + 0.5f;
				color.W = 1.0f;
				return color;
			}
			else {
				int x = varyings.FragPos.X;
				if ((x + 10) % 10 > 4) {
					//设置丢弃
					discard = true;
					//此时不会着色，因此返回的color无效，可以直接返回一个无效值了
					return Vec4{ 0.0f };
				}
				return Vec4{ 1.0f,1.0f,1.0f,0.5f };
			}
		}
	}
}