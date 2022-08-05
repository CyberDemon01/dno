#ifndef _transform_hpp_
#define _transform_hpp_

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"
#include "utils.h"

template <typename T>
void identity(mat2<T>& m) {
  for (int i = 0; i < 2; i++)
		m.m[i][i] = static_cast<T>(1);
}

template <typename T>
void identity(mat3<T>& m) {
  for (int i = 0; i < 3; i++)
		m.m[i][i] = static_cast<T>(1);
}

template <typename T>
void identity(mat4<T>& m) {
  for (int i = 0; i < 4; i++)
		m.m[i][i] = static_cast<T>(1);
}

template <typename T>
auto rotate(const mat3<T>& m, T alpha, const vec3<T>& v) {
  const T a = alpha;
  const T c = cos(a);
  const T s = sin(a);
  vec3<T> axis(normalize(v));
  vec3<T> dd((static_cast<T>(1) - c) * axis);
  mat3<T> rot = {};
  identity(rot);
	rot.m[0][0] = c + dd.x * axis.x;
	rot.m[0][1] =  dd.x * axis.y - s * axis.z;

	rot.m[0][2] = dd.x * axis.z + s * axis.y;
	rot.m[1][0] = dd.y * axis.x + s * axis.z;
	rot.m[1][1] = c + dd.y * axis.y;

	rot.m[1][2] = dd.y * axis.z - s * axis.x;
	rot.m[2][0] = dd.z * axis.x - s * axis.y;
	rot.m[2][1] = dd.z * axis.y + s * axis.x;
	rot.m[2][2] = c + dd.z * axis.z;
	return m * rot;
}

template <typename T>
auto translate(const mat4<T>& m, const vec3<T>& v) {
	mat4<T> n = {
	  {
	    1,    0,    0,  0,
	    0,    1,    0,  0,
	    0,    0,    1,  0,
	    v.x, v.y, v.z,  1,
	  }
	};
	return m * n;
}

template <typename T>
auto scale(mat4<T>& m, const vec3<T>& v) {
	mat4<T> n = {
	  {
	    v.x,   0,   0, 0,
	      0, v.y,   0, 0,
	      0,   0, v.z, 0,
	      0,   0,   0, 1,
	  }
	};
	return m * n;
}

template <typename T>
auto rotate(mat4<T>& m, T alpha, const vec3<T>& v) {
  const T a = alpha;
  const T c = cos(a);
  const T s = sin(a);
  vec3<T> axis(normalize(v));
  vec3<T> dd((static_cast<T>(1) - c) * axis);
  mat4<T> rot = {};
  identity(rot);
	rot.m[0][0] = c + dd.x * axis.x;
	rot.m[0][1] =  dd.x * axis.y - s * axis.z;
	rot.m[0][2] = dd.x * axis.z + s * axis.y;

	rot.m[1][0] = dd.y * axis.x + s * axis.z;
	rot.m[1][1] = c + dd.y * axis.y;
	rot.m[1][2] = dd.y * axis.z - s * axis.x;

	rot.m[2][0] = dd.z * axis.x - s * axis.y;
	rot.m[2][1] = dd.z * axis.y + s * axis.x;
	rot.m[2][2] = c + dd.z * axis.z;
	return m * rot;
}

template <typename T>
auto perspective(T fov, T aspect, T near, T far) {
  const T tan_half_fovy = tan(fov / static_cast<T>(2));
  mat4<T> result = {};
	result.m[0][0] = static_cast<T>(1) / (aspect * tan_half_fovy);
	result.m[1][1] = static_cast<T>(1) / (tan_half_fovy);
	result.m[2][2] = far / (far - near);
	result.m[2][3] = static_cast<T>(1);
	result.m[3][2] = -(far * near) / (far - near);
  return result;
}

#endif
