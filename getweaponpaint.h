#pragma once

constexpr const int GetWeaponPaint(const short& itemDefinition) {
  switch (itemDefinition) {
    case 1:
      return 711;  // deagle
    case 4:
      return 38;  // glock
    case 7:
      return 180;  // ak
    case 9:
      return 344;  // awp
    case 61:
      return 504;  // usps
    case 8:
      return 445;  // aug
    case 16:
      return 309;  // m4a1
    case 19:
      return 111;  // p90
    case 17:
      return 310;  // mac10
    case 13:
      return 647;  // gallil
    case 60:
      return 644;  // m4a1 Silenced
    case 38:
      return 196;  // scar-20
    case 26:
      return 306;  // bizon
    case 33:
      return 696;  // mp7
    case 34:
      return 39;  // mp9
    case 11:
      return 438;  // G3SG1
    case 63:
      return 453;  // cz75
    case 36:
      return 168;  // p250
    case 3:
      return 464;  // five seven
    case 30:
      return 179;  //??
    case 40:
      return 361;  // SSG 08
    case 24:
      return 38;  // ump 45

    default:
      return 0;
  }
};