#include "SSD1306.hpp"

/*!
    @brief  Constructor for I2C-interfaced OLED display.
    @param  DevAddr
            Device i2c address shifted one to the left.
    @param  width
            Display width.
    @param  height
            Display height.
    @param  i2c
            Pointer to an existing i2c instance.
    @return SSD1306 object.
*/
SSD1306::SSD1306(uint16_t const DevAddr, size Size, i2c_inst_t * i2c) : DevAddr(DevAddr), width(width), height(height), i2c(i2c), Size(Size)
{
	this->width = 128;
	this->height = 64;
	
	this->buffer = new unsigned char[this->width*this->height/8];
	this->sendCommand(SSD1306_DISPLAYOFF);

	this->sendCommand(SSD1306_SETLOWCOLUMN);
	this->sendCommand(SSD1306_SETHIGHCOLUMN);
	this->sendCommand(SSD1306_SETSTARTLINE);
	this->sendCommand(SSD1306_MEMORYMODE);  
	this->sendCommand(0x00);  
	
	this->setContrast(0xFF);

	this->rotateDisplay(1);

	this->sendCommand(SSD1306_NORMALDISPLAY);

	this->sendCommand(0xA8);
	this->sendCommand(0x3F);

	this->sendCommand(SSD1306_SETDISPLAYOFFSET);
	this->sendCommand(0x00);

	this->sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
	this->sendCommand(0x80);

	this->sendCommand(SSD1306_SETPRECHARGE);
	this->sendCommand(0x22);

	this->sendCommand(SSD1306_SETCOMPINS);
	this->sendCommand(0x12);
	
	this->sendCommand(SSD1306_SETVCOMDETECT);	
	this->sendCommand(0x40);

	this->sendCommand(SSD1306_CHARGEPUMP);
	this->sendCommand(0x14);

	this->sendCommand(SSD1306_DISPLAYALLON_RESUME);

	this->displayON(1);
	this->clear();
	this->display();
}


/*!
    @brief  Deconstructor for I2C-interfaced OLED display.
*/
SSD1306::~SSD1306() 
{
	delete this->buffer;
}


/*!
 * @brief Send command to display.
 *
 */
void SSD1306::sendCommand(uint8_t command)
{	
	uint8_t mess[2] = {0x00, command};
	i2c_write_blocking(this->i2c, this->DevAddr, mess, 2, false);
}


/*!
 * @brief Invert colors.
 *
 */
void SSD1306::invertColors(uint8_t Invert)
{
	this->sendCommand(Invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}


/*!
 * @brief Rotate display.
 *
 */
void SSD1306::rotateDisplay(uint8_t Rotate)
{
	if(Rotate > 1) Rotate = 1;

	this->sendCommand(0xA0 | (0x01 & Rotate));  // Set Segment Re-Map Default
							// 0xA0 (0x00) => column Address 0 mapped to 127
                			// 0xA1 (0x01) => Column Address 127 mapped to 0

	this->sendCommand(0xC0 | (0x08 & (Rotate<<3)));  // Set COM Output Scan Direction
							// 0xC0	(0x00) => normal mode (RESET) Scan from COM0 to COM[N-1];Where N is the Multiplex ratio.
							// 0xC8	(0xC8) => remapped mode. Scan from COM[N-1] to COM0;;Where N is the Multiplex ratio.
}


/*!
 * @brief Turn on display.
 * 0 – Turn OFF
 * 1 – Turn ON
 */
void SSD1306::displayON(uint8_t On)
{
	this->sendCommand(On ? SSD1306_DISPLAYON : SSD1306_DISPLAYOFF);
}


/*!
 * @brief Set contrast.
 *
 */
void SSD1306::setContrast(uint8_t Contrast)
{
	this->sendCommand(SSD1306_SETCONTRAST);
	this->sendCommand(Contrast);
}


/*!
 * @brief Draw pixel in the buffer.
 * @param x position from the left edge (0, MAX WIDTH)
 * @param y position from the top edge (0, MAX HEIGHT)
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void SSD1306::drawPixel(int16_t x, int16_t y, colors Color)
{

	if ((x < 0) || (x >= this->width) || (y < 0) || (y >= this->height)) return;
	if(Size == size::W128xH32)  y = (y<<1) + 1;

	switch(Color)
	{
		case colors::WHITE:   this->buffer[x+ (y/8) * this->width] |=  (1 << (y&7)); break;
		case colors::BLACK:   this->buffer[x+ (y/8) * this->width] &= ~(1 << (y&7)); break;
		case colors::INVERSE: this->buffer[x+ (y/8) * this->width] ^=  (1 << (y&7)); break;
	}
}


/*!
 * @brief Clear the buffer.
 * @param color colors::BLACK, colors::WHITE or colors::INVERSE
 */
void SSD1306::clear(colors Color)
{
	switch (Color)
	{
		case colors::WHITE:
			memset(buffer, 0xFF, (this->height * this->width / 8));
			break;
		case colors::BLACK:
			memset(buffer, 0x00, (this->height * this->width / 8));
			break;
	}
}


/*!
 * @brief Send buffer to OLED GCRAM.
 * @param data (Optional) Pointer to data array.
 */
void SSD1306::display(unsigned char *data)
{
	if(data == nullptr) data = this->buffer;
	this->sendCommand(0x22);
	this->sendCommand(0x00);
	this->sendCommand(0x07);
	this->sendData(data, this->width*this->height/8);
}


void SSD1306::sendData(uint8_t* buffer, size_t buff_size)
{
	unsigned char mess[buff_size+1];

	mess[0] = 0x40;
	memcpy(mess+1, buffer, buff_size);

	i2c_write_blocking(this->i2c, this->DevAddr, mess, buff_size+1, false);
}


/*!
 * @brief Return display height.
 * @return display height
 */
uint8_t SSD1306::getHeight()
{
	if(this->Size == size::W128xH64) return 64;
	else return 32;
}

/*!
 * @brief Return display width.
 * @return display width
 */
uint8_t SSD1306::getWidth()
{
	return this->width;
}