#include "GFX.hpp"

namespace {

	inline static void swap(int &a, int &b) 
	{
	    int tmp = a;
	    a = b;
	    b = tmp;
	}
	
};


/**
 * Create GFX instantion 
 *
 * @param DevAddr device i2c address.
 * @param Size screen size (W128xH64 or W128xH32)
 * @param i2c i2c instance
 */
GFX::GFX(uint16_t const DevAddr, size Size, i2c_inst_t * i2c) : SSD1306(DevAddr, Size, i2c) {};


/**
 * @brief Draw one char.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param chr char to be written
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
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


/**
 * @brief Draw string.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param str string to be written
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
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


/**
 * @brief Draw empty rectangle.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param w width of the rectangle
 * @param h height of the rectangle
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void GFX::drawRectangle(int x, int y, uint16_t w, uint16_t h, colors color)
{
    this->drawHorizontalLine(x, y, w, color);
    this->drawHorizontalLine(x, y+h-1, w, color);
    this->drawVerticalLine(x, y, h, color);
    this->drawVerticalLine(x+w-1, y, h, color);
}


/**
 * @brief Draw filled rectangle.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param w width of the rectangle
 * @param h height of the rectangle
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void GFX::drawFillRectangle(int x, int y, uint16_t w, uint16_t h, colors color)
{
    for (int i=x; i<x+w; i++) {
    	this->drawVerticalLine(i, y, h, color);
    }
}


/**
 * @brief Draw progress bar.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param w width of the rectangle
 * @param h height of the rectangle
 * @param progress progress (0, 100)
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void GFX::drawProgressBar(int x, int y, uint16_t w, uint16_t h, uint8_t progress, colors color)
{
    this->drawRectangle(x, y, w, h, color);
    this->drawFillRectangle(x, y, (uint8_t)((w*progress)/100), h, color);
}


/**
 * @brief Draw vertical line.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param h height of the line
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void GFX::drawVerticalLine(int x, int y, int h, colors color)
{
	this->drawLine(x, y, x, y+h-1, color);
}


/**
 * @brief Draw horizontal line.
 *
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param w width of the line
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void GFX::drawHorizontalLine(int x, int y, int w, colors color)
{
	this->drawLine(x, y, x+w-1, y, color);
}


/**
 * @brief Draw straight line.
 *
 * @param x_start position of the first point from the left edge (0, MAX WIDTH)
 * @param y_start position of the first point from the top edge  (0, MAX HEIGHT)
 * @param x_end position of the second point from the left edge (0, MAX WIDTH)
 * @param y_end position of the second point from the top edge  (0, MAX HEIGHT)
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void GFX::drawLine(int x_start, int y_start, int x_end, int y_end, colors color)
{
	int16_t steep = abs(y_end - y_start) > abs(x_end - x_start);

	if (steep) 
	{
		swap(x_start, y_start);
		swap(x_end, y_end);
	}

	if (x_start > x_end) 
	{
		swap(x_start, x_end);
		swap(y_start, y_end);
	}

	int16_t dx = x_end - x_start;
	int16_t dy = abs(y_end - y_start);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y_start < y_end) 
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	while (x_start <= x_end) 
	{
		if (steep) 
		{
			this->drawPixel(y_start, x_start, color);
		}
		else
		{
			this->drawPixel(x_start, y_start, color);
		}

		x_start++;
		err -= dy;

		if (err < 0) 
		{
			y_start += ystep;
			err += dx;
		}
	}
}


/**
 * @brief Set your own font
 *
 * @param font Pointer to array with your font
 */
void GFX::setFont(const uint8_t* font)
{
	this->font = font;
}


/**
 * @brief Get pointer to font array
 *
 * @return Pointer to array with the currently used font
 */
const uint8_t* GFX::getFont()
{
	return font;
}