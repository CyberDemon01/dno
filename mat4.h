#ifndef MAT4_HPP
#define MAT4_HPP

#include "vec4.h"
#include <array>

template <typename T>
struct mat4 {
  union {
		std::array<T, 4 * 4> a = {};
		std::array<std::array<T, 4>, 4> m;
  };
  mat4() = default;
  mat4(T value) {
    for (int i = 0; i < 4; i++)
			m[i][i] = value;
  }
	mat4(std::array<T, 4 * 4> _a) : a(_a) {}
  friend mat4 operator*(const mat4& m1, const mat4& m2) {
    mat4 result = {};
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
					result.m[i][j] += m1.m[i][k] * m2.m[k][j];
        }
      }
    }
    return result;
  }
  friend mat4 operator*(const mat4& m1, const vec4<T>& v) {
    vec4<T> result = {};
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
				result.a[i] += m1.m[i][j] * v.a[j];
      }
    }
    return result;
  }
};

using mat4f = mat4<float>;

#endif // MAT4_HPP
