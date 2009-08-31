
#ifndef __IMAGE_DATA_H__
#define __IMAGE_DATA_H__

/*+++
ImageData.h
- a header file for image data read, write and store routines.

  (c)2001, Shin Ogata <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa & Daisuke Ito's code.
  
	First Created: Man Jul 16 11:55:45 JST 2001
	Last Modified: Man Nov 19 19:24:25 JST 2001
	!! under construction for any file-format., p.r.n. !!
---*/

#include <iostream>
#include <fstream>
#include "WinBitmapFormat.h"
#include "SgiImageFormat.h"

/* +++ sharing type of image data +++ */

#ifndef byte
#define byte unsigned char
#endif

// pixel data structure
typedef struct Pixel {
	byte r, g, b, a;
} _Pixel;

// RGB channel structure
typedef struct RGBch {
	byte r, g, b;
} _RGBch;
/* --- sharing type of image data --- */

using namespace std;

class ImageData {
public:
	// image data, I/O process error code
	enum IMAGE_FORM_ERROR_CODE {
		ERROR_NONE		= 0,		// No error!
			ERROR_DATA_EMPTY	= 74,		// Image data empty. "na-shi"
			ERROR_UNDER_CONSTRUCTION	= 101,	// Under construction.
			ERROR_FILE_NOT_SUPPORTED,		// The file form is not supported.
			ERROR_BAD_CHANNEL_NUM,		// Bad channel number, not "rgb" 3ch.
			ERROR_CANT_OPEN_FILE,		// Cannot open file.
			ERROR_DATA_EXIST,			// Image data already exist.
			ERROR_CANT_ALLOCATE_MEMORY,		// Cannot allocate memorys.
			ERROR_DATA_IS_COMPRESSED,		// Image data is compressed.
			ERROR_NOT_BITMAP_FILE,		// The file is not bitmap file.
			ERROR_NOT_SGI_IMAGE_FILE,		// The file is not SGI image file.
	};
	
	// image data, open file format
	enum IMAGE_FORMAT {
		STANDARD_BMP_FORM = 1001,		// [rgbrgbrgb]
			WINDOWS_BMP_FORM,			// [bgrbgrbgr]
			SGI_RGB_FORM,			// [rrrgggbbb]
			PNG_FORM,
	};
	
	// alpha channel information
	enum ALPHA_INFO {
		OFF_ALPHA	= 0,	// off alpha channel
			ON_ALPHA,		// on alpha channel
			SAVE_ALPHA,		// for save mode, cannot set from outside.
	};
	
private:
	// private process functions
	int FileExistenceJudgement(const char *fileName);
	int FileLoadTypeJudgement(const char *fileName);
	int FileSaveTypeJudgement(const char *fileName, int save_mode);
	
	// private pixel data format functions
	Pixel* SetPixel();
	RGBch* SetRGB();

	// private translation data format type functions
	int  TransStandard();
	int  TransFormat(int mode);
	void SaveAlphaMode(int mode);	// [ON_ALPHA | SAVE_ALPHA]	
	
protected:
	// protected data members
	int	  m_width;		// size of image width
	int	  m_height;		// size of image height
	int	  m_depth;		// size of image depth, channel 3 or 4
	byte *m_pData;		// image data pointer
	
	// class information members
	int	  m_alpha;		// alpha channel, [OFF_ALPHA | ON_ALPHA | SAVE_ALPHA]
	int	  m_type;		// data storage type, [rgbrgbrgb | bgrbgrbgr | rrrgggbbb]
	int	  m_nError;		// error information handler
	
	// protected process functions
	int SetAlphaChannel();
	int ResetAlphaChannel();
	
	// protected virtural functions
	virtual int ShelteringAlphaChannel(byte **alpha_buff);
	virtual int ReconstructAlphaChannel(byte *alpha_buff);
	
public:
	// public constructors
	ImageData(int alphaCh = OFF_ALPHA, int type = STANDARD_BMP_FORM);
	ImageData(const char *fileName, int alphaCh = OFF_ALPHA, int type = STANDARD_BMP_FORM);
	ImageData(const ImageData& img);		// copy constructor
	
	// public destructor
	virtual ~ImageData() { delete [] m_pData; }
	
	// operator definition
	ImageData& operator=(ImageData& img);
	
	// public access from I/O functions
	void Setting(int w, int h, int d, byte *dt);
	int& alpha() { return m_alpha; }
	int& type()	 { return m_type; }
	
	// memory free function
	void Cleanup() { delete [] m_pData; m_pData = NULL; m_nError = ERROR_DATA_EMPTY; }
	
	// public process functions, image data I/O.
	void Load(const char *fileName);
	void Save(const char *fileName, int save_mode = 0, int pix_rcvr = 1);
	// default is file-type judgement consigned and recover pixel condition ON.
	
	// public access functions
	int	   width()  { return m_width; }
	int	   height() { return m_height; }
	int	   depth()  { return m_depth; }
	byte  *data()   { return m_pData; }
	Pixel *pixel()  { return SetPixel(); }
	RGBch *rgb()	{ return SetRGB(); }
	
	// access functions. only names different, same behavior.
	int    GetWidth()  { return width(); }
	int	   GetHeight() { return height(); }
	int	   GetDepth()  { return depth(); }
	byte  *GetData()   { return data(); }
	Pixel *GetPixel()  { return pixel(); }
	RGBch *GetRGB()	   { return rgb(); }
	
	int	   getWidth()  { return width(); }
	int	   getHeight() { return height(); }
	int	   getDepth()  { return depth(); }
	byte  *getData()   { return data(); }
	Pixel *getPixel()  { return pixel(); }
	RGBch *getRGB()	   { return rgb(); }
	
	// error judgement function
	int ErrorJudge() { return m_nError; }
};	// class ImageData

inline void ImageData::Setting(int w, int h, int d, byte *dt)
{
	if(m_pData) Cleanup();
	m_width  = w;
	m_height = h;
	m_depth	 = d;
	m_pData  = dt;
	m_nError = ERROR_NONE;
}

#endif	// __IMAGE_DATA_H__

