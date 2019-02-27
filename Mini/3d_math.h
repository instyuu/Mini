#pragma once

#include <cstring>
#include <cmath>

namespace math {

class vec3
{
public:
	vec3() = default;

	vec3(float x, float y, float z)
		: x_(x), y_(y), z_(z)
	{}

	vec3(const vec3 &o)
		: x_(o.x_), y_(o.y_), z_(o.z_)
	{}

	float x_, y_, z_;
};

/*
note:
	[0][0], [0][1],
	[1][0], [1][1]
*/
class mat4x4
{
public:
	// defaults to the identity matrix
	mat4x4()
	{
		memset(c, 0, sizeof(float) * 4 * 4);
		c[0][0] = 1;
		c[1][1] = 1;
		c[2][2] = 1;
		c[3][3] = 1;
	}

	/*
	d 0 0 0
	0 d 0 0
	0 0 d 0
	0 0 0 1
	*/
	mat4x4(float diagonal)
	{
		memset(c, 0, sizeof(float) * 4 * 4);
		c[0][0] = diagonal;
		c[1][1] = diagonal;
		c[2][2] = diagonal;
		c[3][3] = 1;
	}

	/*
	d 0 0 0
	0 d 0 0
	0 0 d 0
	v.x v.y v.z 1, row major
	*/
	static mat4x4 translation_matrix(const vec3 &v)
	{
		mat4x4 result{};
		result.c[3][0] = v.x_;
		result.c[3][1] = v.y_;
		result.c[3][2] = v.z_;
		return result;
	}

	/*
	scl.x 0 0 0
	0 scl.y 0 0
	0 0 scl.z 0
	0 0 0 1, row major
	*/
	static mat4x4 scale_matrix(const vec3 &scl)
	{
		mat4x4 result{};
		result.c[0][0] = scl.x_;
		result.c[1][1] = scl.y_;
		result.c[2][2] = scl.z_;
		return result;
	}

	/*
	No aspect ratio in this one
	Maps to [-1, 1] according to its documentation
	*/
	static mat4x4 perspective_matrix(float fovy, float aspect, float z_near, float z_far)
	{
		mat4x4 result{};
		result.c[0][0] = 1 / (aspect * std::tanf(fovy * 0.5 * 3.14159 / 180));
		result.c[1][1] = 1 / (std::tanf(fovy * 0.5 * 3.14159 / 180));
		result.c[2][2] = (-1 * z_far) / (z_far - z_near);
		result.c[3][2] = (-1 * z_far * z_near) / (z_far - z_near);
		result.c[2][3] = -1;

		return result;
	}

	/*
	Note:
	rotate_x and rotate_y are flipped to get the desired effect
	*/
	static mat4x4 rotate_y(float angle)
	{
		/*
		Clamping is done on this end, for now, but will be done on the
		camera end once the camera is built.
		*/
		if (angle > 90)
			angle = 90;
		if (angle < -90)
			angle = -90;
		float rad_angle = 3.14159 / 180 * angle;
		mat4x4 result{};
		result.c[1][1] = std::cosf(rad_angle);
		result.c[1][2] = -std::sinf(rad_angle);
		result.c[2][1] = std::sinf(rad_angle);
		result.c[2][2] = std::cosf(rad_angle);
		return result;
	}

	static mat4x4 rotate_x(float angle)
	{
		if (angle > 90)
			angle = 90;
		if (angle < -90)
			angle = -90;
		float rad_angle = 3.14159 / 180 * angle;
		mat4x4 result{};

		result.c[0][0] = std::cosf(rad_angle);
		result.c[0][2] = std::sinf(rad_angle);
		result.c[2][0] = -std::sinf(rad_angle);
		result.c[2][2] = std::cosf(rad_angle);

		return result;
	}

	/*
	Implement rotate_z if needed.
	*/

	float c[4][4];
};
/*
TODO: Turn into operator overload
*/
vec3 vec_mat_multiply(const vec3 &v, const mat4x4 &m)
{
	vec3 r(0, 0, 0);
	r.x_ = v.x_ * m.c[0][0] + v.y_ * m.c[1][0] + v.z_ * m.c[2][0] + m.c[3][0];
	r.y_ = v.x_ * m.c[0][1] + v.y_ * m.c[1][1] + v.z_ * m.c[2][1] + m.c[3][1];
	r.z_ = v.x_ * m.c[0][2] + v.y_ * m.c[1][2] + v.z_ * m.c[2][2] + m.c[3][2];
	float w = v.x_ * m.c[0][3] + v.y_ * m.c[1][3] + v.z_ * m.c[2][3] + m.c[3][3];

	if (w != 1)
	{
		r.x_ /= w;
		r.y_ /= w;
		r.z_ /= w;
	}

	return r;
}

mat4x4 mat_mat_multiply(const mat4x4 &a, const mat4x4 &b)
{
	mat4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.c[i][j] = a.c[i][0] * b.c[0][j] + a.c[i][1] * b.c[1][j] +
				a.c[i][2] * b.c[2][j] + a.c[i][3] * b.c[3][j];
		}
	}
	return result;
}

}
