#ifndef _math_functions_h
#define _math_functions_h

#include <cmath>
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

template <typename T>
inline T sqr(T x) {
  return x * x;
}

template <typename T>
inline T radians(T x) {
  return x * M_PI / static_cast<T>(180);
}

template <typename T>
vec2<T> normalize(const vec2<T>& v) {
  const T d = sqrt(sqr(v.x) + sqr(v.y));
  vec2<T> result(v.x / d, v.y / d);
  return result;
}

template <typename T>
vec3<T> normalize(const vec3<T>& v) {
  const T d = sqrt(sqr(v.x) + sqr(v.y) + sqr(v.z));
  vec3<T> result(v.x / d, v.y / d, v.z / d);
  return result;
}

#endif
