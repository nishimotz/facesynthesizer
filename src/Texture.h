
#ifndef __TEXTURE_IMAGE_H__
#define __TEXTURE_IMAGE_H__

/*+++
Texture.h
- a header file for setup texture image routines.

  (c)2001, Shin Ogata <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa & Daisuke Ito's code.
  
	First Created: Man Jul 16 11:55:45 JST 2001
	Last Modified: under development for Change Alpha routine.
---*/

#include "ImageData.h"
#include "glpng/glpng.h"

#include <iostream>
#include <string>

using namespace std;

class Texture : public ImageData {
private:
	int m_fileType;
	unsigned int m_texture_id;
	pngRawInfo m_pngInfo;
	string m_sFileName;
	
public:
	// public constructors, default : with alpha channel.
	Texture(int alphaCh = ON_ALPHA) : ImageData(alphaCh) {
		m_texture_id = 0;
		m_sFileName = string(NULL);
		BindTexture();
	}
	Texture(const char *fileName, int alphaCh = ON_ALPHA) : ImageData(fileName, alphaCh) {
		m_texture_id = 0;
		m_sFileName = string(fileName);
		BindTexture();
	}
	Texture(const Texture &tex);	// copy constructor
	
	// public destructor
	virtual ~Texture() {}
	
	// public data output function
	void GetSize(int& x, int& y) { x = m_width; y = m_height; }
	
	// public process function
	int SetAlpha()	 { return SetAlphaChannel(); }
	int ResetAlpha() { return ResetAlphaChannel(); }
	
	// change alpha value functions
	void ChangeAlpha(int x, int y, byte value);	// for one pixel
	void ChangeAlpha(const byte *lumImg);		// using luminance image
	
	int GetFileType() { return m_fileType; }

	void BindTexture();
	unsigned int GetTextureID() {	return m_texture_id; }

	
};

#endif	// __TEXTURE_IMAGE_H__

