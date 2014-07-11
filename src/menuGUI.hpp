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
#ifndef __MENUGUI_H
#define __MENUGUI_H

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

#define MENUITEM_NORMAL 0
#define MENUITEM_CHECKBOX 1
#define MENUITEM_SEPARATOR 2
#define MENUITEM_VALUE_NONE 0
#define MENUITEM_VALUE_CHECKED 1
typedef struct
{
  char* text; // text to be shown on screen. mandatory, must be a valid pointer to a string.
  int type=MENUITEM_NORMAL; // type of the menu item. use MENUITEM_* to set this
  int value=MENUITEM_VALUE_NONE; // value of the menu item. For example, if type is MENUITEM_CHECKBOX and the checkbox is checked, the value of this var will be MENUITEM_VALUE_CHECKED
  int color=TEXT_COLOR_BLACK; // color of the menu item (use TEXT_COLOR_* to define)
  // The following two settings require the menu type to be set to MENUTYPE_MULTISELECT
  int isfolder=0; // for file browsers, this will signal the item is a folder
  int isselected=0; // for file browsers and other multi-select screens, this will show an arrow before the item
  int icon=-1; //for file browsers, to show a file icon. -1 shows no icon (default)
} MenuItem;

typedef struct
{
  unsigned short data[0x12*0x18];
} MenuItemIcon;

#define MENUTYPE_NORMAL 0
#define MENUTYPE_MULTISELECT 1
#define MENUTYPE_INSTANT_RETURN 2 // this type of menu insantly returns even if user hasn't selected an option (allows for e.g. redrawing the GUI behind it). if user hasn't exited or selected an option, menu will return MENU_RETURN_INSTANT
#define MENUTYPE_NO_KEY_HANDLING 3 //this type of menu doesn't handle any keys, only draws.
#define MENUTYPE_FKEYS 4 // returns GetKey value of a Fkey when one is pressed
typedef struct {
  char* statusText = NULL; // text to be shown on the status bar, may be empty
  char* title = NULL; // title to be shown on the first line if not null
  char* subtitle = NULL;
  int titleColor=TEXT_COLOR_BLUE; //color of the title
  char* nodatamsg; // message to show when there are no menu items to display
  int startX=1; //X where to start drawing the menu. NOTE this is not absolute pixel coordinates but rather character coordinates
  int startY=1; //Y where to start drawing the menu. NOTE this is not absolute pixel coordinates but rather character coordinates
  int width=21; // NOTE this is not absolute pixel coordinates but rather character coordinates
  int height=8; // NOTE this is not absolute pixel coordinates but rather character coordinates
  int scrollbar=1; // 1 to show scrollbar, 0 to not show it.
  int scrollout=0; // whether the scrollbar goes out of the menu area (1) or it overlaps some of the menu area (0)
  int numitems; // number of items in menu
  int type=MENUTYPE_NORMAL; // set to MENUTYPE_* .
  int selection=1; // currently selected item. starts counting at 1
  int scroll=0; // current scrolling position
  int fkeypage=0; // for MULTISELECT menu if it should allow file selecting and show the fkey label
  int numselitems=0; // number of selected items
  int returnOnInfiniteScrolling=0; //whether the menu should return when user reaches the last item and presses the down key (or the first item and presses the up key)
  int darken=0; // for dark theme on homeGUI menus
  int miniMiniTitle=0; // if true, title will be drawn in minimini. for calendar week view
  int pBaRtR=0; //preserve Background And Return To Redraw. Rarely used
  MenuItem* items; // items in menu
} Menu;

#define MENU_RETURN_EXIT 0
#define MENU_RETURN_SELECTION 1
#define MENU_RETURN_INSTANT 2
#define MENU_RETURN_SCROLLING 3 //for returnOnInfiniteScrolling
int doMenu(Menu* menu, MenuItemIcon* icontable=NULL);

#endif