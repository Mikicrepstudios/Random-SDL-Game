/**
 * @brief This namespace contains colors that you can use for all elements
 */

#pragma once

#include "mf/core.h"

namespace colors {
    // 1-8 Colors
    constexpr MF_Color black = {0, 0, 0};              // 0
    constexpr MF_Color gray = {100, 100, 100};         // 1
    constexpr MF_Color darkgray = {74, 73, 87};        // 2
    constexpr MF_Color darkblue = {5, 17, 87};         // 3
    constexpr MF_Color purple = {79, 2, 236};          // 4
    constexpr MF_Color indigo = {45, 105, 203};        // 5
    constexpr MF_Color aqua = {0, 166, 238};           // 6
    constexpr MF_Color lightblue = {111, 235, 255};    // 7

    // 9-16 Colors
    constexpr MF_Color darkgreen = {6, 54, 25};        // 8
    constexpr MF_Color leafgreen = {86, 98, 4};        // 9
    constexpr MF_Color olive = {155, 149, 0};          // 10
    constexpr MF_Color grassgreen = {17, 150, 59};     // 11
    constexpr MF_Color lightgreen = {81, 225, 19};     // 12
    constexpr MF_Color cyan = {42, 102, 106};          // 13
    constexpr MF_Color bluishgreen = {8, 162, 154};    // 14
    constexpr MF_Color sky = {8, 253, 204};            // 15

    // 17-24 Colors
    constexpr MF_Color brown = {90, 59, 28};           // 16
    constexpr MF_Color lightbrown = {174, 101, 7};     // 17
    constexpr MF_Color orange = {247, 170, 48};        // 18
    constexpr MF_Color yellow = {244, 234, 92};        // 19
    constexpr MF_Color lightgray = {172, 190, 156};    // 20
    constexpr MF_Color violet = {142, 123, 164};       // 21
    constexpr MF_Color rose = {214, 160, 144};         // 22
    constexpr MF_Color lavanderblue = {183, 192, 255}; // 23

    // 25-32 Colors
    constexpr MF_Color crimsonred = {71, 1, 31};       // 24
    constexpr MF_Color darkred = {161, 44, 50};        // 25
    constexpr MF_Color red = {254, 59, 30};            // 26
    constexpr MF_Color pink = {250, 47, 122};          // 27
    constexpr MF_Color magenta = {153, 47, 124};       // 28
    constexpr MF_Color lightpurple = {230, 28, 247};   // 29
    constexpr MF_Color lavanderrose = {251, 159, 218}; // 30
    constexpr MF_Color white = {255, 255, 255};        // 31

    // Access using colors::colorID[x] where x would be id of one of colors above
    constexpr MF_Color colorID[] = {
        black, gray, darkgray, darkblue, purple, indigo, aqua, lightblue,
        darkgreen, leafgreen, olive, grassgreen, lightgreen, cyan, bluishgreen, sky,
        brown, lightbrown, orange, yellow, lightgray, violet, rose, lavanderblue,
        crimsonred, darkred, red, pink, magenta, lightpurple, lavanderrose, white
    };
}

/* DUMPSTER
constexpr MF_Color gray = {130, 124, 112};         // 1 OLD GRAY FROM 1.0.0-1.2.0
*/