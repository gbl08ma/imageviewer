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
#ifndef __TEXTGUI_H
#define __TEXTGUI_H
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

typedef struct
{
  char* text;
  int newLine=0; // if 1, new line will be drawn before the text
  color_t color=COLOR_BLACK;
  int spaceAtEnd=0;
  int lineSpacing=0;
  int minimini=0;
} textElement;

#define TEXTAREATYPE_NORMAL 0
#define TEXTAREATYPE_INSTANT_RETURN 1
typedef struct
{
  int x=0;
  int y=0;
  int width=LCD_WIDTH_PX;
  int lineHeight=17;
  textElement* elements;
  int numelements;
  char* title = NULL;
  int scrollbar=1;
  int allowEXE=0; //whether to allow EXE to exit the screen
  int allowF1=0; //whether to allow F1 to exit the screen
  int type=TEXTAREATYPE_NORMAL;
} textArea;

#define TEXTAREA_RETURN_EXIT 0
#define TEXTAREA_RETURN_EXE 1
#define TEXTAREA_RETURN_F1 2
int doTextArea(textArea* text); //returns 0 when user EXITs, 1 when allowEXE is true and user presses EXE, 2 when allowF1 is true and user presses F1.

#endif 
