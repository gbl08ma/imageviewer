/*----------------------------------------------------------------------------/
/ JPEG and PNG Image Viewer for the Casio Prizm
/ Copyright 2014 tny. internet media
/ http://i.tny.im  /  admin@tny.im
/-----------------------------------------------------------------------------/
/ Copyright (c) 2014, Gabriel Maia and the tny. internet media group
/ All rights reserved.
/
/ Redistribution and use in source and binary forms, with or without
/ modification, are permitted provided that the following conditions are met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this list of conditions and the following disclaimer.
/
/ 2. Redistributions in binary form must reproduce the above copyright notice,
/    this list of conditions and the following disclaimer in the documentation
/    and/or other materials provided with the distribution.
/
/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/ IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/ ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/ LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/ CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/ SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/ CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/ ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/ POSSIBILITY OF SUCH DAMAGE.
/
/----------------------------------------------------------------------------*/
#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/misc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/heap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <png.h>
#include <zlib.h>

#include "imageGUI.hpp"
#include "menuGUI.hpp"
#include "graphicsProvider.hpp"
#include "tjpgd.h"

void backupOCRAM2(void* buffer) {
  memcpy(buffer, (char*)0xE5017000, 8192);
}

void restoreOCRAM2(void* buffer) {
  memcpy((char*)0xE5017000, buffer, 8192);
}

void errorMessage(char* l1, char* l2, char* l3) {
  MsgBoxPush(4);
  mPrintXY(3, 2, l1, TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
  mPrintXY(3, 3, l2, TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
  mPrintXY(3, 4, l3, TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
  PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 2, TEXT_COLOR_BLACK); // press exit message
  while(1) {
    int key;
    GetKey(&key);
    if(key == KEY_CTRL_EXIT || key == KEY_CTRL_AC) break;
  }
  MsgBoxPop();
}

int ipow(int base, int exp)
{
  int result = 1;
  while (exp) {
    if (exp & 1) result *= base;
    exp >>= 1;
    base *= base;
  }
  return result;
}

/**************************************************
 * JPEG viewing code
**************************************************/

void viewJPEGimage(char* filename, void* ocram2backup) {
  void *work;     /* Pointer to the decompressor work area */
  JDEC jdec;    /* Decompression object */
  IODEV devid;    /* User defined device identifier */

  /* Allocate a work area for TJpgDec */
  work = (char*)0xE5007000; // first "additional on-chip RAM area", 8192 bytes
  devid.xoff = 0;
  devid.yoff = 0;
  int scale = 0;
  unsigned short filenameshort[0x10A];
  Bfile_StrToName_ncpy(filenameshort, filename, 0x10A);
  while(1) {
    restoreOCRAM2(ocram2backup); // see note on main.cpp
    int key;
    Bdisp_AllClr_VRAM();
    /* Open a JPEG file */
    devid.fp = Bfile_OpenFile_OS(filenameshort, READWRITE, 0);
    if (devid.fp < 0) {
      errorMessage((char*)"An error occurred", (char*)"(failed to open", (char*)"file)");
      return;
    }

    /* Prepare to decompress */
    JRESULT res = jd_prepare(&jdec, jpeg_in_func, work, 8190, &devid);
    if (res == JDR_OK) {
      /* Ready to decompress. Image info is available here. */
      int sdiv = ipow(2, scale);
      if(jdec.width/sdiv < LCD_WIDTH_PX) {
        devid.xoff = -(LCD_WIDTH_PX/2 - (jdec.width/sdiv)/2);
      } else {
        if(devid.xoff<0) devid.xoff = 0;
        if(devid.xoff>(int)jdec.width/sdiv-LCD_WIDTH_PX) devid.xoff=jdec.width/sdiv-LCD_WIDTH_PX;
      }
      if(jdec.height/sdiv < LCD_HEIGHT_PX) {
        devid.yoff = -(LCD_HEIGHT_PX/2 - (jdec.height/sdiv)/2);
      } else {
        if(devid.yoff<0) devid.yoff = 0;
        if(devid.yoff>(int)jdec.height/sdiv-LCD_HEIGHT_PX) devid.yoff=jdec.height/sdiv-LCD_HEIGHT_PX;
      }
      res = jd_decomp(&jdec, jpeg_out_func, scale);   /* Start to decompress with set scaling */
      if (res == JDR_OK || res == JDR_INTR) {
        /* Decompression succeeded or was stopped by output function which determined enough had been decoded.
         * You have the decompressed image in the frame buffer here. */
      } else {
        Bfile_CloseFile_OS(devid.fp);
        errorMessage((char*)"An error occurred", (char*)"(failed to", (char*)"decompress)");
        return;
      }

    } else {
      Bfile_CloseFile_OS(devid.fp);
      errorMessage((char*)"An error occurred", (char*)"(failed to", (char*)"prepare)");
      return;
    }

    Bfile_CloseFile_OS(devid.fp);     /* Close the JPEG file */
    int inkeyloop = 1;
    EnableStatusArea(3);
    while(inkeyloop) {
      GetKey(&key);
      switch(key) {
        case KEY_CTRL_EXIT:
          EnableStatusArea(0);
          return;
        case KEY_CTRL_DOWN:
          devid.yoff += 64;
          inkeyloop = 0;
          break;
        case KEY_CTRL_UP:
          devid.yoff -= 64;
          inkeyloop = 0;
          break;
        case KEY_CTRL_RIGHT:
          devid.xoff += 64;
          inkeyloop = 0;
          break;
        case KEY_CTRL_LEFT:
          devid.xoff -= 64;
          inkeyloop = 0;
          break;
        case KEY_CHAR_2:
          devid.yoff += LCD_HEIGHT_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_8:
          devid.yoff -= LCD_HEIGHT_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_6:
          devid.xoff += LCD_WIDTH_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_4:
          devid.xoff -= LCD_WIDTH_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_7:
          devid.xoff -= LCD_WIDTH_PX;
          devid.yoff -= LCD_HEIGHT_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_9:
          devid.xoff += LCD_WIDTH_PX;
          devid.yoff -= LCD_HEIGHT_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_1:
          devid.xoff -= LCD_WIDTH_PX;
          devid.yoff += LCD_HEIGHT_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_3:
          devid.xoff += LCD_WIDTH_PX;
          devid.yoff += LCD_HEIGHT_PX;
          inkeyloop = 0;
          break;
        case KEY_CHAR_MINUS:
          if(scale < 3) {
            scale++;
            devid.xoff /= 2;
            devid.yoff /= 2;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_PLUS:
          if(scale > 0) {
            scale--;
            devid.xoff *= 2;
            devid.yoff *= 2;
            inkeyloop = 0;
          }
          break;
      }
    }
  }
}

UINT jpeg_in_func (JDEC* jd, BYTE* buff, UINT nbyte)
{
  IODEV *dev = (IODEV*)jd->device;   /* Device identifier for the session (5th argument of jd_prepare function) */


  if (buff) {
  /* Read bytes from input stream */
  return Bfile_ReadFile_OS(dev->fp, buff, nbyte, -1);
  } else {
  /* Remove bytes from input stream */
  int br = Bfile_SeekFile_OS(dev->fp, Bfile_TellFile_OS(dev->fp)+nbyte);
  if(br < 0) return 0;
  else return nbyte;
  }
}
typedef display_graph TDispGraph;
UINT jpeg_out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
  IODEV *dev = (IODEV*)jd->device;
  TDispGraph Graph;
  Graph.x = rect->left - dev->xoff;
  Graph.y = rect->top - dev->yoff;
  if(Graph.x > LCD_WIDTH_PX) return 1; // out of bounds, no need to display
  if(Graph.y > LCD_HEIGHT_PX) return 0; // no need to decode more, it's already out of vertical bound
  Graph.xofs = 0;
  Graph.yofs = 0;
  Graph.width = rect->right - rect->left + 1;
  Graph.height = rect->bottom - rect->top + 1;
  Graph.colormode = 0x02;
  Graph.zero4 = 0x00;
  Graph.P20_1 = 0x20;
  Graph.P20_2 = 0x20;
  Graph.bitmap = (int)bitmap;
  Graph.color_idx1 = 0x00;
  Graph.color_idx2 = 0x05;
  Graph.color_idx3 = 0x01;
  Graph.P20_3 = 0x01;
  Graph.writemodify = 0x01;
  Graph.writekind = 0x00;
  Graph.zero6 = 0x00;
  Graph.one1 = 0x00;
  Graph.transparency = 0;
  Bdisp_WriteGraphVRAM( &Graph );

  return 1;  /* Continue to decompress */
}

/**************************************************
 * PNG viewing code
**************************************************/
/*extern "C" {
// this is needed for libpng:
void abort(void) __attribute__ ((used));
void abort(void) {
  int x=0,y=160;
  PrintMini(&x,&y, (char*)"An error occurred, MENU to exit.",0,0xFFFFFFFF,0,0,0xFFFF,0,1,0);
  int key;
  while(1)
    GetKey(&key);
}
}*/

void viewPNGimage(char* filename) {
  int startx = 0;
  int starty = 0;
  int mode = 0;
  while(1) {
    Bdisp_AllClr_VRAM();
    //load png file
    FILE *fp = fopen(filename, "rb");
    if (!fp){
      errorMessage((char*)"An error occurred", (char*)"(failed to open", (char*)"file)");
      return;
    }
    unsigned char header[8];
    if(fread(header, 1, 8, fp)!=8){
      fclose(fp);
      errorMessage((char*)"An error occurred", (char*)"(failed to read", (char*)"file)");
      return;
    }
    if (png_sig_cmp(header, 0, 8)){
      fclose(fp);
      errorMessage((char*)"An error occurred", (char*)"(not a valid PNG", (char*)"file)");
      return;
    }
    png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if (!png_ptr){
      errorMessage((char*)"An error occurred", (char*)"(failed to create", (char*)"PNG read struct)");
      return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
      png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
      errorMessage((char*)"An error occurred", (char*)"(failed to create", (char*)"PNG info struct)");
      return;
    }
    
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    //get information about the image
    unsigned int width,height;
    int color_type, bit_depth;
    png_get_IHDR(png_ptr, info_ptr,&width,&height,&bit_depth,&color_type,0,0,0);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png_ptr);
    if (bit_depth == 16){
      #if PNG_LIBPNG_VER >= 10504
           png_set_scale_16(png_ptr);
      #else
           png_set_strip_16(png_ptr);
      #endif
    }
    png_color_16 background_color;
    png_set_background_fixed(png_ptr,&background_color,PNG_BACKGROUND_GAMMA_SCREEN, 0, PNG_FP_1);
    png_read_update_info(png_ptr, info_ptr);
    
    unsigned short * vram=(unsigned short*)GetVRAMAddress();

    if(mode) {
      unsigned h;
      if((width==LCD_WIDTH_PX)&&(height<=LCD_HEIGHT_PX)){
        png_bytep row_pointers[LCD_HEIGHT_PX];
        //Simply center image and copy data
        vram+=((LCD_HEIGHT_PX-height)/2)*LCD_WIDTH_PX;
        unsigned char imgTmp[width*height*3];
        unsigned char*d=imgTmp;
        for(h=0;h<height;++h){
          row_pointers[h]=d;
          d+=LCD_WIDTH_PX*3;
        }
        png_read_image(png_ptr, row_pointers);
        d=imgTmp;
        for(h=0;h<width*height;++h){
          *vram++=((d[0] & 0xF8) << 8) | ((d[1] & 0xFC) << 3) | (d[2] >> 3);
          d+=3;
        }
      } else {
        unsigned int w2,h2,centerx,centery;
        int xpick=(int)((LCD_WIDTH_PX<<16)/width)+1,ypick=(int)((LCD_HEIGHT_PX<<16)/height)+1;
        if(xpick==ypick){
          w2=LCD_WIDTH_PX;
          h2=LCD_HEIGHT_PX;
          centerx=centery=0;
        }else if(xpick<ypick){
          w2=LCD_WIDTH_PX;
          h2=height*LCD_WIDTH_PX/width;
          centerx=0;
          centery=(LCD_HEIGHT_PX-h2)/2;
        }else{
          w2=width*LCD_HEIGHT_PX/height;
          h2=LCD_HEIGHT_PX;
          centerx=(LCD_WIDTH_PX-w2)/2;
          centery=0;
        }
        // EDIT: added +1 to account for an early rounding problem
        unsigned x_ratio = ((width<<12)/w2)+1;
        unsigned y_ratio = ((height<<12)/h2)+1;
        unsigned char decodeBuf[width*3*2];//Enough memory to hold two rows of data
        vram+=(centery*LCD_WIDTH_PX)+centerx;
        unsigned i,j,yo=0;
        png_bytep row_pointers[2];
        row_pointers[0]=decodeBuf;
        row_pointers[1]=decodeBuf+(width*3);
        png_read_rows(png_ptr, row_pointers, NULL,2);
        unsigned left=height-2;
        for (i=0;i<h2;i++){
          //Deterimin how many lines to read
          unsigned read=((y_ratio * i)>>12)-yo;
          if(read){
            while(read>=3){
              png_read_row(png_ptr,decodeBuf,NULL);//Apperntly this is the right way to skip a row http://osdir.com/ml/graphics.png.devel/2008-05/msg00038.html
              --read;
              --left;
            }
            if(read==1){
              memcpy(decodeBuf,decodeBuf+(width*3),width*3);
              png_read_row(png_ptr,decodeBuf+(width*3),NULL);
              --left;
            }else{
              png_read_rows(png_ptr, row_pointers, NULL,2);
              left-=2;
            }
          }
          for (j=0;j<w2;j++){
            unsigned A[3],B[3],C[3],D[3];
            unsigned x = x_ratio * j;
            unsigned y = y_ratio * i;
            unsigned x_diff = x&4095;
            unsigned y_diff = y&4095;
            unsigned ix_diff=4095-x_diff;
            unsigned iy_diff=4095-y_diff;
            x>>=12;
            y>>=12;
            yo=y;
            unsigned rgb;
            for(rgb=0;rgb<3;++rgb){
              A[rgb] = (decodeBuf[x*3+rgb]*ix_diff*iy_diff)>>24;
              B[rgb] = (decodeBuf[(x+1)*3+rgb]*(x_diff)*iy_diff)>>24;
              C[rgb] = (decodeBuf[(x+width)*3+rgb]*(y_diff)*ix_diff)>>24;
              D[rgb] = (decodeBuf[(x+width+1)*3+rgb]*(x_diff*y_diff))>>24;
            }
            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
          
            *vram++=(((A[0]+B[0]+C[0]+D[0]) & 0xF8) << 8) | (((A[1]+B[1]+C[1]+D[1]) & 0xFC) << 3) | ((A[2]+B[2]+C[2]+D[2]) >> 3);
            //*out++=A+B+C+D;
          }
          vram+=LCD_WIDTH_PX-w2;
        }
        while(left--){
          png_read_row(png_ptr,decodeBuf,NULL);//Avoid a too much data warning
        }
      }
    } else {
      unsigned char decodeBuf [width*3];
      png_bytep row_pointers[1];
      row_pointers[0]=decodeBuf;

      if(startx>(int)width-LCD_WIDTH_PX) startx=width-LCD_WIDTH_PX;
      if(starty>(int)height-LCD_HEIGHT_PX) starty=height-LCD_HEIGHT_PX;
      if(startx<0) startx=0;
      if(starty<0) starty=0;

      for (int y = 0; y < starty; y++) {
        png_read_row(png_ptr,decodeBuf,NULL); // skip a row
      }
      for (unsigned int y = 0; y < height-starty && y<LCD_HEIGHT_PX; y++) {
        png_read_rows(png_ptr, &row_pointers[0], NULL, 1);
        unsigned char* d=decodeBuf+3*startx;
        for(unsigned int h=0;h<width-startx&&h<LCD_WIDTH_PX;++h){
          *vram++=((d[0] & 0xF8) << 8) | ((d[1] & 0xFC) << 3) | (d[2] >> 3);
          d+=3;
        }
        vram+=(width-startx>=LCD_WIDTH_PX?0:LCD_WIDTH_PX-(width-startx));
      }
    }

    //cleanup
    png_read_end(png_ptr,(png_infop)NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
    fclose(fp);
    int inkeyloop = 1;
    EnableStatusArea(3);
    while(inkeyloop) {
      int key;
      GetKey(&key);
      switch(key) {
        case KEY_CTRL_EXIT:
          EnableStatusArea(0);
          return;
        case KEY_CTRL_DOWN:
          if(!mode) {
            starty += 64;
            inkeyloop = 0;
          }
          break;
        case KEY_CTRL_UP:
          if(!mode) {
            starty -= 64;
            inkeyloop = 0;
          }
          break;
        case KEY_CTRL_RIGHT:
          if(!mode) {
            startx += 64;
            inkeyloop = 0;
          }
          break;
        case KEY_CTRL_LEFT:
          if(!mode) {
            startx -= 64;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_2:
          if(!mode) {
            starty += LCD_HEIGHT_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_8:
          if(!mode) {
            starty -= LCD_HEIGHT_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_6:
          if(!mode) {
            startx += LCD_WIDTH_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_4:
          if(!mode) {
            startx -= LCD_WIDTH_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_7:
          if(!mode) {
            startx -= LCD_WIDTH_PX;
            starty -= LCD_HEIGHT_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_9:
          if(!mode) {
            startx += LCD_WIDTH_PX;
            starty -= LCD_HEIGHT_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_1:
          if(!mode) {
            startx -= LCD_WIDTH_PX;
            starty += LCD_HEIGHT_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_3:
          if(!mode) {
            startx += LCD_WIDTH_PX;
            starty += LCD_HEIGHT_PX;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_MINUS:
          if(!mode && width>LCD_WIDTH_PX && height>LCD_HEIGHT_PX) {
            mode = 1;
            inkeyloop = 0;
          }
          break;
        case KEY_CHAR_PLUS:
          if(mode) {
            mode = 0;
            inkeyloop = 0;
          }
          break;
      }
    }
  }
}