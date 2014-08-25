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

#include "aboutGUI.hpp"
#include "textGUI.hpp"
#include "graphicsProvider.hpp"
#include "versionProvider.hpp"

void showAbout() {
  int key;
  Bdisp_AllClr_VRAM();
  DefineStatusMessage((char*)"About Image Viewer", 1, 0, 0);
  DisplayStatusArea();
  //y increment between each line: 17; between paragraphs: 20
  int textX = 0;
  int textY = 5;
  char verBuffer[100] = "";
  getVersion(verBuffer);
  PrintMini(&textX, &textY, (unsigned char*)"Version ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
  PrintMini(&textX, &textY, (unsigned char*)verBuffer, 0, 0xFFFFFFFF, 0, 0, COLOR_BLUE, COLOR_WHITE, 1, 0);
  textY = textY + 17; textX = 0;
  getTimestamp(verBuffer);
  PrintMini(&textX, &textY, (unsigned char*)verBuffer, 0, 0xFFFFFFFF, 0, 0, COLOR_GRAY, COLOR_WHITE, 1, 0);
  textY = 42;
  textX = 0;
  PrintMini(&textX, &textY, (unsigned char*)"Developed by gbl08ma at", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
  drawtnyimLogo(10, 59+24); //24 pixels for the status bar
  textY = 101;
  textX = 0;
  // PrintMini and its x,y pointers allow for awesome easy color formatting... let's try
  PrintMini(&textX, &textY, (unsigned char*)"tny. ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
  PrintMini(&textX, &textY, (unsigned char*)"i", 0, 0xFFFFFFFF, 0, 0, TNYIM_ORANGE, COLOR_WHITE, 1, 0);
  PrintMini(&textX, &textY, (unsigned char*)"nternet ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
  PrintMini(&textX, &textY, (unsigned char*)"m", 0, 0xFFFFFFFF, 0, 0, TNYIM_ORANGE, COLOR_WHITE, 1, 0);
  PrintMini(&textX, &textY, (unsigned char*)"edia", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
  textY = 125;
  textX = 0;
  PrintMini(&textX, &textY, (unsigned char*)"http://i.tny.im | http://gbl08ma.com", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
  textY = 142;
  textX = 0;
  PrintMini(&textX, &textY, (unsigned char*)"gbl08ma@gmail.com", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
  
  textX = 0; textY = 175;
  PrintMini(&textX, &textY, (unsigned char*)"Press any key", 0, 0xFFFFFFFF, 0, 0, COLOR_WHITE, COLOR_BLACK, 1, 0);
  GetKey(&key);

  Bdisp_AllClr_VRAM();
  Bdisp_EnableColor(1);
  DisplayStatusArea();
 
  textArea text;

  textElement elem[12];
  text.elements = elem;

  elem[0].text = (char*)"This is a JPEG and PNG image viewer for the Casio Prizm calculators. For JPEG images, only baseline is supported, Progressive and Lossless JPEG is not supported. Interlaced PNG is not supported.";

  elem[1].newLine = 1;
  elem[1].lineSpacing = 8;
  elem[1].text = (char*)"Uses TJpgDec - (C)ChaN, 2012.";
  
  elem[2].newLine = 1;
  elem[2].text = (char*)"Uses zlib - Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler";
  
  elem[3].newLine = 1;
  elem[3].text = (char*)"Uses libpng - Copyright (c) 2004, 2006-2014 Glenn Randers-Pehrson";
  
  elem[4].newLine = 1;
  elem[4].lineSpacing = 8;
  elem[4].text = (char*)"This image viewer is free software distributed under the following license:";
  
  elem[5].newLine = 1;
  elem[5].lineSpacing = 8;
  elem[5].text = (char*)"Copyright (C) 2014, Gabriel Maia and the tny. internet media group";
  
  elem[6].newLine = 1;
  elem[6].text = (char*)"All rights reserved.";
  
  elem[7].newLine = 1;
  elem[7].lineSpacing = 8;
  elem[7].text = (char*)"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:";
  
  elem[8].newLine = 1;
  elem[8].lineSpacing = 8;
  elem[8].text = (char*)"1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.";
  
  elem[9].newLine = 1;
  elem[9].lineSpacing = 8;
  elem[9].text = (char*)"2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.";
  
  elem[10].newLine = 1;
  elem[10].lineSpacing = 8;
  elem[10].text = (char*)"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";
  
  elem[11].newLine = 1;
  elem[11].lineSpacing = 10;
  elem[11].text = (char*)"Press EXIT";

  text.numelements = 12;
  doTextArea(&text);
  DefineStatusMessage((char*)"", 1, 0, 0);
}