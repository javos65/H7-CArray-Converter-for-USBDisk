/*****************************************************************************
* | File      	: Image data file
* | Function    :	Demo for Portenta H7 USB-drive conversion program
* | Info        : JV 2024
* | Github      : 
*----------------
* |	This version:   V2.0
* | Date        :   2024-03-16
* | 
*
* Graphic screen Data
*
******************************************************************************/

#ifndef SCREEN_H
#define SCREEN_H
#include <Arduino.h>
#include "stm32h7xx_hal_dma2d.h" // for Video definitions


typedef struct {
  void *imagearray;   ///< Glyph bitmaps, concatenated
  uint16_t width;               /// width
  uint16_t height;              /// height
  uint32_t colormode;           /// Alpha modes : DMA2D_INPUT_ARGB1555 , DMA2D_INPUT_ARGB4444 ,DMA2D_INPUT_ARGB8888 // Fixed color modes :  DMA2D_INPUT_RGB565
  uint8_t bbp;                  /// bits per pixel => /8 bytes per pixel
  char name[24];
  char namea[24];   // array name in text
  char namesx[16];
  char namesy[16];
} h7image;

extern h7image imageC;
//
// SCREEN and GRAPHICS definitions for Game
//
//
//


#define CLEAR     0x0005
#define BLACK     0x0000
#define BLUE      0x001F
#define RED       0xF800
#define GREEN     0x07E0
#define LIME	    0xe7e0
#define CYAN      0x07FF
#define DARK_CYAN 0x0DCB
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF
#define ORANGE    0xfd20



#define ENEMYSX  36
#define ENEMYSY   30

#define PLAYERSX    36
#define PLAYERSY    30

#define ICONHEIGHT  55
#define ICONWIDTH   55

#define MINIX   12
#define MINIY   10

#define BOMBY 12
#define BOMBX 1

#define RAYY 8
#define RAYX 1


// define invader images in struture for test
extern h7image invader11;
extern h7image invader12;
extern h7image invader21;
extern h7image invader22;
extern h7image invader31;
extern h7image invader32;
extern h7image invader41;
extern h7image invader42;


extern const uint16_t shipcount[MINIY][MINIX]; // RGB565
extern const uint16_t Ray[RAYY][RAYX];        // RGB565
extern const uint16_t Bomb[BOMBY][BOMBX];     // RGB565

extern  const uint32_t laser[PLAYERSY][PLAYERSX] ;          // ARGB8888
extern  const uint32_t invader4_2[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader4_1[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader3_2[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader3_1[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader2_2[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader2_1[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader1_2[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t invader1_1[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t Iexplode[ENEMYSY][ENEMYSX];          // ARGB8888
extern  const uint32_t Iexplode2[ENEMYSY][ENEMYSX];          // ARGB8888

extern const uint16_t BLE_on[ICONHEIGHT][ICONWIDTH];       // RGB565
extern const uint16_t BLE_off[ICONHEIGHT][ICONWIDTH];       // RGB565

#endif  SCREEN_H