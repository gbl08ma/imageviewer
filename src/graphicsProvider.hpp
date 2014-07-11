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
#ifndef __GRAPHICSPROVIDER_H
#define __GRAPHICSPROVIDER_H
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

#define TNYIM_ORANGE 0xd222
void plot(int x0, int y0, unsigned short color);
void drawRectangle(int x, int y, int width, int height, unsigned short color);
void drawLine(int x1, int y1, int x2, int y2, int color);
void drawFilledCircle(int centerx, int centery, int radius, color_t color);
void VRAMReplaceColorInRect(int x, int y, int width, int height, color_t color_old, color_t color_new);
void VRAMInvertArea(short x, short y, short width, short height);
void darkenStatusbar();
void darkenFkeys(int numkeys);
void drawArrowDown(int bottomX, int bottomY, int color);
void drawFkeyPopup(int Fkey, char* title);
//void CopySprite(const void* datar, int x, int y, int width, int height);
void CopySpriteMasked(unsigned short* data, int x, int y, int width, int height, unsigned short maskcolor);
void CopySpriteNbit(const unsigned char* data, int x, int y, int width, int height, const color_t* palette, unsigned int bitwidth);
void drawtnyimLogo( int x, int y);
int textColorToFullColor(int textcolor);
void progressMessage(char* message, int cur, int total);
void closeProgressMessage();
void printCentered(unsigned char* text, int y, int FGC, int BGG);
void clearLine(int x, int y, color_t color=COLOR_WHITE);
void mPrintXY(int x, int y, char*msg, int mode, int color);
void mMsgBoxPush(int lines);
void mMsgBoxPop();
void popAllMsgBoxes();
int getNumberOfMsgBoxPushed();
void drawScreenTitle(char* title, char* subtitle = NULL);
void drawFkeyLabels(int f1=-1, int f2=-1, int f3=-1, int f4=-1, int f5=-1, int f6=-1);

#endif