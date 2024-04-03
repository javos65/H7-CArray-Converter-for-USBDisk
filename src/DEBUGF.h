/*****************************************************************************
* | File      	:	Debugf.h
* | Function    :	Debug with C-style printf functionality
* | Info        : JV 2024
*----------------
* |	This version:   V1.0
* | Date        :   2024-01-16
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __DEBUGF_H
#define __DEBUGF_H

#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>

#define _MYSERIALPORT Serial

#define DEBUGF_ 1
extern char _SERIALBUF[256]; // used 128 bytes Memory
extern void DEBUGBUFFER_8H(uint8_t* b,int size,int step);
extern void DEBUGBUFFER_16H(uint16_t* b,int size,int step);
extern void DEBUGBUFFER_32H(uint32_t* b,int size,int step);
extern void DEBUGBUFFER_8B(uint8_t* b,int size,int step);

#if DEBUGF_
#define DEBUGF(__info,...) {_SERIALBUF[0]=0;sprintf(_SERIALBUF,__info,##__VA_ARGS__);_MYSERIALPORT.print(_SERIALBUF);} // Debug Test
#else
	#define Debug(__info,...) ;
#endif


#endif
