#ifndef VEC2_HPP
#define VEC2_HPP

#include <iostream>

template <typename T>
struct vec2 {
  union {
    struct { T x, y; };
    T a[2];
  };
  vec2() = default;
  vec2(T _x, T _y) : x(_x), y(_y) {}
  template <typename U> vec2(const vec2<U>& v) : x(v._x), y(v._y) {}
  friend auto operator+(const vec2<T>& a, const vec2<T>& b) {
    return vec2<T>(a.x + b.x, a.y + b.y);
  }
  friend auto operator-(const vec2<T>& a, const vec2<T>& b) {
    return vec2<T>(a.x - b.x, a.y - b.y);
  }
  friend auto operator*(const vec2<T>& v, T c) {
    return vec2<T>(v.x * c, v.y * c);
  }
  friend auto operator*(T c, const vec2<T>& v) {
    return vec2<T>(v.x * c, v.y * c);
  }
  friend auto operator/(const vec2<T>& v, T c) {
    return vec2<T>(v.x / c, v.y / c);
  }
  friend auto operator+(const vec2<T>& v, T c) {
    return vec2<T>(v.x + c, v.y + c);
  }
  friend auto operator-(const vec2<T>& v, T c) {
    return vec2<T>(v.x - c, v.y - c);
  }
  friend std::ostream& operator<<(std::ostream& os, const vec2& v) {
    os << "(" << v.x << ", " << v.y << v.w << ")";
    return os;
  }
};

typedef vec2<float> vec2f;

#endif // vec2_HPP
