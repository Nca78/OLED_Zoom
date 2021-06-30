# OLED_ZOOM - SSD1306 and SH1106 library based on Adafruit library, with zoom, partial refresh and direct drawing of bitmaps to OLED

## Usage

Works the same way than the original SSD1306 Adafruit library, see here: https://github.com/adafruit/Adafruit_SSD1306
Differences with original library are listed below

### SH1106 support
Some OLEDs use SH1106 controller but might be sold as SSD1306. Usually these are 1.3'' size. Those OLED will only show picture on the top of the screen and then garbage.
In that case you can say it to the library when initializing the OLED in the begin method by setting the new isSH1106 parameter to true:
```C++
matrix.beginDefault(SSD1306_SWITCHCAPVCC, 0x3D, true, true, **true**); // pass in the address
```

To make things more simple I added a beginDefault that will only take I2C address and an optional SH1106 boolean, setting other parameters with their default values.
For an SSD1306 OLED:
```C++
matrix.beginDefault(0x3D); // pass in the address
```
or
```C++
matrix.beginDefault(0x3D, false); // pass in the address
```

For an SH1106 OLED:
```C++
matrix.beginDefault(0x3C, true); // pass in the address
```

### Zoom level
Zoom level is added as third parameter in the constructor :
```C++
SSD1306_Zoom ematrix(128,64,OLEDZoomX8); //I2C OLED simulating 16x8 led matrix
```
Screen size is the actual screen size of the OLED, the library will divide the original size by the zoom level to create a smaller memory buffer
In example above it means the buffer is 16x8 pixels.
Allowed zoom values are 2,4,8 and no Zoom : *OLEDZoomX2, OLEDZoomX4, OLEDZoomX8, OLEDZoomNoZoom*

*OLEDZoomNoZoom* will create a full sized buffer but allow to use the partial refresh for faster update of the OLED.

In X4 and X8 zoom the display is made with small circles to simulate a LED matrix.

### Partial refresh
Instead of using display() method to refresh the full screen, you can use displayArea to refresh only a part of the screen.
This is especially useful when using an I2C OLED as the data transfer is slow
```C++
void displayArea(uint8_t x1, uint8_t y1, uint8_t aWidth, uint8_t aHeight);
```
x1 and y1 are origin coordinates of the rectangle to refresh
aWidth and aHeight are the width and height of the area you want to refresh.

All these coordinates are in zoomed dimensions and *not* the screen native resolution.

Please note that the OLED screen is managing data in pages (rows) 8 pixels high, so the vertical coordinates you pass will be rounded to these rows

### Direct drawing of bitmaps
It is possible to draw a bitmap directly to the OLED screen instead of drawing it in the buffer with drawBitmap and then updating the screen with display() or displayArea().
It will save some data processing and refresh screen automatically for the area you updated with the bitmap.
Another advantage is the bitmap can be drawn with the zoom level you want: even if your OLED is configured in 8X zoom, you can draw a full resolution bitmap to the screen without using a huge memory buffer.
```C++
void displayBitmap(uint8_t x, uint8_t page, const uint8_t bitmap[], uint8_t bWidth, uint8_t bHeight, SSD1306ZoomLevel zoom, bool mirror, bool invert);
```
- x is the horizontal coordinate where to draw the bitmap. This is in OLED full resolution and not influenced by the zoom.
- y is the vertical position where the bitmap will be drawn. This is in OLED full resolution and not influenced by the zoom. Data is managed by pages of 8 pixels high by the OLED, so the y coordinate will be rounded to the lower page. To know exactly where your bitmap will be drawn, set y to one of the following values 0,7,15,23,31,39,47,55
- bitmap[] is the data of the bitmap, in the same format than for drawBitmap method of the original library
- bWidth is the width of the bitmap
- bHeight is the height of the bitmap. Be careful here again it will be rounded to an entire page. For example if there is no zoom and bitmap is 20 pixel high, the bitmap will be drawn on a height of 24 pixels (3 pages) and the missing pixels at the bottom will be off (black color)
- zoom is the zoom level the bitmap will be drawn with. This is independant of the zoom level configured for the OLED screen
- mirror will flip the bitmap horizontally
- invert will make *on* pixels of the bitmap appear *off* on the screen, and *off* pixels *on*.

## Changes
June 30, 2021: name change, add OLEDZoomNoZoom, add partial refresh, add direct bitmap drawing, make this readme file a bit more useful

June 12, 2021: first upload, there are still optimizations/improvements to do, but as least it does work as intended


