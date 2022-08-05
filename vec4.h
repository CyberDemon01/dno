#ifndef VEC4_HPP
#define VEC4_HPP

#include <iostream>

template <typename T>
struct vec4 {
  union {
    struct { T x, y, z, w; };
    T a[4];
  };
  vec4() = default;
  vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
  template <typename U> vec4(const vec4<U>& v) : x(v._x), y(v._y), z(v._z), w(v._w) {}
  friend auto operator+(const vec4<T>& a, const vec4<T>& b) {
    return vec4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
  }
  friend auto operator-(const vec4<T>& a, const vec4<T>& b) {
    return vec4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
  }
  friend auto operator*(const vec4<T>& v, T c) {
    return vec4<T>(v.x * c, v.y * c, v.z * c, v.w * c);
  }
  friend auto operator*(T c, const vec4<T>& v) {
    return vec4<T>(v.x * c, v.y * c, v.z * c, v.w * c);
  }
  friend auto operator/(const vec4<T>& v, T c) {
    return vec4<T>(v.x / c, v.y / c, v.z / c, v.w / c);
  }
  friend auto operator+(const vec4<T>& v, T c) {
    return vec4<T>(v.x + c, v.y + c, v.z + c, v.w + c);
  }
  friend auto operator-(const vec4<T>& v, T c) {
    return vec4<T>(v.x - c, v.y - c, v.z - c, v.w - c);
  }
  friend std::ostream& operator<<(std::ostream& os, const vec4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
  }
};

typedef vec4<float> vec4f;

#endif // VEC4_HPP
