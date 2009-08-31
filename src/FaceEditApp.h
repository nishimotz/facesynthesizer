#ifndef __FACEEDITAPP_H__
#define __FACEEDITAPP_H__

#include <iostream>

class FaceView;
class UnitMove;
class FaceModel;
class SetViseme;
class Configuration;

using namespace std;

class FaceEditApp {
private:
	
protected:
	// Protected data members
	FaceView *m_pView;
	
	char *m_pStandardWireframe, *m_pDefaultLink;
	FaceModel *m_pFaceModel[10];

	UnitMove *m_pAuMove;
	UnitMove *m_pMuMove;
	UnitMove *m_pVmMove;
	
	SetViseme *m_pSetViseme;
	Configuration *m_pConfig;
	
	// Callback functions
public:
	// User definable process functions
	void CreateUIObjects(int id, int mouthScale, string &visemeFile);
	void OpenDocument( char *openFileNameSGI, char *openFilenamePNT,
		char *openEyeImage, char *openEyeDef, char *openTeethDef );
	void HandleDocumentError(int code, char *imagefile, char *modelfile);
		
	// public constructor
	FaceEditApp( char *standardWireframe, char *defaultLink, int id, int mouthScale, string &visemeFile, Configuration *pConfig);
	
	// public destructor
	~FaceEditApp();
	
	// Process function
	UnitMove *GetAUMove() { return m_pAuMove; }
	UnitMove *GetMUMove() { return m_pMuMove; }
	UnitMove *GetVMMove() { return m_pVmMove; }
	FaceModel *GetFaceModel() {return m_pFaceModel[0];}
	SetViseme *GetSetViseme() { return m_pSetViseme; }
	FaceView *GetView() { return m_pView; }
};

#endif

