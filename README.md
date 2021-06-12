# SSD1306_ZOOM - SSD1306 library based on Adafruit library, with added Zoom level

## Usage

Works the same way than the original Adafruit library, see here: https://github.com/adafruit/Adafruit_SSD1306
The only difference is a zoom parameter as third parameter in the constructor :
SSD1306_Zoom ematrix(128,64,SSD1306ZoomX4); //I2C OLED simulating 16x8 led matrix

Screen size is the actual screen size of the OLED, the library will divide by zoom level to create a smaller memory buffer
In example above it means the buffer is 16x8 pixels.
Allowed zoom values are 2,4,8 : SSD1306ZoomX2, SSD1306ZoomX4, SSD1306ZoomX8



## Changes
June 2020: first upload, there are still optimizations/improvements to do, but as least it does work as intended.
