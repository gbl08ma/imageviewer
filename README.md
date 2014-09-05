# Image viewer for the Casio Prizm (fx-CG 10 and 20)

This is a software add-in for the Casio Prizm series of calculators, that allows for viewing pictures in JPEG and PNG format.

## Features

  - View images in JPEG and PNG format;
  
  - Re-opens the image that was being viewed when the add-in was closed;
  
  - Works as an eActivity strip - link to pictures from your eActivity documents;
  
  - Three zoom-out levels supported for JPEG images;
  
  - Fit-to-screen mode supported for PNG images;
  
  - Long file names shown in file browser.
  
## Files supported

  - Baseline JPEG (Progressive and Lossless JPEG not supported), with size up to 65520 x 65520 pixels;
  
  - All PNG files except interlaced ones, with practically unlimited height and sensible width (up to 5000 px of width will work fine).
  
## System requirements

  - Casio fx-CG 10 or fx-CG 20, running any OS version (01.03 and up is recommended);
  
  - 160 KiB of storage memory available (for the add-in g3a);
  
  - 48 bytes of main memory available, for storing the filename of the picture that was being viewed, if any.
  
## Installation instructions

To install, connect the Prizm calculator to the computer with a miniUSB<->USB cable. On the calculator, press [F1] when a pop-up appears on the screen. Wait for the USB connection to be established. When it's finished, your Prizm will appear on your computer as if it were a pendisk.

Copy "imageviewer.g3a" to the root folder of the "pendisk" (i.e., out of any folders but inside the pendisk; if necessary, overwrite the existing file).

Safely disconnect the calculator and wait for it to finish "updating the Main Memory". When it does, you should notice a new Main Menu item, called "Images".

The first time you run the Image Viewer you'll see a file browser. See Usage instructions.
  
## Usage instructions

When opening the image viewer, you'll see a file browser. This file browser lets you select a picture to view. You will only see JPG and PNG files in this file browser. Press EXE to select an item and EXIT to go up one level.

When selecting a JPG file for viewing, you'll be shown the picture at 100% zoom (no scaling). If it is smaller than the screen, it will show in its center. If not, the upper left corner of the picture will display.

Press the directional keys to "move around" and change what part of the picture is displayed. You can also use the number keys to skip one screen in the specified direction (8 acts like "Page Up", 2 like "Page Down", 4 moves one screen to the left, 1 moves down and left one screen, and so on).

You can press the minus key to zoom out, and the plus key to zoom back in. The zoom levels supported are 100% (default), 50%, 25% and 12.5%.

When viewing a PNG file, the instructions are similar. For "moving" in the image, use the same keys (directional pad and numbers). To see the whole image resized to fit in the screen, press the minus key. To go back to non-scaled mode, press the plus key.

PNG images that are smaller than the screen do not show centered, unlike JPEG ones.

### eActivity strip

This add-in can work as an eActivity strip. After selecting the strip from the list to add it to the eActivity, open it. You'll see the file browser. After selecting an image, you'll see it as usual. Now, to leave the image linked with the strip, leave the strip without closing the image. To do this, press Shift and then the "Store" key (arrow above AC/on).

The next time you open this strip, the image viewer will look for the image that was being viewed when the strip was closed (same behavior as when opening from the Main Menu).

Note that the image is not inserted inside the eActivity, the strip merely acts as a link. If the image is deleted or renamed in the storage memory, or the eActivity file is transferred to another calculator without the image at the exact place and name in the storage memory, the link will not work, and the Image Viewer will show an error when opening the strip.

## Compilation instructions

Assuming you are using a setup similar to PrizmSDK, cloning this repository is enough to put you on the right track. You may need to edit the makefile to point it the correct locations for includes and linker scripts.

This add-in needs to link against libfxcg, libc, libgcc, and also zlib and libpng. You'll need to cross-compile the latter two for the Prizm, and then make sure that the Makefile points to their correct locations for static linking.

The Makefile assumes you have a special build of mkg3a with support for eActivity strip creation. If this isn't the case, remove the `-s` and `-i mon:../monoicon.bin` flags from `MKG3AFLAGS` (of course, the resulting g3a will not support running as an eActivity strip).

## Checking for updates
This Image Viewer, like most software, receives updates from time to time. You should check for updates to this add-in periodically, to ensure you have the greatest feature pack and the most stable version. To check for new versions, you should visit the following page:

[http://gbl08ma.com/casio-prizm-software/](http://gbl08ma.com/casio-prizm-software/)

If the above page isn't available, you can try checking for news at the tny. internet media website:

[http://i.tny.im/](http://i.tny.im/)

Finally, if none of these websites are available, or if you need to contact the authors, this is the email:

gbl08ma@gmail.com

## License

The add-in is available under the BSD 2-clause license. A license should have come with this read-me and the imageviewer.g3a file. If not, the license is available online at [http://opensource.org/licenses/BSD-2-Clause](http://opensource.org/licenses/BSD-2-Clause)

See the disclaimer at the end of this file or the messages on the about screen of the add-in (accessible by pressing the F6 key in the add-in, when browsing for images).

Distributing this Read-Me along with the g3a binary is not mandatory.

Any derivative work based on this software should clearly state it is not the Image Viewer for the Prizm by gbl08ma at tny. internet media.

## Last words
This Image Viewer is the result of over one year of research, work and extensive testing. The libraries it uses for image viewing (zlib, libpng and TJpgDec) have been around for even longer. That said, we or the authors of those libraries can't guarantee you will have no problems using the add-in or that everything will work as described. You are welcome to report problems to the contacts above, as well as modify the source code to your needs as long as you respect the license.

We hope you enjoy using this Image Viewer as much as we did developing it. And if this add-in ever becomes useful, you just found a secret feature ;)

---
Copyright (C) 2013-2014 Gabriel Maia (gbl08ma) and the tny. internet media group

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
