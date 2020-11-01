#ifndef HANGAR2_KEYCODES_H
#define HANGAR2_KEYCODES_H

#include <cstdint>
#include <array>
#include <unordered_map>

// Largest Keycode: 155

#define HGR_mouse_left   0
#define HGR_mouse_right  1
#define HGR_mouse_middle 2

#define HGR_escape         143
#define HGR_numlock        144
#define HGR_printscreen    145
#define HGR_delete         127
#define HGR_backspace      8
#define HGR_tab            9
#define HGR_capslock       146
#define HGR_enter          147
#define HGR_shift_left     148
#define HGR_shift_right    149
#define HGR_control_left   150
#define HGR_control_right  151
#define HGR_function       152
#define HGR_super          153
#define HGR_alt_left       154
#define HGR_alt_right      155

#define HGR_up    139
#define HGR_down  140
#define HGR_left  141
#define HGR_right 142

#define HGR_f1  127
#define HGR_f2  128
#define HGR_f3  129
#define HGR_f4  130
#define HGR_f5  131
#define HGR_f6  132
#define HGR_f7  133
#define HGR_f8  134
#define HGR_f9  135
#define HGR_f10 136
#define HGR_f11 137
#define HGR_f12 138

#define HGR_1 49
#define HGR_2 50
#define HGR_3 51
#define HGR_4 52
#define HGR_5 53
#define HGR_6 54
#define HGR_7 55
#define HGR_8 56
#define HGR_9 57
#define HGR_0 48

#define HGR_q 113
#define HGR_w 119
#define HGR_e 101
#define HGR_r 114
#define HGR_t 116
#define HGR_y 121
#define HGR_u 117
#define HGR_i 105
#define HGR_o 111
#define HGR_p 112

#define HGR_a 97
#define HGR_s 115
#define HGR_d 100
#define HGR_f 102
#define HGR_g 103
#define HGR_h 104
#define HGR_j 106
#define HGR_k 107
#define HGR_l 108

#define HGR_z 122
#define HGR_x 120
#define HGR_c 99
#define HGR_v 118
#define HGR_b 98
#define HGR_n 110
#define HGR_m 109

#define HGR_Q 81
#define HGR_W 87
#define HGR_E 69
#define HGR_R 82
#define HGR_T 84
#define HGR_Y 89
#define HGR_U 85
#define HGR_I 73
#define HGR_O 79
#define HGR_P 80

#define HGR_A 65
#define HGR_S 83
#define HGR_D 68
#define HGR_F 70
#define HGR_G 71
#define HGR_H 72
#define HGR_J 74
#define HGR_K 75
#define HGR_L 76

#define HGR_Z 90
#define HGR_X 88
#define HGR_C 67
#define HGR_V 86
#define HGR_B 66
#define HGR_N 78
#define HGR_M 77

#define HGR_space 32

#define HGR_tilde             126
#define HGR_backtick          96
#define HGR_exclamation_mark  33
#define HGR_at                64
#define HGR_pound             35
#define HGR_dollar            36
#define HGR_percent           37
#define HGR_caret             94
#define HGR_ampersand         38
#define HGR_asterisk          42
#define HGR_parenthesis_left  40
#define HGR_parenthesis_right 41
#define HGR_hyphen            45
#define HGR_underscore        95
#define HGR_plus              43
#define HGR_equals            61
#define HGR_bracket_left      91
#define HGR_bracket_right     93
#define HGR_brace_left        123
#define HGR_brace_right       125
#define HGR_backslash         92
#define HGR_pipe              124
#define HGR_semicolon         59
#define HGR_colon             58
#define HGR_apostrophe        39
#define HGR_quotation_mark    34
#define HGR_comma             44
#define HGR_period            46
#define HGR_less_than         60
#define HGR_greater_than      62
#define HGR_forwardslash      47
#define HGR_question_mark     63

namespace Hangar
{
    static std::array<uint32_t, 26> letterKeycodes = {
        HGR_q, HGR_w, HGR_e, HGR_r, HGR_t, HGR_y, HGR_u, HGR_i, HGR_o, HGR_p, HGR_a, HGR_s, HGR_d, HGR_f, HGR_g, HGR_h, HGR_j, HGR_k, HGR_l, HGR_z, HGR_x, HGR_c, HGR_v, HGR_b, HGR_n,
        HGR_m
    };

    static std::array<uint32_t, 21> upperableSymbols = {
        HGR_backtick, HGR_1, HGR_2, HGR_3, HGR_4, HGR_5, HGR_6, HGR_7, HGR_8, HGR_9, HGR_0, HGR_hyphen, HGR_equals, HGR_bracket_left, HGR_bracket_right, HGR_backslash, HGR_semicolon,
        HGR_apostrophe, HGR_comma, HGR_period, HGR_forwardslash
    };

    static std::array<uint32_t, 3> nonCapitalizableCharKeycodes = {
        HGR_tab, HGR_enter, HGR_space
    };

    static std::unordered_map<uint32_t, uint32_t> convertSymbolToUpper = {
        {HGR_backtick, HGR_tilde}, {HGR_1, HGR_exclamation_mark}, {HGR_2, HGR_at}, {HGR_3, HGR_pound}, {HGR_4, HGR_dollar}, {HGR_5, HGR_percent}, {HGR_6, HGR_caret},
        {HGR_7, HGR_ampersand}, {HGR_8, HGR_asterisk}, {HGR_9, HGR_parenthesis_left}, {HGR_0, HGR_parenthesis_right}, {HGR_hyphen, HGR_underscore}, {HGR_equals, HGR_plus},
        {HGR_bracket_left, HGR_brace_left}, {HGR_bracket_right, HGR_brace_right}, {HGR_backslash, HGR_pipe}, {HGR_semicolon, HGR_colon}, {HGR_apostrophe, HGR_quotation_mark},
        {HGR_comma, HGR_less_than}, {HGR_period, HGR_greater_than}, {HGR_forwardslash, HGR_question_mark}
    };
}

#endif
