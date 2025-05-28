#pragma once
#define PI 3.14159265359
#define EPSILON 1e-5f

namespace RGS {
	struct Vec2
	{
		float X, Y;
		constexpr Vec2()
			:X(0.0f), Y(0.0f) {}
		constexpr Vec2(float x, float y)
			: X(x), Y(y) {}
	};

	struct Vec3
	{
		float X, Y, Z;
		constexpr Vec3()
			:X(0.0f), Y(0.0f), Z(0.0f) {}
		constexpr Vec3(const float x, const float y, const float z)
			: X(x), Y(y), Z(z) {}

		operator Vec2() const { return { X,Y }; }
	};

	struct Vec4
	{
		float X, Y, Z, W;
		constexpr Vec4()
			:X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
		constexpr Vec4(float val)
			: X(val), Y(val), Z(val), W(val) {}
		constexpr Vec4(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w) {}
		constexpr Vec4(const Vec3& vec3, const float w)
			: X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w) {}

		operator Vec2()const { return { X,Y }; }
		operator Vec3()const { return { X,Y,Z }; }
	};

	struct Mat4
	{
		float M[4][4];
		Mat4() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					M[i][j] = 0.0f;
				}
			}
		}

		Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);
	};

	float Dot(const Vec3& left, const Vec3& right);
	Vec3 Cross(const Vec3& left, const Vec3& right);
	Vec3 Normalize(const Vec3& vec3);

	Vec2 operator+(const Vec2& left, const Vec2& right);
	Vec2 operator-(const Vec2& left, const Vec2& right);
	Vec2 operator*(const float left, const Vec2& right);
	Vec2 operator*(const Vec2& left, const float right);
	Vec2 operator/(const Vec2& left, const float right);

	Vec3 operator+(const Vec3& left, const Vec3& right);
	Vec3 operator-(const Vec3& left, const Vec3& right);
	Vec3 operator*(const float left, const Vec3& right);
	Vec3 operator*(const Vec3& left, const float right);
	Vec3 operator*(const Vec3& left, const Vec3& right);
	Vec3 operator/(const Vec3& left, const float right);

	Vec4 operator+(const Vec4& left, const Vec4& right);
	Vec4 operator-(const Vec4& left, const Vec4& right);
	Vec4 operator*(const float left, const Vec4& right);
	Vec4 operator*(const Vec4& left, const float right);
	Vec4 operator/(const Vec4& left, const float right);

	Vec4 operator*(const Mat4& left, const Vec4& right);
	Mat4 operator*(const Mat4& left, const Mat4& right);
	Mat4& operator*=(Mat4& left, const Mat4& right);

	float Lerp(const float start, const float end, const float t);
	Vec3 Lerp(const Vec3& start, const Vec3& end, const float t);

	Mat4 Mat4Identity();
	Mat4 Mat4Translate(float tx, float ty, float tz);
	Mat4 Mat4Scale(float sx, float sy, float sz);
	Mat4 Mat4RotateX(float angle);
	Mat4 Mat4RotateY(float angle);
	Mat4 Mat4RotateY(float angle);
	Mat4 Mat4LookAt(const Vec3& xAxis, const Vec3& yAxis, const Vec3& zAxis, const Vec3& eye);
	Mat4 Mat4LookAt(const Vec3& eye, const Vec3& target, const Vec3& up);
	Mat4 Mat4Persepective(const float fovy, const float aspect, const float near, const float far);

	float Clamp(const float val, const float min, const float max);

	unsigned char Float2UChar(const float f);
	float UChar2Float(const unsigned char c);
}