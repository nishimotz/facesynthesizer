#ifdef WIN32
#include <windows.h>
#endif

#include "FaceEditView.h"

#include <iomanip>
#include <GL/gl.h>

#include "FaceEditApp.h"
#include "FaceModel.h"
#include "Texture.h"
#include "ObjectModel.h"
#include "ObjModel.h"
#include "Configuration.h"
#include "Resources.h"
#include "Trans.h"
#include "trace.h"

FaceEditView::FaceEditView(FaceModel **model, FaceEditApp *app, int id, Configuration *config)
: FaceView(model)
{
	TRACE("FaceEditView::FaceEditView(FaceModel **, int, int, FaceEditApp *)");
	

	_times = 0.0;
	_countTime = 0;
	_app = app;
	_ReadFlag = FALSE;
	_upperTeethObj = NULL;
	_lowerTeethObj = NULL;
	_mouthwallObj = NULL;
	_id = id;
	_FaceAlpha = 1.0f;
	m_pConfig = config;
}

FaceEditView::~FaceEditView()
{
	TRACE("FaceEditView::~FaceEditView()");
	
	delete _upperTeethObj;
	delete _lowerTeethObj;
	delete _mouthwallObj;
}

void FaceEditView::drawFaceModel(FaceModel *model)
{
	TRACE("FaceEditView::drawFaceModel(FaceModel *)");	

	GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, model->GetTeethDifColor() );
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.75);
	
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	ObjModel *obj = model->GetEyeObject();

	if(!_ReadFlag) {
		TRACE2("_ReadFlagOKOKOKOK!!!");
		if( _upperTeethObj != NULL ) delete _upperTeethObj;
		if( _lowerTeethObj != NULL ) delete _lowerTeethObj;
		if( _mouthwallObj != NULL ) delete _mouthwallObj;
		
		_upperTeethObj = new ObjectModel(m_pConfig->GetUpperTeethObjName().c_str(), 
			TEETH_UPPER, orgWfm, workWfm);
		_lowerTeethObj = new ObjectModel(m_pConfig->GetLowerTeethObjName().c_str(), 
			TEETH_LOWER, orgWfm, workWfm);
		_mouthwallObj = new ObjectModel(m_pConfig->GetMouthWallObjName().c_str(), 
			MOUTH_WALL, orgWfm, workWfm);
		
		_upperTeethObj->adjustTeeth(TEETH_UPPER);
		_lowerTeethObj->adjustTeeth(TEETH_LOWER);
		_mouthwallObj->calcTexturePoint();
		_mouthwallObj->adjustMouthwall();
		
		_ReadFlag = TRUE;

		m_AxisPos[0] = orgWfm->Ggridx(3,25);
		m_AxisPos[1] = orgWfm->Ggridy(3,25);

		double y1 = orgWfm->Ggridy(6,22);
		double y2 = orgWfm->Ggridy(12,18);

		m_AxisPos[2] = orgWfm->Ggridz(3,25) - (y1 - y2);
		//printf("%lf, %lf, %lf\n", m_AxisPos[0], m_AxisPos[1], m_AxisPos[2]);
	}
	
	glTranslatef(0.0, 0.0, -10.0);
	
	// Draw background image
	Texture *eyeTexture = model->GetEyeTexture();
	Texture *mouthwallTexture = model->GetMouthTexture();
	Texture *frontTexture = model->GetFaceTexture();
	Texture *bgTexture = model->GetBGTexture();
	
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glTranslatef(0.0, 0.0, 5.0);


	if(!(_modelView == FV_ONLY_WIRE || _backgroundVisible == 0)) {

		glBindTexture(GL_TEXTURE_2D, bgTexture->GetTextureID());
		DrawBG();
	}
	

	glPushMatrix();
	// Translation
	glTranslatef((GLfloat)m_trans[0], (GLfloat)m_trans[1], 0.0);
	// Scale
	glScalef((GLfloat)m_scale, (GLfloat)m_scale, (GLfloat)m_scale);
	
	// Rotation
	glRotatef((GLfloat)m_rot[0], 1.0, 0.0, 0.0);
	glRotatef((GLfloat)m_rot[1], 0.0, 1.0, 0.0);
	glRotatef((GLfloat)m_rot[2], 0.0, 0.0, 1.0);

	if(_modelView == FV_ONLY_WIRE) {
		glColor4f(1.0f, 1.0f, 1.0f, _FaceAlpha);
		glDisable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPushMatrix();
		{
			if( m_enable ) {
			DrawEyeModel(model);
			DrawMouthwallModel(model);
			DrawTeethModel(model);
			DrawFaceAndHairMode(model);
			}
		}
		glPopMatrix();
		if( m_enable ) {
		DrawThroatAndSholderModel(model);
		}
		glEnable(GL_TEXTURE_2D);
	} 
	else if(_modelView == FV_WITH_WIRE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushMatrix();
		{
			if( m_enable ) {
			glColor4f(1.0f, 1.0f, 1.0f, _FaceAlpha);
			glTranslated(m_AxisPos[0], m_AxisPos[1], m_AxisPos[2] );
			glRotated( m_HeadRot[0], 1.0, 0.0, 0.0 );
			glRotated( m_HeadRot[1], 0.0, 1.0, 0.0 );
			glRotated( m_HeadRot[2], 0.0, 0.0, 1.0 );
			glTranslated(-m_AxisPos[0], -m_AxisPos[1], -m_AxisPos[2] );

			// eye texture
			glBindTexture(GL_TEXTURE_2D, eyeTexture->GetTextureID());
			DrawEyeModel(model);
			
			if( _FaceAlpha == 1.0f ) {
				// mouthwall texture
				glBindTexture(GL_TEXTURE_2D, mouthwallTexture->GetTextureID());
				DrawMouthwallModel(model);

				// draw teeth
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				DrawTeethModel(model);
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glEnable(GL_TEXTURE_2D);
			}
			// face texture
			glBindTexture(GL_TEXTURE_2D, frontTexture->GetTextureID());
			DrawFaceAndHairMode(model);
			}
		}
		glPopMatrix();
		DrawThroatAndSholderModel(model);
		
		glDisable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPushMatrix();
		{
			if( m_enable ) {
			DrawEyeModel(model);
			DrawMouthwallModel(model);
			DrawTeethModel(model);
			DrawFaceAndHairMode(model);
			}
		}
		glPopMatrix();
		if( m_enable ) {
		DrawThroatAndSholderModel(model);
		}
		glEnable(GL_TEXTURE_2D);
	} 
	else if(_modelView == FV_TEXTURED) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushMatrix();
		{
			if( m_enable ) {
			glColor4f(1.0f, 1.0f, 1.0f, _FaceAlpha);
			glTranslated(m_AxisPos[0], m_AxisPos[1], m_AxisPos[2] );
			glRotated( m_HeadRot[0], 1.0, 0.0, 0.0 );
			glRotated( m_HeadRot[1], 0.0, 1.0, 0.0 );
			glRotated( m_HeadRot[2], 0.0, 0.0, 1.0 );
			glTranslated(-m_AxisPos[0], -m_AxisPos[1], -m_AxisPos[2] );
			glEnable(GL_TEXTURE_2D);

			// eye texture
			glBindTexture(GL_TEXTURE_2D, eyeTexture->GetTextureID());
			DrawEyeModel(model);

			if( _FaceAlpha == 1.0f ) {
				// mouthwall texture
				glBindTexture(GL_TEXTURE_2D, mouthwallTexture->GetTextureID());
				DrawMouthwallModel(model);
			
				// draw teeth
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				DrawTeethModel(model);
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glEnable(GL_TEXTURE_2D);
			}
			
			// face texture
			glBindTexture(GL_TEXTURE_2D, frontTexture->GetTextureID());
			DrawFaceAndHairMode(model);
			}
		}
		glPopMatrix();
		if( m_enable ) {
		DrawThroatAndSholderModel(model);
		}
	}
	glDisable(GL_CULL_FACE);
	glPopMatrix();
	model->ClearModifyFlag();
}

void FaceEditView::setColor(int part)
{
	//	TRACE("FaceEditView::setColor(int)");
	if(part <= 19) glColor4d(0.5, 0.7, 1.0, 1.0);		// Light blue
	else if(part <= 21) glColor4d(1.0, 0.5, 0.7, 1.0);	// Light red
	else if(part <= 24) glColor4d(1.0, 0.0, 0.0, 1.0);	// Red
	else if(part <= 26) glColor4d(0.7, 1.0, 0.5, 1.0);	// Light green
	else glColor4d(1.0, 1.0, 1.0, 1.0);			// White
}


Point FaceEditView::RecalcPoint(double *point)
{
	Point target( point[0], point[1], point[2] );
	Point ans;

	ans = (translate(m_AxisPos[0], m_AxisPos[1], m_AxisPos[2]) *
	       rotateX(m_HeadRot[0] * 3.141592 / 180.0) *
	       rotateY(m_HeadRot[1] * 3.141592 / 180.0) *
	       rotateZ(m_HeadRot[2] * 3.141592 / 180.0) *
	       translate(-m_AxisPos[0], -m_AxisPos[1], -m_AxisPos[2]) * 
	       target);

	return ans;
}

Point FaceEditView::RecalcPoint2(double *point)
{
  Point target( point[0], point[1], point[2] );
  Point ans;

  ans = (translate(m_AxisPos[0], m_AxisPos[1], m_AxisPos[2]) *
	 rotateZ(-1 * m_HeadRot[2] * 3.141592 / 180.0) *
	 rotateY(-1 * m_HeadRot[1] * 3.141592 / 180.0) *	
	 rotateX(-1 * m_HeadRot[0] * 3.141592 / 180.0) *
	 translate(-m_AxisPos[0], -m_AxisPos[1], -m_AxisPos[2]) * 
	       target);

	return ans;
}

void FaceEditView::ReCalcThroat(FaceModel *model)
{
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	double *point;

	for( int i = 0; i < 13; i++ ) {
		point = workWfm->Sgridv(m_RecalcPoint[i]);

		Point ans = RecalcPoint(point);
		point[0] = ans[0];
		point[1] = ans[1];
		point[2] = ans[2];
	}
}

void FaceEditView::UndoCalcThroat(FaceModel *model)
{
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	double *point;
	double *point2;

	for( int i = 0; i < 13; i++ ) {
		point = workWfm->Sgridv(m_RecalcPoint[i]);
		point2 = orgWfm->Sgridv(m_RecalcPoint[i]);
		point[0] = point2[0];
		point[1] = point2[1];
		point[2] = point2[2];
	}
}

void FaceEditView::ReCalcSholder(FaceModel *model)
{
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	double *point;

	for( int i = 0; i < 8; i++ ) {
		point = workWfm->Sgridv(m_RecalcPoint2[i]);

		Point ans = RecalcPoint2(point);
		point[0] = ans[0];
		point[1] = ans[1];
		point[2] = ans[2];
	}
}

void FaceEditView::UndoCalcSholder(FaceModel *model)
{
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	double *point;
	double *point2;

	for( int i = 0; i < 8; i++ ) {
		point = workWfm->Sgridv(m_RecalcPoint2[i]);
		point2 = orgWfm->Sgridv(m_RecalcPoint2[i]);
		point[0] = point2[0];
		point[1] = point2[1];
		point[2] = point2[2];
	}
}

const int FaceEditView::m_RecalcPoint[] = {
		384, 385, 386, 387, 388, 389, 391, 409, 408, 407, 406, 405, 404
};


const int FaceEditView::m_RecalcPoint2[] = {
		768, 769, 350, 367, 363, 346, 354, 371
};

void FaceEditView::DrawThroatAndSholderModel(FaceModel *model)
{
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	int i;
	ReCalcThroat(model);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	
	for(i = 1; i <= workWfm->trn(); i++) {
		if(orgWfm->S2Glinkgn(i) == 16 || orgWfm->S2Glinkgn(i) == 17 || orgWfm->S2Glinkgn(i) == 30) {
			glBegin(GL_TRIANGLES);
			glTexCoord2d(orgWfm->SL2G1x(i), orgWfm->SL2G1y(i));
			glVertex3dv(workWfm->SL2G1v(i));
			glTexCoord2d(orgWfm->SL2G2x(i), orgWfm->SL2G2y(i));
			glVertex3dv(workWfm->SL2G2v(i));
			glTexCoord2d(orgWfm->SL2G3x(i), orgWfm->SL2G3y(i));
			glVertex3dv(workWfm->SL2G3v(i));
			glEnd();
		}
	}

	UndoCalcThroat(model);
}

void FaceEditView::DrawFaceAndHairMode(FaceModel *model)
{
	Wfm *orgWfm = model->GetWfm(WFM_ORG);
	Wfm *workWfm = model->GetWfm(WFM_WORK);
	int i;
	ReCalcSholder(model);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	for(i = 1; i <= workWfm->trn(); i++) {
		if(orgWfm->S2Glinkgn(i) <= 15 || orgWfm->S2Glinkgn(i) == 18 || 
			orgWfm->S2Glinkgn(i) == 19 || orgWfm->S2Glinkgn(i) == 31) {
			glBegin(GL_TRIANGLES);
			glTexCoord2d(orgWfm->SL2G1x(i), orgWfm->SL2G1y(i));
			glVertex3dv(workWfm->SL2G1v(i));
			glTexCoord2d(orgWfm->SL2G2x(i), orgWfm->SL2G2y(i));
			glVertex3dv(workWfm->SL2G2v(i));
			glTexCoord2d(orgWfm->SL2G3x(i), orgWfm->SL2G3y(i));
			glVertex3dv(workWfm->SL2G3v(i));
			glEnd();
		}
	}

	for(i = 0; i < 40; i++) {		// infill the eye hole
		glBegin(GL_TRIANGLES);
		glTexCoord2d(workWfm->Ggridx(Wfm::EyeHole[i][0].gn, Wfm::EyeHole[i][0].pn),
			workWfm->Ggridy(Wfm::EyeHole[i][0].gn, Wfm::EyeHole[i][0].pn));
		glVertex3dv(workWfm->Ggridv(Wfm::EyeHole[i][0].gn, Wfm::EyeHole[i][0].pn));
		glTexCoord2d(workWfm->Ggridx(Wfm::EyeHole[i][1].gn, Wfm::EyeHole[i][1].pn),
			workWfm->Ggridy(Wfm::EyeHole[i][1].gn, Wfm::EyeHole[i][1].pn));
		glVertex3dv(workWfm->Ggridv(Wfm::EyeHole[i][1].gn, Wfm::EyeHole[i][1].pn));
		glTexCoord2d(workWfm->Ggridx(Wfm::EyeHole[i][2].gn, Wfm::EyeHole[i][2].pn),
			workWfm->Ggridy(Wfm::EyeHole[i][2].gn, Wfm::EyeHole[i][2].pn));
		glVertex3dv(workWfm->Ggridv(Wfm::EyeHole[i][2].gn, Wfm::EyeHole[i][2].pn));
		glEnd();
	}
	UndoCalcSholder(model);
}

void FaceEditView::DrawEyeModel(FaceModel *model) 
{
	ObjModel *obj = model->GetEyeObject();
	glPushMatrix();
	glTranslated(model->GetEyeRTrans()[0], model->GetEyeRTrans()[1], model->GetEyeRTrans()[2]);
	glScaled(model->GetEyeRScale(), model->GetEyeRScale(), model->GetEyeRScale());
	glRotated(model->GetEyeRLocalRot()[0] + model->GetAppEyeRotR()[0], 1.0, 0.0, 0.0);
	glRotated(model->GetEyeRLocalRot()[1] + model->GetAppEyeRotR()[1], 0.0, 1.0, 0.0);
	glRotated(model->GetEyeRLocalRot()[2], 0.0, 0.0, 1.0);
	obj->DrawObjModel();
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslated(model->GetEyeLTrans()[0], model->GetEyeLTrans()[1], model->GetEyeLTrans()[2]);
	glScaled(model->GetEyeLScale(), model->GetEyeLScale(), model->GetEyeLScale());
	glRotated(model->GetEyeLLocalRot()[0] + model->GetAppEyeRotL()[0], 1.0, 0.0, 0.0);
	glRotated(model->GetEyeLLocalRot()[1] + model->GetAppEyeRotL()[1], 0.0, 1.0, 0.0);
	glRotated(model->GetEyeLLocalRot()[2], 0.0, 0.0, 1.0);
	obj->DrawObjModel();
	glPopMatrix();
}

void FaceEditView::DrawMouthwallModel(FaceModel *model)
{
	_mouthwallObj->drawMouthwallModel();
}

void FaceEditView::DrawTeethModel(FaceModel *model)
{
	_upperTeethObj->drawTeethModel(TEETH_UPPER);
	_lowerTeethObj->drawTeethModel(TEETH_LOWER);
}

void FaceEditView::DrawBG()
{
	glBegin(GL_QUADS);
		glTexCoord2f(-1.0, -1.0);
		glVertex3f(-1.0, -1.0, -5.0);
		glTexCoord2f(1.0, -1.0);
		glVertex3f(1.0, -1.0, -5.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(1.0, 1.0, -5.0);
		glTexCoord2f(-1.0, 1.0);
		glVertex3f(-1.0, 1.0, -5.0);
	glEnd();
}


