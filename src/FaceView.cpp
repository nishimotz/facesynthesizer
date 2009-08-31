#ifdef WIN32
#include <windows.h>
#endif

#include "FaceView.h"

#include <GL/gl.h>

#include "FaceModel.h"
#include "Texture.h"
#include "Trans.h"
#include "trace.h"

// Constructor of class FaceView
FaceView::FaceView(FaceModel **model)
{
	TRACE("FaceView::FaceView(FaceModel **)");
	
	_model = model;
	_modelView = FV_TEXTURED;
	_backgroundVisible = 1;
	_saveFilename = NULL;
	_saveImageFlag = 0;

	m_point.x = m_point.y = 0;					// mouse position
	m_bLButtonDown = m_bRButtonDown = m_bMButtonDown = false;	// button status bool
	m_enable = true;

	// set position
	ResetTrans();

	ResetHeadTrans();

}

// Destructor of class FaceView
FaceView::~FaceView()
{
	TRACE("FaceView::~FaceView()");
}

// Handling redraw request.
void FaceView::redraw(int num)
{
	TRACE("FaceView::redraw()");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if(_model[num]) drawFaceModel(_model[num]);
}

// Drawing face model. (for use Preview Window)
void FaceView::drawFaceModel(FaceModel *)
{
	TRACE("FaceView::drawFaceModel(FaceModel *)");
}

// Saving image.

int FaceView::saveImage(char *filename, int num, int w, int h)
{
	TRACE("FaceView::saveImage(char *)");

	redraw(num);

	glReadBuffer(GL_FRONT);

	ImageData *image = new ImageData();

	byte *pix = new byte[w * h * 3];
	
	glReadPixels(0 , 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pix);

	image->Setting(w, h, 3, pix);
	image->Save(filename);

	//delete [] pix;
	delete image;
	
	return 0;
}


void FaceView::LeftMousePushEvent(int x, int y, int status)
{	
	m_point.x = x;
	m_point.y = y;
	
	m_bLButtonDown = true;
}

void FaceView::MidMousePushEvent(int x, int y, int status)
{
	m_point.x = x;
	m_point.y = y;
	
	m_bMButtonDown = true;
}

void FaceView::RightMousePushEvent(int x, int y, int status)
{
	
	m_point.x = x;
	m_point.y = y;
	
	m_bRButtonDown = true;
}

void FaceView::MouseDragEvent(int x, int y, int status, int w, int h, 
							  double rot[3], double trans[2], double &scale)
{
	
	if(m_bLButtonDown) {		
		double xr = m_rot[0];
		double yr = m_rot[1];
		double zr = m_rot[2];
		
		xr += (y - m_point.y) / 5.0;
		yr += (x - m_point.x) / 5.0;
		m_point.x = x;
		m_point.y = y;
		
		m_rot[0] = xr;
		m_rot[1] = yr;
		m_rot[2] = zr;
		rot[0] = m_rot[0];
		rot[1] = m_rot[1];
		rot[2] = m_rot[2];
	}
	else if(m_bMButtonDown) {
		double xt = m_trans[0];
		double yt = m_trans[1];
		double zt = m_trans[2];
		
		xt += (x - m_point.x) / (double)(w / 2);
		yt -= (y - m_point.y) / (double)(h / 2);
		m_point.x = x;
		m_point.y = y;
		
		m_trans[0] = xt;
		m_trans[1] = yt;
		m_trans[2] = zt;
		trans[0] = m_trans[0];
		trans[1] = m_trans[1];
	}
	else if(m_bRButtonDown) {
		m_scale -= (y - m_point.y) / ( 1.0 / m_scale * 500.0);
		m_point.x = x;
		m_point.y = y;
		scale = m_scale;
	}
}

void FaceView::LeftMouseReleaseEvent(int x, int y, int status)
{
	m_bLButtonDown = false;
}

void FaceView::RightMouseReleaseEvent(int x, int y, int status)
{
	m_bRButtonDown = false;	
}

void FaceView::MidMouseReleaseEvent(int x, int y, int status)
{
	m_bMButtonDown = false;
}

