/*++
ObjedtModel.h
PDS File
--*/

#ifndef __OBJECTMODEL_H_
#define __OBJECTMODEL_H_

#include <iostream>

#include "Wfm.h"

#ifdef _XtObject_h
#define Object _Object
#endif

using namespace std;

typedef	struct	{
	double x, y;
} Point2f;

typedef	struct	{
	double x, y, z;
} Point3f;

typedef struct {
	int p0, p1, p2;
	int tp0, tp1, tp2;
} Polygon3;

typedef Point3f Vector3f;

class  ObjectModel {
	
public:

	
	
private:
	
	bool _changeFlag;
	
	// for adjust teeth
	double _jox, _joy, _joz;  // jaw original point
	double _jovy, _jovz;      // jaw original vector
	double _jvy, _jvz;        // jaw rotate vector
	double _joa;              // jaw original angle
	double _ja;               // jaw rotate angle
	double _utx, _uty, _utz;  // upper teeth point
	double _utvz;             // upper teeth vector z
	double _ltx, _lty, _ltz;   // lower teeth point
	double _ltvz;             // lower teeth vector z
	
	// object vertex 
	int _pointNum;
	Point3f *_point;
	
	// object polygon
	int _polygonNum;
	Polygon3 *_polygon;
	
	// object pointvetor
	int _pointVectorNum;
	Vector3f *_pointVector;
	
	// object polygonvector
	int _polygonVectorNum;
	Vector3f *_polygonVector;
	
	// unit vector
	Vector3f _unitVector;
	
	// normal vector
	Vector3f _normalVector;
	
	// object texture vertex
	int _texturePointNum;
	Point2f *_texturePoint;
	
	// wfm object
	Wfm *_orgWfm, *_workWfm;
	double mostlengthy, mostlengthz1, mostlengthz2;
	
	Vector3f unitVector(Vector3f v);
	Vector3f normalVector(Point3f p0, Point3f p1, Point3f p2);

public:
	
	double _diff, _calc;
	
	//	constructor
	ObjectModel(const char *fname, int mode, Wfm *model, Wfm *wmodel);
	ObjectModel(const char *, int);
	//	destructor
	~ObjectModel();
	
	//	function
	void adjustTeeth(int);
	void adjustMouthwall();
	void moveMouthwall();
	void calcTexturePoint();
	void drawMouthwallModel();
	void drawTeethModel(int mode);
	void openTeethObject(const char *fname, int mode);
	void openMouthWallObject(const char *);
	
	int loadObjectFile(const char *, int);
	int calcPolygonVector(); 
	int calcPointVector();
	int calcRedrawFlag();
	int getPointNum()   { return(_pointNum); }
	int getPolygonNum() { return(_polygonNum); }
	int getPointVectorNum() { return(_pointVectorNum); }
	int getPolygonVectorNum() { return(_polygonVectorNum); }
	int getTexturePointNum() { return(_texturePointNum); }
	
	double getJawAngle() { return(_ja); }
	double moveTeeth(int);
	
	Point2f *getTexturePoint() { return(_texturePoint); }
	Point3f *getPoint() { return(_point); }
	Polygon3 *getPolygon() { return(_polygon); }
	
	Vector3f *getPointVector() { return(_pointVector); }
	Vector3f *getPolygonVector() { return(_polygonVector); }
	
};
#endif
