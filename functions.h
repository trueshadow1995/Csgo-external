#pragma once
#include "globals.h"
#include "memory.h"

namespace functions {

auto display_name(int Index) -> std::string {
  typedef struct player_info_s {
    DWORD __pad0[2];
    int xuidlow;
    int xuidhigh;
    char name[256];
    int userid;
    char steamid[33];
    UINT steam3id;
    char friendsname[128];
    bool isfakeplayer;
    bool ishltv;
    DWORD customfiles[4];
    BYTE filesdownloaded;
    int __pad1;
  } player_info_t;
  const auto engine = mem.GetModuleAddress("engine.dll");
  std::uintptr_t ClientState =
      mem.Read<std::uintptr_t>(engine + offsets::dwClientState);

  std::uintptr_t UserInfoTable =
      mem.Read<std::uintptr_t>(ClientState + offsets::dwClientState_PlayerInfo);

  std::uintptr_t x = mem.Read<std::uintptr_t>(
      mem.Read<std::uintptr_t>(UserInfoTable + 0x40) + 0xC);

  player_info_s p =
      mem.Read<player_info_s>(mem.Read<uintptr_t>(x + 0x28 + 0x34 * Index));

  return p.name;
}

}  // namespace functions

class c_color {
 public:
  float r, g, b, a;

  c_color(float _r, float _g, float _b, float _a) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
  }

  c_color(uint32_t color) {
    this->a = (color >> 24) & 0xff;
    this->r = (color >> 16) & 0xff;
    this->g = (color >> 8) & 0xff;
    this->b = (color & 0xff);
  }

  static c_color from_hsb(float flHue, float flSaturation, float flBrightness) {
    const float h = std::fmodf(flHue, 1.0f) / (60.0f / 360.0f);
    const int i = static_cast<int>(h);
    const float f = h - static_cast<float>(i);
    const float p = flBrightness * (1.0f - flSaturation);
    const float q = flBrightness * (1.0f - flSaturation * f);
    const float t = flBrightness * (1.0f - flSaturation * (1.0f - f));

    float r = 0.0f, g = 0.0f, b = 0.0f;

    switch (i) {
      case 0:
        r = flBrightness, g = t, b = p;
        break;
      case 1:
        r = q, g = flBrightness, b = p;
        break;
      case 2:
        r = p, g = flBrightness, b = t;
        break;
      case 3:
        r = p, g = q, b = flBrightness;
        break;
      case 4:
        r = t, g = p, b = flBrightness;
        break;
      case 5:
      default:
        r = flBrightness, g = p, b = q;
        break;
    }

    return c_color(r, g, b, 255);
  }

  void random_color(int iTick) { this->r = sin(3.f * iTick + 0.f) * 127 + 128; }
};

struct Vector {
  Vector() noexcept : x(), y(), z() {}

  Vector(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

  Vector& operator+(const Vector& v) noexcept {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector& operator-(const Vector& v) noexcept {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  float x, y, z;
};
