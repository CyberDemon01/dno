#ifndef MAT2_HPP
#define MAT2_HPP

#include "vec2.h"
#include <array>

template <typename T>
struct mat2 {
  union {
		std::array<T, 2 * 2> a = {};
		std::array<std::array<T, 2>, 2> m;
  };
  mat2() = default;
  mat2(T value) {
    for (int i = 0; i < 2; i++)
			m[i][i] = value;
  }
  friend mat2 operator*(const mat2& m1, const mat2& m2) {
    mat2 result = {};
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
					result.m[i][j] += m1.m[i][k] * m2.m[k][j];
        }
      }
    }
    return result;
  }
  friend mat2 operator*(const mat2& m1, const vec2<T>& v) {
    vec2<T> result = {};
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
				result.a[i] += m1.m[i][j] * v.a[j];
      }
    }
    return result;
  }
};

#endif // MAT4_HPP
