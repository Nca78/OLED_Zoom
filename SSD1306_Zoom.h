/*!
 * @file SSD1306_Zoom.h
 *
 * 2021/06/09 Nca78
 *   "Zoom" function added to simulate led panels while having a much lower RAM footprint
 *   Changes:
 *     - deprecated constructors removed
 *     - added zoom parameter in remaining constructors and _zoom private variable
 *     - create Adafruit_GFX underlying object with dimensions divided by the zoom in the constructors
 *     - updated display method to convert a bit to multiple OLED pixels only when sending data to the oled
 *     - added SSD1306_ZoomBytes2 and SSD1306_ZoomBytes4
 *     - temporary add functions writeDisplay and clear for compatibility with Otto Eyes
 * 
 *  Original header from Adafruit below:
 */
/*
 * This is part of for Adafruit's SSD1306 library for monochrome
 * OLED displays: http://www.adafruit.com/category/63_98
 *
 * These displays use I2C or SPI to communicate. I2C requires 2 pins
 * (SCL+SDA) and optionally a RESET pin. SPI requires 4 pins (MOSI, SCK,
 * select, data/command) and optionally a reset pin. Hardware SPI or
 * 'bitbang' software SPI are both supported.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * BSD license, all text above, and the splash screen header file,
 * must be included in any redistribution.
 *
 */

#ifndef _SSD1306_Zoom_H_
#define _SSD1306_Zoom_H_

// ONE of the following three lines must be #defined:
//#define SSD1306_128_64 ///< DEPRECTAED: old way to specify 128x64 screen
#define SSD1306_128_32 ///< DEPRECATED: old way to specify 128x32 screen
//#define SSD1306_96_16  ///< DEPRECATED: old way to specify 96x16 screen
// This establishes the screen dimensions in old SSD1306_Zoom sketches
// (NEW CODE SHOULD IGNORE THIS, USE THE CONSTRUCTORS THAT ACCEPT WIDTH
// AND HEIGHT ARGUMENTS).

#if defined(ARDUINO_STM32_FEATHER)
typedef class HardwareSPI SPIClass;
#endif

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>

#if defined(__AVR__)
typedef volatile uint8_t PortReg;
typedef uint8_t PortMask;
#define HAVE_PORTREG
#elif defined(__SAM3X8E__)
typedef volatile RwReg PortReg;
typedef uint32_t PortMask;
#define HAVE_PORTREG
#elif (defined(__arm__) || defined(ARDUINO_FEATHER52)) && \
    !defined(ARDUINO_ARCH_MBED) && !defined(ARDUINO_ARCH_RP2040)
typedef volatile uint32_t PortReg;
typedef uint32_t PortMask;
#define HAVE_PORTREG
#endif

/// The following "raw" color names are kept for backwards client compatability
/// They can be disabled by predefining this macro before including the Adafruit
/// header client code will then need to be modified to use the scoped enum
/// values directly
#ifndef NO_SSD1306_Zoom_COLOR_COMPATIBILITY
#define BLACK SSD1306_BLACK     ///< Draw 'off' pixels
#define WHITE SSD1306_WHITE     ///< Draw 'on' pixels
#define INVERSE SSD1306_INVERSE ///< Invert pixels
#endif
/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

// Deprecated size stuff for backwards compatibility with old sketches
#if defined SSD1306_128_64
#define SSD1306_LCDWIDTH 128 ///< DEPRECATED: width w/SSD1306_128_64 defined
#define SSD1306_LCDHEIGHT 64 ///< DEPRECATED: height w/SSD1306_128_64 defined
#endif
#if defined SSD1306_128_32
#define SSD1306_LCDWIDTH 128 ///< DEPRECATED: width w/SSD1306_128_32 defined
#define SSD1306_LCDHEIGHT 32 ///< DEPRECATED: height w/SSD1306_128_32 defined
#endif
#if defined SSD1306_96_16
#define SSD1306_LCDWIDTH 96  ///< DEPRECATED: width w/SSD1306_96_16 defined
#define SSD1306_LCDHEIGHT 16 ///< DEPRECATED: height w/SSD1306_96_16 defined
#endif


enum SSD1306ZoomLevel {
  SSD1306ZoomX2 = 2,
  SSD1306ZoomX4 = 4,
  SSD1306ZoomX8 = 8
};


// Possible output values for x2 zoom (based of 4 bits of data in original byte)
const uint8_t SSD1306_ZoomBytes2[] = {B00000000, B00000011, B00001100, B00001111, B00110000, B00110011, B00111100, B00111111, B11000000, B11000011, B11001100, B11001111, B11110000, B11110011, B11111100, B11111111};
// Possible output values for x4 zoom (based of 2 bits of data in original byte)
const uint8_t SSD1306_ZoomBytes4[] = {B00000000, B00001111, B11110000, B11111111};


/*!
    @brief  Class that stores state and functions for interacting with
            SSD1306 OLED displays.
*/
class SSD1306_Zoom : public Adafruit_GFX
{
public:
  // NEW CONSTRUCTORS -- recommended for new projects
  SSD1306_Zoom(uint8_t w, uint8_t h, SSD1306ZoomLevel zoom, TwoWire *twi = &Wire,
               int8_t rst_pin = -1, uint32_t clkDuring = 400000UL,
               uint32_t clkAfter = 100000UL);
  SSD1306_Zoom(uint8_t w, uint8_t h, SSD1306ZoomLevel zoom, int8_t mosi_pin, int8_t sclk_pin,
               int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
  SSD1306_Zoom(uint8_t w, uint8_t h, SSD1306ZoomLevel zoom, SPIClass *spi, int8_t dc_pin,
               int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL);

  ~SSD1306_Zoom(void);

  bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0,
             bool reset = true, bool periphBegin = true);
  void display(void);
  void clearDisplay(void);
  void invertDisplay(bool i);
  void dim(bool dim);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void startscrollright(uint8_t start, uint8_t stop);
  void startscrollleft(uint8_t start, uint8_t stop);
  void startscrolldiagright(uint8_t start, uint8_t stop);
  void startscrolldiagleft(uint8_t start, uint8_t stop);
  void stopscroll(void);
  void ssd1306_command(uint8_t c);
  bool getPixel(int16_t x, int16_t y);
  uint8_t *getBuffer(void);
  // Temporary ? for compatibility with Otto Eyes
  void writeDisplay(void);
  void clear(void);

private:
  inline void SPIwrite(uint8_t d) __attribute__((always_inline));
  void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
  void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);
  void ssd1306_command1(uint8_t c);
  void ssd1306_commandList(const uint8_t *c, uint8_t n);

  SPIClass *spi;
  TwoWire *wire;
  uint8_t *buffer;
  int8_t i2caddr, vccstate, page_end;
  int8_t mosiPin, clkPin, dcPin, csPin, rstPin;
#ifdef HAVE_PORTREG
  PortReg *mosiPort, *clkPort, *dcPort, *csPort;
  PortMask mosiPinMask, clkPinMask, dcPinMask, csPinMask;
#endif
#if ARDUINO >= 157
  uint32_t wireClk;    // Wire speed for SSD1306 transfers
  uint32_t restoreClk; // Wire speed following SSD1306 transfers
#endif
  uint8_t contrast; // normal contrast setting for this device
  uint8_t zoomLevel;
#if defined(SPI_HAS_TRANSACTION)
protected:
  // Allow sub-class to change
  SPISettings spiSettings;
#endif
};

#endif // _SSD1306_Zoom_H_
