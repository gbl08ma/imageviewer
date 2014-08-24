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
#ifndef __FILEPROVIDER_H
#define __FILEPROVIDER_H
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

#include "menuGUI.hpp"

#define MAX_FILENAME_SIZE 270 //full path with //fls0/, extension and everything
#define MAX_NAME_SIZE 128 //friendly name (in "//fls0/folder/file.txt", this would be "file.txt")
#define MAX_ITEMS_IN_DIR 200
#define MAX_ITEMS_IN_CLIPBOARD 51
#define MAX_TEXTVIEWER_FILESIZE 64*1024
typedef struct
{
  char filename[MAX_FILENAME_SIZE]; //filename, not proper for use with Bfile.
  char visname[42]; //visible name, only for menus. use nameFromFilename to get the proper name.
  short action; // mostly for clipboard, can be used to tag something to do with the file
  short isfolder; // because menuitem shouldn't be the only struct holding this info
  int size; // file size
} File; // right now File only holds the filename as other fields are now set directly on a MenuItem array

typedef struct
{
  unsigned short id, type;
  unsigned long fsize, dsize;
  unsigned int property;
  unsigned long address;
} file_type_t;

#define GETFILES_SUCCESS 0
#define GETFILES_MAX_FILES_REACHED 1

int GetFiles(File* files, MenuItem* menuitems, char* basepath, int* count, unsigned char* filter, unsigned char* filter2);
void nameFromFilename(char* filename, char* name);

#define FILE_ICON_FOLDER 0
#define FILE_ICON_G3M 1
#define FILE_ICON_G3E 2
#define FILE_ICON_G3A 3
#define FILE_ICON_G3P 4
#define FILE_ICON_G3B 5
#define FILE_ICON_BMP 6
#define FILE_ICON_TXT 7
#define FILE_ICON_CSV 8
#define FILE_ICON_OTHER 9
int fileIconFromName(char* name);

#endif