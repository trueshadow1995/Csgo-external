
#include <d3d11.h>
#include <dwmapi.h>

#include <array>
#include <format>
#include <iostream>
#include <string>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imstb_truetype.h"
#include "Imgui/imgui.h"
#include "functions.h"
#include "globals.h"
#include "gui.h"
#include "hacks.h"
#include "io.h"

#include "memory.h"
#include "styles.h"
#include "vector.h"
#include "winbase.h"
#include "windows.h"
#include "worldtoscreen.h"

bool menu_open;

// made by: phil, helped by: ntoes,lilegg,spyder,physical ! :D

bool isRunning = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param,
                                  LPARAM l_param) {
  if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
    return 1L;
  }
  if (message == WM_DESTROY) {
    PostQuitMessage(0);
    return 0L;
  }
  return DefWindowProc(window, message, w_param, l_param);
}
void create_directx(HWND window) {}

INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
  // allocate this program a console
  if (!AllocConsole()) {
    return FALSE;
  };

  // redirect IO
  FILE* file{nullptr};
  freopen_s(&file, "CONIN$", "r", stdout);
  freopen_s(&file, "CONOUT$", "w", stdout);
  freopen_s(&file, "CONOUT$", "w", stdout);

  // initialize memory class
  const auto client = mem.GetModuleAddress("client.dll");
  const auto engine = mem.GetModuleAddress("engine.dll");

  // wait for game
  DWORD pid = 0;
  bool gameFound = false;

  while (!gameFound) {
    // Check if CS:GO process exists
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 pe;
      pe.dwSize = sizeof(PROCESSENTRY32);
      if (Process32First(hSnapshot, &pe)) {
        do {
          if (std::string(pe.szExeFile) == "csgo.exe") {
            pid = pe.th32ProcessID;
            gameFound = true;
            break;
          }
        } while (Process32Next(hSnapshot, &pe));
      }

      CloseHandle(hSnapshot);
    }

    if (!gameFound) {
      Sleep(3000);
      std::cout << "Waiting for CS:GO...\n";
    }
  }

  std::cout << std::format("CS:GO Has Been Re-Fuckulated\n");
  std::cout << std::format("Process ID = {}\n", pid);
  std::cout << std::format("Client -> {:#x}\n", client);

  const HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, client);

  WNDCLASSEXW wc{};
  wc.cbSize = sizeof(WNDCLASSEXW);
  wc.style = CS_HREDRAW | CS_VREDRAW, wc.lpfnWndProc = window_procedure;
  wc.hInstance = instance;
  wc.lpszClassName = L"External Overlay Class";

  RegisterClassExW(&wc);

  const HWND window = CreateWindowExW(
      WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
      wc.lpszClassName, L"phil.9", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr,
      wc.hInstance, nullptr);

  SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

  {
    RECT client_area{};
    GetClientRect(window, &client_area);

    RECT window_area{};
    GetWindowRect(window, &window_area);

    POINT diff{};
    ClientToScreen(window, &diff);

    const MARGINS margins{window_area.left + (diff.x - window_area.left),
                          window_area.top + (diff.y - window_area.top),
                          client_area.right, client_area.bottom};
    DwmExtendFrameIntoClientArea(window, &margins);
  }

  DXGI_SWAP_CHAIN_DESC sd{};
  sd.BufferDesc.RefreshRate.Numerator = 60U;
  sd.BufferDesc.RefreshRate.Denominator = 1U;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.SampleDesc.Count = 1U;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = 2U;
  sd.OutputWindow = window;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferDesc.Width = 0U;
  sd.BufferDesc.Height = 0U;
  constexpr D3D_FEATURE_LEVEL levels[2]{D3D_FEATURE_LEVEL_11_0,
                                        D3D_FEATURE_LEVEL_10_0};

  ID3D11Device* device{nullptr};
  ID3D11DeviceContext* device_context{nullptr};
  IDXGISwapChain* swap_chain{nullptr};
  ID3D11RenderTargetView* render_target_view{nullptr};
  D3D_FEATURE_LEVEL level{};

  D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0U,
                                levels, 2U, D3D11_SDK_VERSION, &sd, &swap_chain,
                                &device, &level, &device_context);

  ID3D11Texture2D* back_buffer{nullptr};
  swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

  if (back_buffer) {
    device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
    back_buffer->Release();
  } else {
    return 1;
  }

  HWND menuhandle = FindWindow("Valve001", NULL);

  ShowWindow(window, cmd_show);
  UpdateWindow(window);

  ImGui::CreateContext();
  Style();

  ImGui_ImplWin32_Init(window);
  ImGui_ImplDX11_Init(device, device_context);
  ID3D11ShaderResourceView* logoTexture = nullptr;

  bool isRunning = true;
  while (isRunning) {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if (msg.message == WM_QUIT) {
        isRunning = false;
      }
    }

    if (!isRunning) {
      break;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    auto backgrounddraw = ImGui::GetBackgroundDrawList();
    auto foregrounddraw = ImGui::GetForegroundDrawList();

    ImGuiIO& io = ImGui::GetIO();
    RECT rc;
    POINT xy;

    ZeroMemory(&rc, sizeof(RECT));
    ZeroMemory(&xy, sizeof(POINT));
    GetClientRect(menuhandle, &rc);
    ClientToScreen(menuhandle, &xy);
    rc.left = xy.x;
    rc.top = xy.y;
    io.ImeWindowHandle = menuhandle;
    io.DeltaTime = 1.0f / 60.0f;
    POINT p;
    GetCursorPos(&p);
    io.MousePos.x = p.x - xy.x;
    io.MousePos.y = p.y - xy.y;

    if (GetAsyncKeyState(VK_LBUTTON)) {
      io.MouseDown[0] = true;
      io.MouseClicked[0] = true;
      io.MouseClickedPos[0].x = io.MousePos.x;
      io.MouseClickedPos[0].y = io.MousePos.y;
    } else
      io.MouseDown[0] = false;

    if (GetAsyncKeyState(VK_INSERT) & 1) menu_open ^= 1;

    if (menu_open) {
      ImGui::SetNextWindowSize({660, 380});  // actual ImGui window

      ImVec4 borderColor =
          ImColor::HSV(ImGui::GetTime() / 10, 0.6f, 0.6f).Value;
      ImVec4 borderGradientColorTopLeft =
          ImVec4(borderColor.x * 0.5f, borderColor.y * 0.5f,
                 borderColor.z * 0.5f, borderColor.w);
      ImVec4 borderGradientColorBottomRight =
          ImVec4(borderColor.x * 0.7f, borderColor.y * 0.7f,
                 borderColor.z * 0.7f, borderColor.w);

      ImGui::Begin("phil9", &menu_open,
                   ImGuiWindowFlags_NoSavedSettings |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
      ImGui::PopStyleColor();

      // ImVec2 logoSize(100, 100);  // Initial size of the logo image
      ImVec2 windowPos = ImGui::GetWindowPos();

      ImGui::PushStyleColor(ImGuiCol_Border, borderGradientColorTopLeft);

      ImGui::PushStyleColor(ImGuiCol_BorderShadow,
                            borderGradientColorBottomRight);

      // static ID3D11Texture2D* retardo = nullptr;  // made by ntoes -_-
      // if (!retardo) {
      // }

      static int current_tab = 0;
      auto button_height = 48;
      auto button_length = 110;  // button shenanigans

      ImGui::BeginChild("TabBar", ImVec2(128, ImGui::GetContentRegionAvail().y),
                        true, ImGuiWindowFlags_NoScrollbar);
      ImGui::PopStyleColor(2);
      {
        ImGui::PushStyleColor(ImGuiCol_Border, borderGradientColorTopLeft);

        ImGui::PushStyleColor(ImGuiCol_BorderShadow,
                              borderGradientColorBottomRight);

        // ImGui::GetWindowDrawList()->AddImage(
        //     Image, windowPos,
        //     ImVec2(windowPos.x + logoSize.x, windowPos.y + logoSize.y));

        auto tabButton = [&](const char* label, int tab) {
          ImVec4 accentColor = ImVec4(0.08f, 0.53f, 0.79f, 0.50f);

          if (current_tab == tab) {
            ImGui::PushStyleColor(ImGuiCol_Button, accentColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, accentColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, accentColor);
          }

          if (ImGui::Button(label, ImVec2(-1, button_height))) {
            current_tab = tab;
          }

          if (current_tab == tab) {
            ImGui::PopStyleColor(3);
          }
        };

        tabButton("Aimbot", 0);
        tabButton("Overlay stuff", 1);
        tabButton("Glow n Chams", 2);
        tabButton("Features", 3);
        tabButton("Skin Changer", 4);
      }
      ImGui::EndChild();

      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_Border, borderGradientColorTopLeft);

      ImGui::PushStyleColor(ImGuiCol_BorderShadow,
                            borderGradientColorBottomRight);
      ImGui::BeginChild("TabContent",
                        ImVec2(ImGui::GetContentRegionAvail().x,
                               ImGui::GetContentRegionAvail().y),
                        true);
      ImGui::PopStyleColor(3);

      {
        switch (current_tab) {
          case 0:  // aimbot

            ImGui::Checkbox("aimbot", &globals::Aimbot);
            ImGui::Spacing();

            ImGui::Spacing();
            ImGui::Checkbox("Fov circle", &globals::Fov);
            ImGui::SameLine(0, 25);
            ImGui::ColorEdit4("Fov Circle Color", globals::FovColor,
                              ImGuiColorEditFlags_NoInputs);
            ImGui::Spacing();

            ImGui::Spacing();
            ImGui::SliderFloat("Smoothing", &globals::Aimbotsmoothing, 0.f,
                               10.f, "%.0f");
            ImGui::Spacing();

            ImGui::Spacing();
            ImGui::SliderFloat("AimbotFov", &globals::AimbotFovSize, 0.f, 90.f,
                               "%.0f");
            ImGui::Spacing();

            ImGui::Spacing();

            break;

          case 1:  // ESP and overlay
            ImGui::Checkbox("player esp coner box", &globals::playerespcorner);

            ImGui::Checkbox("player esp box", &globals::playerespfullbox);
            ImGui::SameLine(0, 90);
            ImGui::ColorEdit4("Player Esp Color", globals::EspBoxColor,
                              ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox("player esp Background",
                            &globals::PlayerEspBackGround);

            ImGui::Checkbox("Player Name", &globals::PlayerName);
            ImGui::SameLine(0, 100);
            ImGui::ColorEdit4("Player Name Color", globals::PlayerNameColor,
                              ImGuiColorEditFlags_NoInputs);

            ImGui::Checkbox("Player Health", &globals::PlayerHealth);
            ImGui::SameLine(0, 96);
            ImGui::ColorEdit4("Player Health Color", globals::PlayerHealthColor,
                              ImGuiColorEditFlags_NoInputs);

            ImGui::Checkbox("Snap Lines", &globals::SnapLines);
            ImGui::SameLine(0, 111);
            ImGui::ColorEdit4("Snap Lines Color", globals::SnapLineColor,
                              ImGuiColorEditFlags_NoInputs);
            break;

          case 2:  // chams and glow
            ImGui::Checkbox("glow", &globals::glow);

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::Checkbox("Chams", &globals::Chams);
            ImGui::Checkbox("Rainbow chams", &globals::Rainbowchams);

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::ColorEdit4("Team Glow Color", globals::TeamGlowColor,
                              ImGuiColorEditFlags_NoInputs);

            ImGui::Spacing();
            ImGui::ColorEdit4("Enemy GLow Color", globals::EnemyGlowColor,
                              ImGuiColorEditFlags_NoInputs);

            ImGui::Spacing();
            ImGui::ColorEdit4("TeamchamsColor", globals::TeamChamColor,
                              ImGuiColorEditFlags_NoInputs);

            ImGui::Spacing();
            ImGui::ColorEdit4("EnemychamsColor", globals::EnemyChamColor,
                              ImGuiColorEditFlags_NoInputs);

            ImGui::Spacing();
            ImGui::SliderFloat("brightness", &globals::chamsbrightness, -10,
                               100.f);
            break;

          case 3:  // features
            ImGui::Checkbox("radar", &globals::radar);

            ImGui::Checkbox("bhop", &globals::Bhop);

            ImGui::Checkbox("no Recoil", &globals::NoRecoil);

            ImGui::Checkbox("triggerbot", &globals::Triggerbot);

            ImGui::Checkbox("No Flash", &globals::NoFlash);

            ImGui::Checkbox("Water Mark", &globals::watamark);

            ImGui::Checkbox("Skin Changer", &globals::skinchanger);

            ImGui::Checkbox("Fps Counter", &globals::FpsCounter);

            ImGui::Checkbox("fov changer", &globals::fovchanger);

            ImGui::SliderFloat("fov amount", &globals::Fovamount, 60, 200);

            break;

          case 4:  // skin changer
            ImGui::Checkbox("skin changer", &globals::skinchanger);
            break;
        }
      }
      ImGui::EndChild();

      ImGui::PopStyleColor(4);
      ImGui::PopStyleVar();
      ImGui::EndChild();
    }

    if (globals::FpsCounter) {
      ImDrawList* fps = ImGui::GetBackgroundDrawList();
      int Framerate = round(ImGui::GetIO().Framerate);
      std::string StatStrings = "FPS: " + std::to_string(Framerate);
      void get_fps();

      fps->AddText(ImGui::GetFont(), 15, ImVec2(50, 50), ImColor(255, 255, 255),
                   StatStrings.c_str());
    }

    if (globals::watamark) {
      float time = static_cast<float>(ImGui::GetTime());
      float r = 0.5f + 0.5f * sin(time);  // r
      float g = 0.5f + 0.5f * sin(time +  // g
                                  2.0f);
      float b =  // b
          0.5f + 0.5f * sin(time + 4.0f);

      const char* watermarkText = "Phil was here | priv secret cheat";

      ImVec2 textSize = ImGui::CalcTextSize(watermarkText);

      ImVec2 backgroundPos(1700, 16);
      ImVec2 backgroundSize(textSize.x + 10, textSize.y + 5);

      float cornerRadius = 10.0f;

      backgrounddraw->AddRectFilled(
          backgroundPos,
          ImVec2(backgroundPos.x + backgroundSize.x,
                 backgroundPos.y + backgroundSize.y),
          ImColor(0.0f, 0.0f, 0.0f),  // Black background
          cornerRadius);              // Set corner radius for rounding

      backgrounddraw->AddText(ImVec2(backgroundPos.x + 5, backgroundPos.y + 2),
                              ImColor(r, g, b), watermarkText);
    }

    if (globals::Fov) {
      ImVec2 center = {960, 540};
      float radius = globals::AimbotFovSize * 3.141592;
      int num_segments = 135;
      float thickness = 2.0f;

      ImColor light_blue(0.3f, 0.8f, 1.0f);
      ImColor blue(0.0f, 0.0f, 1.0f);

      float time = static_cast<float>(ImGui::GetTime());

      for (int i = 0; i < num_segments; ++i) {
        float angle_start = (2 * IM_PI * i) / num_segments;
        float angle_end = (2 * IM_PI * (i + 3)) / num_segments;

        ImVec2 p1 = {center.x + radius * cos(angle_start),
                     center.y + radius * sin(angle_start)};
        ImVec2 p2 = {center.x + radius * cos(angle_end),
                     center.y + radius * sin(angle_end)};

        float spin_factor = sin(time + (i * 0.05f));
        float t = (spin_factor + 1.0f) * 0.5f;

        float r = light_blue.Value.x * (1.0f - t) + blue.Value.x * t;
        float g = light_blue.Value.y * (1.0f - t) + blue.Value.y * t;
        float b = light_blue.Value.z * (1.0f - t) + blue.Value.z * t;

        ImColor current_color(r, g, b);

        backgrounddraw->AddLine(p1, p2, current_color, thickness);
      }
    }

    const auto LocalPlayer =
        mem.Read<std::uintptr_t>(client + offsets::Local_Player);

    if (LocalPlayer) {
      hack::skinchanger();
      hack::hacks();
      hack::noRecoil();
      hack::triggerbot();
      hack::Aimbot();

      const auto glowManager =
          mem.Read<std::uintptr_t>(client + offsets::GlowObjectManager);

      if (!glowManager) continue;

      const auto view_matrix =
          mem.Read<ViewMatrix>(client + offsets::View_Matrix);

      for (int i = 1; i < 32; ++i) {
        const auto player =
            mem.Read<std::int32_t>(client + offsets::EntityList + i * 0x10);
        if (!player) continue;

        auto entity_hp = mem.Read<int>(
            player + offsets::m_iHealth);  // can get ent health from here

        const auto glowManager =
            mem.Read<std::uintptr_t>(client + offsets::GlowObjectManager);

        if (!glowManager) continue;

        const auto localTeam =
            mem.Read<std::int32_t>(LocalPlayer + offsets::m_IteamNum);

        if (!localTeam) continue;

        const auto lifeState =
            mem.Read<std::int32_t>(player + offsets::m_lifeState);

        if (lifeState != 0) continue;

        const auto glowIndex =
            mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

        const auto team = mem.Read<std::int32_t>(player + offsets::m_IteamNum);
        if (!team) continue;

        bool isTeam = team == localTeam;
        if (globals::glow)  // glow
        {
          mem.Write(glowManager + (glowIndex * 0x38) + 0x8,
                    isTeam ? globals::TeamGlowColor[0]
                           : globals::EnemyGlowColor[0]);  // red  team
          mem.Write(glowManager + (glowIndex * 0x38) + 0xc,
                    isTeam ? globals::TeamGlowColor[1]
                           : globals::EnemyGlowColor[1]);  // green
          mem.Write(glowManager + (glowIndex * 0x38) + 0x10,
                    isTeam ? globals::TeamGlowColor[2]
                           : globals::EnemyGlowColor[2]);  // blue
          mem.Write(glowManager + (glowIndex * 0x38) + 0x14,
                    1.00f);  // alpha big cock

          mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
          mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
        }

        if (globals::Chams) {
          float time = static_cast<float>(ImGui::GetTime());

          bool isTeam = (team == localTeam);
          bool isEnemy = !isTeam;

          if (isTeam) {
            if (globals::Rainbowchams) {  // rainbow

              float r = 0.5f + 0.5f * sin(time);
              float g = 0.5f + 0.5f * sin(time + 2.0f);
              float b = 0.5f + 0.5f * sin(time + 4.0f);

              std::array<BYTE, 3> chamColor = {(BYTE)(r * 255.00f),
                                               (BYTE)(g * 255.00f),
                                               (BYTE)(b * 255.00f)};

              mem.Write<std::array<BYTE, 3>>(player + offsets::m_clrRender,
                                             chamColor);
            } else {
              std::array<BYTE, 3> chamColor = {
                  (BYTE)(globals::TeamChamColor[0] * 255.00f),  // Red
                  (BYTE)(globals::TeamChamColor[1] * 255.00f),  // Green
                  (BYTE)(globals::TeamChamColor[2] * 255.00f)   // Blue
              };

              mem.Write<std::array<BYTE, 3>>(player + offsets::m_clrRender,
                                             chamColor);
            }
          }

          if (isEnemy) {
            if (globals::Rainbowchams) {  // enemy rainbowchams

              float r = 0.5f + 0.5f * sin(time + 3.0f);
              float g = 0.5f + 0.5f * sin(time + 5.0f);
              float b = 0.5f + 0.5f * sin(time + 7.0f);

              std::array<BYTE, 3> chamColor = {(BYTE)(r * 255.00f),
                                               (BYTE)(g * 255.00f),
                                               (BYTE)(b * 255.00f)};

              mem.Write<std::array<BYTE, 3>>(player + offsets::m_clrRender,
                                             chamColor);
            } else {
              std::array<BYTE, 3> chamColor = {
                  (BYTE)(globals::EnemyChamColor[0] * 255.00f),  // Red
                  (BYTE)(globals::EnemyChamColor[1] * 255.00f),  // Green
                  (BYTE)(globals::EnemyChamColor[2] * 255.00f)   // Blue
              };

              mem.Write<std::array<BYTE, 3>>(player + offsets::m_clrRender,
                                             chamColor);
            }
          }
        }

        if (globals::chamsbrightness) {
          float brightness = 50.0f;

          const auto _this = static_cast<std::uintptr_t>(
              engine + offsets::model_ambient_min - 0x2c);

          mem.Write<std::int32_t>(
              engine + offsets::model_ambient_min,
              *reinterpret_cast<std::uintptr_t*>(&globals::chamsbrightness) ^
                  _this);
        }

        if (mem.Read<bool>(player + offsets::m_bDormant)) {
          continue;
        }

        if (mem.Read<uintptr_t>(player + offsets::m_IteamNum) == localTeam) {
          continue;
        }

        if (mem.Read<int32_t>(player + offsets::m_lifeState) != 0) {
          continue;
        }

        const auto bones = mem.Read<DWORD>(player + offsets::m_BoneMatrix);

        if (!bones) {
          continue;
        }

        Vector head_pos{
            mem.Read<float>(bones + 0x30 * 8 + 0x0c),
            mem.Read<float>(bones + 0x30 * 8 + 0x1c),
            mem.Read<float>(bones + 0x30 * 8 + 0x2c),
        };

        auto feet_pos = mem.Read<Vector>(player + offsets::m_vecOrigin);

        Vector top;
        Vector bottom;
        if (world_to_screen(head_pos + Vector{0, 0, 11.f}, top, view_matrix) &&
            world_to_screen(feet_pos + Vector{0, 0, 11.f}, bottom,
                            view_matrix)) {
          const float h = bottom.y - top.y;
          const float w = h * 0.35f;

          if (globals::playerespfullbox) {
            backgrounddraw->AddRect(
                {top.x - w, top.y}, {top.x + w, bottom.y + h / 3},
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);
            if (globals::PlayerEspBackGround) {
              backgrounddraw->AddRectFilled(
                  {top.x - w, top.y},
                  {top.x + w, bottom.y + h / 3},  // background
                  ImColor(0, 0, 0, 60));
            }
          }
          if (globals::playerespcorner) {  // esp and name ect..

            backgrounddraw->AddLine(
                {top.x - w, top.y},
                {top.x - w, top.y + h / 3},  // top left pointing down
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x + w, top.y},
                {top.x + w, top.y + h / 3},  // top right pointing down
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x - w, bottom.y},
                {top.x - w, bottom.y + h / 3},  // down left pointing up
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x + w, bottom.y},
                {top.x + w, bottom.y + h / 3},  // down right pointing up
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x - w, top.y}, {top.x - w + h / 5, top.y},  // top left top
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x + w, top.y},
                {top.x + w - h / 5, top.y},  // top right top
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x - w, bottom.y + h / 3},
                {top.x - w + h / 5, bottom.y + h / 3},  // bottom left bottom
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);

            backgrounddraw->AddLine(
                {top.x + w, bottom.y + h / 3},
                {top.x + w - h / 5, bottom.y + h / 3},  // bottom right bottom
                ImColor(globals::EspBoxColor[0], globals::EspBoxColor[1],
                        globals::EspBoxColor[2]),
                1.f);
            if (globals::PlayerEspBackGround) {
              backgrounddraw->AddRectFilled(
                  {top.x - w, top.y},
                  {top.x + w, bottom.y + h / 3},  // background
                  ImColor(0, 0, 0, 60));
            }
          }
          if (globals::PlayerName) {
            std::string name = functions::display_name(i);
            ImVec2 textSize = ImGui::CalcTextSize(name.c_str());

            ImColor contourColor(0, 0, 0,
                                 90);  // Black color with full opacity
            ImColor textColor(globals::PlayerNameColor[0],
                              globals::PlayerNameColor[1],
                              globals::PlayerNameColor[2]);

            ImVec2 textPos = {top.x - textSize.x / 2, top.y - 14};

            float contourOffset = 1.0f;
            for (int dx = -1; dx <= 1; ++dx) {
              for (int dy = -1; dy <= 1; ++dy) {  // skip center
                if (dx == 0 && dy == 0) continue;
                backgrounddraw->AddText({textPos.x + dx * contourOffset,
                                         textPos.y + dy * contourOffset},
                                        contourColor, name.c_str());
              }
            }

            // Draw  name on top
            backgrounddraw->AddText(textPos, textColor, name.c_str());
          }

          if (globals::PlayerHealth) {
            ImGui::GetBackgroundDrawList()->AddText(  // health digits
                {top.x + w + 20, top.y},
                ImColor(globals::PlayerHealthColor[0],
                        globals::PlayerHealthColor[1],
                        globals::PlayerHealthColor[2]),
                std::to_string(entity_hp).c_str());

            float health_pos = (h - (h * entity_hp / 100.f));
            float box = std::ceil(entity_hp / 12.00f);
            float multiplier = 12.00f / 360.f;  // draws health
            multiplier *= box - 1;
            c_color col_health = c_color::from_hsb(multiplier, 1, 1);

            backgrounddraw->AddRectFilled(
                {bottom.x + w + 10, top.y + 5},  // health box
                {bottom.x + w + 8, bottom.y + 20}, ImColor(0, 0, 0));

            backgrounddraw->AddRectFilled(
                {bottom.x + w + 10, top.y + health_pos + 5},
                {bottom.x + w + 8, bottom.y + 20},  // health box
                ImColor(col_health.r, col_health.g, col_health.b,
                        col_health.a));
          }

          if (globals::SnapLines) {
            if (isTeam = team == localTeam)
              backgrounddraw->AddLine(
                  {(float)1920, (float)1080}, {bottom.x, +bottom.y},
                  ImColor(globals::SnapLineColor[0], globals::SnapLineColor[1],
                          globals::SnapLineColor[2]),
                  1.00f);

            else
              backgrounddraw->AddLine(
                  {(float)960, (float)1080}, {bottom.x, +bottom.y},
                  ImColor(globals::SnapLineColor[0], globals::SnapLineColor[1],
                          globals::SnapLineColor[2]),
                  1.00f);
          }
        }
      }
    }

    ImGui::Render();

    constexpr float clear_color[4] = {0.f, 0.f, 0.f, 0.f};
    device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
    device_context->ClearRenderTargetView(render_target_view, clear_color);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    swap_chain->Present(0U, 0U);
  }

  void ImGui_ImplDX11_Shutdown();
  void ImGui_ImplWin32_Shutdown();

  ImGui::DestroyContext();

  if (swap_chain) {
    swap_chain->Release();
  }

  if (device_context) {
    device_context->Release();
  }

  if (device) {
    device->Release();
  }

  if (render_target_view) {
    render_target_view->Release();
  }

  DestroyWindow(window);
  UnregisterClassW(wc.lpszClassName, wc.hInstance);
}
