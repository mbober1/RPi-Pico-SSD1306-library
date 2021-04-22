#pragma once

#include "hardware/i2c.h"
#include "string.h"
#include "stdint.h"


#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2


enum class colors {
	BLACK,
	WHITE,
	INVERSE
};

enum class size {
	W128xH64,
	W128xH32
};


class SSD1306 {
	protected:
        uint16_t DevAddr;
		i2c_inst_t * i2c;
		uint8_t width;
		uint8_t height;
		size Size;
		
		unsigned char * buffer;

		void sendData(uint8_t* buffer, size_t buff_size);
		void sendCommand(uint8_t command);

	public:
		SSD1306(uint16_t const DevAddr, size Size, i2c_inst_t * i2c);
		~SSD1306();

		void displayON(uint8_t On);
		void invertColors(uint8_t Invert);
		void rotateDisplay(uint8_t Rotate);
		void setContrast(uint8_t Contrast);

		void drawPixel(int16_t x, int16_t y, colors Color = colors::WHITE);
		void clear(colors Color = colors::BLACK);
		void display(unsigned char *data = nullptr);
		uint8_t getHeight();
		uint8_t getWidth();
};