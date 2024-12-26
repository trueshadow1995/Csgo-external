#pragma once
#include <d3d11.h>

#include <cstdint>
#include <string>

#include "imgui/imgui.h"
#include "cstddef"
#include "cstdint"

namespace globals {
inline std::uintptr_t clientAddress = 0;
inline std::uintptr_t engine = 0;
inline bool glow = false;
inline bool playerhealth = true;
inline float TeamGlowColor[]{0.f, 1.f, 0.f};
inline float EnemyGlowColor[]{1.f, 0.f, 0.f};
inline bool playerespfullbox = false;
inline bool playerespcorner = false;
inline bool radar = false;
inline bool Bhop = false;
inline bool Triggerbot = false;
inline float Fovamount = 90.f;
inline bool Fov = false;
inline bool NoFlash = false;
inline bool Aimbot = false;
inline bool fovchanger = false;
inline bool NoRecoil = false;
inline float FovColor[3]{0.f, 0.f, 1.f};
inline bool Smoothing = false;
inline bool chams = false;
inline float chamsbrightness = 25.f;
inline float EnemyChamColor[]{1.f, 0.f, 0.f, 4.f};
inline float TeamChamColor[]{0.f, 1.f, 0.f, 4.f};
inline bool skinchanger = false;
inline bool AimbotSmoothing = false;
inline float Aimbotsmoothing = 5.f;
inline bool watamark = false;
inline float AimbotFovSize = 3.f;
inline bool PlayerName = false;
inline bool PlayerHealth = false;
inline bool SnapLines = false;
inline float PlayerNameColor[3] = {1.f, 0.f, 0.f};
inline float PlayerHealthColor[3] = {0.f, 1.f, 0.f};
inline float SnapLineColor[3] = {0.f, 1.f, 0.f};
inline float EspBoxColor[3] = {0.f, 1.f, 0.f};
inline bool PlayerEspBackGround = false;
inline bool FpsCounter = false;


}  // namespace globals

namespace offsets {
constexpr auto m_iFOV = 0x31F4;
constexpr auto m_iShotsFired = 0x103E0;
constexpr auto View_Matrix = 0x4DF6024;
constexpr auto Local_Player = 0xDEF97C;
constexpr auto GlowObjectManager = 0x535FCB8;
constexpr auto EntityList = 0x4E051DC;
constexpr auto ForceJump = 0x52C0F50;
constexpr auto ForceAttack = 0x3233024;
constexpr auto m_clrRender = 0x70;
constexpr auto model_ambient_min = 0x5A1194;
constexpr auto m_IteamNum = 0xF4;
constexpr auto m_lifeState = 0x25F;
constexpr auto m_iGlowIndex = 0x10488;
constexpr auto m_bSpotted = 0x93D;
constexpr auto m_iHealth = 0x100;
constexpr auto m_iCrosshairId = 0x11838;
constexpr auto m_BoneMatrix = 0x26A8;
constexpr auto m_vecOrigin = 0x138;
constexpr auto m_bDormant = 0xED;
constexpr auto m_fFlags = 0x104;
constexpr auto m_bSpottedByMask = 0x980;
constexpr auto m_vecViewOffset = 0x108;
constexpr auto m_aimPunchAngleVel = 0x3048;
constexpr auto aimPunchAngle = 0x303C;
constexpr auto dwClientState = 0x59F19C;
constexpr auto ClientState_GetLocalPlayer = 0x180;
constexpr auto ClientState_ViewAngles = 0x4D90;
constexpr auto m_viewPunchAngle = 0x3030;
constexpr auto dwClientState_PlayerInfo = 0x52C0;
constexpr auto m_hMyWeapons = 0x2E08;
constexpr auto m_iItemDefinitionIndex = 0x2FBA;
constexpr auto m_nFallbackPaintKit = 0x31D8;
constexpr auto m_iItemIDHigh = 0x2FD0;
constexpr auto m_flFallbackWear = 0x31E0;
constexpr auto m_flFlashDuration = 0x10470;
constexpr auto RadarBase = 0x523BCB4;
}  // namespace offsets

  



