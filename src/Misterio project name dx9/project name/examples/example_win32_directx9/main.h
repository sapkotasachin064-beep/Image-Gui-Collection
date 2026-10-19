#pragma once


#define LOADER_BRAND "Loader"
#define WINDOW_WIDTH  620
#define WINDOW_HEIGHT 470
bool loader_active = true;
namespace fonts {
    ImFont* font = nullptr;
    ImFont* font2 = nullptr;
    ImFont* Main_Font = nullptr;
    ImFont* Regylar = nullptr;
    ImFont* icons = nullptr;
    ImFont* ico_list = nullptr;
    ImFont* ico = nullptr;
}
namespace images {
    IDirect3DTexture9* profile_img = nullptr;
    IDirect3DTexture9* profiles_img = nullptr;
    IDirect3DTexture9* car_img = nullptr;
    IDirect3DTexture9* yeas_img = nullptr;
    IDirect3DTexture9* aim_img = nullptr;
    IDirect3DTexture9* gun_img = nullptr;
    IDirect3DTexture9* settings_img = nullptr;
    IDirect3DTexture9* visual_img = nullptr;
}
namespace settings {
    int tab;
    bool check1, check2, check3, check4, check5, check6, check7;
    float sliderfloat = 60.f;
    int sliderint = 90;
    static bool multi_items_count[5];
    const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };
    static int select = 0;
    const char* items[3]{ "One", "Two", "Three" };
}

ImColor color_edit4 = { 110 / 255.f, 117 / 255.f, 255 / 255.f, 255.00f };
ImVec4 accentcolor = ImColor(250, 121, 58);



