#include "graphics.h"
#include <cstdlib>

namespace graphics {
    Colors* InitColors() {
        Colors* colors = static_cast<Colors*>(std::malloc(32 * sizeof(Colors)));

        // R                // G                // B
        colors[0].r = 0;    colors[0].g = 0;    colors[0].b = 0;
        colors[1].r = 130;  colors[1].g = 124;  colors[1].b = 112;
        colors[2].r = 74;   colors[2].g = 73;   colors[2].b = 87;
        colors[3].r = 5;    colors[3].g = 17;   colors[3].b = 87;
        colors[4].r = 79;   colors[4].g = 2;    colors[4].b = 236;
        colors[5].r = 45;   colors[5].g = 105;  colors[5].b = 203;
        colors[6].r = 0;    colors[6].g = 166;  colors[6].b = 238;
        colors[7].r = 111;  colors[7].g = 235;  colors[7].b = 255;

        colors[8].r = 6;    colors[8].g = 54;   colors[8].b = 25;
        colors[9].r = 86;   colors[9].g = 98;   colors[9].b = 4;
        colors[10].r = 155; colors[10].g = 149; colors[10].b = 0;
        colors[11].r = 17;  colors[11].g = 150; colors[11].b = 59;
        colors[12].r = 81;  colors[12].g = 225; colors[12].b = 19;
        colors[13].r = 42;  colors[13].g = 102; colors[13].b = 106;
        colors[14].r = 8;   colors[14].g = 162; colors[14].b = 154;
        colors[15].r = 8;   colors[15].g = 253; colors[15].b = 204;

        colors[16].r = 90;  colors[16].g = 59;  colors[16].b = 28;
        colors[17].r = 174; colors[17].g = 101; colors[17].b = 7;
        colors[18].r = 247; colors[18].g = 170; colors[18].b = 48;
        colors[19].r = 244; colors[19].g = 234; colors[19].b = 92;
        colors[20].r = 172; colors[20].g = 190; colors[20].b = 156;
        colors[21].r = 142; colors[21].g = 123; colors[21].b = 164;
        colors[22].r = 214; colors[22].g = 160; colors[22].b = 144;
        colors[23].r = 183; colors[23].g = 192; colors[23].b = 255;

        colors[24].r = 71;  colors[24].g = 1;   colors[24].b = 31;
        colors[25].r = 161; colors[25].g = 44;  colors[25].b = 50;
        colors[26].r = 254; colors[26].g = 59;  colors[26].b = 30;
        colors[27].r = 250; colors[27].g = 47;  colors[27].b = 122;
        colors[28].r = 153; colors[28].g = 47;  colors[28].b = 124;
        colors[29].r = 230; colors[29].g = 28;  colors[29].b = 247;
        colors[30].r = 251; colors[30].g = 159; colors[30].b = 218;
        colors[31].r = 255; colors[31].g = 255; colors[31].b = 255;

        return colors;
    }
}
