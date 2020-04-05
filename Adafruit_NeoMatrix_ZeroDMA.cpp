// BEFORE MAKING CHANGES TO THIS CODE: note that this is 99+% IDENTICAL to
// the Adafruit_NeoMatrix library, only small changes to the class name and
// inheriting from Adafruit_NeoPixel_ZeroDMA instead of Adafruit_NeoPixel.
// For any significant changes, you may want to make them in the original
// Adafruit_NeoMatrix instead.

/*-------------------------------------------------------------------------
  Arduino library to control single and tiled matrices of WS2811- and
  WS2812-based RGB LED devices such as the Adafruit NeoPixel Shield or
  displays assembled from NeoPixel strips, making them compatible with
  the Adafruit_GFX graphics library.  Requires both the Adafruit_NeoPixel
  and Adafruit_GFX libraries.

  Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  -------------------------------------------------------------------------
  This file is part of the Adafruit NeoMatrix ZertoDMA library.

  NeoMatrix is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoMatrix is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoMatrix.  If not, see
  <http://www.gnu.org/licenses/>.
  -------------------------------------------------------------------------*/
#include <Adafruit_NeoMatrix_ZeroDMA.h>

#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b)                                                   \
  {                                                                            \
    uint16_t t = a;                                                            \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

//
// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

/**
 * @brief Constructor for single matrix
 * @param w  width of NeoPixel matrix
 * @param h Heigth of matrix
 * @param pin Pin number (most are valid)
 * @param matrixType matrix layout flags. **Add together as needed**:
 * * Position flags for the FIRST LED in the matrix; **Pick two**, e.g.
 NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
 *  * **NEO_MATRIX_TOP**: The first LED is in the topmost row
 *  * **NEO_MATRIX_BOTTOM**: The first LED is in the bottommost row
 *  * **NEO_MATRIX_LEFT**: The first LED is in the leftmost column
 *  * **NEO_MATRIX_RIGHT**: The first LED is in the rightmost column

 * * LED arrangement flags: **Pick one**
 *  * **NEO_MATRIX_ROWS**: Horizontal rows
 *  * **NEO_MATRIX_COLUMNS**: Vertical columns

 * * NeoPixel routing: **Pick one**
 *  * **NEO_MATRIX_PROGRESSIVE**: All rows/columns proceed in the same order
 *  * **NEO_MATRIX_ZIGZAG**: Alternate lines reverse direction
 *
 * @param ledType Pixel type flags. **Add together as needed**:
 * * **NEO_KHZ800**: 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
 * * **NEO_KHZ400**: 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811
 drivers)
 * * **NEO_GRB**: Pixels are wired for GRB bitstream (most NeoPixel products)
 * * **NEO_GRBW**: Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
 * * **NEO_RGB**: Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 */
Adafruit_NeoMatrix_ZeroDMA::Adafruit_NeoMatrix_ZeroDMA(int w, int h,
                                                       uint8_t pin,
                                                       uint8_t matrixType,
                                                       neoPixelType ledType)
    : Adafruit_GFX(w, h), Adafruit_NeoPixel_ZeroDMA(w * h, pin, ledType),
      type(matrixType), matrixWidth(w), matrixHeight(h), tilesX(0), tilesY(0),
      remapFn(NULL) {}

/**
 * @brief Constructor for tiled matrices
 *
 * @param mW width of EACH NEOPIXEL MATRIX (not total display)
 * @param mH height of each matrix
 * @param tX number of matrices arranged horizontally
 * @param tY number of matrices arranged vertically
 * @param pin pin number (most are valid)
 * @param matrixType matrix layout flags. **Add together as needed**:
 * * Position flags for the FIRST LED in the matrix; **Pick two**, e.g.
 NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
 *  * **NEO_MATRIX_TOP**: The first LED is in the topmost row
 *  * **NEO_MATRIX_BOTTOM**: The first LED is in the bottommost row
 *  * **NEO_MATRIX_LEFT**: The first LED is in the leftmost column
 *  * **NEO_MATRIX_RIGHT**: The first LED is in the rightmost column

 * * LED arrangement flags: **Pick one**
 *  * **NEO_MATRIX_ROWS**: Horizontal rows
 *  * **NEO_MATRIX_COLUMNS**: Vertical columns

 * * NeoPixel routing: **Pick one**
 *  * **NEO_MATRIX_PROGRESSIVE**: All rows/columns proceed in the same order
 *  * **NEO_MATRIX_ZIGZAG**: Alternate lines reverse direction
 *
 * @param ledType Pixel type flags. **Add together as needed**:
 * * **NEO_KHZ800**: 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
 * * **NEO_KHZ400**: 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811
 drivers)
 * * **NEO_GRB**: Pixels are wired for GRB bitstream (most NeoPixel products)
 * * **NEO_GRBW**: Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
 * * **NEO_RGB**: Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 */
Adafruit_NeoMatrix_ZeroDMA::Adafruit_NeoMatrix_ZeroDMA(uint8_t mW, uint8_t mH,
                                                       uint8_t tX, uint8_t tY,
                                                       uint8_t pin,
                                                       uint8_t matrixType,
                                                       neoPixelType ledType)
    : Adafruit_GFX(mW * tX, mH * tY),
      Adafruit_NeoPixel_ZeroDMA(mW * mH * tX * tY, pin, ledType),
      type(matrixType), matrixWidth(mW), matrixHeight(mH), tilesX(tX),
      tilesY(tY), remapFn(NULL) {}

/**
 * @brief Expand 16-bit input color (Adafruit_GFX colorspace) to 24-bit
(NeoPixel) (w/gamma adjustment)
 *
 * @param color The 16-bit color to be converted
 * @return uint32_t The resulting 24-bit+gamma color
 */
static uint32_t expandColor(uint16_t color) {
  return ((uint32_t)pgm_read_byte(&gamma5[color >> 11]) << 16) |
         ((uint32_t)pgm_read_byte(&gamma6[(color >> 5) & 0x3F]) << 8) |
         pgm_read_byte(&gamma5[color & 0x1F]);
}

//
/**
 * @brief Downgrade 24-bit color to 16-bit (RGB565)
 *
 * @param r The 8-bit red value
 * @param g The 8-bit green value
 * @param b The 8-bit blue value
 * @return uint16_t The resulting 16-bit value
 */
uint16_t Adafruit_NeoMatrix_ZeroDMA::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

/**
 * @brief Set the pass-thru color
 *
 * Pass-through is a kludge that lets you override the current drawing
 *        color with a 'raw' RGB (or RGBW) value that's issued directly to
 * pixel(s) side-stepping the 16-bit color limitation of Adafruit_GFX. This is
 * not without some limitations of its own -- for example, it won't work in
 * conjunction with the background color feature when drawing text or bitmaps
 * (you'll just get a solid rect of color), only 'transparent' text/bitmaps.
 * Also, no gamma correction. Remember to UNSET the passthrough color
 * immediately when done with it by calling `setPassThruColor()` (call with no
 * value)!
 *
 * @param c The color to pass through.
 */
void Adafruit_NeoMatrix_ZeroDMA::setPassThruColor(uint32_t c) {
  passThruColor = c;
  passThruFlag = true;
}

//
/**
 * @brief Disable color passthrough
 *
 */
void Adafruit_NeoMatrix_ZeroDMA::setPassThruColor(void) {
  passThruFlag = false;
}

/**
 * @brief Set a pixel at the given (X, Y) coordinate in the given color
 *
 * @param x The X/Horizontal position of the pixel
 * @param y The Y/Vertical position of the pixel
 * @param color The 16-bit (RGB565) color to set the pixel to
 */
void Adafruit_NeoMatrix_ZeroDMA::drawPixel(int16_t x, int16_t y,
                                           uint16_t color) {

  if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
    return;

  int16_t t;
  switch (rotation) {
  case 1:
    t = x;
    x = WIDTH - 1 - y;
    y = t;
    break;
  case 2:
    x = WIDTH - 1 - x;
    y = HEIGHT - 1 - y;
    break;
  case 3:
    t = x;
    x = y;
    y = HEIGHT - 1 - t;
    break;
  }

  int tileOffset = 0, pixelOffset;

  if (remapFn) { // Custom X/Y remapping function
    pixelOffset = (*remapFn)(x, y);
  } else { // Standard single matrix or tiled matrices

    uint8_t corner = type & NEO_MATRIX_CORNER;
    uint16_t minor, major, majorScale;

    if (tilesX) { // Tiled display, multiple matrices
      uint16_t tile;

      minor = x / matrixWidth;           // Tile # X/Y; presume row major to
      major = y / matrixHeight,          // start (will swap later if needed)
          x = x - (minor * matrixWidth); // Pixel X/Y within tile
      y = y - (major * matrixHeight);    // (-* is less math than modulo)

      // Determine corner of entry, flip axes if needed
      if (type & NEO_TILE_RIGHT)
        minor = tilesX - 1 - minor;
      if (type & NEO_TILE_BOTTOM)
        major = tilesY - 1 - major;

      // Determine actual major axis of tiling
      if ((type & NEO_TILE_AXIS) == NEO_TILE_ROWS) {
        majorScale = tilesX;
      } else {
        _swap_uint16_t(major, minor);
        majorScale = tilesY;
      }

      // Determine tile number
      if ((type & NEO_TILE_SEQUENCE) == NEO_TILE_PROGRESSIVE) {
        // All tiles in same order
        tile = major * majorScale + minor;
      } else {
        // Zigzag; alternate rows change direction.  On these rows,
        // this also flips the starting corner of the matrix for the
        // pixel math later.
        if (major & 1) {
          corner ^= NEO_MATRIX_CORNER;
          tile = (major + 1) * majorScale - 1 - minor;
        } else {
          tile = major * majorScale + minor;
        }
      }

      // Index of first pixel in tile
      tileOffset = tile * matrixWidth * matrixHeight;

    } // else no tiling (handle as single tile)

    // Find pixel number within tile
    minor = x; // Presume row major to start (will swap later if needed)
    major = y;

    // Determine corner of entry, flip axes if needed
    if (corner & NEO_MATRIX_RIGHT)
      minor = matrixWidth - 1 - minor;
    if (corner & NEO_MATRIX_BOTTOM)
      major = matrixHeight - 1 - major;

    // Determine actual major axis of matrix
    if ((type & NEO_MATRIX_AXIS) == NEO_MATRIX_ROWS) {
      majorScale = matrixWidth;
    } else {
      _swap_uint16_t(major, minor);
      majorScale = matrixHeight;
    }

    // Determine pixel number within tile/matrix
    if ((type & NEO_MATRIX_SEQUENCE) == NEO_MATRIX_PROGRESSIVE) {
      // All lines in same order
      pixelOffset = major * majorScale + minor;
    } else {
      // Zigzag; alternate rows change direction.
      if (major & 1)
        pixelOffset = (major + 1) * majorScale - 1 - minor;
      else
        pixelOffset = major * majorScale + minor;
    }
  }

  setPixelColor(tileOffset + pixelOffset,
                passThruFlag ? passThruColor : expandColor(color));
}

/**
 * @brief Fill the matrix with the given color
 *
 * @param color The 16-bit (RGB565) color to set the matrix to
 */
void Adafruit_NeoMatrix_ZeroDMA::fillScreen(uint16_t color) {
  uint16_t i, n;
  uint32_t c;

  c = passThruFlag ? passThruColor : expandColor(color);
  n = numPixels();
  for (i = 0; i < n; i++)
    setPixelColor(i, c);
}
/**
 * @brief Set a function to be used to remap a pixel's `(X, Y)` coordinate to
 * its index within the strip
 *
 * @param fn Pointer to the remap function to use. The function should be the
 * form `uint16_t remapFn(uint16_t pixel_x, uint16_t pixel_y)`
 */
void Adafruit_NeoMatrix_ZeroDMA::setRemapFunction(uint16_t (*fn)(uint16_t,
                                                                 uint16_t)) {
  remapFn = fn;
}
