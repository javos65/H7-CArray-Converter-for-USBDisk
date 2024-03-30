/*
   QSPI as USB Mass Storage example -> c-array conversion

   This example shows how to expose a QSPIF BlockDevice (16MB external flash on the Portenta H7)
   as an USB stick. It can be adapted to any kind of BlockDevice (FlashIAP or either RAM via HeapBlockDevice)
   Before loading this example, make sure you execute WiFiFirmwareUpdater sketch
   to create and format the proper partitions.

Used to create a C-Array format converter:
Use your C-array in .h include file, convert and write to USB MAss Storage to read it back on your system for re-use
OR .. drop your png files on the usb drive and reset the board :)

Function 1: Convert all png files on the USB drive
Function 2: A4 Font format converter : Swop nibbles in the byte-wise data [7:4] and [3:0]
Function 3: C-array converter : Reduces ARGB8888 (32 bit) to ARGB15555 (16 bit) - 1 Alpha bit for transparancy blending or other formats
Function 4: read PNG files and convert to C-array by filename

Functions required for proper use of the STM32 ChromArt DMA2D functions, and online converters dont support these formats very well
Use the Portenta H7 USB Massstorage example to setup your drive properly using partition for Wifi and ota (standard setup)

https://github.com/javos65/H7-CArray-Converter-for-USBDisk

*/

#include "PluggableUSBMSD.h"
#include "QSPIFBlockDevice.h"
#include "MBRBlockDevice.h"
#include "FATFileSystem.h"
#include <PNGdec.h>
#include "lvgl_A4.h" // font arrays and structures
#include "screen.h"  // image array and structures
#include "SDRAM.h"   // needed fro large png files

#define DRAMMAXX 480
#define DRAMMAXY 320

// FOR USB READABLE  DISK
static QSPIFBlockDevice root;
mbed::MBRBlockDevice wifi_data(&root, 1);
mbed::MBRBlockDevice ota_data(&root, 2);
static mbed::FATFileSystem wifi("wifi");
static mbed::FATFileSystem ota("ota");


// FOR PNG DECODER
PNG png;
uint32_t *RAWIMAGE;
uint8_t pngcounter = 0;
REDIRECT_STDOUT_TO(Serial3)     // JV Console DEBUG for stdio info from Doom library - need to initialise Serial3 in setup()



void USBMSD::begin()
{
  int err = wifi.mount(&wifi_data);
  if (err) {
    while (!Serial);
    Serial.println("Please run WiFiFirmwareUpdater before");
    return;
  }
  ota.mount(&ota_data);
}
USBMSD MassStorage(&root);



// functions for PNG DECODER
FILE *mypfile;
void * myOpen(const char *filename, int32_t *size) {
  mypfile = fopen(filename, "r");
  if (mypfile != NULL){
      fseek(mypfile, 0L, SEEK_END); 
      uint32_t s = ftell(mypfile); rewind(mypfile);
      Serial.print("  Opened file, size");Serial.println(s);
      *size =  s;
      return (void *) mypfile;
  }
  else return NULL;
}
void myClose(void *handle) {
  //Serial.println("Closed file");
   fclose(mypfile);
}
int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  if (mypfile != NULL){  
    //Serial.print("Read ");Serial.println(length);
     return (uint32_t) fread(buffer, 1, length, mypfile);
    }
  else return NULL; 
}
int32_t mySeek(PNGFILE *handle, int32_t position) {
  if (mypfile != NULL){    
      //Serial.print("Seek ");Serial.println(position);
      rewind(mypfile);
     return (uint32_t) fseek(mypfile,position,SEEK_CUR);
    }
  else return NULL;   
}
void PNGDraw(PNGDRAW *pDraw) {
uint16_t usPixels[36];
  //png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
  //Serial.println("Draw called");
  //tft.writeRect(0, pDraw->y + 24, pDraw->iWidth, 1, usPixels);
}
// PNG read functions



void setup() {
  Serial.begin(115200);
  Serial3.begin(38400);         // JV needed for debug interface can 
  delay(3000);
  SDRAM.begin();
  RAWIMAGE= (uint32_t*)ea_malloc(DRAMMAXX*DRAMMAXY*4);  // Allocate Canvas using 480*200 pixels max
  Serial.println(" * Starting");
  MassStorage.begin();
    //Serial.println("Content of WiFi partition:");
    //printDirectory("/wifi");
    //Serial.println("Content of OTA partition:");
    //printDirectory("/ota");
char myFileName[] = "ota/info.txt"; 
FILE *myFile = fopen(myFileName, "w");
fprintf(myFile,"Test file, to see how cool this is \r\n");
fprintf(myFile,"Function 1: Convert all png files on the USB drive\r\n");
fprintf(myFile,"Function 2: A4 Font format converter : Swop nibbles in the byte-wise data [7:4] and [3:0]\r\n");
fprintf(myFile,"Function 3: C-array converter : ie Reduces ARGB8888 (32 bit) to ARGB15555 (16 bit) - incl Alpha\r\n");
fprintf(myFile,"Function 4: read PNG files and convert to C-array by filename\r\n\r\n");
fprintf(myFile,"https://github.com/javos65/H7-CArray-Converter-for-USBDisk\r\n");
fclose(myFile);
  ota.unmount();delay(1000);
  ota.mount(&ota_data);
}

void printDirectory(char* name) {
  DIR *d;
  struct dirent *p;
  d = opendir(name);
  if (d != NULL) {
    while ((p = readdir(d)) != NULL) {
      Serial.println(p->d_name);
    }
  }
  closedir(d);
}






void loop() {
  ConvertAllPNG(DMA2D_INPUT_ARGB4444);
  //ConvertPNG("invader4_1.png",DMA2D_INPUT_ARGB4444); // convert png file to 4444
  //Convert(invader12,false,DMA2D_INPUT_ARGB4444);     // convert image array to 4444
  //Convert(invader11,false,DMA2D_INPUT_ARGB1555);     // convert image array to 1555
  //ConvertA4_A4(VantaFAT24p);                         // convert fontfile A4 nibble swop
  Serial.println("Halted");
  while(1);

}







// Make your defines according your lvgl_A4 font names:
void ConvertA4_A4(font4bpp font)
{
font4bpp myfont = font;
char CFileName[30];
sprintf(CFileName,"ota/%s.h",myfont.name);
FILE *CFile = fopen(CFileName, "w");
fprintf(CFile,"// A4 / 4bbp Alpha Font Format Converter by JV\n"); 
fprintf(CFile,"// Convert A4 Litte-endian to A4 Big endian type\n"); 
fprintf(CFile,"// Swops for every byte the upper and lower nibble\n"); 
fprintf(CFile,"// Required for STM32 ChromArt DMA2D support \n\n"); 
fprintf(CFile,"const uint8_t %s[] = {\n",myfont.table); 

char c=0;
uint16_t bytew,width,ip,cc,height = myfont.height;
uint8_t  byte4,*fmap= (uint8_t*) myfont.fontmap;
uint16_t *findex = (uint16_t*) myfont.fontindex;
uint16_t charcursor=0; 

for (c=myfont.first;c<=myfont.last;++c){
      ip =  *( (uint16_t*) findex + (c-myfont.first)*2 + 1);         // pointer to first pixel
      width= *( (uint16_t*) findex + (c-myfont.first)*2 + 0);        // number of pixels wide
      if (width%2) {bytew=(width+1)/2; }                       // odd pixels, stored 1x 4bpp more, compensate cursor move.
      else { bytew=width/2;}
       fprintf(CFile,"\n // Character \"%c/\" (0x%02x,d%i), Pixelsize %i*%i",c,c,c,width,height);      
       fprintf(CFile,"\n // Character \"%c\" (0x%02x,d%i), Size: %i bytes, Index d%i,0x%02x\n",c,c,c,bytew*height,ip,ip);
      for(int t=0;t<height;++t) {
        for(int i=0;i<bytew;++i) {
            byte4 = (uint8_t) *(fmap + ip + i + t*bytew ); byte4 = (uint8_t) ((byte4<<4)&0xF0) + ((byte4 >>4)&0X0F); // swop nibbles
            fprintf(CFile,"0x%02x,",byte4);
            }
          fprintf(CFile,"\n");  
        }
      }
  fprintf(CFile,"};"); 
  fclose(CFile);
  ota.unmount();delay(1000); ota.unmount();delay(1000);
  ota.mount(&ota_data);  
  Serial.print("Created Conversionfile Font4bbp: ");  Serial.println(CFileName);
}



// Wrapper function for all single array conversions incl file headers
uint8_t Convert(h7image image, bool swaprb,uint32_t colormode){
if ( (colormode!=DMA2D_INPUT_ARGB1555) && (colormode!=DMA2D_INPUT_ARGB4444) && (colormode!= DMA2D_INPUT_RGB565) )  // DMA2D_INPUT_ARGB1555 , DMA2D_INPUT_ARGB4444 ,DMA2D_INPUT_ARGB8888 // Fixed color modes :  DMA2D_INPUT_RGB565
  { return(0); }
else   {
  char CFileName[30];
  sprintf(CFileName,"ota/%s.h",image.name);
  FILE *CFile = fopen(CFileName, "w");
  fprintf(CFile,"// C-array IMAGE Converter by JV\n"); 
  fprintf(CFile,"// Required for STM32 ChromArt DMA2D support \n");
  fprintf(CFile,"https://github.com/javos65/H7-CArray-Converter-for-USBDisk\n\n");
  fprintf(CFile,"// Red-Blue swop used : %i  \n",swaprb); 
  switch (colormode){
      case DMA2D_INPUT_ARGB1555 : Convert1555(image, swaprb, CFile) ; break;
      case DMA2D_INPUT_ARGB4444 : Convert4444(image, swaprb, CFile) ; break;
      case DMA2D_INPUT_ARGB8888 : Convert8888(image, swaprb, CFile) ; break;      
      case DMA2D_INPUT_RGB565   : Convert565(image, swaprb, CFile) ; break;
      default : break;
  }
  fclose(CFile);
  ota.unmount();delay(1000);  ota.unmount();delay(1000);
  ota.mount(&ota_data);  
  Serial.print("Created Conversionfile: ");  Serial.println(CFileName);
  return (1);
  }

}


// Make your defines according your screen.h definitions
void Convert1555(h7image image, bool swaprb, FILE *CFile)
{
uint32_t *p = (uint32_t *) image.imagearray;  

fprintf(CFile,"#define %s %i\n",image.namesy, image.height); 
fprintf(CFile,"#define %s %i\n\n",image.namesx, image.width); 
fprintf(CFile,"h7image %s ={\n",image.name); 
fprintf(CFile,"(void*) %s,%s,%s,\n",image.namea,image.namesx,image.namesy); 
fprintf(CFile,"DMA2D_INPUT_ARGB1555,16,0,0, // Alpha modes \n"); 
fprintf(CFile,"\"%s\\0\",\n",image.name);
//fprintf(CFile,"\"%s\\0\",\n",image.namea);
//fprintf(CFile,"\"%s\\0\",\n",image.namesx);
//fprintf(CFile,"\"%s\\0\",\n",image.namesy);
fprintf(CFile,"};\n\n"); 

fprintf(CFile,"const uint16_t %s[%s][%s] = {\n",image.namea,image.namesy,image.namesx); 

      for(int t=0;t<image.height;++t) {
        for(int i=0;i<image.width;++i) {
          uint32_t  argb8888Pixel = *(p+i+t*image.width);
          uint8_t blue   = (uint8_t) ((argb8888Pixel   ) & 0x000000FF);
          uint8_t green = (uint8_t) ((argb8888Pixel>>8) & 0x000000FF);
          uint8_t red  = (uint8_t) ((argb8888Pixel>>16)& 0x000000FF);
          uint8_t alpha = (uint8_t) ((argb8888Pixel>>24)& 0x000000FF);  
          if (swaprb==true) {uint8_t unu = blue; blue=red; red=unu;}                     
          uint16_t b = ((uint16_t) blue >> 3)  & 0b0000000000011111;
          uint16_t g = ((uint16_t) green <<2 ) & 0b0000001111100000;
          uint16_t r = ((uint16_t) red <<7)    & 0b0111110000000000;
          uint16_t a = 0x8000; if (alpha <0x40) a=0x0000;
          int16_t argb1555Pixel  =  (uint16_t) (a | r | g | b);
          fprintf(CFile,"0x%04x,",argb1555Pixel);
            }
          fprintf(CFile,"\n");  
        }
  fprintf(CFile,"};"); 
  if (swaprb) fprintf(CFile,"// Convert from ABGR8888 to ARGB1555\n\n");
  else fprintf(CFile,"// Convert from ARGB8888 to ARGB1555\n\n");
}


// Make your defines according your screen.h definitions
void Convert4444(h7image image,bool swaprb,FILE *CFile)
{
uint32_t *p = (uint32_t *) image.imagearray;  

fprintf(CFile,"#define %s %i\n",image.namesy, image.height); 
fprintf(CFile,"#define %s %i\n\n",image.namesx, image.width); 
fprintf(CFile,"h7image %s ={\n",image.name); 
fprintf(CFile,"(void*) %s,%s,%s,\n",image.namea,image.namesx,image.namesy); 
fprintf(CFile,"DMA2D_INPUT_ARGB4444,16,0,0, // Alpha modes \n"); 
fprintf(CFile,"\"%s\\0\",\n",image.name);
//fprintf(CFile,"\"%s\\0\",\n",image.namea);
//fprintf(CFile,"\"%s\\0\",\n",image.namesx);
//fprintf(CFile,"\"%s\\0\",\n",image.namesy);
fprintf(CFile,"};\n\n"); 

fprintf(CFile,"const uint16_t %s[%s][%s] = {\n",image.namea,image.namesy,image.namesx); 

      for(int t=0;t<image.height;++t) {
        for(int i=0;i<image.width;++i) {
          uint32_t  argb8888Pixel = *(p+i+t*image.width);
          uint8_t blue   = (uint8_t) ((argb8888Pixel   ) & 0x000000FF);
          uint8_t green = (uint8_t) ((argb8888Pixel>>8) & 0x000000FF);
          uint8_t red  = (uint8_t) ((argb8888Pixel>>16)& 0x000000FF);
          uint8_t alpha = (uint8_t) ((argb8888Pixel>>24)& 0x000000FF);    
          if (swaprb==true) {uint8_t unu = blue; blue=red; red=unu;}                
          uint16_t b = ((uint16_t) blue >> 4) & 0b0000000000001111;
          uint16_t g = ((uint16_t) green    ) & 0b0000000011110000;
          uint16_t r = ((uint16_t) red  <<4 ) & 0b0000111100000000;
          uint16_t a = ((uint16_t) alpha <<8 )& 0b1111000000000000;
          uint16_t argb4444Pixel  =  (uint16_t) (a | r | g | b);          
          fprintf(CFile,"0x%04x,",argb4444Pixel);
            }
          fprintf(CFile,"\n");  
        }
  fprintf(CFile,"};"); 
  if (swaprb) fprintf(CFile,"// Convert from ABGR8888 to ARGB4444\n\n");
  else fprintf(CFile,"// Convert from ARGB8888 to ARGB4444\n\n");
}

// Make your defines according your screen.h definitions
void Convert8888(h7image image,bool swaprb,FILE *CFile)
{
uint32_t *p = (uint32_t *) image.imagearray;  
fprintf(CFile,"#define %s %i\n",image.namesy, image.height); 
fprintf(CFile,"#define %s %i\n\n",image.namesx, image.width); 
fprintf(CFile,"h7image %s ={\n",image.name); 
fprintf(CFile,"(void*) %s,%s,%s,\n",image.namea,image.namesx,image.namesy); 
fprintf(CFile,"DMA2D_INPUT_ARGB8888,32,0,0, // Alpha modes \n"); 
fprintf(CFile,"\"%s\\0\",\n",image.name);
//fprintf(CFile,"\"%s\\0\",\n",image.namea);
//fprintf(CFile,"\"%s\\0\",\n",image.namesx);
//fprintf(CFile,"\"%s\\0\",\n",image.namesy);
fprintf(CFile,"};\n\n"); 

fprintf(CFile,"const uint32_t %s[%s][%s] = {\n",image.namea,image.namesy,image.namesx); 

      for(int t=0;t<image.height;++t) {
        for(int i=0;i<image.width;++i) {
          uint32_t  argb8888Pixel = *(p+i+t*image.width);
          uint32_t blue  =  ((argb8888Pixel    ) & 0x000000FF);
          uint32_t green =  ((argb8888Pixel>>8 ) & 0x000000FF);
          uint32_t red   =  ((argb8888Pixel>>16) & 0x000000FF);
          uint32_t alpha =  ((argb8888Pixel>>24) & 0x000000FF);    
          if (swaprb==true) {uint32_t unu = blue; blue=red; red=unu;} 
          uint32_t b=  (blue      ) & 0x000000FF;
          uint32_t g = (green <<8 ) & 0x0000FF00;  
          uint32_t r = (red  <<16 ) & 0x00FF0000;
          uint32_t a = (alpha <<24) & 0xFF000000;            
          argb8888Pixel  =  (uint32_t) (a | r | g | b);          
          fprintf(CFile,"0x%08x,",argb8888Pixel);
            }
          fprintf(CFile,"\n");  
        }
  fprintf(CFile,"};"); 
  if (swaprb) fprintf(CFile,"// Convert from ABGR8888 to ARGB8888\n\n");
  else fprintf(CFile,"// Convert from ARGB8888 to ARGB8888\n\n");
}


// Make your defines according your screen.h definitions
void Convert565(h7image image,bool swaprb,FILE *CFile)
{
uint32_t *p = (uint32_t *) image.imagearray;  
fprintf(CFile,"#define %s %i\n",image.namesy, image.height); 
fprintf(CFile,"#define %s %i\n\n",image.namesx, image.width); 
fprintf(CFile,"h7image %s ={\n",image.name); 
fprintf(CFile,"(void*) %s,%s,%s,\n",image.namea,image.namesx,image.namesy); 
fprintf(CFile,"DMA2D_INPUT_RGB565,16,0,0, // Alpha modes and position \n"); 
fprintf(CFile,"\"%s\\0\",\n",image.name);
//fprintf(CFile,"\"%s\\0\",\n",image.namea);
//fprintf(CFile,"\"%s\\0\",\n",image.namesx);
//fprintf(CFile,"\"%s\\0\",\n",image.namesy);
fprintf(CFile,"};\n\n"); 
fprintf(CFile,"const uint16_t %s[%s][%s] = {\n",image.namea,image.namesy,image.namesx); 

      for(int t=0;t<image.height;++t) {
        for(int i=0;i<image.width;++i) {
          uint32_t  argb8888Pixel = *(p+i+t*image.width);
          uint32_t blue   = ((argb8888Pixel   ) & 0x000000FF);
          uint32_t green =  ((argb8888Pixel>>8) & 0x000000FF);
          uint32_t red  =   ((argb8888Pixel>>16)& 0x000000FF);
          uint32_t alpha =  ((argb8888Pixel>>24)& 0x000000FF);
          red=(red*alpha)/0xFF; green=(green*alpha)/0xFF; blue=(blue*alpha)/0xFF;     
          if (swaprb==true) {uint32_t unu = blue; blue=red; red=unu;}          
          uint16_t b = ((uint16_t) blue >> 3 ) & 0b0000000000011111;
          uint16_t g = ((uint16_t) green <<3 ) & 0b0000011111100000;
          uint16_t r = ((uint16_t) red  <<8  ) & 0b1111100000000000;
          uint16_t rgb565Pixel  =  (uint16_t) ( r | g | b);          
          fprintf(CFile,"0x%04x,",rgb565Pixel);
            }
          fprintf(CFile,"\n");  
        }
  fprintf(CFile,"};"); 
  if (swaprb) fprintf(CFile,"// Convert from ABGR8888 to RGB565\n\n");
  else fprintf(CFile,"// Convert from ARGB8888 to RGB565\n\n");
}

void printStructure(FILE *CFile)
{
  fprintf(CFile,"\ntypedef struct {\n");
  fprintf(CFile,"void *imagearray;  // array pointer\n");
  fprintf(CFile,"uint16_t width;    // width image\n");               
  fprintf(CFile,"uint16_t height;   // heigt image\n");             
  fprintf(CFile,"uint32_t colormode;// colormode DMA2D\n");           
  fprintf(CFile,"uint16_t xpos;     // x position\n");   
  fprintf(CFile,"uint16_t ypos;     // y position\n");                  
  fprintf(CFile,"char name[24];     // name in text\n");
//  fprintf(CFile,"char namea[24];    // array name in text\n");   // for debug, you can re-use array for C-array conversion
//  fprintf(CFile,"char namesx[16];   // sizex in text\n");       // for debug, you can re-use array for C-array conversion
//  fprintf(CFile,"char namesy[16];   // sizey in text\n");       // for debug, you can re-use array for C-array conversion
  fprintf(CFile,"} h7image;         // sizey in text\n\n");
}




// Main loop, scan for all .PNG files on the card and put them in a list, then convert
void ConvertAllPNG(uint32_t colormode) {
  // extern h7image imageCONV // global defined  
  int t,i,rc, filecount = 0;
  char c,BUF[128],fname[24],list[16][24],fullname[30],objectname[24];// sprintf buffer
  const char *cpointer;
  DIR *d;
  struct dirent *p;
  d = opendir("/ota");
  if (d != NULL) {
     sprintf(BUF,"\n\n Found /ota directory, reading files\n"); Serial.print(BUF);
    while ((p = readdir(d)) != NULL) {
      if (p->d_type == DT_REG) {          // regular file
          sprintf(fname,"%s",p->d_name);
          const int len = strlen(fname);
          if ( (len > 3 && strcmp(fname + len - 3, "png") == 0) ) {
              sprintf(BUF,".PNG Image : %s\n", fname); Serial.print(BUF);
              sprintf(list[filecount],"%s",fname);   // save name incl direcory name = full path 
              filecount = filecount + 1;
          } // a PNG found
      } // a valid file found 
    } // a Directory-entry found
  } // Directory found
  closedir(d);
  if (filecount == 0) { Serial.println("No .PNG files found"); }
  else
  {
      sprintf(BUF,"%d png-files found, converting.\n\n", filecount); Serial.print(BUF);
      char CFileName[30];
        sprintf(CFileName,"ota/All_PNG_%d.h",pngcounter++);
        FILE *CFile = fopen(CFileName, "w");
        fprintf(CFile,"// C-array PNG Converter by JV\n"); 
        fprintf(CFile,"// Required for STM32 ChromArt DMA2D support \n");
        fprintf(CFile,"// Red-Blue swop used : %i  \n",true); 
        printStructure(CFile);

        for( i=0;i<filecount;++i) {
              sprintf(fullname,"ota/%s",list[i]);
              cpointer = (const char *) fullname; // make const char pointer to list
              sprintf(BUF,"  #%d file %s \n",i+1, fullname); Serial.print(BUF);
              for(t=0;t<strlen(list[i])-4;t++) {objectname[t]=list[i][t];}objectname[t]=0;    // create item name without extension png
              rc = png.open((const char *) cpointer, myOpen, myClose, myRead, mySeek, PNGDraw);
              if (rc == PNG_SUCCESS) {
                  //sprintf(BUF,"  image specs: (%d x %d), %d bpp, pixel type: %d, alpha %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType(),png.hasAlpha()); Serial.print(BUF);
                  png.setBuffer( (uint8_t *) RAWIMAGE); // receive decodedpng in sdram image buffer !!!! Red and blue are swopped ! ABGR8888, not ARGB8888
                  rc = png.decode(NULL, 0);
                  // create structure for conversion
                  imageCONV.imagearray = (uint32_t *) RAWIMAGE;                          // 32bit wide buffer ARGB8888
                  imageCONV.height=png.getHeight();
                  imageCONV.width=png.getWidth();
                  imageCONV.bbp = png.getBpp()*4;
                  imageCONV.colormode=DMA2D_INPUT_ARGB8888;
                  for(t=0;t<strlen(list[i])-4;t++) { // create item name without extension png en free from special characters
                        c = list[i][t];
                        if ( (c>0x2F) && (c!=0x5F) ) objectname[t]=c;
                        }
                        objectname[t]=0;    
                  sprintf(imageCONV.namea,"%s_array",objectname);      // arrayname
                  if (strlen(objectname) > 3 ) sprintf(imageCONV.name,"%s",objectname);
                  else sprintf(imageCONV.name,"%s__",objectname);    // item name long enough
                  for(t=0;t<strlen(objectname);t++) {if (objectname[t]>0x60) objectname[t]-=0x20;} objectname[t]=0;   // make name in capitals
                  sprintf(imageCONV.namesx,"%.5s_X",objectname);       // define X name
                  sprintf(imageCONV.namesy,"%.5s_Y",objectname);       // define y name
                  // structure complete
                  switch (colormode){
                      case DMA2D_INPUT_ARGB1555 : Convert1555(imageCONV, true, CFile) ; break;
                      case DMA2D_INPUT_ARGB4444 : Convert4444(imageCONV, true, CFile) ; break;
                      case DMA2D_INPUT_ARGB8888 : Convert8888(imageCONV, true, CFile) ; break;      
                      case DMA2D_INPUT_RGB565   : Convert565(imageCONV, true, CFile) ; break;
                      default : break;
                      }

                  png.close();
              }

      }
    fclose(CFile);
    ota.unmount();delay(1000);  ota.unmount();delay(1000);
    ota.mount(&ota_data);  
    Serial.print("Created Multiple Conversions to file: ");  Serial.println(CFileName);      
  }
}


void ConvertPNG(char * fname, uint32_t colormode)
{
// extern h7image imageCONV // global defined
char c,BUF[128],objectname[24],fullname[32]; // sprintf buffer
sprintf(fullname,"ota/%s",fname);      // create full path name 
const char *cpointer = (const char *)fullname;
int t,i,rc;
Serial.println("");
        rc = png.open((const char *)cpointer, myOpen, myClose, myRead, mySeek, PNGDraw);
       if (rc == PNG_SUCCESS) {
          sprintf(BUF,"  Image specs: (%d x %d), %d bpp, pixel type: %d, aplha %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType(),png.hasAlpha()); Serial.print(BUF);
          png.setBuffer( (uint8_t *) RAWIMAGE); // receive decodedpng in sdram image buffer !!!! Red and blue are swopped ! ABGR8888, not ARGB8888
          rc = png.decode(NULL, PNG_CHECK_CRC);
          sprintf(BUF,"  Error: %d , BuffSize %d\n", png.getLastError(), png.getBufferSize()); Serial.print(BUF);
                  // create structure for conversion
                  imageCONV.imagearray = (uint32_t *) RAWIMAGE;                          // 32bit wide buffer ARGB8888
                  imageCONV.height=png.getHeight();
                  imageCONV.width=png.getWidth();
                  imageCONV.bbp = png.getBpp()*4;
                  imageCONV.colormode=DMA2D_INPUT_ARGB8888;
                  for(t=0;t<strlen(fname)-4;t++) { // create item name without extension png en free from special characters
                        c = fname[t];
                        if ( (c>0x2F) && (c!=0x5F) ) objectname[t]=c;
                        }
                        objectname[t]=0;    
                  sprintf(imageCONV.namea,"%s_array",objectname);      // arrayname
                  if (strlen(objectname) > 5 ) sprintf(imageCONV.name,"%s",objectname);
                  else sprintf(imageCONV.name,"%sfox",objectname);    // item name long enough
                  for(t=0;t<strlen(objectname);t++) {if (objectname[t]>0x60) objectname[t]-=0x20;} objectname[t]=0;   // make name in capitals
                  sprintf(imageCONV.namesx,"%.5s_X",objectname);       // define X name
                  sprintf(imageCONV.namesy,"%.5s_Y",objectname);       // define y name
                  // structure complete
          switch (colormode){
              case DMA2D_INPUT_ARGB1555 : Convert(imageCONV, true, colormode) ; break;
              case DMA2D_INPUT_ARGB4444 : Convert(imageCONV, true, colormode) ; break;
              case DMA2D_INPUT_ARGB8888 : Convert(imageCONV, true, colormode) ; break;      
              case DMA2D_INPUT_RGB565   : Convert(imageCONV, true, colormode) ; break;
              default : break;
              }
          png.close();
       }
}

