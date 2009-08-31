#ifndef __FACEEDITVIEW_H__
#define __FACEEDITVIEW_H__

#include <iostream>

#include "FaceView.h"

enum {
	TEX_ID_BACKGROUND = 100,
		TEX_ID_FRONT = 200,
		TEX_ID_EYE = 300,
		TEX_ID_MOUTHWALL = 400,
		
		MODEL_ID_FACE = 500,
		MODEL_ID_NON_FACE = 600,
		MODEL_ID_EYE = 700,
		MODEL_ID_MOUTHWALL = 800,
		MODEL_ID_TEETH = 900,
};

enum {
	TEETH_UPPER = 0,
		TEETH_LOWER,
		MOUTH_WALL,
};

class FaceEditApp;
class FaceModel;
class ObjectModel;
class Point;
class Configuration;

using namespace std;



class FaceEditView : public FaceView {
private:

	double _times;
	int _countTime;
	int _id;
	ObjectModel *_upperTeethObj, *_lowerTeethObj, *_mouthwallObj; 
	Configuration *m_pConfig;
	
	bool _ReadFlag;

	double m_AxisPos[3];
	Point RecalcPoint(double *point);
	Point RecalcPoint2(double *point);

	void ReCalcThroat(FaceModel *model);
	void ReCalcSholder(FaceModel *model);
	void UndoCalcSholder(FaceModel *model);
	void UndoCalcThroat(FaceModel *model);

	void DrawThroatAndSholderModel(FaceModel *model);
	void DrawFaceAndHairMode(FaceModel *model);
	void DrawEyeModel(FaceModel *model);
	void DrawTeethModel(FaceModel *model);
	void DrawMouthwallModel(FaceModel *model);
	void DrawBG();

	static const int m_RecalcPoint[];
	static const int m_RecalcPoint2[];
protected:
	// Protected data members
	FaceEditApp *_app;
	
	// User definable process function
	virtual void drawFaceModel(FaceModel *model);
	virtual void setColor(int part);
	
public:
	// Public constructors
	FaceEditView(FaceModel **model, FaceEditApp *app, int _id, Configuration *config);

	// Destructor
	virtual ~FaceEditView();

	

};

#endif

