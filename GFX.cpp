#include "GFX.hpp"


GFX::GFX(uint16_t const DevAddr, size Size, i2c_inst_t * i2c) : SSD1306(DevAddr, Size, i2c) {};


void GFX::drawChar(int x, int y, char chr, colors color)
{
	if(chr > 0x7E) return; // chr > '~'

	for(uint8_t i=0; i < this->font[1]; i++ )
	{
        uint8_t line = (uint8_t)(this->font)[(chr-0x20) * (this->font)[1] + i + 2];

        for(int8_t j=0; j<this->font[0]; j++, line >>= 1)
        {
            if(line & 1)
            {
            	this->drawPixel(x+i, y+j, color);
            }
        }
    }
}


void GFX::drawString(int x, int y, std::string str, colors color)
{
	int x_tmp = x;

	while(str.length())
	{
		this->drawChar(x_tmp, y, str.front(), color);
		x_tmp += ((uint8_t)font[1]) + 1;
		str.erase(str.begin());
	}
}


void GFX::drawRectangle(int x, int y, uint16_t w, uint16_t h, colors color)
{
    this->drawFastHLine(x, y, w, color);
    this->drawFastHLine(x, y+h-1, w, color);
    this->drawFastVLine(x, y, h, color);
    this->drawFastVLine(x+w-1, y, h, color);
}


void GFX::drawFillRectangle(int x, int y, uint16_t w, uint16_t h, colors color)
{
    for (int i=x; i<x+w; i++) {
    	this->drawFastVLine(i, y, h, color);
    }
}


void GFX::drawProgressBar(int x, int y, uint16_t w, uint16_t h, uint8_t progress, colors color)
{
    this->drawRectangle(x, y, w, h, color);
    this->drawFillRectangle(x, y, (uint8_t)((w*progress)/100), h, color);
}


void GFX::drawFastVLine(int x_start, int y_start, int h, colors color)
{
	this->writeLine(x_start, y_start, x_start, y_start+h-1, color);
}


void GFX::drawFastHLine(int x_start, int y_start, int w, colors color)
{
	this->writeLine(x_start, y_start, x_start+w-1, y_start, color);
}


void GFX::writeLine(int x_start, int y_start, int x_end, int y_end, colors color)
{
	int16_t steep = abs(y_end - y_start) > abs(x_end - x_start);

	    if (steep) {
	        _swap_int(x_start, y_start);
	        _swap_int(x_end, y_end);
	    }

	    if (x_start > x_end) {
	        _swap_int(x_start, x_end);
	        _swap_int(y_start, y_end);
	    }

	    int16_t dx, dy;
	    dx = x_end - x_start;
	    dy = abs(y_end - y_start);

	    int16_t err = dx / 2;
	    int16_t ystep;

	    if (y_start < y_end) {
	        ystep = 1;
	    } else {
	        ystep = -1;
	    }

	    for (; x_start<=x_end; x_start++) {
	        if (steep) {
	        	this->drawPixel(y_start, x_start, color);
	        } else {
	        	this->drawPixel(x_start, y_start, color);
	        }
	        err -= dy;
	        if (err < 0) {
	            y_start += ystep;
	            err += dx;
	        }
	    }
}

