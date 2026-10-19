//
// Created by n08i40k on 14.04.2024.
//

#ifndef TEXTWITHDESCRIPTION_H
#define TEXTWITHDESCRIPTION_H
#include <cstdarg>

namespace ImGui {
void TextWithDescription(const char* fmt, ...);
void TextWithDescriptionV(const char* fmt, va_list args);
}



#endif //TEXTWITHDESCRIPTION_H
