#ifndef MAT3_HPP
#define MAT3_HPP

#include "vec3.h"
#include <array>

template <typename T>
struct mat3 {
  union {
		std::array<T, 3 * 3> a = {};
		std::array<std::array<T, 3>, 3> m;
  };
  mat3() = default;
  friend mat3 operator*(const mat3& m1, const mat3& m2) {
    mat3 result = {};
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
					result.m[i][j] += m1.m[i][k] * m2.m[k][j];
        }
      }
    }
    return result;
  }
  friend mat3 operator*(const mat3& m1, const vec3<T>& v) {
    vec3<T> result = {};
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
				result.a[i] += m1.m[i][j] * v.a[j];
      }
    }
    return result;
  }
};

typedef mat3<float> mat3f;

#endif // mat3_HPP
