// BEFORE MAKING CHANGES TO THIS CODE: note that this is 99+% IDENTICAL to
// the Adafruit_NeoMatrix library, only small changes to the class name and
// inheriting from Adafruit_NeoPixel_ZeroDMA instead of Adafruit_NeoPixel.
// For any significant changes, you may want to make them in the original
// Adafruit_NeoMatrix instead.

/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoMatrix ZeroDMA library.

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
  --------------------------------------------------------------------*/

#ifndef _ADAFRUIT_NEOMATRIX_ZERODMA_H_
#define _ADAFRUIT_NEOMATRIX_ZERODMA_H_

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include <Arduino.h>
#include <gamma.h>
// Matrix layout information is passed in the 'matrixType' parameter for
// each constructor (the parameter immediately following is the LED type
// from NeoPixel.h).

// These define the layout for a single 'unified' matrix (e.g. one made
// from NeoPixel strips, or a single NeoPixel shield), or for the pixels
// within each matrix of a tiled display (e.g. multiple NeoPixel shields).

#define NEO_MATRIX_TOP 0x00         // Pixel 0 is at top of matrix
#define NEO_MATRIX_BOTTOM 0x01      // Pixel 0 is at bottom of matrix
#define NEO_MATRIX_LEFT 0x00        // Pixel 0 is at left of matrix
#define NEO_MATRIX_RIGHT 0x02       // Pixel 0 is at right of matrix
#define NEO_MATRIX_CORNER 0x03      // Bitmask for pixel 0 matrix corner
#define NEO_MATRIX_ROWS 0x00        // Matrix is row major (horizontal)
#define NEO_MATRIX_COLUMNS 0x04     // Matrix is column major (vertical)
#define NEO_MATRIX_AXIS 0x04        // Bitmask for row/column layout
#define NEO_MATRIX_PROGRESSIVE 0x00 // Same pixel order across each line
#define NEO_MATRIX_ZIGZAG 0x08      // Pixel order reverses between lines
#define NEO_MATRIX_SEQUENCE 0x08    // Bitmask for pixel line order

// These apply only to tiled displays (multiple matrices):

#define NEO_TILE_TOP 0x00         // First tile is at top of matrix
#define NEO_TILE_BOTTOM 0x10      // First tile is at bottom of matrix
#define NEO_TILE_LEFT 0x00        // First tile is at left of matrix
#define NEO_TILE_RIGHT 0x20       // First tile is at right of matrix
#define NEO_TILE_CORNER 0x30      // Bitmask for first tile corner
#define NEO_TILE_ROWS 0x00        // Tiles ordered in rows
#define NEO_TILE_COLUMNS 0x40     // Tiles ordered in columns
#define NEO_TILE_AXIS 0x40        // Bitmask for tile H/V orientation
#define NEO_TILE_PROGRESSIVE 0x00 // Same tile order across each line
#define NEO_TILE_ZIGZAG 0x80      // Tile order reverses between lines
#define NEO_TILE_SEQUENCE 0x80    // Bitmask for tile line order

/**
 * @brief Class to control NeoPixel matricies using DMA
 *
 */
class Adafruit_NeoMatrix_ZeroDMA : public Adafruit_GFX,
                                   public Adafruit_NeoPixel_ZeroDMA {

public:
  // Constructor for single matrix:
  Adafruit_NeoMatrix_ZeroDMA(int w, int h, uint8_t pin = 5,
                             uint8_t matrixType = NEO_MATRIX_TOP +
                                                  NEO_MATRIX_LEFT +
                                                  NEO_MATRIX_ROWS,
                             neoPixelType ledType = NEO_GRB + NEO_KHZ800);

  // Constructor for tiled matrices:
  Adafruit_NeoMatrix_ZeroDMA(
      uint8_t matrixW, uint8_t matrixH, uint8_t tX, uint8_t tY, uint8_t pin = 5,
      uint8_t matrixType = NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS +
                           NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS,
      neoPixelType ledType = NEO_GRB + NEO_KHZ800);

  void drawPixel(int16_t x, int16_t y, uint16_t color),
      fillScreen(uint16_t color), setPassThruColor(uint32_t c),
      setPassThruColor(void),
      setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t));

  static uint16_t Color(uint8_t r, uint8_t g, uint8_t b);

private:
  const uint8_t type;
  const uint8_t matrixWidth, matrixHeight, tilesX, tilesY;
  uint16_t (*remapFn)(uint16_t x, uint16_t y);

  uint32_t passThruColor;
  boolean passThruFlag = false;
};

#endif // _ADAFRUIT_NEOMATRIX_ZERODMA_H_
