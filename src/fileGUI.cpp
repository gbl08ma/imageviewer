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
#include <alloca.h>

#include "menuGUI.hpp"
#include "fileProvider.hpp" 
#include "fileGUI.hpp"
#include "graphicsProvider.hpp"
#include "aboutGUI.hpp"

int fileBrowser(char* filename, char* filter, char* filter2, char* title) {
  // returns 1 when user selects a file, 0 when aborts (exit)
  int res = 1;
  char browserbasepath[MAX_FILENAME_SIZE+1] = "\\\\fls0\\";  
  while(res) {
    strcpy(filename, (char*)"");
    res = fileBrowserSub(browserbasepath, filename, filter, filter2, title);
    if(res==2) return 1; // user selected a file
  }
  return 0;
}

int fileBrowserSub(char* browserbasepath, char* filename, char* filter, char* filter2, char* title) {
  Menu menu;
  MenuItemIcon icontable[12];
  buildIconTable(icontable);
  
  // first get file count so we know how much to alloc
  GetFiles(NULL, NULL, browserbasepath, &menu.numitems, filter, filter2);
  MenuItem* menuitems = NULL;
  File* files = NULL;
  if(menu.numitems > 0) {
    menuitems = (MenuItem*)alloca(menu.numitems*sizeof(MenuItem));
    files = (File*)alloca(menu.numitems*sizeof(File));
    // populate arrays
    GetFiles(files, menuitems, browserbasepath, &menu.numitems, filter, filter2);
    menu.items = menuitems;
  }
  
  char titleBuffer[120];
  char titleBufferBuf[120];
  int smemfree;
  unsigned short smemMedia[10]={'\\','\\','f','l','s','0',0};
  Bfile_GetMediaFree_OS( smemMedia, &smemfree );
  
  char friendlypath[MAX_FILENAME_SIZE];
  strcpy(friendlypath, browserbasepath+6);
  friendlypath[strlen(friendlypath)-1] = '\0'; //remove ending slash like OS does
  // test to see if friendlypath is too big
  int jump4=0;
  while(1) {
    int temptextX=7*18+10; // px length of menu title + 10, like menuGUI goes.
    int temptextY=0;
    PrintMini(&temptextX, &temptextY, (char*)friendlypath, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 0, 0); // fake draw
    if(temptextX>LCD_WIDTH_PX-6) {
      char newfriendlypath[MAX_FILENAME_SIZE];
      shortenDisplayPath(friendlypath, newfriendlypath, (jump4 ? 4 : 1));
      if(strlen(friendlypath) > strlen(newfriendlypath) && strlen(newfriendlypath) > 3) { // check if len > 3 because shortenDisplayPath may return just "..." when the folder name is too big
        // shortenDisplayPath still managed to shorten, copy and continue
        jump4 = 1; //it has been shortened already, so next time jump the first four characters
        strcpy(friendlypath, newfriendlypath);
      } else {
        // shortenDisplayPath can't shorten any more even if it still
        // doesn't fit in the screen, so give up.
        break;
      }
    } else break;
  }
  menu.subtitle = friendlypath;
  menu.type = MENUTYPE_MULTISELECT;
  menu.scrollout=1;
  menu.nodatamsg = (char*)"No Data";
  menu.title = title;
  menu.height=7;
  while(1) {
    Bdisp_AllClr_VRAM();
    drawFkeyLabels((menu.numitems ? 0x03B1 : 0), -1, -1, -1, -1, 0x03DF); // OPEN, VERSION
    itoa(smemfree, (unsigned char*)titleBuffer);
    LocalizeMessage1( 340, titleBufferBuf ); //"bytes free"
    strncat((char*)titleBuffer, (char*)titleBufferBuf, 65);
    menu.statusText = (char*)titleBuffer;
    int res = doMenu(&menu, icontable);
    switch(res) {
      case MENU_RETURN_EXIT:
        if(!strcmp(browserbasepath,"\\\\fls0\\")) { //check that we aren't already in the root folder
          //we are, return 0 so we exit
          //return 0; // in this add-in, the file browser shouldn't return
        } else {
          int i=strlen(browserbasepath)-2;
          while (i>=0 && browserbasepath[i] != '\\')
                  i--;
          if (browserbasepath[i] == '\\') {
            char tmp[MAX_FILENAME_SIZE] = "";
            memcpy(tmp,browserbasepath,i+1);
            tmp[i+1] = '\0';
            strcpy(browserbasepath, tmp);
          }
          return 1; //reload at new folder
        }
        break;
      case KEY_CTRL_F1:
      case MENU_RETURN_SELECTION:
        if(!menu.numitems) break;
        if(menuitems[menu.selection-1].isfolder) {
          strcpy(browserbasepath, files[menu.selection-1].filename); //switch to selected folder
          strcat(browserbasepath, "\\");
          return 1; //reload at new folder
        } else {
          strcpy(filename,files[menu.selection-1].filename);
          return 2;
        }
        break;
      case KEY_CTRL_F6:
        showAbout();
        break;
    }
  }
  return 1;
}

void shortenDisplayPath(char* longpath, char* shortpath, int jump) {
  //this function takes a long path for display, like \myfolder\long\display\path
  //and shortens it one level, like this: ...\long\display\path
  //putting the result in shortpath
  strcpy(shortpath, (char*)"...");
  int i = jump; // jump the specified amount of characters... by default it jumps the first /
  // but it can also be made to jump e.g. 4 characters, which would jump ".../" (useful for when the text has been through this function already)
  int max = strlen(longpath);
  while (i<max && longpath[i] != '\\')
          i++;
  if (longpath[i] == '\\') {
    strcat(shortpath, longpath+i);
  }
}

void buildIconTable(MenuItemIcon* icontable) {
  unsigned int msgno;
  unsigned short folder[7]={'\\','\\','f','l','s','0',0};

  const char *bogusFiles[] = {"t", // for folder
                              "t.g3m",
                              "t.g3e",
                              "t.g3a",
                              "t.g3p",
                              "t.g3b",
                              "t.bmp",
                              "t.txt",
                              "t.csv",
                              "t.abc" //for "unsupported" files
                             };

  for(int i = 0; i < 10; i++)
    SMEM_MapIconToExt( (unsigned char*)bogusFiles[i], (i==0 ? folder : (unsigned short*)"\x000\x000"), &msgno, icontable[i].data );
}