// #pragma once

#ifndef _GFX_H
#define _GFX_H

#include "SSD1306.hpp"
#include "font.hpp"
#include <stdlib.h>
#include <string>

#define _swap_int(a, b) { int t = a; a = b; b = t; }


class GFX : public SSD1306 {
    const uint8_t* font = font_8x5;
    uint8_t size = 1;


    public:
        GFX(uint16_t const DevAddr, uint8_t const width, uint8_t const height, i2c_inst_t * i2c);

        void drawChar(int x, int y, char chr, colors color = colors::WHITE);
        void drawString(int x, int y, std::string str, colors color = colors::WHITE);
        void drawString(int x, int y, char* str, colors color = colors::WHITE);

        void drawProgressBar(int x, int y, uint16_t w, uint16_t h, uint8_t progress, colors color = colors::WHITE);
        void drawFillRectangle(int x, int y, uint16_t w, uint16_t h, colors color = colors::WHITE);
        void drawRectangle(int x, int y, uint16_t w, uint16_t h, colors color = colors::WHITE);
        void drawFastHLine(int x_start, int y_start, int w, colors color = colors::WHITE);
        void drawFastVLine(int x_start, int y_start, int w, colors color = colors::WHITE);
        void writeLine(int x_start, int y_start, int x_end, int y_end, colors color = colors::WHITE);

};

#endif