/*+++
ImageData.cpp
- a image data read, write and store routines.
supported : [24bit bmp] [24bit rgb/sgi] [32bit rgb/rgba/sgi]

  (c)2001, Shin Ogata <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa & Daisuke Ito's code.
  
	First Created: Man Jul 16 11:55:45 JST 2001
	Last Modified: Man Nov 28 07:41:29 JST 2001
	!! under construction for any file-format., p.r.n. !!
---*/

#include "ImageData.h"
#include <string.h>
#include <stdio.h>

// +++ pulic constructor +++
ImageData::ImageData(int alphaCh, int type)
{
	m_pData	 = NULL;
	m_alpha	 = (alphaCh) ? (ON_ALPHA) : (OFF_ALPHA);
	m_type	 = type;
	m_nError = ERROR_DATA_EMPTY;
}

ImageData::ImageData(const char *fileName, int alphaCh, int type)
{
	m_pData	= NULL;
	m_alpha	= (alphaCh) ? (ON_ALPHA) : (OFF_ALPHA);
	m_type	= type;
	
	Load(fileName);		// image data loading
}

ImageData::ImageData(const ImageData& img)
{
	this->m_width  = img.m_width;
	this->m_height = img.m_height;
	this->m_depth  = img.m_depth;
	this->m_alpha  = img.m_alpha;
	this->m_type   = img.m_type;
	
	// image data, memory allocation
	int all_data_size = img.m_width * img.m_height * img.m_depth;
	this->m_pData = new byte[all_data_size];
	
	if(!(this->m_pData))
		this->m_nError = ERROR_CANT_ALLOCATE_MEMORY;
	
	else {
		::memcpy(this->m_pData, img.m_pData, sizeof(byte) * all_data_size);
		this->m_nError	 = img.m_nError;
	}
}
// --- public constructor ---

// +++ operator definition +++
ImageData& ImageData::operator=(ImageData& img)
{
	this->m_width  = img.m_width;
	this->m_height = img.m_height;
	this->m_depth  = img.m_depth;
	this->m_alpha  = img.m_alpha;
	this->m_type   = img.m_type;
	
	// image data, memory allocation
	int all_data_size = img.m_width * img.m_height * img.m_depth;
	if(this->m_pData)	delete [] this->m_pData;
	this->m_pData = new byte[all_data_size];
	
	if(!(this->m_pData))
		this->m_nError = ERROR_CANT_ALLOCATE_MEMORY;
	
	else {
		::memcpy(this->m_pData, img.m_pData, sizeof(byte) * all_data_size);
		this->m_nError = img.m_nError;
	}
	
	return *this;
}
// --- operator definition ---

// +++ private process functions +++
int ImageData::FileExistenceJudgement(const char *fileName)
{
	FILE *checker = fopen(fileName, "r");
	if(!checker)
		return ERROR_CANT_OPEN_FILE;
	else {
		fclose(checker);
		return ERROR_NONE;
	}
}

int ImageData::FileLoadTypeJudgement(const char *fileName)
{
	unsigned short ckHdr;								// for file header check, 2 byte.
	ifstream file(fileName, ios::in | ios::binary);		// image file open, binary mode
	file.read((char *)&ckHdr, sizeof(unsigned short));
	
	// check the file type.
	int read_format = ERROR_FILE_NOT_SUPPORTED;			// bitmap format mode
	if(ckHdr == 0x4d42) {								// type Windows Bitmap format
		file.close();
		return WINDOWS_BMP_FORM;
	}
	
	else if(ckHdr == 0xda01) {							// type SGI Bitmap format
		file.close();
		return SGI_RGB_FORM;
	}
	else if(ckHdr == 0x5089) {
		file.close();
		return PNG_FORM;
	}
	else {												// case of no supported.
		file.close();
		return ERROR_FILE_NOT_SUPPORTED;
	}
}

int ImageData::FileSaveTypeJudgement(const char *fileName, int save_mode)
{
	// if save mode designated, return. 
	switch(save_mode) {
	case WINDOWS_BMP_FORM:
		
	case SGI_RGB_FORM:
		return save_mode;
		
	case SAVE_ALPHA:
		SaveAlphaMode(SAVE_ALPHA);
		return SGI_RGB_FORM;
		
	default:
		break;
	}
	
	// get file's suffix
	int	  f_len = strlen(fileName);				// file name length
	int	  p_len = strcspn(fileName, ".");		// path length
	
	// default is windows bmp form. case of no suffix file name.
	if(f_len == p_len)	return WINDOWS_BMP_FORM;
	
	// suffix, memory allocation
	int   s_len  = f_len - p_len - 1;
	char *suffix = new char[s_len + 1];
	if(!suffix)	return ERROR_CANT_ALLOCATE_MEMORY;
	
	// set pointer address
	fileName += (p_len + 1);
	
	// set suffix
	strncpy(suffix, fileName, s_len);
	suffix[s_len] = '\0';
	
	// judgement
	if(!strcmp(suffix, "bmp") || !strcmp(suffix, "BMP")) {
		delete [] suffix;
		return WINDOWS_BMP_FORM;
	}
	else if(!strcmp(suffix, "png") || !strcmp(suffix, "PNG")) {
		delete [] suffix;
		return PNG_FORM;
	}
	else if(!strcmp(suffix, "rgb") || !strcmp(suffix, "RGB")
		|| !strcmp(suffix, "sgi") || !strcmp(suffix, "SGI")) {
		delete [] suffix;
		return SGI_RGB_FORM;
	}
	
	else if(!strcmp(suffix, "rgba") || !strcmp(suffix, "RGBA")) {
		delete [] suffix;
		SaveAlphaMode(SAVE_ALPHA);
		return SGI_RGB_FORM;
	}
	
	// memory free
	delete [] suffix;
	
	// default is windows bmp form.
	return WINDOWS_BMP_FORM;
}

Pixel* ImageData::SetPixel()
{
	// judgement of bmp format
	if(m_type != STANDARD_BMP_FORM)
		if(m_nError = TransStandard())	return NULL;
		
		// judgement of alpha channel existence
		if(!m_alpha)
			if(m_nError = SetAlphaChannel()) return NULL;
			
			return((Pixel *)m_pData);
}

RGBch* ImageData::SetRGB()
{
	// judgement of bmp format
	if(m_type != STANDARD_BMP_FORM)
		if(m_nError = TransStandard())	return NULL;
		
		// judgement of alpha channel existence
		if(m_alpha)
			if(m_nError = ResetAlphaChannel()) return NULL;
			
			return((RGBch *)m_pData);
}

// pixel format translation functions
int ImageData::TransStandard()
{
	int   i, j;
	int	  one_data_size = m_width * m_height;
	int	  all_data_size = one_data_size * m_depth;
	byte *pDummy = NULL;
	
	switch(m_type) {
	case WINDOWS_BMP_FORM:
		// dummy memory allocation
		if(!(pDummy = new byte[all_data_size]))
			return ERROR_CANT_ALLOCATE_MEMORY;
		
		// copy to dummy memory
		::memcpy(pDummy, m_pData, sizeof(byte) * all_data_size);
		
		// set standard form	[bgrbgrbgr] -> [rgbrgbrgb] 
		if(!m_alpha) {
			for(i = 0; i < all_data_size; i += 3) {
				*(m_pData + i + 0)	= *(pDummy + i + 2);
				*(m_pData + i + 1)	= *(pDummy + i + 1);
				*(m_pData + i + 2)	= *(pDummy + i + 0);
			}
		}
		
		// for case of alpha existence.
		else {
			for(i = 0; i < all_data_size; i += 4) {
				*(m_pData + i + 0)	= *(pDummy + i + 3);
				*(m_pData + i + 1)	= *(pDummy + i + 2);
				*(m_pData + i + 2)	= *(pDummy + i + 1);
				*(m_pData + i + 3)	= *(pDummy + i + 0);
			}
		}
		break;
		
	case SGI_RGB_FORM:
		// dummy memory allocation
		if(!(pDummy = new byte[all_data_size]))
			return ERROR_CANT_ALLOCATE_MEMORY;
		
		// copy to dummy memory
		::memcpy(pDummy, m_pData, sizeof(byte) * all_data_size);
		
		// set standard form	[rrrgggbbb] -> [rgbrgbrgb] 
		if(!m_alpha) {
			for(i = 0, j = 0; i < all_data_size; i += 3, j++) {
				*(m_pData + i + 0)	= *(pDummy + one_data_size * 0 + j);
				*(m_pData + i + 1)	= *(pDummy + one_data_size * 1 + j);
				*(m_pData + i + 2)	= *(pDummy + one_data_size * 2 + j);
			}
		}
		
		// for case of alpha existence.
		else {
			for(i = 0, j = 0; i < all_data_size; i += 4, j++) {
				*(m_pData + i + 0)	= *(pDummy + one_data_size * 0 + j);
				*(m_pData + i + 1)	= *(pDummy + one_data_size * 1 + j);
				*(m_pData + i + 2)	= *(pDummy + one_data_size * 2 + j);
				*(m_pData + i + 3)	= *(pDummy + one_data_size * 3 + j);
			}
		}
		break;
		
	default:
		return ERROR_NONE;
	}
	
	// type set and memory free.
	delete [] pDummy;
	m_type = STANDARD_BMP_FORM;
	
	return ERROR_NONE;
}

int ImageData::TransFormat(int mode)
{
	// m_type = mode return.
	if(m_type == mode) return ERROR_NONE;
	
	// translation standard bmp format
	if(m_type != STANDARD_BMP_FORM)
		if(int err = TransStandard()) return err;
		
		// mode was standard, return.
		if(mode == STANDARD_BMP_FORM)	return ERROR_NONE;
		
		// translation pixel format
		int   i, j;
		int	  one_data_size = m_width * m_height;
		int	  all_data_size = one_data_size * m_depth;
		byte *pDummy = NULL;
		
		switch(mode) {
		case WINDOWS_BMP_FORM:
			// dummy memory allocation
			if(!(pDummy = new byte[all_data_size]))
				return ERROR_CANT_ALLOCATE_MEMORY;
			
			// copy to dummy memory
			::memcpy(pDummy, m_pData, sizeof(byte) * all_data_size);
			
			// set standard form	[rgbrgbrgb] -> [bgrbgrbgr] 
			if(!m_alpha) {
				for(i = 0; i < all_data_size; i += 3) {
					*(m_pData + i + 0)	= *(pDummy + i + 2);
					*(m_pData + i + 1)	= *(pDummy + i + 1);
					*(m_pData + i + 2)	= *(pDummy + i + 0);
				}
			}
			
			// for case of alpha existence.
			else {
				for(i = 0; i < all_data_size; i += 4) {
					*(m_pData + i + 0)	= *(pDummy + i + 3);
					*(m_pData + i + 1)	= *(pDummy + i + 2);
					*(m_pData + i + 2)	= *(pDummy + i + 1);
					*(m_pData + i + 3)	= *(pDummy + i + 0);
				}
			}
			break;
			
		case SGI_RGB_FORM:
			// dummy memory allocation
			if(!(pDummy = new byte[all_data_size]))
				return ERROR_CANT_ALLOCATE_MEMORY;
			
			// copy to dummy memory
			::memcpy(pDummy, m_pData, sizeof(byte) * all_data_size);
			
			// set standard form	[rrrgggbbb] -> [rgbrgbrgb] 
			if(!m_alpha) {
				for(i = 0, j = 0; j < all_data_size; i ++, j+= 3) {
					*(m_pData + one_data_size * 0 + i) = *(pDummy + j + 0);
					*(m_pData + one_data_size * 1 + i) = *(pDummy + j + 1);
					*(m_pData + one_data_size * 2 + i) = *(pDummy + j + 2);
				}
			}
			
			// for case of alpha existence.
			else {
				for(i = 0, j = 0; j < all_data_size; i ++, j+= 4) {
					*(m_pData + one_data_size * 0 + i) = *(pDummy + j + 0);
					*(m_pData + one_data_size * 1 + i) = *(pDummy + j + 1);
					*(m_pData + one_data_size * 2 + i) = *(pDummy + j + 2);
					*(m_pData + one_data_size * 3 + i) = *(pDummy + j + 3);
				}
			}
			break;
		}
		
		// type set and memory free.
		delete [] pDummy;
		m_type = mode;
		
		return ERROR_NONE;
}

// save alpha mode, change function
void ImageData::SaveAlphaMode(int mode)
{
	if(!m_alpha) SetAlphaChannel();
	m_alpha = mode;
}
// --- private process functions ---

// +++ protected process functions +++
int ImageData::SetAlphaChannel()
{
	// check current number of channel.
	if(m_depth == 4) { m_alpha = ON_ALPHA; return ERROR_NONE; }
	
	byte *pDummy;
	int   one_data_size = m_width * m_height;
	int   all_data_size = one_data_size * 3;
	
	// allocate dummy image memory.
	if(!(pDummy = new byte[all_data_size]))
		return ERROR_CANT_ALLOCATE_MEMORY;
	
	// copy image data.
	::memcpy(pDummy, m_pData, sizeof(byte) * all_data_size);
	delete [] m_pData;
	
	// renewal all data size
	all_data_size += one_data_size;
	
	// allocate image memory.
	if(!(m_pData = new byte[all_data_size]))
		return ERROR_CANT_ALLOCATE_MEMORY;
	
	// format, depth 3 -> 4.			0xFF = 255 : alpha default value.
	::memset(m_pData, 0xFF, sizeof(byte) * all_data_size);
	for(int i = 0, j = 0; i < all_data_size; i += 4, j += 3) {
		*(m_pData + i + 0) = *(pDummy + j + 0);
		*(m_pData + i + 1) = *(pDummy + j + 1);
		*(m_pData + i + 2) = *(pDummy + j + 2);
	}
	delete [] pDummy;
	
	// set information
	m_depth = 4;
	m_alpha = ON_ALPHA;
	
	return ERROR_NONE;
}

int ImageData::ResetAlphaChannel()
{
	// check current number of channel.
	if(m_depth == 3) { m_alpha = OFF_ALPHA; return ERROR_NONE; }
	
	byte *pDummy;
	int   one_data_size = m_width * m_height;
	int   all_data_size = one_data_size * 4;
	
	// allocate dummy image memory.
	if(!(pDummy = new byte[all_data_size]))
		return ERROR_CANT_ALLOCATE_MEMORY;
	
	// copy image data.
	::memcpy(pDummy, m_pData, sizeof(byte) * all_data_size);
	delete [] m_pData;
	
	// renewal all data size
	all_data_size -= one_data_size;
	
	// allocate image memory.
	if(!(m_pData = new byte[all_data_size]))
		return ERROR_CANT_ALLOCATE_MEMORY;
	
	// format, depth 4 -> 3.
	for(int i = 0, j = 0; i < all_data_size; i += 3, j += 4) {
		*(m_pData + i + 0) = *(pDummy + j + 0);
		*(m_pData + i + 1) = *(pDummy + j + 1);
		*(m_pData + i + 2) = *(pDummy + j + 2);
	}
	delete [] pDummy;
	
	// set information
	m_depth = 3;
	m_alpha = OFF_ALPHA;
	
	return ERROR_NONE;
}
// --- protected process functions ---

// +++ protected virtual functions +++
int ImageData::ShelteringAlphaChannel(byte **alpha_buff)
{
	// pixel setup
	Pixel *pix = pixel();
	if(!pix)	return ERROR_CANT_ALLOCATE_MEMORY;
	
	// shelter alpha buffer, memory allocation
	int	   one_data_size = m_width * m_height;
	*alpha_buff = new byte[one_data_size];
	if(!alpha_buff[0])	return ERROR_CANT_ALLOCATE_MEMORY;
	
	// set alpha channel
	for(int i = 0; i < one_data_size; i ++)
		alpha_buff[0][i] = pix[i].a;
	
	// reset alpha channel
	return ResetAlphaChannel();
}

int ImageData::ReconstructAlphaChannel(byte *alpha_buff)
{
	// pixel setup
	Pixel *pix = pixel();
	if(!pix)	return ERROR_CANT_ALLOCATE_MEMORY;
	
	// set alpha channel
	int one_data_size = m_width * m_height;
	for(int i = 0; i < one_data_size; i ++)
		pix[i].a = alpha_buff[i];
	
	return ERROR_NONE;
}
// --- protected virtual functions ---

// +++ public process functions, image data I/O. +++ 
void ImageData::Load(const char *fileName)
{
	// check file's existence
	m_nError = FileExistenceJudgement(fileName);
	if(m_nError)	return;
	
	// decide read file-type
	int read_format = FileLoadTypeJudgement(fileName);
	
	// load image data
	WinBmpIO bmpIO;				// bitmap I/O function
	SgiImgIO rgbIO;				// sgi image I/O function
	switch(read_format) {
	case WINDOWS_BMP_FORM:
		m_nError = bmpIO.LoadImage(fileName, this);
		break;
		
	case SGI_RGB_FORM:
		m_nError = rgbIO.LoadImage(fileName, this);
		break;
	case PNG_FORM:
		// skip
		return;
	default:
		m_nError = read_format;
		return;
	}
	
	// set alpha channel
	if(!(m_nError) && (m_alpha == ON_ALPHA))
		m_nError = SetAlphaChannel();
	
	// reset alpha save mode
	else if(m_alpha == SAVE_ALPHA) m_alpha = ON_ALPHA;
}

void ImageData::Save(const char *fileName, int save_mode, int pix_rcvr)
{
	// error check.
	if(m_nError) return;
	
	// decide save-file type
	int write_format = FileSaveTypeJudgement(fileName, save_mode);
	
	// alpha channel judgement, for no save mode.
	int ogn_type = m_type; int alpha_flag = 0;
	byte *alpha_buff = NULL;
	if((m_alpha == ON_ALPHA)) {
		if(pix_rcvr) {
			ShelteringAlphaChannel((byte **)&alpha_buff);
			alpha_flag = 1;
		}
		else ResetAlphaChannel();
	}
	
	// save image data
	WinBmpIO bmpIO;				// bitmap I/O function
	SgiImgIO rgbIO;				// sgi image I/O function
	int err = ERROR_NONE;
	switch(write_format) {
	case WINDOWS_BMP_FORM:
		if(err = TransFormat(write_format)) break;
		err = bmpIO.SaveImage(fileName, this);
		break;
		
	case SGI_RGB_FORM:
		if(err = TransFormat(write_format)) break;
		err = rgbIO.SaveImage(fileName, this);
		break;
		
	default:
		err = write_format;
		break;
	}
	
	// reconstruction alpha channel
	if(alpha_flag) {
		ReconstructAlphaChannel(alpha_buff);
		delete [] alpha_buff;
	}
	
	// reset alpha save mode
	else if(m_alpha == SAVE_ALPHA) SaveAlphaMode(ON_ALPHA);
	
	// recover original type
	if(pix_rcvr) TransFormat(ogn_type);
	
	// error information set
	m_nError = err;
}
// --- public process functions, image data I/O. ---

