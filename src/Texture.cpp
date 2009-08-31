/*+++
Texture.cpp
- a setup texture image routines.

  (c)2001, Shin Ogata <shin@ee.seikei.ac.jp>
  for Morishima-lab., Seikei University.
  Referred to Takafumi Misawa & Daisuke Ito's code.
  
	First Created: Man Jul 16 11:55:45 JST 2001
	Last Modified: under development.
---*/

#include "Texture.h"
#include "GL/glut.h"
#include <string.h>
#include <stdlib.h>

// copy constructor
Texture::Texture(const Texture& tex)
{
	this->m_width  = tex.m_width;
	this->m_height = tex.m_height;
	this->m_depth  = tex.m_depth;
	this->m_alpha  = tex.m_alpha;
	this->m_type   = STANDARD_BMP_FORM;
	this->m_sFileName = tex.m_sFileName;
	this->m_texture_id = tex.m_texture_id;

	// image data, memory allocation
	int all_data_size = m_width * m_height * m_depth;
	this->m_pData = new byte[all_data_size];
	
	if(!(this->m_pData))
		this->m_nError = ERROR_CANT_ALLOCATE_MEMORY;
	
	else {
		::memcpy(this->m_pData, tex.m_pData, sizeof(byte) * all_data_size);
		this->m_nError	 = tex.m_nError;
	}
}

void Texture::BindTexture()
{
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(0.5, 0.5, 0.0);
		glScalef(0.5, 0.5, 1.0);
	glMatrixMode(GL_MODELVIEW);


	if ( m_pData == NULL ) { // pnt format

		pngLoadRaw(m_sFileName.c_str(), &m_pngInfo);
		m_width = m_pngInfo.Width;
		m_height = m_pngInfo.Height;
		m_alpha = OFF_ALPHA;
		m_depth = 3;
		m_pData = new byte[m_height * m_width * m_depth];
	
		//swap
		for( int i = 0; i < m_height; i++ ) {
			for( int j = 0; j < m_width; j++ ) {
				m_pData[(i * m_width + j) * 3 + 0] = m_pngInfo.Data[((m_height - i) * m_width + j) * 3 + 0];
				m_pData[(i * m_width + j) * 3 + 1] = m_pngInfo.Data[((m_height - i) * m_width + j) * 3 + 1];
				m_pData[(i * m_width + j) * 3 + 2] = m_pngInfo.Data[((m_height - i) * m_width + j) * 3 + 2];
			}
		}
		free(m_pngInfo.Data);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, GetWidth(), GetHeight(), 
			0, GL_RGB, GL_UNSIGNED_BYTE, GetData());
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, 3, GetWidth(), GetHeight(), 
			0, GL_RGBA, GL_UNSIGNED_BYTE, GetData());
	}
}

// ++ public process function ++
void Texture::ChangeAlpha(int x, int y, byte value)
{
	Pixel *pix = pixel();
	pix[y * m_width + x].a = value;
}

void Texture::ChangeAlpha(const byte *LumImg)
{
	Pixel  *pix = pixel();
	int		i, j;
	int		diff, left, right, top, bottom;
	double	ratio;
	
	// search top and bottom points from horizontal
	for(i = 0; i < m_width; i++) {
		top = bottom = 0;
		for(j = 0; j < m_height; j++) {
			if(LumImg[(m_width * j) + i] == 255) { // white area detection
				bottom = j; 
				break; 
			} 
		}
		
		for(j = m_height - 1; j >= 0; j--) { 
			if(LumImg[(m_width * j) + i] == 255) { // white area detection
				top = j; 
				break; 
			} 
		}
		
		// calculation brending ratio
		ratio = 0.0;
		diff = top - bottom;
		for(j = 0; j < m_height; j++) {
			if(diff) {
				if(j >= bottom && j < top - 10) { 
					ratio += 0.1; 		
					if(ratio > 1.0)	ratio = 1.0; 
				}
				else if(j >= top - 10 && j < top) { 
					ratio -= 0.1; 	
					if(ratio < 0.0) ratio = 0.0; 
				}
				else ratio = 0.0;
			} 
			
			else ratio = 0.0;
			
			// set alpha value
			pix[(j * m_width) + i].a = (byte)(pix[(j * m_width) + i].a * ratio);
		}
	}
	
	// search left and right points from vertical
	for(j = 0; j < m_height; j++) {
		left = right = 0;
		for(i = 0; i < m_width; i++) {
			if(LumImg[(m_width * j) + i] == 255) { // white area detection
				left = i; 
				break; 
			} 
		}
		
		for(i = m_width - 1; i >= 0; i--) {
			if(LumImg[(m_width * j) + i] == 255) { // white area detection
				right = i;
				break;
			}
		}
		
		// calculation brending ratio
		ratio = 0.0;
		diff = right - left;
		for(i = 0; i < m_width; i++) {
			if(diff) {
				if(i >= left && i < right - 10) {
					ratio += 0.1;
					if(ratio > 1.0) ratio = 1.0;
				}
				else if(i >= right - 10 && i < right) {
					ratio -= 0.1;
					if(ratio < 0.0) ratio = 0.0;
				}
				else ratio = 0.0;
			}
			
			else ratio = 0.0;
			
			// set alpha value
			pix[(j * m_width) + i].a = (byte)(pix[(j * m_width) + i].a * ratio);
		}
	}
}
// -- public process function --

