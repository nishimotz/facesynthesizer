/*+++
SgiImageFormat.cpp
- a I/O process for SGI RGB or RGBA image routines.

  (c)2001, Shin Ogata <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa's code.
  
	First Created: Thu Sep 27 16:27:45 JST 2001
	Last Modified: Fri Sep 28 22:20:22 JST 2001
---*/

#include "SgiImageFormat.h"
#include <string.h>

int SgiImgIO::LoadImage(const char *fileName, ImageData *imgPtr)
{
	// SGI image file open, binary mode.
	ifstream file(fileName, ios::in | ios::binary);
	if(!file) return ImageData::ERROR_CANT_OPEN_FILE;
	
	// read image file header
	SgiFileInfoHeader lcSgiFileInfoHdr;
	file.read((char *)&lcSgiFileInfoHdr, sizeof(SgiFileInfoHeader));
	
	// ++ Big Endian -> Little Endian ++
	lcSgiFileInfoHdr.sgiSM		= ChangeShort(lcSgiFileInfoHdr.sgiSM);
	lcSgiFileInfoHdr.sgiNumOfD	= ChangeShort(lcSgiFileInfoHdr.sgiNumOfD);
	lcSgiFileInfoHdr.sgiWidth	= ChangeShort(lcSgiFileInfoHdr.sgiWidth);
	lcSgiFileInfoHdr.sgiHeight	= ChangeShort(lcSgiFileInfoHdr.sgiHeight);
	lcSgiFileInfoHdr.sgiNumOfC	= ChangeShort(lcSgiFileInfoHdr.sgiNumOfC);
	lcSgiFileInfoHdr.sgiMin		= ChangeLong(lcSgiFileInfoHdr.sgiMin);
	lcSgiFileInfoHdr.sgiMax		= ChangeLong(lcSgiFileInfoHdr.sgiMax);
	lcSgiFileInfoHdr.sgiCMapID	= ChangeLong(lcSgiFileInfoHdr.sgiCMapID);
	// -- Big Endian -> Little Endian --
	
	// check the file, either "rgb" or not.
	if(lcSgiFileInfoHdr.sgiSM != 0x01da) {
		file.close(); 
		return ImageData::ERROR_NOT_SGI_IMAGE_FILE;
	}
	
	// if file was compressed, return error.
	if(lcSgiFileInfoHdr.sgiSF != 0x00) {
		file.close();
		return ImageData::ERROR_DATA_IS_COMPRESSED;
	}
	
	// check byte per channel of pixel, 1byte fix.
	if(lcSgiFileInfoHdr.sgiNumOfB != 0x0001) {
		file.close();
		return ImageData::ERROR_FILE_NOT_SUPPORTED;
	}
	
	// check number of channels, 3 or 4 fix. under development.
	if(lcSgiFileInfoHdr.sgiNumOfC != 0x0003 && lcSgiFileInfoHdr.sgiNumOfC != 0x0004) {
		file.close();
		return ImageData::ERROR_BAD_CHANNEL_NUM;
	}
	
	// alpha evaluation, case of read alpha channel.
	if(lcSgiFileInfoHdr.sgiNumOfC == 0x0004) imgPtr->alpha() = ImageData::SAVE_ALPHA;
	
	// Image size/channel
	byte *rgb_data;
	int   rgb_width  = (int)lcSgiFileInfoHdr.sgiWidth;
	int   rgb_height = (int)lcSgiFileInfoHdr.sgiHeight;
	int   rgb_depth  = (int)lcSgiFileInfoHdr.sgiNumOfC;
	
	int one_data_size = rgb_width * rgb_height;
	int all_data_size = one_data_size * rgb_depth;
	
	// dummy data, memory allocation
	byte *pDummy = new byte[all_data_size];
	if(!pDummy){
		file.close();
		return ImageData::ERROR_CANT_ALLOCATE_MEMORY;
	}
	
	// seek to image data head
	file.seekg(sizeof(SgiFileInfoHeader), ios::beg);
	
	// read buffer
	file.read((char *)pDummy, sizeof(byte) * all_data_size);
	file.close();
	
	// image data memory allocation
	if(!(rgb_data = new byte[all_data_size])) {
		delete [] pDummy;
		return ImageData::ERROR_CANT_ALLOCATE_MEMORY;
	}
	
	// set format image data
	int i, j;
	switch(imgPtr->type()) {
	default:
		// change type [rgbrgbrgb]
		imgPtr->type() = ImageData::STANDARD_BMP_FORM;
		
	case ImageData::STANDARD_BMP_FORM:
		// SGI form -> standard BMP form [rrrgggbbb] -> [rgbrgbrgb]
		if(imgPtr->alpha() != ImageData::SAVE_ALPHA) {
			for(i = 0, j = 0; i < all_data_size; i += 3, j++) {
				*(rgb_data + i + 0)	= *(pDummy + one_data_size * 0 + j);
				*(rgb_data + i + 1)	= *(pDummy + one_data_size * 1 + j);
				*(rgb_data + i + 2)	= *(pDummy + one_data_size * 2 + j);
			}
		}
		
		// for case of 32 bit with alpha channel.
		else {
			for(i = 0, j = 0; i < all_data_size; i += 4, j++) {
				*(rgb_data + i + 0)	= *(pDummy + one_data_size * 0 + j);
				*(rgb_data + i + 1)	= *(pDummy + one_data_size * 1 + j);
				*(rgb_data + i + 2)	= *(pDummy + one_data_size * 2 + j);
				*(rgb_data + i + 3)	= *(pDummy + one_data_size * 3 + j);
			}
		}
		delete [] pDummy;
		break;
		
	case ImageData::SGI_RGB_FORM:
		// direct read [rrrgggbbb] -> [rrrgggbbb]
		::memcpy(rgb_data, pDummy, sizeof(byte) * all_data_size); 
		delete [] pDummy;
		break;
		
	case ImageData::WINDOWS_BMP_FORM:
		// SGI form -> BMP form of Windows [rrrgggbbb] -> [bgrbgrbgr]
		if(imgPtr->alpha() != ImageData::SAVE_ALPHA) {
			for(i = 0, j = 0; i < all_data_size; i += 3, j++) {
				*(rgb_data + i + 0)	= *(pDummy + one_data_size * 2 + j);
				*(rgb_data + i + 1)	= *(pDummy + one_data_size * 1 + j);
				*(rgb_data + i + 2)	= *(pDummy + one_data_size * 0 + j);
			}
		}
		
		// for case of 32 bit with alpha channel.
		else {
			for(i = 0, j = 0; i < all_data_size; i += 4, j++) {
				*(rgb_data + i + 0)	= *(pDummy + one_data_size * 3 + j);
				*(rgb_data + i + 1)	= *(pDummy + one_data_size * 2 + j);
				*(rgb_data + i + 2)	= *(pDummy + one_data_size * 1 + j);
				*(rgb_data + i + 3)	= *(pDummy + one_data_size * 0 + j);
			}
		}
		delete [] pDummy;
		break;
	}
	
	// setup to image data pointer
	imgPtr->Setting(rgb_width, rgb_height, rgb_depth, rgb_data);
	
	return ImageData::ERROR_NONE;
}

int SgiImgIO::SaveImage(const char *fileName, ImageData *imgPtr)
{
	unsigned int all_data_size = imgPtr->width() * imgPtr->height() * imgPtr->depth();
	
	// set bitmap file header
	SgiFileInfoHeader lcSgiFileInfoHdr;
	lcSgiFileInfoHdr.sgiSM		= 0x01da;
	lcSgiFileInfoHdr.sgiSF		= 0x00;
	lcSgiFileInfoHdr.sgiNumOfB	= 0x0001;
	lcSgiFileInfoHdr.sgiNumOfD	= 0x0003;
	lcSgiFileInfoHdr.sgiWidth	= imgPtr->width();
	lcSgiFileInfoHdr.sgiHeight	= imgPtr->height();
	lcSgiFileInfoHdr.sgiNumOfC	= imgPtr->depth();
	lcSgiFileInfoHdr.sgiMin		= 0x00000000;
	lcSgiFileInfoHdr.sgiMax		= 0x000000ff;
	//	sprintf((char *)&lcSgiFileInfoHdr.sgiIg1, "4 character reserved");
	//	sprintf((char *)&lcSgiFileInfoHdr.sgiImgName, "80 character reserved");
	lcSgiFileInfoHdr.sgiCMapID	= 0x00000000;
	//	sprintf((char *)&lcSgiFileInfoHdr.sgiIg2, "404 character reserved");
	
	// ++ Little Endian -> Big Endian ++
	lcSgiFileInfoHdr.sgiSM		= ChangeShort(lcSgiFileInfoHdr.sgiSM);
	lcSgiFileInfoHdr.sgiNumOfD	= ChangeShort(lcSgiFileInfoHdr.sgiNumOfD);
	lcSgiFileInfoHdr.sgiWidth	= ChangeShort(lcSgiFileInfoHdr.sgiWidth);
	lcSgiFileInfoHdr.sgiHeight	= ChangeShort(lcSgiFileInfoHdr.sgiHeight);
	lcSgiFileInfoHdr.sgiNumOfC	= ChangeShort(lcSgiFileInfoHdr.sgiNumOfC);
	lcSgiFileInfoHdr.sgiMin		= ChangeLong(lcSgiFileInfoHdr.sgiMin);
	lcSgiFileInfoHdr.sgiMax		= ChangeLong(lcSgiFileInfoHdr.sgiMax);
	lcSgiFileInfoHdr.sgiCMapID	= ChangeLong(lcSgiFileInfoHdr.sgiCMapID);
	// -- Little Endian -> Big Endian --
	
	// write file open
	ofstream file(fileName, ios::out | ios::binary);
	
	// write SGI image information header
	file.write((char *)&lcSgiFileInfoHdr, sizeof(SgiFileInfoHeader));
	
	// write image data
	file.write((char *)imgPtr->data(), sizeof(byte) * all_data_size);
	
	// file close
	file.close();
	
	return ImageData::ERROR_NONE;
}

