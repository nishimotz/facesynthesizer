
#ifndef __SGI_RGB_IMAGE_FORMAT_H__
#define __SGI_RGB_IMAGE_FORMAT_H__

/*+++
SgiImageFormat.h
- a header file for SGI RGB image I/O process routines.

  (c)2001, Shin Ogata <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa's code.
  
	First Created: Thu Sep 27 16:27:45 JST 2001
	Last Modified: Thu Sep 27 20:22:25 JST 2001
---*/

#include "ImageData.h"
class ImageData;

#ifndef byte
#define byte unsigned char
#endif

class SgiImgIO {
private:
	// sgi image file's information header structure
	typedef struct SgiFileInfoHeader {
		short	sgiSM;				// sgi magic
		byte	sgiSF;				// storage format(compressed(1) or not(0))
		byte	sgiNumOfB;			// number of byte per pixel channel. (256 level(1) or 65536 level(2))
		short	sgiNumOfD;			// number of dimensions, line demension. "3" fix.
		short	sgiWidth;			// x size in pixels
		short	sgiHeight;			// y size in pixels
		short	sgiNumOfC;			// number of channels (4:rgba,3:rgb,2:luminance(gray)+alpha,1:luminance(gray))
		long	sgiMin;				// minimum pixel value
		long	sgiMax;				// maximum pixel value
		byte	sgiIg1[4];			// ignored
		byte	sgiImgName[80];		// image name
		long	sgiCMapID;			// colormap ID, always "0".
		byte	sgiIg2[404];		// ignored
	} _SgiFileInfoHeader;
	
	// private process function
	short ChangeShort(short src) { return (((src >> 8) & 0x00ff) | ((src << 8) & 0xff00)); }
	long  ChangeLong(long src)	 { return (((src & 0xff000000) >> 24) | ((src & 0x00ff0000) >> 8) | ((src & 0x0000ff00) << 8) | ((src & 0x000000ff) << 24)); }
	
protected:
	friend class ImageData;
	// public constructor
	SgiImgIO() {}
	
	// public destructor
	virtual ~SgiImgIO() {}
	
	// load and save enter bitmap
	int LoadImage(const char *fileName, ImageData *imgPtr);
	int SaveImage(const char *fileName, ImageData *imgPtr);
};

#endif	// __SGI_RGB_IMAGE_FORMAT_H__


