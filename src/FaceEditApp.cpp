#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <vector>
#include <string>

#include "FaceEditApp.h"

#include "FaceModel.h"
#include "FaceEditView.h"
#include "UnitMove.h"
#include "SetViseme.h"
#include "trace.h"

#include "Configuration.h"

#include "Resources_AU.h"
#include "Resources_MU.h"
#include "Resources_VM.h"

// Constructor
FaceEditApp::FaceEditApp( char *standardWireframe, char *defaultLink, int id, int mouthScale, string &visemeFile,Configuration *pConfig)
{
	m_pView = NULL;
	m_pDefaultLink = defaultLink;
	m_pStandardWireframe = standardWireframe;
	
	for( int i = 0; i < 10; i++ ){
		m_pFaceModel[i] = NULL;
	}
	m_pAuMove = NULL;
	m_pMuMove = NULL;
	m_pVmMove = NULL;
	m_pSetViseme = NULL;
	m_pConfig = pConfig;
	
	CreateUIObjects(id, mouthScale, visemeFile);
}

// Destructor
FaceEditApp::~FaceEditApp()
{
	if( m_pView != NULL ) { delete m_pView; }
	for( int i = 0; i < 10; i++ ) {
		if( m_pFaceModel[i] != NULL) { delete m_pFaceModel[i]; }
	}
	if( m_pAuMove != NULL ) { delete m_pAuMove; }
	if( m_pMuMove != NULL ) { delete m_pMuMove; }
	if( m_pVmMove != NULL ) { delete m_pVmMove; }
	
	if( m_pSetViseme != NULL ) { delete m_pSetViseme; }
}

void FaceEditApp::CreateUIObjects(int id, int mouthScale, string &visemeFile)
{
	// Create face edit view area.
	m_pView = new FaceEditView( m_pFaceModel, this, id, m_pConfig);
	
	m_pAuMove = new UnitMove( AU_MOVE_NUM, DEFAULT_AU_DIR, AU_FILE_NAME );
	m_pMuMove = new UnitMove( MU_MOVE_NUM, DEFAULT_MU_DIR, MU_FILE_NAME );
	m_pVmMove = new UnitMove( VM_MOVE_NUM, DEFAULT_VM_DIR, VM_FILE_NAME );
	m_pSetViseme = new SetViseme( mouthScale, visemeFile );
}

void FaceEditApp::OpenDocument( char *openFileNameSGI, char *openFileNamePNT,
							   char *openEyeImage, char *openEyeDef, char *openTeethDef )
{
		m_pFaceModel[0] = new FaceModel(openFileNamePNT, m_pStandardWireframe, 
		m_pDefaultLink, openFileNameSGI,
		openEyeImage, openEyeDef, openTeethDef, m_pConfig);
	
	if( m_pFaceModel[0]->Error()) {
		HandleDocumentError( m_pFaceModel[0]->Error(), openFileNamePNT, openFileNameSGI);
		if(m_pFaceModel[0]->Error() != FM_ERROR_TEXOPEN && 
			m_pFaceModel[0]->Error() != FM_ERROR_TEXRES) {
			delete m_pFaceModel[0];
			m_pFaceModel[0] = NULL;
		}
	}
	if( m_pFaceModel[0] ) {
		m_pFaceModel[0]->SetModifyFlag();
		m_pView->setBackgroundVisible(1);
	}
}

// Handle errors in opening document
void FaceEditApp::HandleDocumentError(int code, char *imagefile, char *modelfile)
{
	char str[1024];
	switch(code) {
	case FM_ERROR_STDWFM:
		strcpy(str, "Can't open standard wireframe file : ");
		strcat(str, m_pStandardWireframe);
		break;
	case FM_ERROR_LINK:
		strcpy(str, "Can't open link file : ");
		strcat(str, m_pDefaultLink);
		break;
	case FM_ERROR_WIREFRAME:
		strcpy(str, "Can't open wireframe file : ");
		strcat(str, modelfile);
		break;
	case FM_ERROR_TEXOPEN:
		strcpy(str, "Can't open image file : ");
		strcat(str, imagefile);
		break;
	}
	printf("%s\n", str );
}

