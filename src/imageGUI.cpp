/*----------------------------------------------------------------------------/
/ JPEG Image Viewer for the Casio Prizm - Copyright 2014 tny. internet media
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

#include "imageGUI.hpp"
#include "menuGUI.hpp"
#include "graphicsProvider.hpp"
#include "tjpgd.h"

void viewImage(char* filename) {
  void *work;     /* Pointer to the decompressor work area */
  JDEC jdec;    /* Decompression object */
  JRESULT res;    /* Result code of TJpgDec API */
  IODEV devid;    /* User defined device identifier */

  /* Allocate a work area for TJpgDec */
  //work = alloca(3100*2);
  work = (char*)0xE5007000; // first "additional on-chip RAM area", 8192 bytes

  devid.xoff = 0;
  devid.yoff = 0;
  int scale = 0;
  unsigned short filenameshort[0x10A];
  Bfile_StrToName_ncpy(filenameshort, (unsigned char*)filename, 0x10A);
  while(1) {
    //int init_time = RTC_GetTicks();
    int key;
    Bdisp_AllClr_VRAM();
    /* Open a JPEG file */
    devid.fp = Bfile_OpenFile_OS(filenameshort, READWRITE, 0);
    if (devid.fp < 0) {
      MsgBoxPush(4);
      mPrintXY(3, 2, (char*)"An error occurred", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
      mPrintXY(3, 3, (char*)"(failed to open", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
      mPrintXY(3, 4, (char*)"file)", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
      PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 2, TEXT_COLOR_BLACK); // press exit message
      while(1) {
        GetKey(&key);
        if(key == KEY_CTRL_EXIT || key == KEY_CTRL_AC) return;
      }
    }

    /* Prepare to decompress */
    res = jd_prepare(&jdec, in_func, work, 8190, &devid);
    if (res == JDR_OK) {
      /* Ready to dcompress. Image info is available here. */
      //printf("Image dimensions: %u by %u. %u bytes used.\n", jdec.width, jdec.height, 3100 - jdec.sz_pool);
      if(jdec.width < LCD_WIDTH_PX) {
        devid.xoff = -(LCD_WIDTH_PX/2 - jdec.width/2);
      }
      if(jdec.height < LCD_HEIGHT_PX) {
        devid.yoff = -(LCD_HEIGHT_PX/2 - jdec.height/2);
      }
      res = jd_decomp(&jdec, out_func, scale);   /* Start to decompress with set scaling */
      if (res == JDR_OK || res == JDR_INTR) {
        /* Decompression succeeded or was stopped by output function which determined enough had been decoded.
         * You have the decompressed image in the frame buffer here. */
      } else {
        //printf("Failed to decompress: rc=%d\n", res);
        Bfile_CloseFile_OS(devid.fp);
        MsgBoxPush(4);
        mPrintXY(3, 2, (char*)"An error occurred", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
        mPrintXY(3, 3, (char*)"(failed to", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
        mPrintXY(3, 4, (char*)"decompress)", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
        PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 2, TEXT_COLOR_BLACK); // press exit message
        while(1) {
          GetKey(&key);
          if(key == KEY_CTRL_EXIT || key == KEY_CTRL_AC) return;
        }
      }

    } else {
      //printf("Failed to prepare: rc=%d\n", res);
      Bfile_CloseFile_OS(devid.fp);
      MsgBoxPush(4);
      mPrintXY(3, 2, (char*)"An error occurred", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
      mPrintXY(3, 3, (char*)"(failed to", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
      mPrintXY(3, 4, (char*)"prepare)", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
      PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 2, TEXT_COLOR_BLACK); // press exit message
      while(1) {
        GetKey(&key);
        if(key == KEY_CTRL_EXIT || key == KEY_CTRL_AC) return;
      }
    }

    Bfile_CloseFile_OS(devid.fp);     /* Close the JPEG file */
    /*int finish_time = RTC_GetTicks();
    char buffer[20] = "";
    itoa(finish_time-init_time, (unsigned char*)buffer);
    mPrintXY(1,1,buffer,TEXT_MODE_NORMAL,TEXT_COLOR_BLACK);*/
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

UINT in_func (JDEC* jd, BYTE* buff, UINT nbyte)
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

UINT out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
  IODEV *dev = (IODEV*)jd->device;
  /* Put progress indicator */
  /*if (rect->left == 0) {
    printf("\r%lu%%", (rect->top << jd->scale) * 100UL / jd->height);
  }*/
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