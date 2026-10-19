//ImGui 
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

// Custom ImGui Addition

#include "imguipp_v2.h"

// Bytes
#include "GTA5.h"
#include "FiveM.h"
#include "CSGO.h"
#include "Warzone.h"

// Important
#include "main.h"
#include "globals.h"

// Font and Icon related
#include "font.h"
#include "icons.h"

// D3DX

#include <d3dx9.h>
#pragma comment(lib, "D3dx9")

// Variables

int APIENTRY WindownsMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,"Loader", NULL };
	RegisterClassEx(&wc);
	main_hwnd = CreateWindow(wc.lpszClassName, "Loader", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

	if (!CreateDeviceD3D(main_hwnd)) {
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}
	ShowWindow(main_hwnd, SW_HIDE);
	UpdateWindow(main_hwnd);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
		{
			return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
		};

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 19.5, &CustomFont);
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 32.5f, &icons_config, icons_ranges);
	io.Fonts->AddFontDefault();

	ImGuiStyle& Style = ImGui::GetStyle();
	auto Color = Style.Colors;

	//Style.WindowMinSize = ImVec2(700, 450);
	Style.WindowBorderSize = 7;

	Style.ChildRounding = 0;
	Style.FrameRounding = 0;
	Style.ScrollbarRounding = 0;
	Style.GrabRounding = 0;
	Style.PopupRounding = 0;
	Style.WindowRounding = 3;


	Color[ImGuiCol_WindowBg] = ImColor(18, 18, 18, 255);

	Color[ImGuiCol_FrameBg] = ImColor(31, 31, 31, 255);
	Color[ImGuiCol_FrameBgActive] = ImColor(41, 41, 41, 255);
	Color[ImGuiCol_FrameBgHovered] = ImColor(41, 41, 41, 255);

	Color[ImGuiCol_Button] = ImColor(0.6f, 0.0f, 1.0f, 0.5f);
	Color[ImGuiCol_ButtonActive] = ImColor(0.6f, 0.0f, 1.0f, 0.2f);
	Color[ImGuiCol_ButtonHovered] = ImColor(0.6f, 0.0f, 1.0f, 0.2f);

	Color[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	Color[ImGuiCol_Separator] = ImColor(36, 36, 36, 255);

	Color[ImGuiCol_ResizeGrip] = ImColor(30, 30, 30, 255);
	Color[ImGuiCol_ResizeGripActive] = ImColor(30, 30, 30, 255);
	Color[ImGuiCol_ResizeGripHovered] = ImColor(30, 30, 30, 255);

	Color[ImGuiCol_ChildBg] = ImColor(26, 26, 26, 255);

	Color[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
	Color[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
	Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
	Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);

	Color[ImGuiCol_Header] = ImColor(39, 39, 39, 255);
	Color[ImGuiCol_HeaderActive] = ImColor(39, 39, 39, 255);
	Color[ImGuiCol_HeaderHovered] = ImColor(39, 39, 39, 255);
	Color[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
	Color[ImGuiCol_CheckMark] = ImVec4(0.5f, 0.f, 0.5f, 1.0f);

	// Give the index of the images that each game represents

	LoadImageFromMemory(FiveM, sizeof(FiveM), 0); // Load the first image at index 0
	LoadImageFromMemory(GTA5, sizeof(GTA5), 1); // Load the second image at index 1
	LoadImageFromMemory(CSGO, sizeof(CSGO), 2); // Load the first image at index 0
	LoadImageFromMemory(Warzone, sizeof(Warzone), 3); // Load the second image at index 1

	ImGui_ImplWin32_Init(main_hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	DWORD window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;
	DWORD window_flags2 = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{

			if (loader_active)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800, 600));

				ImGui::Begin("Loader", &loader_active, window_flags2);
				{

					static auto G = Globals::Get();
					static ImVec4 active = ImGuiPP::ToVec4(153, 0, 255, 128);
					static ImVec4 inactive = ImGuiPP::ToVec4(255, 255, 255, 255);

					ImGui::BeginChild("##TopBar", ImVec2(ImGui::GetContentRegionAvail().x, 40), TRUE);
					ImGuiPP::CenterText("Revamp by: voidsmells", 0, 0);
					ImGui::EndChild();

					ImGuiPP::Line(1);

					ImGui::BeginChild("##LeftSide", ImVec2(200, ImGui::GetContentRegionAvail().y), TRUE);

					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 75);

						ImGui::PushStyleColor(ImGuiCol_Text, G->MenuTab == 0 ? active : inactive);
						ImGuiPP::CenterTextEx(ICON_FA_HOME, 205, 0, 0);
						if (ImGui::IsItemClicked()) G->MenuTab = 0;

						ImGui::NewLine();
						ImGui::PushStyleColor(ImGuiCol_Text, G->MenuTab == 1 ? active : inactive);
						ImGuiPP::CenterTextEx(ICON_FA_GAMEPAD, 205, 0, 0);
						if (ImGui::IsItemClicked()) G->MenuTab = 1;

						ImGui::NewLine();
						ImGui::PushStyleColor(ImGuiCol_Text, G->MenuTab == 2 ? active : inactive);
						ImGuiPP::CenterTextEx(ICON_FA_COGS, 205, 0, 0);
						if (ImGui::IsItemClicked()) G->MenuTab = 2;

						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
						ImGui::NewLine();
						ImGuiPP::CenterTextEx(ICON_FA_TIMES_CIRCLE, 205, 0, 0);
						if (ImGui::IsItemClicked()) ExitProcess(0);

						ImGui::PopStyleColor(4);
					}


					ImGui::EndChild();
					ImGuiPP::Linevertical();

					{
						ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
						ImGui::BeginChild("##RightSide", ImVec2(ImGuiPP::GetX(), ImGuiPP::GetY()), TRUE);
						ImGui::PopStyleColor();
						{

							switch (G->MenuTab)
							{

							case 0:
								ImGuiPP::CenterText("CHAIR NAME", 1, TRUE);

								ImGui::NewLine();

								ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.6f, 0.0f, 1.0f, 0.5f));
								ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f); // Worked rounds style
								ImGui::SetCursorPosX(290);
								ImGui::BeginChild("Loader Status", ImVec2(250, 200), true);
								{
									ImGuiPP::CenterText("--- Loader Info: ---", 1, TRUE);

									ImGui::Text("Status:");
									ImGui::SameLine();
									ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Online");

									ImGui::Spacing();

									ImGui::Text("Web:");
									ImGui::SameLine();
									ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Offline");

									ImGui::Spacing();


									ImGui::Text("Last Update:");
									ImGui::SameLine();
									ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "1 Day Ago");

									ImGui::Spacing();

									ImGui::Text("Version:");
									ImGui::SameLine();
									ImGui::TextColored(ImColor(220, 190, 0, 255), "1.8");
								}
								ImGui::EndChild();

								ImGui::SetCursorPosY(62);

								ImGui::BeginChild("Change Log", ImVec2(250, 400), true);
								{
									ImGuiPP::CenterText("--- Change Logs: ---", 1, TRUE);

									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[+]");
									ImGui::Text("[-]");
									ImGui::Text("[-]");
									ImGui::Text("[-]");
									ImGui::Text("[-]");
									ImGui::Text("[-]");
									ImGui::Text("[-]");
									ImGui::Text("[-]");
								}
								ImGui::EndChild();
								ImGui::PopStyleVar();
								ImGui::PopStyleColor();

								break;

							case 1:
								ImGui::ListBoxHeader("##GamesChoice", ImVec2(ImGuiPP::GetX(), ImGuiPP::GetY() - 36.5));
								for (int i = 0; i < G->Games.size(); i++)
								{
									int& selectedImageIndex = selectedImageIndices[i]; // Initialize with the current selected image index for this game
									const bool selected = (G->Game == i);
									if (ImGui::Selectable(G->Games[i].c_str(), selected))
									{
										G->Game = i;

										selectedImageIndex = i;
									}
									if (selected)
										ImGui::SetItemDefaultFocus();
								}

								if (ImGui::BeginChild("Test"))
								{
									ImGuiIO& io = ImGui::GetIO();
									ImDrawList* drawList = ImGui::GetWindowDrawList();

									ImGui::SetCursorPos(ImVec2(50, 80));

									// Define the glow border colors and thicknesses
									ImVec4 glowColors[] = {
										ImVec4(0.6f, 0.0f, 1.0f, 0.2f),  // Purple glow
										ImVec4(0.6f, 0.0f, 1.0f, 0.2f),  // Purple glow
										ImVec4(0.6f, 0.0f, 1.0f, 0.2f)   // Purple glow
									};

									float glowThicknesses[] = { 4.0f, 8.0f, 12.0f }; // Adjust thickness levels

									// Calculate the position and size for the image
									ImVec2 imageSize(250, 200);
									ImVec2 imagePosition = ImGui::GetCursorScreenPos();
									ImVec2 imageMin = imagePosition;
									ImVec2 imageMax = ImVec2(imageMin.x + imageSize.x, imageMin.y + imageSize.y);

									// Draw multiple layers of semi-transparent borders to create a purple glow effect
									for (int i = 0; i < 3; ++i) {
										ImGui::GetWindowDrawList()->AddRect(
											imageMin, imageMax, ImGui::ColorConvertFloat4ToU32(glowColors[i]), 0.0f, 0, glowThicknesses[i]);
									}


									if (G->Game >= 0 && G->Game < G->Games.size())
									{
										int selectedGameIndex = G->Game;
										if (selectedGameIndex >= 0 && selectedGameIndex < MaxGames)
										{
											ImGui::Image((void*)g_Textures[selectedImageIndices[selectedGameIndex]], imageSize);

											ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.6f, 0.0f, 1.0f, 0.5f));
											ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
											ImGui::SetCursorPos(ImVec2(320, 77));
											ImGui::BeginChild(("GameChild" + std::to_string(selectedGameIndex)).c_str(), ImVec2(205, 207), true);
											{


												if (selectedGameIndex == 0)
												{
													ImGuiPP::CenterText("--- FiveM Status: ---", 1, TRUE);

													ImGui::Text("Status:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Online");

													ImGui::Spacing();

													ImGui::Text("Web:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Offline");

													ImGui::Spacing();

													ImGui::Text("Last Update:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "2 Days Ago");

													ImGui::Spacing();

													ImGui::Text("Version:");
													ImGui::SameLine();
													ImGui::TextColored(ImColor(220, 190, 0, 255), "2.3");
												}
												else if (selectedGameIndex == 1)
												{
													ImGuiPP::CenterText("--- GTA5 Status: ---", 1, TRUE);

													ImGui::Text("Status:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Online");

													ImGui::Spacing();

													ImGui::Text("Web:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Offline");

													ImGui::Spacing();

													ImGui::Text("Last Update:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "3 Days Ago");

													ImGui::Spacing();

													ImGui::Text("Version:");
													ImGui::SameLine();
													ImGui::TextColored(ImColor(220, 190, 0, 255), "3.2");
												}
												else if (selectedGameIndex == 2)
												{
													ImGuiPP::CenterText("--- CSGO Status: ---", 1, TRUE);

													ImGui::Text("Status:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Online");

													ImGui::Spacing();

													ImGui::Text("Web:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Offline");

													ImGui::Spacing();

													ImGui::Text("Last Update:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "4 Days Ago");

													ImGui::Spacing();

													ImGui::Text("Version:");
													ImGui::SameLine();
													ImGui::TextColored(ImColor(220, 190, 0, 255), "1.2");
												}
												else if (selectedGameIndex == 3)
												{
													ImGuiPP::CenterText("--- Warzone Status: ---", 1, TRUE);

													ImGui::Text("Status:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Online");

													ImGui::Spacing();

													ImGui::Text("Web:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Offline");

													ImGui::Spacing();

													ImGui::Text("Last Update:");
													ImGui::SameLine();
													ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "5 Days Ago");

													ImGui::Spacing();

													ImGui::Text("Version:");
													ImGui::SameLine();
													ImGui::TextColored(ImColor(220, 190, 0, 255), "2.8");
												}

											}
											ImGui::EndChild();
											ImGui::PopStyleColor();
											ImGui::PopStyleVar();
										}
										if (selectedGameIndex == 0)
										{
											// Display the message above the button if Injection is true
											if (Injection)
											{
												ImGui::SetCursorPosY(360);
												ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
												ImGuiPP::CenterText("Injecting Please Wait...", 0, 0);
												ImGui::PopStyleColor();

												if (ImGui::GetTime() - InjectionMessageTimer >= InjectionMessageDuration)
												{
													Injection = false;
												}
											}
										}

										if (selectedGameIndex == 1)
										{
											// Display the message above the button if Injection is true
											if (Injection)
											{
												ImGui::SetCursorPosY(360);
												ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
												ImGuiPP::CenterText("Injecting Please Wait...", 0, 0);
												ImGui::PopStyleColor();

												if (ImGui::GetTime() - InjectionMessageTimer >= InjectionMessageDuration)
												{
													Injection = false;
												}
											}
										}

										if (selectedGameIndex == 2)
										{
											// Display the message above the button if Injection is true
											if (Injection)
											{
												ImGui::SetCursorPosY(360);
												ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
												ImGuiPP::CenterText("Injecting Please Wait...", 0, 0);
												ImGui::PopStyleColor();

												if (ImGui::GetTime() - InjectionMessageTimer >= InjectionMessageDuration)
												{
													Injection = false;
												}
											}
										}

										if (selectedGameIndex == 3)
										{
											// Display the message above the button if Injection is true
											if (Injection)
											{
												ImGui::SetCursorPosY(360);
												ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
												ImGuiPP::CenterText("Injecting Please Wait...", 0, 0);
												ImGui::PopStyleColor();

												if (ImGui::GetTime() - InjectionMessageTimer >= InjectionMessageDuration)
												{
													Injection = false;
												}
											}
										}
							
									}
								}
								ImGui::EndChild();

								ImGui::ListBoxFooter();

								if (ImGui::Button(("Inject " + G->Games[G->Game]).c_str(), ImVec2(ImGuiPP::GetX(), 33)))
								{
									Injection = true;
									InjectionMessageTimer = ImGui::GetTime();

									// Add your own injection code here the code above is just for the text that will disappear after 1.5 seconds

								}
								break;

							case 2:
								ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
								ImGui::Checkbox("Automatic Injection", &G->AutoInject);
								HELPMARKER("Automatic Injection will automatically wait for your game to load then inject.");
								ImGui::Checkbox("Delay Injection", &G->DelayInjection);
								HELPMARKER("Delay Injection will delay the time it takes for the loader to inject your dll");
								ImGui::PopStyleVar();
								break;
							}
						}
						ImGui::EndChild();
					}
				}
				ImGui::End();
				ImGui::PopStyleVar();
			}
		}
		ImGui::EndFrame();

		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ResetDevice();
		}

		if (!loader_active)
		{
			msg.message = WM_QUIT;
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	//DestroyWindow(main_hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
	//return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	WindownsMain(0, 0, 0, 0);
}
