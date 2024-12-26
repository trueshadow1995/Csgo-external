#pragma once
#include <d3d11.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_truetype.h"
#include "Imgui/imgui.h"

void Style() {
  ImGuiIO& io = ImGui::GetIO();
  auto& style = ImGui::GetStyle();
  io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 16.0f);
  style.Alpha = 1.0f;
  style.WindowPadding = ImVec2(15, 15);
  style.WindowRounding = 10.f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(32, 32);
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style.ChildRounding = 10.f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 10.f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(1, 1);
  style.FrameRounding = 10.f;
  style.FrameBorderSize = 1.0f;
  style.ItemSpacing = ImVec2(8, 4);
  style.ItemInnerSpacing = ImVec2(4, 4);
  style.IndentSpacing = 6.0f;
  style.ColumnsMinSpacing = 6.0f;
  style.ScrollbarSize = 8.0f;
  style.ScrollbarRounding = 9.0f;
  style.GrabMinSize = 10.f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 10.f;
  style.TabBorderSize = 1.0f;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.5f);
  style.MouseCursorScale = 0.f;

  style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);  // primtv 0
  style.Colors[ImGuiCol_TextDisabled] =
      ImVec4(0.75f, 0.75f, 0.75f, 0.85f);  // primtv 2
  style.Colors[ImGuiCol_TextSelectedBg] =
      ImColor(20, 148, 20, 150);  // accent 1

  style.Colors[ImGuiCol_WindowBg] = ImColor(9, 17, 27, 0);  // primtv 3
  style.Colors[ImGuiCol_ChildBg] =
      ImColor(0.031f, 0.066f, 0.10f, 0.35f);                  // primtv 1
  style.Colors[ImGuiCol_PopupBg] = ImColor(30, 30, 30, 255);  // primtv 1

  style.Colors[ImGuiCol_Border] =
      ImVec4(0.00f, 0.00f, 0.00f, 0.10f);  // primtv 4
  style.Colors[ImGuiCol_BorderShadow] =
      ImVec4(0.00f, 0.00f, 0.00f, 0.00f);  // clear

  style.Colors[ImGuiCol_FrameBg] = ImColor(8, 53, 79, 150);         // primtv 3
  style.Colors[ImGuiCol_FrameBgHovered] = ImColor(8, 53, 79, 150);  // accent 1
  style.Colors[ImGuiCol_FrameBgActive] = ImColor(8, 53, 79, 255);   // accent 0

  // style.Colors[ ImGuiCol_ControlBg ] = ImVec4 ( 0.11f, 0.14f, 0.20f, 1.00f );
  // //prim3
  //
  //  style.Colors[ ImGuiCol_ControlBgHovered ] = ImVec4 ( 0.30f,
  //  0.30f, 0.30f, 1.00f );		// primtv 5 style.Colors[
  //
  //   ImGuiCol_ControlBgActive ] = ImVec4 ( 0.75f, 0.75f, 0.75f, 0.10f );
  //   primtv 2

  style.Colors[ImGuiCol_TitleBg] = ImColor(9, 17, 27, 10);         // accent 0
  style.Colors[ImGuiCol_TitleBgActive] = ImColor(9, 17, 27, 150);  // accent 1
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(9, 17, 27, 150);

  style.Colors[ImGuiCol_MenuBarBg] = ImColor(30, 30, 30, 255);  // primtv 1

  style.Colors[ImGuiCol_ScrollbarBg] =
      ImVec4(0.11f, 0.14f, 0.20f, 0.30f);  // primtv 3

  style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(20, 148, 20, 100);  // accent 3

  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.30f, 0.30f, 0.30f, 0.90f);  // primtv 5

  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.75f, 0.75f, 0.75f, 0.10f);  // primtv 2

  style.Colors[ImGuiCol_CheckMark] = ImColor(255, 0, 0, 255);  // accent 0

  style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 0, 0, 255);  // accent 0

  style.Colors[ImGuiCol_SliderGrabActive] =
      ImColor(255, 0, 0, 150);  // accent 1

  style.Colors[ImGuiCol_Button] = ImColor(8, 53, 79, 1);           // primtv 3
  style.Colors[ImGuiCol_ButtonHovered] = ImColor(7, 53, 79, 255);  // primtv 5
  style.Colors[ImGuiCol_ButtonActive] = ImColor(7, 53, 79, 255);   // accent 0

  style.Colors[ImGuiCol_Header] = ImColor(255, 0, 0, 255);  // accent 0
  style.Colors[ImGuiCol_HeaderHovered] =
      ImVec4(0.30f, 0.30f, 0.30f, 1.00f);                          // primtv 5
  style.Colors[ImGuiCol_HeaderActive] = ImColor(40, 40, 40, 255);  // primtv 3

  style.Colors[ImGuiCol_Separator] = ImColor(25, 25, 25, 255);  // primtv 3
  style.Colors[ImGuiCol_SeparatorHovered] =
      ImColor(40, 40, 40, 255);                                      // primtv 5
  style.Colors[ImGuiCol_SeparatorActive] = ImColor(255, 0, 0, 255);  // accent 0

  style.Colors[ImGuiCol_ResizeGrip] = ImColor(20, 148, 20, 255);  // accent 0
  style.Colors[ImGuiCol_ResizeGripHovered] =
      ImVec4(0.30f, 0.30f, 0.30f, 0.70f);  // primtv 5
  style.Colors[ImGuiCol_ResizeGripActive] =
      ImColor(20, 148, 20, 255);  // accent 1

  style.Colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.50f, 1.00f, 0.35f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.0f, 0.f, 1.00f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.24f, 0.80f, 1.00f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
  style.Colors[ImGuiCol_TabUnfocusedActive] =
      ImVec4(0.14f, 0.26f, 0.42f, 1.00f);

  style.Colors[ImGuiCol_PlotLines] = ImColor(20, 148, 20, 160);  // accent 0
  style.Colors[ImGuiCol_PlotLinesHovered] =
      ImColor(20, 148, 20, 255);                                     // accent 0
  style.Colors[ImGuiCol_PlotHistogram] = ImColor(20, 148, 20, 255);  // accent 0
  style.Colors[ImGuiCol_PlotHistogramHovered] =
      ImColor(20, 148, 20, 255);  // accent 0

  style.Colors[ImGuiCol_DragDropTarget] =
      ImColor(20, 148, 20, 255);  // accent 3

  ImGui::PushStyleColor(
      ImGuiCol_Text,
      ImVec4(0.07f, 0.5f, 0.07f, 1.f));  // changes color of title
}
