#include <imgui.h>
#include <random>
#include <array>

float generateRandomFloat(float min, float max) {
    static std::random_device rd;  // Obtain a random number from hardware
    static std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> distr(min, max); // Define the range

    return distr(gen);
}

int Particle_rotation_start_index;
void ParticleImRotateStart()
{
    Particle_rotation_start_index = ImGui::GetForegroundDrawList()->VtxBuffer.Size;
}

ImVec2 ParticleImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetForegroundDrawList()->VtxBuffer;
    for (int i = Particle_rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}


void ParticleImRotateEnd(float rad, ImVec2 center = ParticleImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetForegroundDrawList()->VtxBuffer;
    for (int i = Particle_rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

static std::array<ImVec2, 50> particle_pos{ImVec2{0, 0}};
static std::array<float, 50> particle_speed{};
static std::array<float, 50> particle_drift{};
static std::array<float, 50> particle_rotation{};
static std::array<float, 50> particle_angular_velocity{};
static std::array<int, 50> particle_angular_size{};

static bool initialized = false;

class animatedBackground {
private:
    const int line_count = 20; // Убедитесь, что line_count <= 100

public:
    animatedBackground() {
        if (!initialized) {
            for (int i = 0; i < line_count; i++) {
                particle_pos[i].x = generateRandomFloat(ImGui::GetWindowPos().x, ImGui::GetWindowPos().x + ImGui::GetWindowSize().x);
                particle_pos[i].y = ImGui::GetWindowPos().y - generateRandomFloat(0, 250.f);
                particle_speed[i] = generateRandomFloat(100.f, 200.f); // Скорость падающего листа
                particle_drift[i] = generateRandomFloat(-50.f, 50.f); // Дрейф по горизонтали
                particle_rotation[i] = generateRandomFloat(0.f, 2 * 3.14159f); // Начальный угол вращения
                particle_angular_velocity[i] = generateRandomFloat(-1.f, 1.f); // Угол вращения скорости
                particle_angular_size[i] = generateRandomFloat(0, 5); // Размер листа
            }
            initialized = true;
        }
    }

    void Render(ImGuiWindow* window, ImColor color) {
        if (window == nullptr) return; // Проверка на nullptr

        for (int i = 0; i < line_count; i++) {
            // Отрисовка иконки падающего листа с учётом вращения
            ImVec2 pos = ImVec2(particle_pos[i].x, particle_pos[i].y);

            ImGui::PushFont(imgui_dynamic_size::medium_fonts[particle_angular_size[i]]);
            ParticleImRotateStart();
            ImGui::GetForegroundDrawList()->AddText(ImVec2(particle_pos[i].x, particle_pos[i].y), ImGui::GetColorU32(ImGuiCol_Text), ICON_RESHOT_ICON_FOREST_LEAF_LSCJ9B4X6H);
            ParticleImRotateEnd(particle_rotation[i]);
            ImGui::PopFont();

            // Обновление позиции и вращения
            particle_pos[i].y += ImGui::GetIO().DeltaTime * particle_speed[i];
            particle_pos[i].x += ImGui::GetIO().DeltaTime * particle_drift[i];
            particle_rotation[i] += ImGui::GetIO().DeltaTime * particle_angular_velocity[i];

            // Сброс аргументации, если вышло за пределы области экрана
            if (particle_pos[i].y > window->Pos.y + window->Size.y + 10.f) {
                particle_pos[i].y = window->Pos.y - generateRandomFloat(0, 250.f);
                particle_pos[i].x = generateRandomFloat(window->Pos.x, window->Pos.x + window->Size.x);
                particle_speed[i] = generateRandomFloat(100.f, 200.f);
                particle_drift[i] = generateRandomFloat(-50.f, 50.f);
                particle_rotation[i] = generateRandomFloat(0.f, 2 * 3.14159f);
                particle_angular_velocity[i] = generateRandomFloat(-1.f, 1.f);
                particle_angular_size[i] = generateRandomFloat(0, 5);
            }
        }
    }
};
