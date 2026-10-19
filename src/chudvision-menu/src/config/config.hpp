#pragma once

#include <string>
#include <vector>

namespace config
{
    inline constexpr int kMaxConfigs = 32;
    inline constexpr int kNameLength = 64;

    enum class VarType
    {
        Bool,
        Int,
        Float,
        Color
    };

    struct VarEntry
    {
        std::string name;
        VarType type;
        void* target;
    };

    class VarRegistry
    {
    public:
        void AddBool(const char* name, bool* target);
        void AddInt(const char* name, int* target);
        void AddFloat(const char* name, float* target);
        void AddColor(const char* name, float* target);

        std::vector<VarEntry> entries;
    };

    bool TrySave(const char* name);
    bool TryLoad(const char* name);
    bool TryRemove(const char* name);

    int ListConfigs(char names[][kNameLength], int maxCount);
}
