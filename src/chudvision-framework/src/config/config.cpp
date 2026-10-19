#include "config/config.hpp"

#include "gui/pages/pages.hpp"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

namespace config
{
    void VarRegistry::AddBool(const char* name, bool* target)
    {
        entries.push_back({ name, VarType::Bool, target });
    }

    void VarRegistry::AddInt(const char* name, int* target)
    {
        entries.push_back({ name, VarType::Int, target });
    }

    void VarRegistry::AddFloat(const char* name, float* target)
    {
        entries.push_back({ name, VarType::Float, target });
    }

    void VarRegistry::AddColor(const char* name, float* target)
    {
        entries.push_back({ name, VarType::Color, target });
    }

    static std::vector<VarEntry>& GetRegistry()
    {
        static std::vector<VarEntry> entries;
        if (entries.empty())
        {
            VarRegistry registry;
            pages::RegisterAimbotVars(registry);
            pages::RegisterEspVars(registry);
            pages::RegisterVisualsVars(registry);
            pages::RegisterMiscVars(registry);
            pages::RegisterPlayersVars(registry);
            pages::RegisterConfigVars(registry);
            entries = std::move(registry.entries);
        }
        return entries;
    }

    static bool TryBuildConfigDir(char* out, size_t capacity)
    {
        char exePath[MAX_PATH];
        const DWORD length = GetModuleFileNameA(nullptr, exePath, MAX_PATH);
        if (length == 0 || length >= MAX_PATH)
        {
            return false;
        }

        char* lastSlash = strrchr(exePath, '\\');
        if (lastSlash == nullptr)
        {
            return false;
        }
        *lastSlash = '\0';

        snprintf(out, capacity, "%s\\configs", exePath);
        CreateDirectoryA(out, nullptr);
        return true;
    }

    static bool TryBuildConfigPath(const char* name, char* out, size_t capacity)
    {
        if (name == nullptr || name[0] == '\0')
        {
            return false;
        }

        char directory[MAX_PATH];
        if (!TryBuildConfigDir(directory, sizeof(directory)))
        {
            return false;
        }

        snprintf(out, capacity, "%s\\%s.cfg", directory, name);
        return true;
    }

    bool TrySave(const char* name)
    {
        char path[MAX_PATH];
        if (!TryBuildConfigPath(name, path, sizeof(path)))
        {
            return false;
        }

        FILE* file = nullptr;
        if (fopen_s(&file, path, "w") != 0 || file == nullptr)
        {
            return false;
        }

        for (const VarEntry& entry : GetRegistry())
        {
            if (entry.type == VarType::Bool)
            {
                fprintf(file, "%s=%d\n", entry.name.c_str(), *(bool*)entry.target ? 1 : 0);
            }
            else if (entry.type == VarType::Int)
            {
                fprintf(file, "%s=%d\n", entry.name.c_str(), *(int*)entry.target);
            }
            else if (entry.type == VarType::Float)
            {
                fprintf(file, "%s=%.4f\n", entry.name.c_str(), *(float*)entry.target);
            }
            else
            {
                const float* color = (float*)entry.target;
                fprintf(file, "%s=%.4f %.4f %.4f %.4f\n", entry.name.c_str(), color[0], color[1], color[2], color[3]);
            }
        }

        fclose(file);
        return true;
    }

    bool TryLoad(const char* name)
    {
        char path[MAX_PATH];
        if (!TryBuildConfigPath(name, path, sizeof(path)))
        {
            return false;
        }

        FILE* file = nullptr;
        if (fopen_s(&file, path, "r") != 0 || file == nullptr)
        {
            return false;
        }

        std::unordered_map<std::string, const VarEntry*> lookup;
        for (const VarEntry& entry : GetRegistry())
        {
            lookup[entry.name] = &entry;
        }

        char line[256];
        while (fgets(line, sizeof(line), file) != nullptr)
        {
            char* separator = strchr(line, '=');
            if (separator == nullptr)
            {
                continue;
            }
            *separator = '\0';

            const auto found = lookup.find(line);
            if (found == lookup.end())
            {
                continue;
            }

            const VarEntry& entry = *found->second;
            const char* payload = separator + 1;

            if (entry.type == VarType::Bool)
            {
                *(bool*)entry.target = atoi(payload) != 0;
            }
            else if (entry.type == VarType::Int)
            {
                *(int*)entry.target = atoi(payload);
            }
            else if (entry.type == VarType::Float)
            {
                *(float*)entry.target = (float)atof(payload);
            }
            else
            {
                float* color = (float*)entry.target;
                sscanf_s(payload, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3]);
            }
        }

        fclose(file);
        return true;
    }

    bool TryRemove(const char* name)
    {
        char path[MAX_PATH];
        if (!TryBuildConfigPath(name, path, sizeof(path)))
        {
            return false;
        }
        return DeleteFileA(path) != 0;
    }

    int ListConfigs(char names[][kNameLength], int maxCount)
    {
        char directory[MAX_PATH];
        if (!TryBuildConfigDir(directory, sizeof(directory)))
        {
            return 0;
        }

        char pattern[MAX_PATH];
        snprintf(pattern, sizeof(pattern), "%s\\*.cfg", directory);

        WIN32_FIND_DATAA found;
        HANDLE search = FindFirstFileA(pattern, &found);
        if (search == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        int count = 0;
        do
        {
            if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                continue;
            }

            char* extension = strrchr(found.cFileName, '.');
            if (extension != nullptr)
            {
                *extension = '\0';
            }

            strncpy_s(names[count], kNameLength, found.cFileName, _TRUNCATE);
            count++;
        } while (count < maxCount && FindNextFileA(search, &found) != 0);

        FindClose(search);
        return count;
    }
}
