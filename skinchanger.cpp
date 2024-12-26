
#include <array>
#include <format>

#include "memory.h"
#include "hacks.h"
#include "getweaponpaint.h"

void hack::skinchanger() {
  
  const auto LocalPlayer = mem.Read<uintptr_t>(client + offsets::Local_Player);

  if (globals::skinchanger) {
    const auto& weapons = mem.Read<std::array<unsigned long, 8>>(
        LocalPlayer + offsets::m_hMyWeapons);

    // local player weapon iteration
    for (const auto& handle : weapons) {
      
        const auto& weapon = mem.Read<std::uintptr_t>(
          (client + offsets::EntityList + (handle & 0xFFF) * 0x10) - 0x10);

      if (!weapon) continue;

      // look if we want to apply a skin
      if (const auto paint = GetWeaponPaint(
              mem.Read<short>(weapon + offsets::m_iItemDefinitionIndex))) {
        
          const bool shouldUpdate =
            mem.Read<std::int32_t>(weapon + offsets::m_nFallbackPaintKit) !=
            paint;

        // force weapon to use fallback values
        mem.Write<std::int32_t>(weapon + offsets::m_iItemIDHigh, -1);
        mem.Write<std::int32_t>(weapon + offsets::m_nFallbackPaintKit, paint);
        mem.Write<float>(weapon + offsets::m_flFallbackWear, 0.1f);

        if (shouldUpdate)
          mem.Write<int32_t>(
              mem.Read<uintptr_t>(engine + offsets::dwClientState) + 0x174, -1);
      }
    }
  }
}
