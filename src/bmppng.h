
#ifndef _BMPPNG
#define _BMPPNG

/* PNGDEC DEFINITIONS FOR ERROR AND PIXEL TYPE

// source pixel type
enum {
  PNG_PIXEL_GRAYSCALE=0,
    PNG_PIXEL_TRUECOLOR=2,
    PNG_PIXEL_INDEXED=3,
    PNG_PIXEL_GRAY_ALPHA=4,
    PNG_PIXEL_TRUECOLOR_ALPHA=6
};

// Error codes returned by getLastError()
enum {
    PNG_SUCCESS = 0,
    PNG_INVALID_PARAMETER,
    PNG_DECODE_ERROR,
    PNG_MEM_ERROR,
    PNG_NO_BUFFER,
    PNG_UNSUPPORTED_FEATURE,
    PNG_INVALID_FILE,
    PNG_TOO_BIG
};
*/

//
// BMP structure according  https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
// BMP structure in picture https://upload.wikimedia.org/wikipedia/commons/7/75/BMPfileFormat.svg 
// file check : https://hexed.it/ 
//
typedef struct 
{
// Header                   //    Size  Offset Remark
  uint16_t signature;       // 	2 bytes	0000h	 'BM'
  uint32_t filesize;        // 	4 bytes	0002h	 File size in bytes
  uint32_t reserved;        //	4 bytes	0006h	 unused (=0)
  uint32_t dataoffset;      //	4 bytes	000Ah	 Offset from beginning of file to the beginning of the bitmap data
// Info header
  uint32_t headersize;	    //  4 bytes	000Eh	 Size of InfoHeader =40 
  uint32_t width;	          //  4 bytes	0012h	 Horizontal width of bitmap in pixels
  uint32_t height;	        //  4 bytes	0016h	 Vertical height of bitmap in pixels
  uint16_t planes;	        //  2 bytes	001Ah	 Number of Planes (=1)
  uint16_t bpp;             //  2 bytes	001Ch	 Bits per Pixel used to store palette entry information. 
                            //      1 = monochrome palette. NumColors = 1  
                            //      4 = 4bit palletized. NumColors = 16  
                            //      8 = 8bit palletized. NumColors = 256 
                            //      16 = 16bit RGB. NumColors = 65536
                            //      24 = 24bit RGB. NumColors = 16M
  uint32_t compression;	    //  4 bytes 001Eh  0 = BI_RGB   no compression  1 = BI_RLE8 8bit RLE encoding  2 = BI_RLE4 4bit RLE encoding
  uint32_t imagesize;	      //  4 bytes 0022h  (compressed) Size of Image  It is valid to set this =0 if Compression = 0
  uint32_t xpixelsperm;     //  4 bytes 0026h  horizontal resolution: Pixels/meter
  uint32_t ypixelsperm;     //  4 bytes 002Ah  vertical resolution: Pixels/meter
  uint32_t usedcolors;      //  4 bytes 002Eh  Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256
  uint32_t importantcolors; //  4 bytes 0032h  Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256
// Extended data for bitmasks
  uint32_t redmask;         // 4 bytes  0036h 
  uint32_t greenmask;       // 4 bytes  003Ah 
  uint32_t bluemask;        // 4 bytes  003Eh 
  uint32_t alphamask;       // 4 bytes  0042h 
  uint32_t colorspace;      // 4 bytes  0046h     
  uint32_t buffercolor;    // MODIFIED colortype of image buffer : ARGB8888 XRGB8888 etc.
  uint8_t *imagebuffer;    // Pointer to Memory containing buffer image data - excl padding, starting pixel(0,0)@(upper left)
                            // 
} BMPstruct;


#endif //BMPPNG