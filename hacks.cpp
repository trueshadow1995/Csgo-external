#include "hacks.h"
#include <dwmapi.h>
#include <array>
#include <format>


void hack::hacks() {

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

    if (globals::fovchanger) {
    pseudo:
      mem.Write<std::uintptr_t>(LocalPlayer + offsets::m_iFOV,
                                globals::Fovamount);
    }

    if (globals::Bhop) {
      auto on_ground = mem.Read<bool>(LocalPlayer + offsets::m_fFlags);

      if (GetAsyncKeyState(VK_SPACE) && on_ground & (1 << 0))
        mem.Write<BYTE>(client + offsets::ForceJump, 6);
    }

    if (globals::radar) {
      mem.Write(player + offsets::m_bSpotted, true);
    }

    if (globals::NoFlash) {
      int flashDuration = 0;

      flashDuration = mem.Read<int>(LocalPlayer + offsets::m_flFlashDuration);

      if (flashDuration > 0)
        mem.Write<int>(LocalPlayer + offsets::m_flFlashDuration, 0);
    }
  }
}