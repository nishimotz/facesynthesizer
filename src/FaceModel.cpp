#include "FaceModel.h"

#include <math.h>
#include <string.h>

#include "Texture.h"
#include "ObjModel.h"
#include "Configuration.h"
#include "Resources.h"
#include "trace.h"


FaceModel::FaceModel(char *pntfile, char *stdwfm, char *lnkfile, char *texfile,
			 		char *openEyeImage, char *openEyeDef, char *openTeethDef, Configuration *config)
{
	// Initialize all data members.
	m_pConfig = config;

	m_Error = 0;
	m_Modify = 0;

	m_ModifyTexture = 0;
	m_ModifyBGTexture = 0;
	
	m_pWfm = new Wfm [WFM_NUM];

	m_pWfm[WFM_ORG].load(pntfile);
	m_pWfm[WFM_STD].load(stdwfm);
	m_pWfm[WFM_WORK].load(pntfile);
	m_pWfm[WFM_TRNS].load(pntfile);

	for( int i = 0; i < WFM_NUM; i++ ) {
		m_pWfm[i].loadLink(lnkfile);
		m_pWfm[i].setLink();
	}

	TransWfm(&m_pWfm[WFM_TRNS]);
	
	m_pTexFace = NULL;
	m_pTexEye = NULL;
	m_pTexMouth = NULL;
	m_pTexBG = NULL;
	m_pEyeObj = NULL;
	
	m_Modify = 1;
	
	m_pFaceTexName = texfile;
	m_pEyeTexName = openEyeImage;
	m_pEyeDefName = openEyeDef;
	m_pTeethDefName = openTeethDef;

	int err;
		
	err = LoadTexture();
	if( err == ImageData::ERROR_NONE ) {
		m_ModifyTexture = 1;
	}

	m_dTeethAmbColor[0] = m_dTeethAmbColor[1] = m_dTeethAmbColor[2] = 0.0;
	m_dTeethDifColor[0] = m_dTeethDifColor[1] = m_dTeethDifColor[2] = 1.0;
	m_dTeethSpeColor[0] = m_dTeethSpeColor[1] = m_dTeethSpeColor[2] = 1.0;
	m_dTeethAmbColor[3] = m_dTeethDifColor[3] = m_dTeethSpeColor[3] = 1.0;
	
	m_EyeR_Trans[0] = m_EyeR_Trans[1] = m_EyeR_Trans[2] = 0.0;
	m_EyeL_Trans[0] = m_EyeL_Trans[1] = m_EyeL_Trans[2] = 0.0;
	m_EyeR_Rot[0] = m_EyeR_Rot[1] = m_EyeR_Rot[2] = 0.0;
	m_EyeL_Rot[0] = m_EyeL_Rot[1] = m_EyeL_Rot[2] = 0.0;
	m_EyeR_Scale = m_EyeR_Scale = 1.0;

	m_EyeR_Local_T[0] = m_EyeR_Local_T[1] = m_EyeR_Local_T[2] = 0.0;
	m_EyeL_Local_T[0] = m_EyeL_Local_T[1] = m_EyeL_Local_T[2] = 0.0;
	m_EyeR_Local_R[0] = m_EyeR_Local_R[1] = m_EyeR_Local_R[2] = 0.0;
	m_EyeL_Local_R[0] = m_EyeL_Local_R[1] = m_EyeL_Local_R[2] = 0.0;
	m_EyeR_Local_S = m_EyeR_Local_S = 1.0;

	m_Eye_Tex_S = 1.0;

	m_AppEyeR_Rot[0] = m_AppEyeR_Rot[1] = 0.0;
	m_AppEyeL_Rot[0] = m_AppEyeL_Rot[1] = 0.0;


	LoadEyeAndTeeth();
}

FaceModel::~FaceModel()
{
	if( m_pWfm != NULL ) { delete [] m_pWfm; }
	if( m_pTexMouth != NULL ) { delete m_pTexMouth; }
	if( m_pTexFace != NULL ) { delete m_pTexFace; }
	if( m_pTexEye != NULL ) { delete m_pTexEye; }
	if( m_pEyeObj != NULL ) { delete m_pEyeObj; }
}

void FaceModel::LoadEyeAndTeeth()
{
	TeethFileOpen(m_pTeethDefName);
	EyeFileOpen(m_pEyeDefName);

	m_pEyeObj->ReCalcTexture(GetEyeTexScale());
}

bool FaceModel::TeethFileOpen(char *filename)
{
	char temp[256];
	std::ifstream in(filename);
	if( !in ) {
		return false;
	}
	in.getline(temp, sizeof(temp) );
	
	sscanf(temp, "%f %f %f", &m_dTeethDifColor[0], &m_dTeethDifColor[1], &m_dTeethDifColor[2]);

	in.close();

	return true;
}

bool FaceModel::EyeFileOpen(char *filename)
{
	m_pEyeObj = new ObjModel(m_pConfig->GetEyeObjName().c_str());

	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if( !in ) {
		//fl_message("Cannot Open File!! [%s]", filename);
		return false;
	}
	in.read((char *) m_EyeR_Trans, sizeof(double) * 3);
	in.read((char *) m_EyeR_Rot, sizeof(double) * 3);
	in.read((char *) &m_EyeR_Scale, sizeof(double));
	in.read((char *) m_EyeL_Trans, sizeof(double) * 3);
	in.read((char *) m_EyeL_Rot, sizeof(double) * 3);
	in.read((char *) &m_EyeL_Scale, sizeof(double));
	in.read((char *) m_EyeR_Local_T, sizeof(double) * 3);
	in.read((char *) m_EyeR_Local_R, sizeof(double) * 3);
	in.read((char *) &m_EyeR_Local_S, sizeof(double));
	in.read((char *) m_EyeL_Local_T, sizeof(double) * 3);
	in.read((char *) m_EyeL_Local_R, sizeof(double) * 3);
	in.read((char *) &m_EyeL_Local_S, sizeof(double));
	in.read((char *) &m_Eye_Tex_S, sizeof(double));
	
	in.close();

	return true;
}

int FaceModel::LoadTexture()
{
	int err = 0;
	
	m_pTexFace = new Texture( m_pFaceTexName );
	err =  m_pTexFace->ErrorJudge();
	
	if( err != ImageData::ERROR_NONE ) {
		printf("err no = %d\n", err);
		return err;
	}

	m_pTexEye = new Texture( m_pEyeTexName );
	err =  m_pTexFace->ErrorJudge();
	
	if( err != ImageData::ERROR_NONE ) {
		printf("err no = %d\n", err);
		return err;
	}

	m_pTexMouth = new Texture( m_pConfig->GetMouthWallTexture().c_str() );
	err =  m_pTexMouth->ErrorJudge();
	
	if( err != ImageData::ERROR_NONE ) {
		printf("err no = %d\n", err);
		return err;
	}
	
	return err;
}

int FaceModel::Save(char *filename)
{
	TRACE("FaceModel::save(char *)");
	
	if(!m_pWfm[WFM_WORK].save(filename)) {
		TRACE( "Error in saving point file." );
		return 1;
	}
	return 0;
}

void FaceModel::TransWfm(Wfm *wfm)
{
	int i;

	double max = -60000.0;
	for(i = 1; i <= wfm->grn(); i++) {   //transform about Vertex.
		double x = wfm->Sgridx(i);
		double y = wfm->Sgridy(i);
		double z = wfm->Sgridz(i);
		double r = sqrt(x * x + z * z);
		double O = acos(z/r);
		double R = sqrt(x * x + y * y + z * z);
		double phi = acos(r/R);
		
		if(x < 0.0) wfm->Sgridx(i) = -O;
		else wfm->Sgridx(i) = O;
		
		if(y < 0.0) wfm->Sgridy(i) = -phi;
		else wfm->Sgridy(i) = phi;
		
		wfm->Sgridz(i) = -O;
		if(O > max) max = O;
	}
	
	max = 1.99/max;
	for(i = 1; i <= wfm->grn(); i++) {
		wfm->Sgridx(i) *= max;
		wfm->Sgridy(i) *= max;
	}
}

