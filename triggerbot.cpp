#pragma once
#include <thread>

#include "globals.h"
#include "hacks.h"

void hack::triggerbot() {

  const auto& LocalPlayer =
      mem.Read<std::uintptr_t>(client + offsets::Local_Player);

  for (int i = 1; i < 32; ++i) {
    const auto player =
        mem.Read<std::int32_t>(client + offsets::EntityList + i * 0x10);
    if (!player) continue;

    const auto localTeam =
        mem.Read<std::int32_t>(LocalPlayer + offsets::m_IteamNum);

    const auto team = mem.Read<std::int32_t>(player + offsets::m_IteamNum);
    if (!team) continue;

    if (globals::Triggerbot && GetAsyncKeyState(VK_SHIFT)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));

      const auto crosshairId =
          mem.Read<std::int32_t>(LocalPlayer + offsets::m_iCrosshairId);

      if (!crosshairId || crosshairId > 64) continue;
      // gets the player entity then minuses 1 from crosshair id to get the
      // actual Index of player
      const auto player = mem.Read<std::uintptr_t>(
          client + offsets::EntityList + (crosshairId - 1) * 0x10);

      // if player is ded
      if (!mem.Read<std::int32_t>(player + offsets::m_iHealth)) continue;

      // skip player if hes on our team
      if (mem.Read<std::int32_t>(player + offsets::m_IteamNum) != localTeam)

      {
        mem.Write<std::int32_t>(client + offsets::ForceAttack, 6);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        mem.Write<std::uintptr_t>(client + offsets::ForceAttack, 4);
      }
    }
  }
}