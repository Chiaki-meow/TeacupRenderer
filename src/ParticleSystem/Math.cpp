#include "Math.h"

namespace Particle2D
{
	const Vec3 Vec3::ZERO	= Vec3( 0,  0,  0);
	const Vec3 Vec3::ONE	= Vec3( 1,  1,  1);
	const Vec3 Vec3::LEFT	= Vec3(-1,  0,  0);
	const Vec3 Vec3::RIGHT	= Vec3( 1,  0,  0);
	const Vec3 Vec3::UP		= Vec3( 0,  1,  0);
	const Vec3 Vec3::DOWN	= Vec3( 0, -1,  0);
	const Vec3 Vec3::FRONT	= Vec3( 0,  0,  1);
	const Vec3 Vec3::BACK	= Vec3( 0,  0, -1);

	const Matrix4 Matrix4::ZERO = Matrix4(
		0, 0, 0, 0, 
		0, 0, 0, 0, 
		0, 0, 0, 0, 
		0, 0, 0, 0);

	const Matrix4 Matrix4::ONE = Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	Matrix4 Matrix4::ortho(GLfloat fLeft, GLfloat fRight, GLfloat fBottom, GLfloat fTop, GLfloat fNear, GLfloat fFar)
	{
		Matrix4 mat4 = Matrix4::ZERO;

		mat4.m[0][0] = 2 / (fRight - fLeft);
		mat4.m[1][1] = 2 / (fTop - fBottom);
		mat4.m[2][2] = 2 / (fNear - fFar);
		mat4.m[3][3] = 1;

		mat4.m[0][3] = -(fRight + fLeft) / (fRight - fLeft);
		mat4.m[1][3] = -(fTop + fBottom) / (fTop - fBottom);
		mat4.m[2][3] = (fNear + fFar) / (fNear - fFar);

		return mat4;
	}
}