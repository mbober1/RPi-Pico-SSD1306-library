// #pragma once

#ifndef _GFX_H
#define _GFX_H

#include "SSD1306.hpp"
#include "font.hpp"
#include <stdlib.h>
#include <string>



class GFX : public SSD1306 {
    const uint8_t* font = font_8x5;

    public:
        GFX(uint16_t const DevAddr, size Size, i2c_inst_t * i2c);

        void drawChar(int x, int y, char chr, colors color = colors::WHITE);
        void drawString(int x, int y, std::string str, colors color = colors::WHITE);
        void drawProgressBar(int x, int y, uint16_t w, uint16_t h, uint8_t progress, colors color = colors::WHITE);
        void drawFillRectangle(int x, int y, uint16_t w, uint16_t h, colors color = colors::WHITE);
        void drawRectangle(int x, int y, uint16_t w, uint16_t h, colors color = colors::WHITE);
        void drawHorizontalLine(int x, int y, int w, colors color = colors::WHITE);
        void drawVerticalLine(int x, int y, int w, colors color = colors::WHITE);
        void drawLine(int x_start, int y_start, int x_end, int y_end, colors color = colors::WHITE);

        void setFont(const uint8_t* font);
        const uint8_t* getFont();
};

#endif
