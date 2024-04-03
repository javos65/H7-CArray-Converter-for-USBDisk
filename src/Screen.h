/*****************************************************************************
* | File      	: Image data file
* | Function    :	Demo for Portenta H7 USB-drive conversion program
* | Info        : JV 2024
* | Github      : 
*----------------
* |	This version:   V2.0
* | Date        :   2024-03-16
* | https://github.com/javos65/H7-CArray-Converter-for-USBDisk
*
* Graphic screen Data
*
******************************************************************************/

#ifndef SCREEN_H
#define SCREEN_H
#include <Arduino.h>

// from "stm32h7xx_hal_dma2d.h" // for Video definitions _ addeed RB swapped inputs
#define COLORMODE_ARGB8888        0x00000000U  /*!< ARGB8888 color mode */
#define COLORMODE_RGB888          0x00000001U  /*!< RGB888 color mode   */
#define COLORMODE_RGB565          0x00000002U  /*!< RGB565 color mode   */
#define COLORMODE_ARGB1555        0x00000003U  /*!< ARGB1555 color mode */
#define COLORMODE_ARGB4444        0x00000004U  /*!< ARGB4444 color mode */
#define COLORMODE_L8              0x00000005U  /*!< L8 color mode       */
#define COLORMODE_AL44            0x00000006U  /*!< AL44 color mode     */
#define COLORMODE_AL88            0x00000007U  /*!< AL88 color mode     */
#define COLORMODE_L4              0x00000008U  /*!< L4 color mode       */
#define COLORMODE_A8              0x00000009U  /*!< A8 color mode       */
#define COLORMODE_A4              0x0000000AU  /*!< A4 color mode       */
#define COLORMODE_YCBCR           0x0000000BU  /*!< YCbCr color mode */

#define COLORMODE_ABGR8888        0x00000010U  /*!< ARGB8888 color mode RB swapped */
#define COLORMODE_BGR888          0x00000011U  /*!< RGB888 color mode  RB swapped  */
#define COLORMODE_BGR565          0x00000012U  /*!< RGB565 color mode  RB swapped  */
#define COLORMODE_ABGR1555        0x00000013U  /*!< ARGB1555 color mode  RB swapped*/
#define COLORMODE_ABGR4444        0x00000014U  /*!< ARGB4444 color mode RB swapped */
#define COLORMODE_A4N             0x0000001AU  /*!< A4 color mode nibbel swapped   */

#define COLORMODE_XRGB8888        0x00000020U  /*!< ARGB8888 color mode alpha ignored*/
#define COLORMODE_XRGB1555        0x00000023U  /*!< ARGB1555 color mode alpha ignored */
#define COLORMODE_XRGB4444        0x00000024U  /*!< ARGB4444 color mode alpha ignored */

#define COLORMODE_XBGR8888        0x00000030U  /*!< ARGB8888 color mode alpha ignored RB swapped*/
#define COLORMODE_XBGR1555        0x00000033U  /*!< ABGR1555 color mode alpha ignored  RB swapped*/
#define COLORMODE_XBGR4444        0x00000034U  /*!< ABGR4444 color mode alpha ignored  RB swapped*/

// ALPHA REPLACEMENT VALUES
#define ZALPHAREPLACE   0x01000000
#define NOZALPHAREPLACE 0x00000000 // color to replace near alpha-0 pixels  is XRGB8888, with X > 0x01 (any ARGB888 color with non-zero transparancy)
#define ALPHANEARZERO  0x10       // Alpha limit where below its marked and zero-alpha, and replaced by the ALPHA-0 value
#define TRANSPARANT       true
#define NONTRANSPARANT    false
#define OPAQUE            false
#define REDBLUESWAP       true
#define NOREDBLUESWAP     false

// RGB565 colors
// online 565 colors https://rgbcolorpicker.com/565
// online 1555 colors https://roccoloxprograms.github.io/1555ColorPicker/
// online 444 colors : https://www.w3schools.com/colors/colors_picker.asp
#define CLEAR_565     0x0005
#define BLACK_565     0x0000
#define BLUE_565      0x001F
#define RED_565       0xF800
#define GREEN_565     0x07E0
#define LIME_565	    0xe7e0
#define CYAN_565      0x07FF
#define DARKCYAN_565  0x0DCB
#define MAGENTA_565   0xF81F
#define YELLOW_565    0xFFE0
#define WHITE_565     0xFFFF
#define ORANGE_565    0xfd20

#define WHITE_8888  0xFFFFFFFF // opaque color
#define BLACK_8888  0xFF000000 // opaque color
#define RED_8888    0xFFFF0000 // opaque color
#define GREEN_8888  0xFF00FF00 // opaque color
#define BLUE_8888   0xFF0000FF // opaque color

// Structure for Array conversion, incl text names
typedef struct {
  void *imagearray;             /// pointer to array
  uint16_t width;               /// width
  uint16_t height;              /// height
  uint32_t colormode;           /// Alpha modes : COLORMODE_ARGB1555 , COLORMODE_ARGB4444 ,COLORMODE_ARGB8888 // Fixed color modes :  COLORMODE_RGB565
  uint8_t bpp;                  /// bits per pixel => /8 bytes per pixel
  uint16_t xpos;
  uint16_t ypos;
  char name[24];
  char namea[24];             // array name in text -
  char namesx[16];
  char namesy[16];
} h7image;

extern h7image imageCONV; // Global Used structure


// OUTPUT structure, without the un-needed text names
typedef struct {
void *imagearray;  // array pointer
uint16_t width;    // width image
uint16_t height;   // heigt image
uint32_t colormode;// colormode DMA2D
uint8_t bpp;                  /// bits per pixel => /8 bytes per pixel
uint16_t xpos;     // x position
uint16_t ypos;     // y position
char name[24];     // name in text
} G_image;         // end structure

#define INVAD_Y 30
#define INVAD_X 36

extern const uint32_t Invaders1_1_array[INVAD_Y][INVAD_X]; // return-copied test array




// example array definitions
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


// define invader images in structure for test of C-array conversion
extern h7image invader11;
extern h7image invader12;
//extern h7image invader21;
//extern h7image invader22;
//extern h7image invader31;
//extern h7image invader32;
//extern h7image invader41;
//extern h7image invader42;


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