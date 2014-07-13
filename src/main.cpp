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
#include "fileProvider.hpp"
#include "fileGUI.hpp"

#define DIRNAME (unsigned char*)"@IMAGES"
#define STATEFILE (unsigned char*)"LastImg"

int main()
{
  // Add-in entry point
  // second on-chip RAM area must be backed up,
  // because we have code on it, but it is cleared
  // on shutdown.
  // crt0.S put the code in there, so we back it
  // up now and restore right before we need it,
  // to ensure it's there.
  char ocram2backup[8192];
  backupOCRAM2(ocram2backup);

  // disable Catalog function throughout the add-in, as we don't know how to make use of it:
  Bkey_SetAllFlags(0x80);
  
  Bdisp_EnableColor(1);
  EnableStatusArea(0);
  DefineStatusAreaFlags(3, SAF_BATTERY | SAF_TEXT | SAF_GLYPH | SAF_ALPHA_SHIFT, 0, 0);
  Bdisp_AllClr_VRAM();
  
  while(1) {
    int MCSsize;
    MCSGetDlen2(DIRNAME, STATEFILE, &MCSsize);
    char filename[MAX_FILENAME_SIZE+1];
    if (MCSsize == 0) {
      if(fileBrowser(filename, (unsigned char*)"*.jpg", (char*)"Images")) {
        int createResult = MCS_CreateDirectory( DIRNAME );
        if(createResult != 0) // Check directory existence
        { // directory already exists, so delete the exiting file that may be there
          MCSDelVar2(DIRNAME, STATEFILE);
        }
        MCSPutVar2(DIRNAME, STATEFILE, strlen(filename), (unsigned char*)filename); //so that if user leaves when viewing an image, we have that saved.
      } else continue;
    } else {
      //we were viewing an image when we left...
      MCSGetData1(0, MCSsize, filename); //recover filename, view that image
    }
    viewImage(filename, ocram2backup);
    MCSDelVar2(DIRNAME, STATEFILE); //if user reaches this point, then he didn't leave the add-in when viewing an image. delete statefile.
  }
}