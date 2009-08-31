/*+++
WinBitmapFormat.cpp
- a I/O process for Windows Bitmap routines.

  (c)2001, Shin OGATA <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa's code.
  
	First Created: Wed Sep 19 19:55:45 JST 2001
	Last Modified: Man Oct  1 16:41:29 JST 2001
---*/

#include "WinBitmapFormat.h"
#include <string.h>

int WinBmpIO::LoadImage(const char *fileName, ImageData *imgPtr)
{
	// Bitmap file open, binary mode.
	ifstream file(fileName, ios::in | ios::binary);
	if(!file) return ImageData::ERROR_CANT_OPEN_FILE;
	
	// read Bitmap type
	unsigned short bfType;
	file.read((char *)&bfType, sizeof(bfType));
	
	// check the file, either Bitmap or not.
	if(bfType != 0x4d42) {
		file.close(); 
		return ImageData::ERROR_NOT_BITMAP_FILE;
	}
	
	// read temporary members
	unsigned char temp[8];
	file.read((char *)&temp, sizeof(temp));
	
	// read offset info
	unsigned long bfOffBits;
	file.read((char *)&bfOffBits, sizeof(bfOffBits));
	
	// read Bitmap information header
	BmpInfoHeader lcBmpInfoHdr;
	file.read((char *)&lcBmpInfoHdr, sizeof(BmpInfoHeader));
	
	// check bit per pixel, 24 bit fix. under development.
	if(lcBmpInfoHdr.biBitCount != 0x0018) {
		file.close();
		return ImageData::ERROR_BAD_CHANNEL_NUM;
	}
	
	// check compression. if data was compressed, return. no compression's ID "0".
	if(lcBmpInfoHdr.biCompression) {
		file.close();
		return ImageData::ERROR_DATA_IS_COMPRESSED;
	}
	
	// setup member
	byte *bmp_data;
	int   bmp_width  = (int)lcBmpInfoHdr.biWidth;
	int   bmp_height = (int)lcBmpInfoHdr.biHeight;
	int   bmp_depth  = lcBmpInfoHdr.biBitCount / (sizeof(byte) * 8);
	
	// for padding, bitmap width scale is multiple of 4 bytes.
	int surplus = ((bmp_width * bmp_depth) % 4);
	int padding = surplus ? (4 - surplus) : (0);
	
	int all_size_with_padding = (bmp_width + padding) * bmp_height * bmp_depth;
	int all_data_size		  =  bmp_width * bmp_height * bmp_depth;
	
	// dummy buffer, memory allocation.
	byte *pDummy = new byte[all_size_with_padding];
	if(!pDummy){
		file.close();
		return ImageData::ERROR_CANT_ALLOCATE_MEMORY;
	}
	
	// seek set. top of the file.
	file.seekg(bfOffBits, ios::beg);
	
	// read buffer
	file.read((char *)pDummy, sizeof(byte) * all_size_with_padding);
	file.close();
	
	// for padding cut off
	int i, j;
	if(padding) {
		byte *pSend	   = pDummy;
		byte *pReceive = pDummy;
		for(j = 0; j < bmp_height; j++) {
			for(i = 0; i < bmp_width; i++) {
				*(pReceive + 0) = *(pSend + 0);
				*(pReceive + 1) = *(pSend + 1);
				*(pReceive + 2) = *(pSend + 2);
				pReceive += 3;
				pSend	 += 3;
			}
			pSend += padding;
		}
	}
	
	// image data, memory allocation
	if(!(bmp_data = new byte[all_data_size])) {
		delete [] pDummy;
		return ImageData::ERROR_CANT_ALLOCATE_MEMORY;
	}
	
	// set format image data
	switch(imgPtr->type()) {
	default:
		// change type [rgbrgbrgb]
		imgPtr->type() = ImageData::STANDARD_BMP_FORM;
		
	case ImageData::STANDARD_BMP_FORM:
		// Windows BMP form -> standard BMP form [bgrbgrbgr -> rgbrgbrgb]
		for(i = 0; i < all_data_size; i += 3) {
			*(bmp_data + i + 0)	= *(pDummy + i + 2);
			*(bmp_data + i + 1)	= *(pDummy + i + 1);
			*(bmp_data + i + 2)	= *(pDummy + i + 0);
		}
		delete [] pDummy;
		break;
		
	case ImageData::WINDOWS_BMP_FORM:
		// direct read [bgrbgrbgr -> bgrbgrbgr]
		::memcpy(bmp_data, pDummy, sizeof(byte) * all_data_size); 
		delete [] pDummy;
		break;
	}
	
	// setup to image data pointer
	imgPtr->Setting(bmp_width, bmp_height, bmp_depth, bmp_data);
	
	return ImageData::ERROR_NONE;
}

int WinBmpIO::SaveImage(const char *fileName, ImageData *imgPtr)
{
	// for padding, bitmap width scale is multiple of 4 bytes.
	int surplus = ((imgPtr->width() * imgPtr->depth()) % 4);
	int padding = surplus ? (4 - surplus) : (0);
	unsigned int all_data_size = (imgPtr->width() + padding) * imgPtr->height() * imgPtr->depth();
	
	// set bitmap file header
	unsigned short bfType	   = 0x4D42;						// bitmap ID
	unsigned long  bfSize	   = all_data_size;					// all image data size
	unsigned short bfReserved1 = 0x0000;						// reserved : 0
	unsigned short bfReserved2 = 0x0000;						// reserved : 0
	unsigned long  bfOffBits   = 14 + sizeof(BmpInfoHeader);	// "14" is size of bitmap file header
	
	// set bitmap information header
	BmpInfoHeader lcBmpInfoHdr;
	lcBmpInfoHdr.biSize			 = sizeof(BmpInfoHeader);		// info header size
	lcBmpInfoHdr.biWidth		 = imgPtr->width();				// image width
	lcBmpInfoHdr.biHeight		 = imgPtr->height();			// image height
	lcBmpInfoHdr.biPlanes		 = 0x0001;						// always "1"
	lcBmpInfoHdr.biBitCount		 = 0x0018;						// 24 bit fix.
	lcBmpInfoHdr.biCompression	 = 0x0000;						// no compression. "0"
	lcBmpInfoHdr.biSizeImage	 = all_data_size + bfOffBits;	// all file size
	lcBmpInfoHdr.biXPelsPerMeter = 0x0B12;						// horizontal resolution, fix.
	lcBmpInfoHdr.biYPelsPerMeter = 0x0B12;						// vertical resolution, fix.
	lcBmpInfoHdr.biClrUsed		 = 0x0000;						// using color index, "0"
	lcBmpInfoHdr.biClrImportant	 = 0x0000;						// important color code "0"
	
	// write file open
	ofstream file(fileName, ios::out | ios::binary);
	
	// write bitmap file header
	file.write((char *)&bfType, sizeof(unsigned short));
	file.write((char *)&bfSize, sizeof(unsigned long));
	file.write((char *)&bfReserved1, sizeof(unsigned short));
	file.write((char *)&bfReserved2, sizeof(unsigned short));
	file.write((char *)&bfOffBits, sizeof(unsigned long));
	
	// write bitmap information header
	file.write((char *)&lcBmpInfoHdr, sizeof(BmpInfoHeader));
	
	
	// write image data
	if(padding) {
		// write data, memory allocation
		byte *pSend	   = imgPtr->data();
		byte *pReceive = new byte[all_data_size];
		if(!pReceive){
			file.close();
			return ImageData::ERROR_CANT_ALLOCATE_MEMORY;
		}
		
		// set padding data
		byte *pData = pReceive;
		for(int j = 0; j < imgPtr->height(); j++) {
			for(int i = 0; i < imgPtr->width(); i++) {
				*(pReceive + 0) = *(pSend + 0);
				*(pReceive + 1) = *(pSend + 1);
				*(pReceive + 2) = *(pSend + 2);
				pReceive += 3;
				pSend	 += 3;
			}
			
			for(int k = 0; k < padding; k++) {
				*pReceive = 0x00;		// padding is 0 set.
				pReceive ++;			
			}
		}
		
		file.write((char *)pData, sizeof(byte) * all_data_size);
		file.close();
		delete [] pData;
	}
	
	// direct write
	else {
		file.write((char *)imgPtr->data(), sizeof(byte) * all_data_size);
		file.close();
	}
	
	return ImageData::ERROR_NONE;
}

