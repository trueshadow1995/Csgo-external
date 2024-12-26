#include <iostream>

#include "hacks.h"


struct vector2d {
  float x = {}, y = {};
};

auto oldPunchy = vector2d{};

void hack::noRecoil() {
 

  if (!globals::NoRecoil) return;
  const auto& LocalPlayer = mem.Read<uintptr_t>(client + offsets::Local_Player);

  const auto& shotsFired = mem.Read<int32_t>(engine + offsets::m_iShotsFired);

  if (shotsFired) {
    const auto& clientState =
        mem.Read<uintptr_t>(engine + offsets::dwClientState);
    const auto& viewAngles =
        mem.Read<vector2d>(clientState + offsets::ClientState_ViewAngles);

    const auto& aimPunch =
        mem.Read<vector2d>(LocalPlayer + offsets::aimPunchAngle);

    auto newangles = vector2d{viewAngles.x + oldPunchy.x - aimPunch.x * 2.f,
                              viewAngles.y + oldPunchy.y - aimPunch.y * 2.f};
    if (newangles.x > 89.f) newangles.x = 89.f;

    if (newangles.x < -89.f) newangles.x = -89.f;

    while (newangles.y > 180.f) newangles.y -= 360.f;

    while (newangles.y < -180.f) newangles.y += 360.f;

    mem.Write<vector2d>(clientState + offsets::ClientState_ViewAngles,
                        newangles);

    oldPunchy.x = aimPunch.x * 2.f;
    oldPunchy.y = aimPunch.y * 2.f;

  } else {
    oldPunchy.x = oldPunchy.y = 0.f;
  }
}