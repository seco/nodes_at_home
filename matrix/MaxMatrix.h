/*
 * MaxMatrix
 * Version 1.0 Feb 2013
 * Copyright 2013 Oscar Kin-Chung Au
 */

#ifndef _MaxMatrix_H_
#define _MaxMatrix_H_

#include "Arduino.h"

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

const int NUM_DISPLAY_COLS = 8 * 8;
const int NUM_BUF_COLS = 3 * NUM_DISPLAY_COLS;

class MaxMatrix
{
  private:
    byte data;
    byte load;
    byte clock;
    byte num;
    
    void reload();
    
    void shiftOutCol ( byte col, byte value );
    int maxCol;
	
  public:
    byte pixelBuffer [NUM_BUF_COLS];

    MaxMatrix(byte data, byte load, byte clock, byte num);
    
    void shiftOutMatrix ( int startCol );
    void shiftOutMatrix ( int startCol, int verticalShift );
    void printSprite ( int col, const byte* sprite );
    void printEmptyCol ( int col );
    
    void init();
    void clear();
    void setCommand(byte command, byte value);
    void setIntensity(byte intensity);
    void setColumn(byte col, byte value);
    void setColumnAll(byte col, byte value);
    void setDot(byte col, byte row, byte value);
    void writeSprite(int x, int y, const byte* sprite);
    
    void shiftLeft(bool rotate = false, bool fill_zero = true);
    void shiftRight(bool rotate = false, bool fill_zero = true);
    void shiftUp(bool rotate = false);
    void shiftDown(bool rotate = false);
};

#endif
