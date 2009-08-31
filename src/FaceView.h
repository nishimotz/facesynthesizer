#ifndef __FACEVIEW_H__
#define __FACEVIEW_H__

#include <iostream>
#include <string>
#include <string.h>

#include "Wfm.h"

#ifndef __FV_TEXTURED__ 
#define __FV_TEXTURED__

enum {
	FV_TEXTURED = 0,
		FV_WITH_WIRE,
		FV_ONLY_WIRE,
};

#endif

class FaceModel;


using namespace std;

#ifndef __CPOINT__
#define __CPOINT__
class CPoint
{
public:
	int x;
	int y;
};

#endif

class FaceView {
private:
	
protected:
	FaceModel **_model;
	int _modelView;
	char *_saveFilename;
	int _backgroundVisible;
	int _saveImageFlag;
	float _FaceAlpha;

	// mouse
	CPoint m_point;												// mouse point
	bool   m_bLButtonDown, m_bRButtonDown, m_bMButtonDown;		// mouse button status

	// object position
	double m_trans[3];
	double m_rot[3];
	double m_scale;
	bool m_enable;

	double m_HeadTrans[3];
	double m_HeadRot[3];


	// User definable process functions
	virtual void drawFaceModel(FaceModel *);
	
public:
	// Public constructors
	FaceView(FaceModel **model);
	
	// Destructor
	virtual ~FaceView();
	
	// Process function
	virtual void redraw(int num);
	virtual void setModelView(int mode) { _modelView = mode; }
	virtual int getModelView() { return _modelView; }
	virtual void setBackgroundVisible(int n) { _backgroundVisible = n; }
	void setSaveFilename(char *filename) {
		if(_saveFilename) delete [] _saveFilename;
		_saveFilename = new char[strlen(filename) + 1];
		strcpy(_saveFilename, filename);
	}
	
	// Data output function
	virtual int saveImage(char *filename, int num, int w, int h);

	// mouse
	bool GetLButtonDown() { return m_bLButtonDown; }
	bool GetRButtonDown() { return m_bRButtonDown; }
	bool GetMButtonDown() { return m_bMButtonDown; }
	void GetPoint( int &x, int &y ) { x = m_point.x; y = m_point.y; }
	
	void SetLButtonDown(bool flag) { m_bLButtonDown = flag; }
	void SetRButtonDown(bool flag) { m_bRButtonDown = flag; }
	void SetMButtonDown(bool flag) { m_bMButtonDown = flag; }
	void SetPoint( int x, int y ) { m_point.x = x; m_point.y = y; }

	void LeftMousePushEvent(int x, int y, int status);
	void RightMousePushEvent(int x, int y, int status);
	void MidMousePushEvent(int x, int y, int status);
	void LeftMouseReleaseEvent(int x, int y, int status);
	void RightMouseReleaseEvent(int x, int y, int status);
	void MidMouseReleaseEvent(int x, int y, int status);
	void MouseDragEvent(int x, int y, int status, int w, int h,
		double rot[3], double trans[2], double &scale);


	void ResetTrans() { 
		m_trans[0] = m_trans[1] = m_trans[2] = m_rot[0] = m_rot[1] = m_rot[2] = 0.0;
		m_scale = 1.0;
	}

	
	// Data input function
	void setTranslate(double x, double y) { m_trans[0] = x; m_trans[1] = y; }
	void setRotate(double x, double y, double z) { m_rot[0] = x; m_rot[1] = y; m_rot[2] = z; }
	void setScale(double x) { m_scale = x; }
	void setEnable(bool b) { m_enable = b; }
	void setAgentAlpha(double f) { _FaceAlpha = (float)f; }
	
	void ResetHeadTrans() {
		m_HeadTrans[0] = m_HeadTrans[1] = m_HeadTrans[2] = 0.0;
		m_HeadRot[0] = m_HeadRot[1] = m_HeadRot[2] = 0.0;
	}

	double *GetHeadRot() { return m_HeadRot; }

};

#endif

