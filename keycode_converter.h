#ifndef HANGAR2_KEYCODE_CONVERTER_H
#define HANGAR2_KEYCODE_CONVERTER_H

#include <unordered_map>

#include "keycodes.h"

/*

HGR_tilde
HGR_exclamation_mark
HGR_at
HGR_pound
HGR_dollar
HGR_percent
HGR_caret
HGR_ampersand
HGR_asterisk
HGR_parenthesis_left
HGR_parenthesis_right
HGR_underscore
HGR_plus
HGR_brace_left
HGR_brace_right
HGR_pipe
HGR_colon
HGR_quotation_mark
HGR_less_than
HGR_greater_than
HGR_question_mark

*/

namespace Hangar
{
    #ifdef __linux__
    // {0, 0},
        std::unordered_map<int, int> convertKeycode = {
            {0, HGR_mouse_left}, {1, HGR_mouse_right}, {2, HGR_mouse_middle},
            {XK_Escape, HGR_escape}, {65407, HGR_numlock}, {HGR_printscreen, HGR_printscreen}, {XK_Delete, HGR_delete}, {XK_BackSpace, HGR_backspace}, {XK_Tab, HGR_tab},
            {XK_Caps_Lock, HGR_capslock}, {XK_Return, HGR_enter}, {XK_Shift_L, HGR_shift_left}, {XK_Shift_R, HGR_shift_right}, {XK_Control_L, HGR_control_left}, {XK_Control_R, HGR_control_right},
            {HGR_function, HGR_function}, {XK_Super_L, HGR_super}, {XK_Super_R, HGR_super}, {XK_Alt_L, HGR_alt_left}, {XK_Alt_R, HGR_alt_right}, {XK_Up, HGR_up}, {XK_Up, HGR_down},
            {XK_Left, HGR_left}, {XK_Right, HGR_right}, {XK_F1, HGR_f1}, {XK_F2, HGR_f2}, {XK_F3, HGR_f3}, {XK_F4, HGR_f4}, {XK_F5, HGR_f5}, {XK_F6, HGR_f6}, {XK_F7, HGR_f7},
            {XK_F8, HGR_f8}, {XK_F9, HGR_f9}, {XK_F10, HGR_f10}, {XK_F11, HGR_f11}, {XK_F12, HGR_f12}, {XK_1, HGR_1}, {XK_2, HGR_2}, {XK_3, HGR_3}, {XK_4, HGR_4},
            {XK_5, HGR_5}, {XK_6, HGR_6}, {XK_7, HGR_7}, {XK_8, HGR_8}, {XK_9, HGR_9}, {XK_0, HGR_0}, {XK_q, HGR_q}, {XK_w, HGR_w}, {XK_e, HGR_e}, {XK_r, HGR_r}, {XK_t, HGR_t},
            {XK_y, HGR_y}, {XK_u, HGR_u}, {XK_i, HGR_i}, {XK_o, HGR_o}, {XK_p, HGR_p}, {XK_a, HGR_a}, {XK_s, HGR_s}, {XK_d, HGR_d}, {XK_f, HGR_f}, {XK_g, HGR_g}, {XK_h, HGR_h},
            {XK_j, HGR_j}, {XK_k, HGR_k}, {XK_l, HGR_l}, {XK_z, HGR_z}, {XK_x, HGR_x}, {XK_c, HGR_c}, {XK_v, HGR_v}, {XK_b, HGR_b}, {XK_n, HGR_n}, {XK_m, HGR_m}, {XK_Q, HGR_Q},
            {XK_W, HGR_W}, {XK_E, HGR_E}, {XK_R, HGR_R}, {XK_T, HGR_T}, {XK_Y, HGR_Y}, {XK_U, HGR_U}, {XK_I, HGR_I}, {XK_O, HGR_O}, {XK_P, HGR_P}, {XK_A, HGR_A}, {XK_S, HGR_S},
            {XK_D, HGR_D}, {XK_F, HGR_F}, {XK_G, HGR_G}, {XK_H, HGR_H}, {XK_J, HGR_J}, {XK_K, HGR_K}, {XK_L, HGR_L}, {XK_Z, HGR_Z}, {XK_X, HGR_X}, {XK_C, HGR_C}, {XK_V, HGR_V},
            {XK_B, HGR_B}, {XK_N, HGR_N}, {XK_M, HGR_M}, {XK_space, HGR_space}, {96, HGR_backtick}, {XK_hyphen, HGR_hyphen}, {XK_equal, HGR_equals}, {XK_bracketleft, HGR_bracket_left},
            {XK_bracketright, HGR_bracket_right}, {XK_backslash, HGR_backslash}, {XK_semicolon, HGR_semicolon}, {XK_apostrophe, HGR_apostrophe}, {XK_comma, HGR_comma},
            {XK_period, HGR_period}, {XK_slash, HGR_fowardslash}
        };
    #endif // __linux__
}

#endif
