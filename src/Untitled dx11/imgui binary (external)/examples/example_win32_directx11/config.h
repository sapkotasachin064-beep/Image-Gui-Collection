#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "imgui.h"
#include "imgui_internal.h"
#include <thread>
#include <chrono>
#include <random>

using namespace std::chrono;
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

static float timer_anim = 0.f;

namespace GUI {
    float color[4] = { 230 / 255.f, 99 / 255.f, 240 / 255.f, 255 / 255.f };
    bool hide_gui = false; /* */ int hide_bind;
    bool hide_menu = false;

    float menu_scale = 1.f;

    bool particle = true;
    int snow_lim = 15;

}

namespace cfg {

    bool check_creater = false;
    std::string path_name = "C:\\VISION\\";
    std::string line_checker;
    std::string line;
    std::string text;
    std::string s;
    int findnum;
    int findpos;
    const char* delete_file;
    char add_cfg[16] = { "" };
    char name_cfg[16] = { "" };
    char search_cfg[16] = { "" };
    char selected_cfg[16] = { "" };
    std::string plus = path_name + add_cfg + ".ini";
    char buf[100];
}

bool readfile(std::string& s, std::string filename) {
    std::ifstream fp(filename);
    if (!fp.is_open())
        return false;

    char nil = '\0';
    std::getline(fp, s, nil);
    fp.close();
    return (s.length() > 0);
}

void cleaning() {
    memset(&cfg::add_cfg[0], 0, sizeof(cfg::add_cfg));
    memset(&cfg::search_cfg[0], 0, sizeof(cfg::search_cfg));
}

namespace config_base {

    void load_cfg() {

        std::ifstream download(cfg::path_name + cfg::name_cfg + ".ini");

        if (download.is_open())
        {

        }
        download.close();
    }

    void add_cfg() {
        std::ofstream read;
        read.open(cfg::path_name + cfg::add_cfg + ".ini");
        {
 
        }
        read.close();
    }

    void reset_cfg() {

    }

    namespace delete_file {

        void eraseFileLine(std::string path, std::string eraseLine) {
            std::string line;
            std::ifstream fin;
            fin.open(path);

            std::ofstream temp;
            temp.open("temp.ini");

            while (getline(fin, line)) {

                if (line != eraseLine)
                    temp << line << std::endl;
            }

            temp.close();
            fin.close();

            const char* p = path.c_str();
            remove(p);
            rename("temp.ini", p);
        }
    }

    namespace create_config {

        void create() {

            for (int i = 0; cfg::add_cfg[i]; i++) {
                if (cfg::add_cfg[i] == ' ') {
                    cfg::add_cfg[i] = *"_";
                }
            }

            std::ifstream general_path("C:\\VISION\\Config_List.ini");

            std::ofstream data;

            using In = std::istream_iterator<std::string>;

            auto pos = std::find(In(general_path), In(), cfg::add_cfg);
            if (pos != In())
                cfg::check_creater = false;
            else
                cfg::check_creater = true;

            general_path.close();

            if (cfg::add_cfg[0] && cfg::check_creater) {

                std::ofstream outfile(cfg::path_name + cfg::add_cfg + ".ini");

                config_base::add_cfg();

                data.open(cfg::path_name + "Config_List.ini", std::ios_base::app);
                data << cfg::add_cfg << "\n";
                data.close();

                cleaning();
            }
        }
    }

}

void config_list() {

    SetConsoleOutputCP(65001);

    setlocale(LC_ALL, "Russian");

    CreateDirectory(L"C:\\VISION", NULL);

    ImGui::InputTextEx("##0", "Search Config", cfg::search_cfg, 16, ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35), ImGuiInputTextFlags_None);
    if (ImGui::IsItemClicked()) PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, INPUTLANGCHANGE_SYSCHARSET, 0x409);

    ImGui::Separator();

    ImGui::InputTextEx("##1", "Config Name", cfg::add_cfg, 16, ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35), ImGuiInputTextFlags_None);

    if (ImGui::IsItemClicked()) PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, INPUTLANGCHANGE_SYSCHARSET, 0x409);

    ImGui::Separator();

    if (ImGui::Button("Create", ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35))) {
        config_base::create_config::create();
    };

    if (ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35))) {

        cfg::plus = cfg::path_name + cfg::add_cfg + ".ini";
        cfg::delete_file = cfg::plus.c_str();
        remove(cfg::delete_file);

        config_base::delete_file::eraseFileLine("C:\\VISION\\Config_List.ini", cfg::add_cfg);
        cleaning();
    };

    if (GetAsyncKeyState(VK_DELETE) & 0x01) {
        cfg::plus = cfg::path_name + cfg::add_cfg + ".ini";
        cfg::delete_file = cfg::plus.c_str();
        remove(cfg::delete_file);

        config_base::delete_file::eraseFileLine("C:\\VISION\\Config_List.ini", cfg::add_cfg);
        cleaning();
    }

    ImGui::Separator();

    if (ImGui::Button("Load", ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35))) {
        config_base::load_cfg();
        cleaning();
    };

    if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
        config_base::load_cfg();
        cleaning();
    }

    if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35))) {
        config_base::add_cfg();
        cleaning();
    };

    ImGui::Separator();

    if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x - (ImGui::GetStyle().WindowPadding.x), 35)) && cfg::add_cfg[0]) {
        config_base::reset_cfg();
        config_base::add_cfg();
        cleaning();
    };

}

void config_add() {

    std::ifstream in("C:\\VISION\\Config_List.ini");
    if (in.is_open())
    {
        while (getline(in, cfg::line))
        {
            if (cfg::line.find(cfg::search_cfg) != std::string::npos && ImGui::config_selector(cfg::line.c_str(), ImVec2(ImGui::GetWindowWidth() - (ImGui::GetStyle().WindowPadding.x * 2), 45))) {

                cleaning();
                memset(&cfg::name_cfg[0], 0, sizeof(cfg::name_cfg));

                char* myChar = new char(cfg::line.length());
                for (int i = 0; i < cfg::line.length(); ++i) {
                    cfg::add_cfg[i] = cfg::line[i];
                    cfg::name_cfg[i] = cfg::line[i];
                }
            };
        }
    }
}