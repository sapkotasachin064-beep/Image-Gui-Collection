#pragma once

#include "imgui.h"

namespace ksd
{
    void PlaceRight(const ImVec2& size);

    void BeginColumn(float widthRatio = 0.f);
    void EndColumn();

    float ColumnWidth();
    float ColumnBoxHeight(float heightRatio);

    void Indent();
    void Unindent();
}
