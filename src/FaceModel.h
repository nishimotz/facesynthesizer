/*++
FaceModel.h
Copyright (c) 1999 - 2000, Takafumi MISAWA. ALL RIGHTS RESERVED.
Modified by Shin OGATA, 2000.
--*/

#ifndef __FACEMODEL_H__
#define __FACEMODEL_H__

#include <iostream>

#include "Texture.h"
#include "Wfm.h"

enum {
	FM_ERROR_NONE,
		FM_ERROR_STDWFM,
		FM_ERROR_LINK,
		FM_ERROR_WIREFRAME,
		FM_ERROR_TEXOPEN,
		FM_ERROR_TEXRES,
};

enum {
	WFM_ORG = 0,	// Original Wireframe Model ... After fitting
		WFM_STD,	// Standard Wireframe Model ... Before fitting
		WFM_WORK,	// Wireframe Model for working
		WFM_TRNS,	// After Transformation Wireframe Model
		
		WFM_NUM,
};

enum {
	TEXTURE_FRONT = 0,
		TEXTURE_EYE,
		TEXTURE_MOUTHWALL,
		TEXTURE_NUM,
};

using namespace std;

class ObjModel;
class Configuration;

class FaceModel {
protected:
	int m_Error;			// Error code
	int m_Modify;			// Modify flag. Non zero if modified.

	/* Protected data members */
	Wfm *m_pWfm;
	
	Texture *m_pTexMouth;
	Texture *m_pTexFace;
	Texture *m_pTexEye;
	Texture *m_pTexBG;

private:
	char *m_pFaceTexName;
	char *m_pEyeTexName;
	char *m_pEyeDefName;
	char *m_pTeethDefName;

	Configuration *m_pConfig;

	int LoadTexture();
	void LoadEyeAndTeeth();
	void TransWfm(Wfm *wfm);

protected:
	char *m_pFilename;
	int m_ModifyTexture;
	int m_ModifyBGTexture;
	
private:
	ObjModel *m_pEyeObj;
	double m_EyeR_Trans[3], m_EyeR_Rot[3], m_EyeR_Scale;
	double m_EyeL_Trans[3], m_EyeL_Rot[3], m_EyeL_Scale;
	double m_EyeR_Local_T[3], m_EyeR_Local_R[3], m_EyeR_Local_S;
	double m_EyeL_Local_T[3], m_EyeL_Local_R[3], m_EyeL_Local_S;
	double m_Eye_Tex_S;

	double m_AppEyeR_Rot[2], m_AppEyeL_Rot[2];

	float m_dTeethAmbColor[4];
	float m_dTeethDifColor[4];
	float m_dTeethSpeColor[4];

	bool TeethFileOpen(char *filename);
	bool EyeFileOpen(char *filename);

public:
	
	/* Public constructors */
	FaceModel(char *pntfile, char *stdwfm, char *lnkfile, char *texfile,
		char *openEyeImage, char *openEyeDef, char *openTeethDef, Configuration *config);
	
	/* Destructor */
	~FaceModel();
	
	/* Process function */
	void ClearModifyFlag() { m_Modify = m_ModifyTexture = m_ModifyBGTexture = 0;}
	int Modify() { return m_Modify; }	// Modify flag
	int ModifyTexture() { return m_ModifyTexture; }
	int ModifyBGTexture() { return m_ModifyBGTexture; }

	int Save(char *filename);
	int Error() { return m_Error; }

	void SetBGTexture(Texture *tex) { m_pTexBG = tex; m_ModifyBGTexture = 1;}
	void SetWireframeModifyFlag() { m_Modify = 1; }
	void SetTextureModifyFlag() { m_ModifyTexture = 1; }
	void SetBGTexrureModifyFlag() { m_ModifyBGTexture = 1; }
	void SetModifyFlag() { m_Modify = m_ModifyTexture = 1; }
	
	Wfm *GetWfm(int num) { return &m_pWfm[num]; }
	
	Texture *GetFaceTexture() { return m_pTexFace; }
	Texture *GetMouthTexture() { return m_pTexMouth; }
	Texture *GetEyeTexture() { return m_pTexEye; }
	Texture *GetBGTexture() { return m_pTexBG; }


public:
	ObjModel*	GetEyeObject()			{ return m_pEyeObj; }
	float *		GetTeethAmbColor()		{ return m_dTeethAmbColor; }
	float *		GetTeethDifColor()		{ return m_dTeethDifColor; }
	float *		GetTeethSpeColor()		{ return m_dTeethSpeColor; }
	double* GetEyeRTrans() { return m_EyeR_Trans; }
	double* GetEyeLTrans() { return m_EyeL_Trans; }
	double* GetEyeRRot() { return m_EyeR_Rot; }
	double* GetEyeLRot() { return m_EyeL_Rot; }
	double& GetEyeRScale() { return m_EyeR_Scale; }
	double& GetEyeLScale() { return m_EyeL_Scale; }

	double* GetEyeRLocalTrans() { return m_EyeR_Local_T; }
	double* GetEyeLLocalTrans() { return m_EyeL_Local_T; }
	double* GetEyeRLocalRot() { return m_EyeR_Local_R; }
	double* GetEyeLLocalRot() { return m_EyeL_Local_R; }
	double& GetEyeRLocalScale() { return m_EyeR_Local_S; }
	double& GetEyeLLocalScale() { return m_EyeL_Local_S; }
	
	double& GetEyeTexScale() { return m_Eye_Tex_S; }

	double* GetAppEyeRotR() { return m_AppEyeR_Rot; }
	double* GetAppEyeRotL() { return m_AppEyeL_Rot; }

};

#endif

