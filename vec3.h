#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>

template <typename T>
struct vec3 {
  union {
    struct { T x, y, z; };
    T a[3];
  };
  vec3() = default;
  vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
  template <typename U> vec3(const vec3<U>& v) : x(v._x), y(v._y), z(v._z) {}
  friend auto operator+(const vec3<T>& a, const vec3<T>& b) {
    return vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
  }
  friend auto operator-(const vec3<T>& a, const vec3<T>& b) {
    return vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
  }
  friend auto operator*(const vec3<T>& v, T c) {
    return vec3<T>(v.x * c, v.y * c, v.z * c);
  }
  friend auto operator*(T c, const vec3<T>& v) {
    return vec3<T>(v.x * c, v.y * c, v.z * c);
  }
  friend auto operator/(const vec3<T>& v, T c) {
    return vec3<T>(v.x / c, v.y / c, v.z / c);
  }
  friend auto operator+(const vec3<T>& v, T c) {
    return vec3<T>(v.x + c, v.y + c, v.z + c);
  }
  friend auto operator-(const vec3<T>& v, T c) {
    return vec3<T>(v.x - c, v.y - c, v.z - c);
  }
  friend std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
  }
};

typedef vec3<float> vec3f;

#endif // vec3_HPP
