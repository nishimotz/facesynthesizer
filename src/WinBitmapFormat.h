
#ifndef __WINDOWS_BITMAP_FORMAT_H__
#define __WINDOWS_BITMAP_FORMAT_H__

/*+++
WinBitmapFormat.h
- a header file for Windows Bitmap I/O process routines.

  (c)2001, Shin OGATA <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa's code.
  
	First Created: Wed Sep 19 19:55:45 JST 2001
	Last Modified: Thu Sep 27 16:27:44 JST 2001
---*/

#include "ImageData.h"
class ImageData;

class WinBmpIO {
private:
	// bitmap file's information header structure
	typedef struct BmpInfoHeader {
		unsigned long  biSize;			// size of this structure (4bytes)
		long		   biWidth;			// image width (4bytes)
		long		   biHeight;		// image height (4bytes)
		unsigned short biPlanes;		// number of plane (always 1) (2bytes)
		unsigned short biBitCount;		// number of color bits/pixel = 1, 4, 8, 16, 24, 32 (2bytes)
		unsigned long  biCompression;	// compression type (4bytes)
		unsigned long  biSizeImage;		// all image data size, including header size(4bytes)
		long		   biXPelsPerMeter;	// horizontal resolution (4bytes)
		long		   biYPelsPerMeter;	// vertical resolution (4bytes)
		unsigned long  biClrUsed;		// number of used color index (4bytes)
		unsigned long  biClrImportant;	// number of importance color index (4bytes)
	} _BmpInfoHeader;
	
protected:
	friend class ImageData;
	// public constructor
	WinBmpIO() {}
	
	// public destructor
	virtual ~WinBmpIO() {}
	
	// load and save enter bitmap
	int LoadImage(const char *fileName, ImageData *imgPtr);
	int SaveImage(const char *fileName, ImageData *imgPtr);
};


#endif	// __WINDOWS_BITMAP_FORMAT_H__

