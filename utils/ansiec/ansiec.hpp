#pragma once

// External includes
/// @todo Standardize OS-specific compiler macros using conan.
// #ifdef _WIN32
// #include <windows.h>
// #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
// #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
// #endif
// #endif

// ANSI escape codes
namespace ansiec {

const char* const RESET = "\033[0m";
const char* const BOLD = "\033[1m";
const char* const FAINT = "\033[2m";
const char* const ITALIC = "\033[3m";
const char* const UNDERLINE = "\033[4m";
const char* const SLOW_BLINK = "\033[5m";
const char* const RAPID_BLINK = "\033[6m";
const char* const INVERT = "\033[7m";
const char* const HIDE = "\033[8m";
const char* const STRIKE = "\033[9m";
const char* const RESET_FONT = "\033[10m";
const char* const ALT_FONT_1 = "\033[11m";
const char* const ALT_FONT_2 = "\033[12m";
const char* const ALT_FONT_3 = "\033[13m";
const char* const ALT_FONT_4 = "\033[14m";
const char* const ALT_FONT_5 = "\033[15m";
const char* const ALT_FONT_6 = "\033[16m";
const char* const ALT_FONT_7 = "\033[17m";
const char* const ALT_FONT_8 = "\033[18m";
const char* const ALT_FONT_9 = "\033[19m";
const char* const FRAKTUR = "\033[20m";
const char* const DOUBLE_UNDERLINE = "\033[21m";
const char* const RESET_INTENSITY = "\033[22m";
const char* const NO_ITALIC = "\033[23m";
const char* const NO_UNDERLINE = "\033[24m";
const char* const NO_BLINK = "\033[25m";
const char* const PROPORTIONAL_SPACING = "\033[26m";
const char* const NO_INVERT = "\033[27m";
const char* const SHOW = "\033[28m";
const char* const NO_STRIKE = "\033[29m";
const char* const BLACK_FG = "\033[30m";
const char* const RED_FG = "\033[31m";
const char* const GREEN_FG = "\033[32m";
const char* const YELLOW_FG = "\033[33m";
const char* const BLUE_FG = "\033[34m";
const char* const MAGENTA_FG = "\033[35m";
const char* const CYAN_FG = "\033[36m";
const char* const WHITE_FG = "\033[37m";
const char* const RESET_FG = "\033[39m";
const char* const BLACK_BG = "\033[40m";
const char* const RED_BG = "\033[41m";
const char* const GREEN_BG = "\033[42m";
const char* const YELLOW_BG = "\033[43m";
const char* const BLUE_BG = "\033[44m";
const char* const MAGENTA_BG = "\033[45m";
const char* const CYAN_BG = "\033[46m";
const char* const WHITE_BG = "\033[47m";
const char* const RESET_BG = "\033[49m";
const char* const NO_PROPORTIONAL_SPACING = "\033[50m";
const char* const FRAMED = "\033[51m";
const char* const ENCIRCLED = "\033[52m";
const char* const OVERLINED = "\033[53m";
const char* const NO_ENCIRCLED_NO_OVERLINED = "\033[54m";
const char* const BLACK_BFG = "\033[90m";
const char* const RED_BFG = "\033[91m";
const char* const GREEN_BFG = "\033[92m";
const char* const YELLOW_BFG = "\033[93m";
const char* const BLUE_BFG = "\033[94m";
const char* const MAGENTA_BFG = "\033[95m";
const char* const CYAN_BFG = "\033[96m";
const char* const WHITE_BFG = "\033[97m";
const char* const BLACK_BBG = "\033[100m";
const char* const RED_BBG = "\033[101m";
const char* const GREEN_BBG = "\033[102m";
const char* const YELLOW_BBG = "\033[103m";
const char* const BLUE_BBG = "\033[104m";
const char* const MAGENTA_BBG = "\033[105m";
const char* const CYAN_BBG = "\033[106m";
const char* const WHITE_BBG = "\033[107m";

} // namespace ansiec