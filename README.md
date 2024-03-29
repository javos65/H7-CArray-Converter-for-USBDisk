# C-array converter for image files an Fonts
![logo](/images/Iexplode.png?raw=true)
## A handy program tool for generating C-array for graphics - images and / or fonts
____

## About

Annoyed by the limited online conversion possibilites, this tools makes it easier to generate your versions+
It make use of the USB-Disk option to read and write files o nthe FatFilesystem.+
+
This code was made by using Portenta H7 (and HAT expansion board) supporting HDMI over USB-C+
The STM32H7 has a fancy Chromart accelerator, using a special 2D DMA function (DMA2D) to transfer +
Images to memory, including format and blending. Unfortunatly, its not easy to start with the right file formats.+
Best online C-array programs are as folllows:

[C-Array converter for images](https://notisrac.github.io/FileToCArray/)
[C-Array converter for fonts](https://lvgl.io/tools/font_conv_v5_3)

+
Still some formats are hard to get like ARGB4444, ARGB1555 or the nibble-swopped A4 for fonts.

![logo](/images/Laser.png?raw=true)

### Wiring

Connect H7 to HAT expansion board , load the program, and fiddle around.
*  Portenta Carrier :     Portenta Hat Carrier  SKU ASX00049 Barcode 7630049204041
*  Portenta board :       Portenta H7           SKU ABX00042 Barcode 7630049202252 

___

### Features:
+ Convert A4 fonts to the right nibble posisiton (swopping nibles in the byte)
+ Convert exsisting C-array into new arrays, swopping Red/Blue or to creating Alpha channels
+ Convert single PNG file into C-array
+ Convert all PNG files on the USB-drive into one C-array file, incl structures


___

## To do:
+ adding mote Font support
+ adding BMP-format read support

___

### And please, don't ask about the 0xB19B00B5

