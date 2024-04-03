
// Pixel conversion functions

#ifndef _ICONVERSIONS_H
#define _ICONVERSIONS_H
#include <Arduino.h>
#include "screen.h"   // for h7image definition
#include "lvgl_a4.h"    // for font4bpp definitions

uint16_t Convert8888to1555(uint32_t argb8888Pixel,bool swaprb,bool transparant, uint32_t alpha0); 
uint16_t Convert8888to4444(uint32_t argb8888Pixel,bool swaprb,bool transparant, uint32_t alpha0); 
uint32_t Convert8888to8888(uint32_t argb8888Pixel,bool swaprb,bool transparant, uint32_t alpha0);
uint16_t Convert8888to565(uint32_t argb8888Pixel,bool swaprb, uint32_t alpha0); 
void convertLittleBig(uint8_t * pixel,uint8_t size);

uint16_t Convert565to1555(uint16_t rgb565Pixel,bool swaprb); 
uint16_t Convert1555to1555(uint16_t argb1555Pixel,bool swaprb,bool transparant, uint32_t alpha0); 
uint16_t Convert4444to1555(uint16_t argb4444Pixel,bool swaprb,bool transparant, uint32_t alpha0); 
#endif //_ICONVERSIONS_H