#pragma once
#include "globals.h"
#include "memory.h"
#include "string"

namespace hack {
const auto client = mem.GetModuleAddress("client.dll");
const auto engine = mem.GetModuleAddress("engine.dll");
void hacks();

void noRecoil();

void skinchanger();

void triggerbot();

void Aimbot();

}  // namespace hack


