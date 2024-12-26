#include "hacks.h"
#include "vector.h"
#include <cmath>

constexpr Vector3 CalculateAngle(const Vector3& localPosition,
    const Vector3& enemyPosition,
    const Vector3& viewAngles) noexcept {
    return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

float CalculateDistance(const Vector3& point1, const Vector3& point2) {
    return std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2) + std::pow(point2.z - point1.z, 2));
}

void hack::Aimbot() {
    const auto client = mem.GetModuleAddress("client.dll");
    const auto engine = mem.GetModuleAddress("engine.dll");
    const auto& LocalPlayer = mem.Read<std::uintptr_t>(client + offsets::Local_Player);

    // Get local player team
    const auto localTeam = mem.Read<std::int32_t>(LocalPlayer + offsets::m_IteamNum);
    if (!localTeam) return;

    //local player eye position
    const auto localEyePosition = mem.Read<Vector3>(LocalPlayer + offsets::m_vecOrigin) +
        mem.Read<Vector3>(LocalPlayer + offsets::m_vecViewOffset);

    // client viewstate angles
    const auto clientState = mem.Read<std::uintptr_t>(engine + offsets::dwClientState);
    const auto localPlayerId = mem.Read<std::int32_t>(clientState + offsets::ClientState_GetLocalPlayer);
    Vector3 viewAngles = mem.Read<Vector3>(clientState + offsets::ClientState_ViewAngles);
    const auto aimPunch = mem.Read<Vector3>(LocalPlayer + offsets::aimPunchAngle) * 2;

    // Convert AimbotFovSize to radians
    const float FovSize = globals::AimbotFovSize * 3.141592f;

    Vector3 bestAngle;
    float closestFov = FovSize;  // Start with the largest possible FOV value

    if (globals::Aimbot) {
        if (!GetAsyncKeyState(VK_SHIFT)) return;

        for (int i = 1; i < 32; ++i) {
            const auto player = mem.Read<std::int32_t>(client + offsets::EntityList + i * 0x10);
            if (!player) continue;

            const auto team = mem.Read<std::int32_t>(player + offsets::m_IteamNum);
            if (!team || team == localTeam) continue;

            if (mem.Read<bool>(player + offsets::m_bDormant)) continue;
            if (mem.Read<std::int32_t>(player + offsets::m_lifeState)) continue;

            if (mem.Read<std::int32_t>(player + offsets::m_bSpottedByMask) & (1 << localPlayerId)) {
                const auto boneMatrix = mem.Read<std::uintptr_t>(player + offsets::m_BoneMatrix);

                // player head position
                const auto playerHeadPosition = Vector3{
                    mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
                    mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
                    mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
                };

                // calculate angle between local player and the enemy head
                const auto angle = CalculateAngle(localEyePosition, playerHeadPosition, viewAngles + aimPunch);

                //  FOV angular distance
                const auto fov = std::hypot(angle.x, angle.y);

                // Only target within the FOV range 
                if (fov < FovSize && fov < closestFov) {
                    closestFov = fov;
                    bestAngle = angle;
                }
            }
        }

        // aimbot if a valid target was found
        if (!bestAngle.IsZero()) {
            if (globals::AimbotSmoothing) {
                
                mem.Write<Vector3>(clientState + offsets::ClientState_ViewAngles, viewAngles + bestAngle / 5.f); //if smoothing is enabled
            }
            else {
               
                mem.Write<Vector3>(clientState + offsets::ClientState_ViewAngles, viewAngles + bestAngle);  //if no smoothing
            }
        }
    }
}
