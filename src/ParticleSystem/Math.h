#pragma once
#include "Common.h"

#include <math.h>

namespace Particle2D
{
	static inline float toRadian(float angle)
	{
		return angle * 0.0174532925f;
	}

	static inline float toDegrees(float radian)
	{
		return radian * 57.29577951f;
	}

	//------------------------------------------------------------------------
	// Vec2
	//------------------------------------------------------------------------
	class Vec2
	{
	public:
		union { GLfloat x; GLfloat w; };
		union { GLfloat y; GLfloat h; };

	public:
		Vec2() : x(0), y(0) {}

		Vec2(GLfloat nx, GLfloat ny) : x(nx), y(ny) {}

		void set(GLfloat nx, GLfloat ny)
		{
			x = nx; y = ny;
		}

		GLfloat length() const
		{
			return sqrtf(x * x + y * y);
		}

		const Vec2& normalize()
		{
			if ( this->length() == 0 ) return *this;

			GLfloat n = 1 / this->length();
			x *= n; y *= n;
			return *this;
		}
	};
	typedef Vec2 Size;

	/* + */
	inline Vec2 operator + (const Vec2 &v1, const Vec2 &v2) { return Vec2(v1.x + v2.x, v1.y + v2.y); }
	inline Vec2 operator + (GLfloat s, const Vec2 &v) { return Vec2(v.x + s, v.y + s); }
	inline Vec2 operator + (const Vec2 &v, GLfloat s) { return Vec2(v.x + s, v.y + s); }

	/* - */
	inline Vec2 operator - (const Vec2 &v1, const Vec2 &v2) { return Vec2(v1.x - v2.x, v1.y - v2.y); }
	inline Vec2 operator - (GLfloat s, const Vec2 &v) { return Vec2(s - v.x, s - v.y); }
	inline Vec2 operator - (const Vec2 &v, GLfloat s) { return Vec2(v.x - s, v.y - s); }

	/* * */
	inline Vec2 operator * (const Vec2 &v1, const Vec2 &v2) { return Vec2(v1.x * v2.x, v1.y * v2.y); }
	inline Vec2 operator * (GLfloat s, const Vec2 &v) { return Vec2(v.x * s, v.y * s); }
	inline Vec2 operator * (const Vec2 &v, GLfloat s) { return Vec2(v.x * s, v.y * s); }

	/* / */
	inline Vec2 operator / (const Vec2 &v1, const Vec2 &v2) { return Vec2(v1.x / v2.x, v1.y / v2.y); }
	inline Vec2 operator / (GLfloat s, const Vec2 &v) { return Vec2(s / v.x, s / v.y); }
	inline Vec2 operator / (const Vec2 &v, GLfloat s) { return Vec2(v.x / s, v.y / s); }

	//------------------------------------------------------------------------
	// Vec3
	//------------------------------------------------------------------------
	class Vec3
	{
	public:
		GLfloat x, y, z;

	public:
		Vec3() : x(0), y(0), z(0) {}

		Vec3(GLfloat nx, GLfloat ny, GLfloat nz) : x(nx), y(ny), z(nz) {}

		void set(GLfloat nx, GLfloat ny, GLfloat nz)
		{
			x = nx; y = ny; z = nz;
		}

		GLfloat length()
		{
			return sqrtf(x * x + y * y + z * z);
		}

		static Vec3 normalize(Vec3& v)
		{
			GLfloat len = 1.0f / v.length();
			return Vec3(v.x * len, v.y * len, v.z * len);
		}

		const Vec3& normalize()
		{
			*this = Vec3::normalize(*this);
			return *this;
		}

		static GLfloat dot(const Vec3 &v1, const Vec3 &v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}

		static Vec3 cross(const Vec3 &v1, const Vec3 &v2)
		{
			return Vec3(
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x);
		}

		static const Vec3 ZERO;
		static const Vec3 ONE;
		static const Vec3 LEFT;
		static const Vec3 RIGHT;
		static const Vec3 UP;
		static const Vec3 DOWN;
		static const Vec3 FRONT;
		static const Vec3 BACK;
	};

	/* + */
	inline Vec3 operator + (const Vec3 &v1, const Vec3 &v2) { return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	inline Vec3 operator + (GLfloat s, const Vec3 &v) { return Vec3(v.x + s, v.y + s, v.z + s); }
	inline Vec3 operator + (const Vec3 &v, GLfloat s) { return Vec3(v.x + s, v.y + s, v.z + s); }

	/* - */
	inline Vec3 operator - (const Vec3 &v1, const Vec3 &v2) { return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	inline Vec3 operator - (GLfloat s, const Vec3 &v) { return Vec3(s - v.x, s - v.y, s - v.z); }
	inline Vec3 operator - (const Vec3 &v, GLfloat s) { return Vec3(v.x - s, v.y - s, v.z - s); }

	/* * */
	inline Vec3 operator * (const Vec3 &v1, const Vec3 &v2) { return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }
	inline Vec3 operator * (GLfloat s, const Vec3 &v) { return Vec3(v.x * s, v.y * s, v.z * s); }
	inline Vec3 operator * (const Vec3 &v, GLfloat s) { return Vec3(v.x * s, v.y * s, v.z * s); }

	/* / */
	inline Vec3 operator / (const Vec3 &v1, const Vec3 &v2) { return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z); }
	inline Vec3 operator / (GLfloat s, const Vec3 &v) { return Vec3(s / v.x, s / v.y, s / v.z); }
	inline Vec3 operator / (const Vec3 &v, GLfloat s) { return Vec3(v.x / s, v.y / s, v.z / s); }

	//------------------------------------------------------------------------
	// Rect
	//------------------------------------------------------------------------
	class Rect
	{
	public:
		GLfloat x, y, w, h;

	public:
		Rect() : x(0), y(0), w(0), h(0) {}

		void set(GLfloat nx, GLfloat ny, GLfloat nw, GLfloat nh)
		{
			x = nx; y = ny; w = nw; h = nh;
		}

		void setPos(const Vec2& pos) { x = pos.x; y = pos.y; }
		void setSize(const Size& size) { w = size.w; h = size.h; }

		Vec2 getPos() { return Vec2(x, y); }
		Size getSize() { return Size(w, h); }
	};

	//------------------------------------------------------------------------
	// Matrix4
	//------------------------------------------------------------------------
	class Color
	{
	public:
		GLfloat r, g, b, a;

	public:
		Color() : r(0), g(0), b(0), a(0) {}
		Color(GLfloat nr, GLfloat ng, GLfloat nb, GLfloat na) : r(nr), g(ng), b(nb), a(na) {}

		void set(GLfloat nr, GLfloat ng, GLfloat nb, GLfloat na)
		{
			r = nr; g = ng; b = nb; a = na;
		}
	};

	inline Color operator+ (const Color& color, const Color& o)
	{
		return Color(color.r + o.r, color.g + o.g, color.b + o.b, color.a + o.a);
	}
	inline Color operator+ (const Color& color, GLfloat f)
	{
		return Color(color.r + f, color.g + f, color.b + f, color.a + f);
	}
	inline Color operator- (const Color& color, const Color& o)
	{
		return Color(color.r - o.r, color.g - o.g, color.b - o.b, color.a - o.a);
	}
	inline Color operator- (const Color& color, GLfloat f)
	{
		return Color(color.r - f, color.g - f, color.b - f, color.a - f);
	}
	inline Color operator* (const Color& color, const Color& o)
	{
		return Color(color.r * o.r, color.g * o.g, color.b * o.b, color.a * o.a);
	}
	inline Color operator* (const Color& color, GLfloat f)
	{
		return Color(color.r * f, color.g * f, color.b * f, color.a * f);
	}
	inline Color operator/ (const Color& color, const Color& o)
	{
		return Color(color.r / o.r, color.g / o.g, color.b / o.b, color.a / o.a);
	}
	inline Color operator/ (const Color& color, GLfloat f)
	{
		return Color(color.r / f, color.g / f, color.b / f, color.a / f);
	}

	//------------------------------------------------------------------------
	// Matrix4
	//------------------------------------------------------------------------
	class Matrix4
	{
	public:
		union
		{
			GLfloat m[4][4];
			GLfloat _m[16];
		};

	public:
		Matrix4() {}

		Matrix4(
			GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
			GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
			GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
			GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;
			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;
			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;
			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}

		inline Matrix4 transpose(void) const
		{
			return Matrix4(
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]);
		}

		inline void makeTrans(GLfloat tx, GLfloat ty, GLfloat tz)
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		static inline Matrix4 makeTransform(const Vec3& position, const Vec3& scale)
		{
			Matrix4 mat = Matrix4::ONE;

			mat.m[0][0] = scale.x;
			mat.m[1][1] = scale.y;
			mat.m[2][2] = scale.z;

			mat.m[0][3] = scale.z * position.x;
			mat.m[1][3] = scale.z * position.y;
			mat.m[2][3] = scale.z * position.z;

			return mat;
		}

		inline Vec3 operator*(const Vec3& v) const
		{
			Vec3 newv;

			newv.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
			newv.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
			newv.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];

			return newv;
		}

		inline Matrix4 operator*(const Matrix4 &m2) const
		{
			Matrix4 r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return r;
		}

		static Matrix4 ortho(GLfloat fLeft, GLfloat fRight, GLfloat fBottom, GLfloat fTop, GLfloat fNear, GLfloat fFar);

		static const Matrix4 ZERO;
		static const Matrix4 ONE;
	};


}