#pragma once

#include <cmath>
#include <numbers>

struct Vector3 {
  // constructor
  constexpr Vector3(const float x = 0.f, const float y = 0.f,
                    const float z = 0.f) noexcept
      : x(x), y(y), z(z) {}

  // operator overloads
  constexpr const Vector3& operator-(const Vector3& other) const noexcept {
    return Vector3{x - other.x, y - other.y, z - other.z};
  }

  constexpr const Vector3& operator+(const Vector3& other) const noexcept {
    return Vector3{x + other.x, y + other.y, z + other.z};
  }

  constexpr const Vector3& operator/(const float factor) const noexcept {
    return Vector3{x / factor, y / factor, z / factor};
  }

  constexpr const Vector3& operator*(const float factor) const noexcept {
    return Vector3{x * factor, y * factor, z * factor};
  }

  // utils
  constexpr const Vector3& ToAngle() const noexcept {
    return Vector3{
        std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
        std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>), 0.0f};
  }

  constexpr const bool IsZero() const noexcept {
    return x == 0.f && y == 0.f && z == 0.f;
  }

  // struct data
  float x, y, z;
};

namespace vec2 {
class vec2_t {
 public:
  float x, y;

  vec2_t() { x = y = 0.f; }

  vec2_t(float x, float y) {
    this->x = x;
    this->y = y;
  }

  auto operator-(const vec2::vec2_t& v) -> vec2::vec2_t {
    return vec2::vec2_t(x - v.x, y - v.y);
  }

  auto is_empty() -> bool {
    if (this->x == 0 || this->y == 0)
      return true;
    else
      return false;
  }

  auto dot(vec2::vec2_t input) -> float {
    return (x * input.x) + (y * input.y);
  }

  auto length_sqr() -> float { return (x * x) + (y * y); }

  auto length() -> float { return (float)sqrt(length_sqr()); }

  auto distance(vec2::vec2_t input) -> float {
    return (*this - input).length();
  }

  auto clear() -> void {
    this->x = 0.f;
    this->y = 0.f;
  }
};

}  // namespace vec2