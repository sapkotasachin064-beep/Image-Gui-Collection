#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void
glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int tabs{};

int
main(int, char **)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
  const char *glsl_version = "#version 300 es";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "imgui-menu", nullptr, nullptr);
  if (window == nullptr)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  ImFontConfig config;
  config.PixelSnapH = true;
  config.OversampleH = 2;
  config.OversampleV = 2;
  constexpr ImWchar glyphRanges[] = {
      0x0020, 0x00FF, /* Basic Latin + Latin Supplement */
      0x0400, 0x052F, /* Cyrillic + Cyrillic Supplement */
      0x2DE0, 0x2DFF, /* Cyrillic Extended-A */
      0xA640, 0xA69F, /* Cyrillic Extended-B */
      0,
  };
  ImFont *IBMPlexFont = io.Fonts->AddFontFromFileTTF(
      "fonts/smallest-pixel-7.regular.ttf", 16.0f, &config, glyphRanges);
  if (!IBMPlexFont)
    fprintf(stderr, "Unable to load font.\n");
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4 clear_color = ImVec4(0.3607843137254902f, 0.12549019607843137f,
                              0.24313725490196078, 1.f);
  io.IniFilename = nullptr;

  /* vars */
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      ImGui::SetNextWindowSize({600, 650});
      ImGui::SetNextWindowBgAlpha(0.0f);
      ImGui::SetNextWindowPos({io.DisplaySize.x / 2, io.DisplaySize.y / 2},
                              ImGuiCond_Once, {0.5f, 0.5f});
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGuiWindowFlags flags =
          ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar;
      ImGui::Begin("menu", nullptr, flags);
      auto draw = ImGui::GetWindowDrawList();
      auto pos = ImGui::GetWindowPos();

      const float borderWidth = 8.0f, innerBorderWidth = 3.0f,
                  innerOffset = borderWidth - innerBorderWidth;

      const auto darker = ImColor(63, 25, 38), dark = ImColor(43, 0, 24),
                 darkLighter = ImColor(47, 11, 28), light = ImColor(59, 13, 35);

      const auto mainWindow = dark, borderColor = light,
                 insideBorderColor = darkLighter;

      const ImVec2 mainWindowSize = ImVec2{600.f, 650.f};

      /* main window */
      draw->AddRectFilled(ImVec2(pos.x + borderWidth, pos.y + borderWidth),
                          ImVec2(pos.x + (mainWindowSize.x - borderWidth),
                                 pos.y + (mainWindowSize.y - borderWidth)),
                          mainWindow, 0.0f, ImDrawFlags_None);

      /* top border */
      draw->AddRectFilled(ImVec2(pos.x, pos.y),
                          ImVec2(pos.x + mainWindowSize.x,
                                 pos.y + (borderWidth - innerBorderWidth)),
                          borderColor, 0.0f, ImDrawFlags_None);

      /* bottom border */
      draw->AddRectFilled(
          ImVec2(pos.x,
                 pos.y + mainWindowSize.y - (borderWidth - innerBorderWidth)),
          ImVec2(pos.x + mainWindowSize.x, pos.y + mainWindowSize.y),
          borderColor, 0.0f, ImDrawFlags_None);

      /* left border */
      draw->AddRectFilled(ImVec2(pos.x, pos.y),
                          ImVec2(pos.x + (borderWidth - innerBorderWidth),
                                 pos.y + mainWindowSize.y),
                          borderColor, 0.0f, ImDrawFlags_None);

      /* right border */
      draw->AddRectFilled(
          ImVec2(pos.x + mainWindowSize.x - (borderWidth - innerBorderWidth),
                 pos.y),
          ImVec2(pos.x + mainWindowSize.x, pos.y + mainWindowSize.y),
          borderColor, 0.0f, ImDrawFlags_None);

      /* left inner border */
      draw->AddRectFilledMultiColor(
          ImVec2(pos.x + innerOffset, pos.y + innerOffset),
          ImVec2(pos.x + innerOffset + innerBorderWidth,
                 pos.y + mainWindowSize.y - innerOffset),
          insideBorderColor, darker, darker, insideBorderColor);

      /* right inner border */
      draw->AddRectFilledMultiColor(
          ImVec2(pos.x + mainWindowSize.x - innerOffset - innerBorderWidth,
                 pos.y + innerOffset),
          ImVec2(pos.x + mainWindowSize.x - innerOffset,
                 pos.y + mainWindowSize.y - innerOffset),
          darker, insideBorderColor, insideBorderColor, darker);

      /* top inner border */
      draw->AddRectFilledMultiColor(
          ImVec2(pos.x + innerOffset, pos.y + innerOffset),
          ImVec2(pos.x + mainWindowSize.x - innerOffset,
                 pos.y + innerOffset + innerBorderWidth),
          insideBorderColor, insideBorderColor, darker, darker);

      /* bottom inner border */
      draw->AddRectFilledMultiColor(
          ImVec2(pos.x + innerOffset,
                 pos.y + mainWindowSize.y - innerOffset - innerBorderWidth),
          ImVec2(pos.x + mainWindowSize.x - innerOffset,
                 pos.y + mainWindowSize.y - innerOffset),
          darker, darker, insideBorderColor, insideBorderColor);

      draw->AddRect(ImVec2(pos.x + borderWidth, pos.y + borderWidth),
                    ImVec2(pos.x + (mainWindowSize.x - borderWidth),
                           pos.y + (mainWindowSize.y - borderWidth)),
                    ImColor(19, 0, 10), 0.0f, 0, 1.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));

      static int         tabs = 0;
      static bool        someVar = false;
      static float       someVal = 0.0f;
      static const char *items[]{"One", "Two", "Three"};
      static int         selectedItem = 0;
      float              availableWidth = mainWindowSize.x - (2 * borderWidth),
            buttonWidth = availableWidth / 3.0f;

      ImGui::SetCursorPos({borderWidth, borderWidth});
      if (ImGui::ButtonCustom("main", {buttonWidth, 50.f}))
        tabs = 0;

      ImGui::SameLine();
      if (ImGui::ButtonCustom("other", {buttonWidth, 50.f}))
        tabs = 1;

      ImGui::SameLine();
      if (ImGui::ButtonCustom("settings", {buttonWidth, 50.f}))
        tabs = 2;

      const ImVec2 tabSize(
          (mainWindowSize.x - (2 * (borderWidth + innerBorderWidth))) / 2.0f -
              15.f,
          565.f);

      float tabLeftButtonAvailableWidth = tabSize.x + 10.f,
            tabLeftButtonWidth = (tabLeftButtonAvailableWidth / 3.0f) - 5.0f;

      float tabRightButtonAvailableWidth = tabSize.x + 5.0f,
            tabRightButtonWidth = (tabRightButtonAvailableWidth / 2.0f) - 5.0f;
      switch (tabs) {
      case 0: /* main tab */
      {
        ImGui::SetCursorPos({borderWidth + innerBorderWidth + 10.f, 70.f});
        ImGui::BeginChildCustom("leftTab", tabSize);
        ImGui::ButtonCustom("Enemy", {tabLeftButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Friendly", {tabLeftButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Local", {tabLeftButtonWidth, 30});
        ImGui::SetCursorPos({0.0f, 35.f});
        ImGui::Checkbox("Something", &someVar);
        ImGui::Checkbox("Something1", &someVar);
        ImGui::Checkbox("Something2", &someVar);
        ImGui::Checkbox("Something3", &someVar);
        ImGui::SameLine();
        ImGui::Dummy({137.f, 0.f});
        ImGui::SameLine();
        ImGui::ColorEdit4("##Somecol", reinterpret_cast<float *>(&clear_color),
                          ImGuiColorEditFlags_NoInputs);
        ImGui::SliderFloat("Someval", &someVal, 0.0f, 1.0f, "%.2f");
        ImGui::EndChild();
        ImGui::SetCursorPos(
            {borderWidth + innerBorderWidth + tabSize.x + 25.f, 70.f});
        ImGui::BeginChildCustom("rightTab", tabSize);
        ImGui::ButtonCustom("World", {tabRightButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Other", {tabRightButtonWidth, 30});
        ImGui::SetCursorPos({0.0f, 35.f});
        ImGui::Checkbox("Something10", &someVar);
        ImGui::Checkbox("Something11", &someVar);
        ImGui::Checkbox("Something12", &someVar);
        ImGui::Checkbox("Something13", &someVar);
        ImGui::Combo("Combo", &selectedItem, items, IM_ARRAYSIZE(items));
        ImGui::EndChild();
        break;
      }
      case 1: /* other tab */
      {
        ImGui::SetCursorPos({borderWidth + innerBorderWidth + 10.f, 70.f});
        ImGui::BeginChildCustom("leftTabOther", tabSize);
        ImGui::ButtonCustom("Exploits", {tabLeftButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Bannable", {tabLeftButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Misc", {tabLeftButtonWidth, 30});
        ImGui::SetCursorPos({0.0f, 5.0f});
        ImGui::EndChild();
        ImGui::SetCursorPos(
            {borderWidth + innerBorderWidth + tabSize.x + 25.f, 70.f});
        ImGui::BeginChildCustom("rightTabOther", tabSize);
        ImGui::ButtonCustom("Players", {tabRightButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Other", {tabRightButtonWidth, 30});
        ImGui::SetCursorPos({0.0f, 5.0f});
        ImGui::EndChild();
        break;
      }
      case 2: /* settings tab */
      {
        ImGui::SetCursorPos({borderWidth + innerBorderWidth + 10.f, 70.f});
        ImGui::BeginChildCustom("leftTabSettings", tabSize);
        ImGui::ButtonCustom("Safety", {tabLeftButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Custom", {tabLeftButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Properties", {tabLeftButtonWidth, 30});
        ImGui::SetCursorPos({0.0f, 5.0f});
        ImGui::EndChild();
        ImGui::SetCursorPos(
            {borderWidth + innerBorderWidth + tabSize.x + 25.f, 70.f});
        ImGui::BeginChildCustom("rightTabSettings", tabSize);
        ImGui::ButtonCustom("Configs", {tabRightButtonWidth, 30});
        ImGui::SameLine();
        ImGui::ButtonCustom("Menu", {tabRightButtonWidth, 30});
        ImGui::SetCursorPos({0.0f, 5.0f});
        ImGui::EndChild();
        break;
      }
      }

      ImGui::PopStyleVar(2);
      ImGui::End();
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
