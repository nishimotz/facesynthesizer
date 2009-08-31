/*++
ObjectModel.c++
Modified by Shin OGATA, 2000
--*/
#ifdef WIN32
#include <windows.h>
#endif


#include "ObjectModel.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <GL/gl.h>

#include "trace.h"

#ifndef M_PI
#define M_PI 3.1415926425
#endif

#ifndef MAXDOUBLE
#define MAXDOUBLE 1.7976931348623158e+308
#endif

enum{
	TEETH_UPPER = 0, // modified by Shin OGATA, for Reading Background file.
		TEETH_LOWER,
		MOUTH_WALL,
};

char* getFileData(FILE *fp, char *buf, char *str)
{
	int c;
	char *ptr;
	
	ptr = buf;
	while(1){
		if((c = fgetc(fp)) == EOF)
			return(NULL);
		if(strchr(str, c) == NULL)
			*ptr = c;
		else{
			*ptr = '\0';
			break;
		}
		ptr++;
	}
	return(buf);
}

Vector3f ObjectModel::unitVector(Vector3f v)
{
	Vector3f u;
	double l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	u.x = v.x / l;
	u.y = v.y / l;
	u.z = v.z / l;
	return(u);
}

Vector3f ObjectModel::normalVector(Point3f p0, Point3f p1, Point3f p2)
{
	double dx0, dy0, dz0,
		dx1, dy1, dz1;
	Vector3f n;
	
	dx0 = p1.x - p0.x; dy0 = p1.y - p0.y; dz0 = p1.z - p0.z;
	dx1 = p2.x - p1.x; dy1 = p2.y - p1.y; dz1 = p2.z - p1.z;
	n.x =(dz1 * dy0) -(dy1 * dz0);
	n.y =(dx1 * dz0) -(dz1 * dx0);
	n.z =(dy1 * dx0) -(dx1 * dy0);
	n = unitVector(n);
	return(n);
}

ObjectModel::ObjectModel(const char *fname, int mode, Wfm *model, Wfm *wmodel)
{
	TRACE("ObjectModel::ObjectModel(char *, int, Wfm *, Wfm *)");
	
	_changeFlag = false;
	
	_pointNum = 0;
	_point = NULL;
	
	_polygonNum = 0;
	_polygon = NULL;
	
	_pointVectorNum = 0;
	_pointVector = NULL;
	
	_polygonVectorNum = 0;
	_polygonVector = NULL;
	
	
	_orgWfm = model;
	_workWfm = wmodel;
	
	// open objectFile
	if(mode == TEETH_UPPER || mode == TEETH_LOWER)
		openTeethObject(fname, mode);
	else if(mode == MOUTH_WALL)
		openMouthWallObject(fname);
}

ObjectModel::ObjectModel(const char *fname, int mode)
{
	TRACE("ObjectModel::ObjectModel(char *, int)");
	
	_changeFlag = false;
	
	_pointNum = 0;
	_point = NULL;
	
	_polygonNum = 0;
	_polygon = NULL;
	
	_pointVectorNum = 0;
	_pointVector = NULL;
	
	_polygonVectorNum = 0;
	_polygonVector = NULL;
	
	
	// open objectFile
	if(mode == TEETH_UPPER || mode == TEETH_LOWER)
		openTeethObject(fname, mode);
	else if(mode == MOUTH_WALL)
		openMouthWallObject(fname);
}


ObjectModel::~ObjectModel()
{
	TRACE("ObjectModel::~ObjectModel()");
	
	delete _point;
	delete _polygon;
	delete _pointVector;
	delete _polygonVector;
	delete _texturePoint;
	delete _orgWfm;
	delete _workWfm;
}

void ObjectModel::openTeethObject(const char *fname, int mode)
{
	TRACE("ObjectModel::openTeethObject(char , int)");
	
	char message[128];
	
	if(loadObjectFile(fname, mode) != 0){
		sprintf(message, "cannot open [%s]", fname);
		TRACE(message);
		exit(-1);
	}
	if(loadObjectFile(fname, mode) != 0){
		sprintf(message, "cannot open [%s]", fname);
		TRACE(message);
		exit(-1);
	}
	
	sprintf(message," %d Points, %d Polygons.\n",
		getPointNum(), getPolygonNum());
	TRACE(message);
	
	sprintf(message, " %d Point Vectors, %d Polygon Vectors.\n\n",
		getPointVectorNum(), getPolygonVectorNum());
	TRACE(message);
    
	// calculation polygon vectors
	if(getPolygonVectorNum() == 0) calcPolygonVector();
	
	// calculation point vectors
	if(getPointVectorNum() == 0) calcPointVector();
}

void ObjectModel::openMouthWallObject(const char *fname)
{
	TRACE("ObjectModel::openMouthWallObject(char)");
	
	char message[128];
	
	if(loadObjectFile(fname, MOUTH_WALL) != 0){
		sprintf(message, "cannot open [%s]", fname);
		printf("ERROR %s\n", message);
		exit(-1);
	}
	
	sprintf(message, " %d Points, %d Polygons.\n",
		getPointNum(), getPolygonNum());
	TRACE( message );
	
	sprintf(message, " %d Point Vectors, %d Polygon Vectors.\n\n",
		getPointVectorNum(), getPolygonVectorNum());
	TRACE( message );
	
	// calculation polygon vectors
	//printf("PolygonVector Num = %d\n", getPolygonVectorNum());
	if(getPolygonVectorNum() == 0) calcPolygonVector();
	
	// calculation point vectors
	if(getPointVectorNum() == 0) calcPointVector();
	
}

int ObjectModel::loadObjectFile(const char *fname, int mode)
{
	TRACE("ObjectModel::loadObjectFile(char ,int)");
	FILE *fp;
	char tok[256];
	char message[128];
	int pointNum = 0;
	int polygonNum = 0;
	int pointVectorNum = 0;
	int polygonVectorNum = 0;
	int texturePointNum = 0;
	Point3f *pointPtr = NULL;
	Polygon3 *polygonPtr = NULL;
	Vector3f *pointVectorPtr = NULL;
	Vector3f *polygonVectorPtr = NULL;
	Point2f *texturePointPtr = NULL;
	
	if(NULL ==(fp = fopen(fname, "r"))){
		sprintf(message, "loadObjectFile: file open error [%s]", fname);
		TRACE( message );
		return(-1);
	}
	
	while((getFileData(fp, tok, " \t\n")) != NULL){
		if(!strcmp(tok, "v"))
			pointNum++;
		else if(!strcmp(tok, "f"))
			polygonNum++;
		else if(!strcmp(tok, "vn"))
			pointVectorNum++;
		else if(!strcmp(tok, "tv"))
			texturePointNum++;
	}
	
	// allocate object elements
	if(pointNum != 0){
		if((pointPtr = new Point3f[ pointNum ]) == NULL){
			fputs("\7loadObjectFile: memory allocation error\n", stderr);
			fclose(fp);
			return(-1);
		}
	}
	if(polygonNum != 0){
		if((polygonPtr = new Polygon3[ polygonNum ]) == NULL){
			fputs("\7loadObjectFile: memory allocation error\n", stderr);
			fclose(fp);
			return(-1);
		}
	}
	if(pointVectorNum != 0){
		if((pointVectorPtr = new Vector3f[ pointVectorNum ]) == NULL){
			fputs("\7loadObjectFile: memory allocation error\n", stderr);
			fclose(fp);
			return(-1);
		}
	}
	
	if(polygonVectorNum != 0){
		if((polygonVectorPtr = new Vector3f[ polygonVectorNum ]) == NULL){
			fputs("\7loadObjectFile: memory allocation error\n", stderr);
			fclose(fp);
			return(-1);
		}
	}
	
	if(texturePointNum != 0){
		if((texturePointPtr = (Point2f *)new Point2f[ texturePointNum ]) == NULL){
			fputs("\7loadObjectFile: memory allocation error\n",stderr);
			fclose(fp);
			return(-1);
		}
	}
	
	_pointNum = pointNum;
	_polygonNum = polygonNum;
	_pointVectorNum = pointVectorNum;
	_polygonVectorNum = polygonVectorNum;
	_texturePointNum = texturePointNum;
	
	_point = pointPtr;
	_polygon = polygonPtr;
	_pointVector = pointVectorPtr;
	_polygonVector = polygonVectorPtr;
	_texturePoint = texturePointPtr;
	
	fseek(fp, 0, SEEK_SET);
	while((getFileData(fp, tok, " \t\n")) != NULL){
		if(!strcmp(tok, "v")){
			// Get Points Data
			switch(mode){
			case TEETH_UPPER:
				pointPtr->x = atof(getFileData(fp, tok, " \t\n"));
				pointPtr->y = atof(getFileData(fp, tok, " \t\n"));
				pointPtr->z = atof(getFileData(fp, tok, " \t\n"));
				break;
			case TEETH_LOWER:
				pointPtr->x = atof(getFileData(fp, tok, " \t\n"));
				pointPtr->y = atof(getFileData(fp, tok, " \t\n"));
				pointPtr->z = atof(getFileData(fp, tok, " \t\n"));
				break;
			case MOUTH_WALL:
				pointPtr->x = atof(getFileData(fp, tok, " \t\n"));
				pointPtr->y = atof(getFileData(fp, tok, " \t\n"));
				pointPtr->z = atof(getFileData(fp, tok, " \t\n"));
				break;
			default:
				break;
			}
			pointPtr++;
		}
		else if(!strcmp(tok, "f")){
			// Get Points of Link Data
			polygonPtr->p0 = atoi(getFileData(fp, tok, " /\t\n")) - 1;
			polygonPtr->tp0 = atoi(getFileData(fp, tok, " /\t\n")) - 1;
			polygonPtr->p1 = atoi(getFileData(fp, tok, " /\t\n")) - 1;
			polygonPtr->tp1 = atoi(getFileData(fp, tok, " /\t\n")) - 1;
			polygonPtr->p2 = atoi(getFileData(fp, tok, " /\t\n")) - 1;
			polygonPtr->tp2 = atoi(getFileData(fp, tok, " /\t\n")) - 1;
			polygonPtr++;
		}
		else if(!strcmp(tok, "vn")) {
			// Get Points of Polygon Vector
			pointVectorPtr->x = atof(getFileData(fp, tok, " \t\n"));
			pointVectorPtr->y = atof(getFileData(fp, tok, " \t\n"));
			pointVectorPtr->z = atof(getFileData(fp, tok, " \t\n"));
			pointVectorPtr++;
		}
	}
	fclose(fp);
	return(0);
}


int ObjectModel::calcPolygonVector()
{
	TRACE("ObjectModel::calcPolygonVector()");
	char message[128];
	int polygonVectorNum;
	Vector3f *polygonVectorPtr;
	Polygon3 *polygonPtr;
	Point3f *pointPtr;
	
	TRACE("recalculation teeth Polygon Vector: ");
	
	if(_polygonVector == NULL){
		polygonVectorNum = _polygonNum;
		polygonVectorPtr = new Vector3f[polygonVectorNum];
	}
	else{
		polygonVectorNum = _polygonVectorNum;
		polygonVectorPtr =(Vector3f *)realloc(_polygonVector, polygonVectorNum * sizeof(Vector3f));
	}
	
	// error
	if(polygonVectorPtr == NULL){
		sprintf(message, "calcPolygonVector: memory allocation error");
		TRACE( message );
		return(-1);
	}
	
	_polygonVectorNum = polygonVectorNum;
	_polygonVector = polygonVectorPtr;
	
	// calculate polygon vector
	polygonPtr = _polygon;
	pointPtr = _point;
	for(int i = 0; i < polygonVectorNum; i++){
		
		*polygonVectorPtr = normalVector(*(pointPtr +(polygonPtr->p0)),
			*(pointPtr +(polygonPtr->p1)),
			*(pointPtr +(polygonPtr->p2)));
		
		polygonPtr++;
		polygonVectorPtr++;
	}
	_changeFlag = true;
	TRACE("	Ok.\n");
	sprintf(message, "%d Polygon Vectors.\n\n", polygonVectorNum);
	TRACE( message );
	return(0);
}

int ObjectModel::calcPointVector()
{
	TRACE("ObjectModel::calcPointVector()");
	int pointVectorNum;
	int polygonVectorNum;
	Vector3f *pointVectorPtr;
	Vector3f *polygonVectorPtr;
	Polygon3 *polygonPtr;
	Vector3f n;
	char message[256];
	
	TRACE("Recalculation Teeth Point Vector: ");
	
	if(_pointVector == NULL){
		pointVectorNum = _pointNum;
		pointVectorPtr = new Vector3f[ pointVectorNum ];
	}
	else {
		pointVectorNum = _pointVectorNum;
		pointVectorPtr =(Vector3f *)realloc(_pointVector, pointVectorNum * sizeof(Vector3f));
	}
	
	if(pointVectorPtr == NULL){
		TRACE("calcPointVector: memory allocation error\n");
		return(-1);
	}
	
	_pointVectorNum = pointVectorNum;
	_pointVector = pointVectorPtr;
	
	// calculate point vector
	polygonVectorNum = _polygonVectorNum;
	
	for(int i = 0; i < pointVectorNum; i++){
		polygonPtr = _polygon;
		polygonVectorPtr = _polygonVector;
		n.x = n.y = n.z = 0.0;
		
		for(int j = 0; j < polygonVectorNum; j++){
			if((polygonPtr->p0 == i) ||(polygonPtr->p1 == i) ||
				(polygonPtr->p2 == i)){
				
				n.x += polygonVectorPtr->x;
				n.y += polygonVectorPtr->y;
				n.z += polygonVectorPtr->z;
			}
			polygonPtr++;
			polygonVectorPtr++;
		}
		// Normalized Polygon Vector
		*pointVectorPtr = unitVector(n);
		pointVectorPtr++;
	}
	_changeFlag = true;
	TRACE("	Ok.\n");
	sprintf(message, "%d Point Vectors.\n\n", pointVectorNum);
	TRACE( message );
	return(0);
}

void ObjectModel::adjustTeeth(int mode)
{
	TRACE("ObjectModel::adjustTeeth(int)");
	int i;

	int pointNum = getPointNum();
	Point3f *pointPtr = getPoint();
	double xmin, xmax, ymin, ymax, zmin, zmax;
	double x, y, z;
	
	/******** scaling ********/
	xmin = ymin = zmin = MAXDOUBLE;
	xmax = ymax = zmax = -MAXDOUBLE;
	
	// model scale
	double xscale = fabs( _orgWfm->Ggridx(13, 15) -
		_orgWfm->Ggridx(12, 15));
	double upper_yscale = fabs(_orgWfm->Ggridy(12, 17) -
		_orgWfm->Ggridy(1, 36));
	double lower_yscale = fabs(_orgWfm->Ggridy(1, 37) -
		_orgWfm->Ggridy(1, 40));
	
	for(i = 0; i < pointNum; i++){
		x = pointPtr->x;
		y = pointPtr->y;
		z = pointPtr->z;
		
		if(x < xmin) xmin = x;
		if(y < ymin) ymin = y;
		if(z < zmin) zmin = z;
		
		if(x > xmax) xmax = x;
		if(y > ymax) ymax = y;
		if(z > zmax) zmax = z;
		
		pointPtr++;
	}
	
	double xcenter =(xmax + xmin) / 2.0;
	double xrange = fabs(xmax - xmin);
	double upper_yrange, lower_yrange;
	
	// Scaling Teeth Data
	switch(mode){
	case TEETH_UPPER:
		upper_yrange = fabs(ymax - ymin);
		pointPtr = getPoint();
		for(i = 0; i < pointNum; i++){
			pointPtr->x =  pointPtr->x * xscale / xrange;
			pointPtr->y =  pointPtr->y * upper_yscale / upper_yrange;
			//pointPtr->z -= upper_ztrans;
			pointPtr++;
		}
		break;
	case TEETH_LOWER:
		lower_yrange = fabs(ymax - ymin);
		pointPtr = getPoint();
		for(i = 0; i < pointNum; i++){
			pointPtr->x = pointPtr->x * xscale  / xrange;
			pointPtr->y = pointPtr->y * lower_yscale / lower_yrange;
			//pointPtr->z -= lower_ztrans;
			pointPtr++;
		}
		break;
	default:
		break;
	}
	
	// Translation Teeth Data
	xmin = ymin = zmin = MAXDOUBLE;
	xmax = ymax = zmax = -MAXDOUBLE;
	pointPtr = getPoint();
	
	for(i = 0; i < pointNum; i++){
		x = pointPtr->x;
		y = pointPtr->y;
		z = pointPtr->z;
		
		if(x < xmin) xmin = x;
		if(y < ymin) ymin = y;
		if(z < zmin) zmin = z;
		
		if(x > xmax) xmax = x;
		if(y > ymax) ymax = y;
		if(z > zmax) zmax = z;
		
		pointPtr++;
	}
	
	xcenter =(xmax + xmin) / 2.0; 
	double upper_xtrans = fabs(_orgWfm->Ggridx(1, 35) - xcenter);
	double lower_xtrans = fabs(_orgWfm->Ggridx(1, 35) - xcenter);
	double upper_ytrans = fabs((_orgWfm->Ggridy(1, 35) 
			     + _orgWfm->Ggridy(1, 36)) / 2.0 - ymin);
	double lower_ytrans = fabs((_orgWfm->Ggridy(1, 35) 
			     + _orgWfm->Ggridy(1, 36)) / 2.0 - ymax);
	double upper_ztrans = fabs(_orgWfm->Ggridz(1, 10) - zmax);
	double lower_ztrans = fabs(_orgWfm->Ggridz(1, 22) - zmax);
	
	switch(mode){
	case TEETH_UPPER:
		pointPtr = getPoint();
		for(i = 0; i < pointNum; i++){
			if(xcenter > _orgWfm->Ggridx(1, 36))
				pointPtr->x -= upper_xtrans;
			else
				pointPtr->x += upper_xtrans;
			pointPtr->y -= upper_ytrans;
			
			if(zmax > _orgWfm->Ggridz(1, 8))
				pointPtr->z -= upper_ztrans;
			else
				pointPtr->z += upper_ztrans;
			if(i == 7)
				mostlengthz1 = pointPtr->z;
			pointPtr++;
		}
		break;
	case TEETH_LOWER:
		pointPtr = getPoint();
		for(i = 0; i < pointNum; i++){
			if(xcenter > _orgWfm->Ggridx(1, 36))
				pointPtr->x -= lower_xtrans;
			else
				pointPtr->x += lower_xtrans;
			
			pointPtr->y -= lower_ytrans;
			
			if(zmax > _orgWfm->Ggridz(1, 8))
				pointPtr->z -= lower_ztrans;
			else
				pointPtr->z += lower_ztrans;
			
			if(i == 7){
				mostlengthy = pointPtr->y;
				mostlengthz2 = pointPtr->z;
			}
			pointPtr++;
		}
		break;
	default:
		break;
	}
	TRACE("	Ok.\n\n");
}

double ObjectModel::moveTeeth(int mode)
{
	TRACE("ObjectModel::moveTeeth(int)");
	
	double diff;
	// Set Jaw Scale
	_jox =(_orgWfm->Ggridx(14, 5)
		+ _orgWfm->Ggridx(15, 5)) / 2.0; 
	_joy =(_orgWfm->Ggridy(18, 12)
		+ _orgWfm->Ggridy(19, 12)) / 2.0;
	_joz =(_orgWfm->Ggridz(14, 5)
		+ _orgWfm->Ggridz(15, 5)) / 2.0;
	
	// Set Upper Teeth Scale
	_utx =(_orgWfm->Ggridx(1, 33) 
		+ _orgWfm->Ggridx(2, 33)) / 2.0;
	_uty =  _orgWfm->Ggridy(1, 36);
	_utz = _orgWfm->Ggridz(1, 36);
	_utvz = _utz - _joz;
	
	// Set Lower Teeth Scale 
	_ltx =(_orgWfm->Ggridx(1, 33) 
		+ _orgWfm->Ggridx(2, 33)) / 2.0;
	_lty = _orgWfm->Ggridy(1, 37);
	_ltz = _orgWfm->Ggridz(1, 37);
	_ltvz = _ltz - _joz;
	
	//Comparing OriginalWfm and WorkWfm
	_jovy = _orgWfm->Ggridy(18, 20) - _joy;
	_jovz = _orgWfm->Ggridz(18, 20) - _joz;
	_jvy = _workWfm->Ggridy(18, 20) - _joy;
	_jvz = _workWfm->Ggridz(18, 20) - _joz;
	
	_calc = (_jovy * _jvy + _jovz * _jvz)
		/(sqrt(_jovy * _jovy + _jovz * _jovz) 
		* sqrt(_jvy * _jvy + _jvz * _jvz));
	if(_calc > 1.0) _calc = 1.0;
	_ja = acos(_calc) * 180.0 / M_PI;
	
	diff = _workWfm->Ggridx(1, 32)
		- _orgWfm->Ggridx(1, 32);
	
	switch(mode){
	case TEETH_UPPER:
		if(diff > 0.0){
			_calc = _workWfm->Ggridz(1, 36) 
				- 0.05;
			_calc = _calc - mostlengthz1;
		}
		else{
			_calc = _workWfm->Ggridz(1, 36)
				- mostlengthz1-0.03;
		}
		break;
	case TEETH_LOWER:
		diff = _ja * 0.45 * M_PI / 180.0;
		_calc =  cos(diff) * mostlengthz2    
			+ sin(diff) * mostlengthy;
		break;
	}
	
	/*******************************************/
	return(_calc);
}

void ObjectModel::adjustMouthwall()
{
	TRACE("ObjectModel::adjustMouthwall()");
	int i;

	int pointNum = getPointNum();
	Point3f *pointPtr = getPoint();
	double xmin, xmax, ymin, ymax, zmin, zmax;
	double x, y, z;
	
	TRACE("AdjustMouthwall:");
	
	// for mouthwall model vertex
	xmin = ymin = zmin = MAXDOUBLE;
	xmax = ymax = zmax = -MAXDOUBLE;
	
	// get wireframe scale
	double xscale = fabs(_orgWfm->Ggridx(13, 16) 
		      - _orgWfm->Ggridx(12, 16));
	double yscale = fabs(_orgWfm->Ggridy(3, 25) 
		      - _orgWfm->Ggridy(14, 14));
	double zscale = fabs(_orgWfm->Ggridz(13, 11) 
		      - _orgWfm->Ggridz(19, 12));
	
	// calc max and min on mouthwall model
	for(i = 0; i < pointNum; i++){
		x = pointPtr->x;
		y = pointPtr->y;
		z = pointPtr->z;
		
		if(x < xmin) xmin = x;
		if(x < ymin) ymin = y;
		if(z < zmin) zmin = z;
		
		if(x > xmax) xmax = x;
		if(y > ymax) ymax = y;
		if(z > zmax) zmax = z;
		
		pointPtr++;
	}
	
	// calculation mouthwall model x center
	double xrange = fabs(xmax - xmin);
	double yrange = fabs(ymax - ymin);
	double zrange = fabs(zmax - zmin);
	
	// reset pointer
	pointPtr = getPoint();
	
	// scaling
	for(i = 0; i < pointNum; i++){
		pointPtr->x = pointPtr->x * xscale / xrange;
		pointPtr->y = pointPtr->y * yscale / yrange;
		pointPtr->z = pointPtr->z * zscale / zrange;
		pointPtr++;
	}
	pointPtr = getPoint();
	
	// about translation
	xmin = ymin = zmin = MAXDOUBLE;
	xmax = ymax = zmax = -MAXDOUBLE;
	for(i = 0; i < pointNum; i++){
		x = pointPtr->x;
		y = pointPtr->y;
		z = pointPtr->z;
		
		if(x < xmin) xmin = x;
		if(y < ymin) ymin = y;
		if(z < zmin) zmin = z;
		
		if(x > xmax) xmax = x;
		if(y > ymax) ymax = y;
		if(z > zmax) zmax = z;
		
		pointPtr++;
	}
	
	double xcenter =(xmax + xmin) / 2.0;
	double ycenter =(ymax + ymin) / 2.0;
	double xtrans = fabs(_orgWfm->Ggridx(1, 36) - xcenter);
	double ytrans = fabs(_orgWfm->Ggridy(1, 36) - ycenter);
	double ztrans = fabs(_orgWfm->Ggridz(1, 33) - zmax);
	
	
	// translate
	pointPtr = getPoint();
	for(i = 0; i < pointNum; i++){
		// x trans
		if(xcenter > _orgWfm->Ggridx(1, 36))
			pointPtr->x -= xtrans;
		else
			pointPtr->x += xtrans;
		// ytrans
		if(ycenter > _orgWfm->Ggridy(1, 36))
			pointPtr->y -= ytrans;
		else
			pointPtr->y += ytrans;
		// ztrans
		if(zmax > _orgWfm->Ggridz(1,33))
			pointPtr->z -= ztrans;
		else
			pointPtr->z += ztrans;
		pointPtr++;
	}
	TRACE("	Ok.\n\n");
}

void ObjectModel::moveMouthwall()
{
	TRACE("ObjectModel::moveMouthwall()");
	
	Point3f *pointPtr = getPoint();
	
	pointPtr[56].x = _workWfm->Ggridx(1,36);
	pointPtr[56].y = _workWfm->Ggridy(1,36);
	pointPtr[56].z = _workWfm->Ggridz(1,36);
	pointPtr[65].x = _workWfm->Ggridx(1,36);
	pointPtr[65].y = _workWfm->Ggridy(1,36)+0.01;
	pointPtr[65].z = _workWfm->Ggridz(1,36)-0.02;
	
	pointPtr[44].x = _workWfm->Ggridx(1,11);
	pointPtr[44].y = _workWfm->Ggridy(1,11);
	pointPtr[44].z = _workWfm->Ggridz(1,11);
	pointPtr[48].x = _workWfm->Ggridx(1,11);
	pointPtr[48].y = _workWfm->Ggridy(1,11)+0.01;
	pointPtr[48].z = _workWfm->Ggridz(1,11)-0.02;
	
	pointPtr[49].x = _workWfm->Ggridx(2,11);
	pointPtr[49].y = _workWfm->Ggridy(2,11);
	pointPtr[49].z = _workWfm->Ggridz(2,11);
	pointPtr[46].x = _workWfm->Ggridx(2,11);
	pointPtr[46].y = _workWfm->Ggridy(2,11)+0.01;
	pointPtr[46].z = _workWfm->Ggridz(2,11)-0.02;
	
	pointPtr[39].x = _workWfm->Ggridx(1,12);
	pointPtr[39].y = _workWfm->Ggridy(1,12);
	pointPtr[39].z = _workWfm->Ggridz(1,12);
	pointPtr[58].x = _workWfm->Ggridx(1,12);
	pointPtr[58].y = _workWfm->Ggridy(1,12)+0.01;
	pointPtr[58].z = _workWfm->Ggridz(1,12)-0.02;
	
	pointPtr[45].x = _workWfm->Ggridx(2,12);
	pointPtr[45].y = _workWfm->Ggridy(2,12);
	pointPtr[45].z = _workWfm->Ggridz(2,12);
	pointPtr[34].x = _workWfm->Ggridx(2,12);
	pointPtr[34].y = _workWfm->Ggridy(2,12)+0.01;
	pointPtr[34].z = _workWfm->Ggridz(2,12)-0.02;
	
	pointPtr[51].x = _workWfm->Ggridx(1,13);
	pointPtr[51].y = _workWfm->Ggridy(1,13);
	pointPtr[51].z = _workWfm->Ggridz(1,13);
	pointPtr[38].x = _workWfm->Ggridx(1,13)-0.01;
	pointPtr[38].y = _workWfm->Ggridy(1,13)+0.01;
	pointPtr[38].z = _workWfm->Ggridz(1,13)-0.02;
	
	pointPtr[32].x = _workWfm->Ggridx(2,13);
	pointPtr[32].y = _workWfm->Ggridy(2,13);
	pointPtr[32].z = _workWfm->Ggridz(2,13);
	pointPtr[59].x = _workWfm->Ggridx(2,13)+0.01;
	pointPtr[59].y = _workWfm->Ggridy(2,13)+0.01;
	pointPtr[59].z = _workWfm->Ggridz(2,13)-0.02;
	
	pointPtr[24].x = _workWfm->Ggridx(1,14);
	pointPtr[24].y = _workWfm->Ggridy(1,14);
	pointPtr[24].z = _workWfm->Ggridz(1,14);
	pointPtr[61].x = _workWfm->Ggridx(1,14)-0.01;
	pointPtr[61].y = _workWfm->Ggridy(1,14)+0.01;
	pointPtr[61].z = _workWfm->Ggridz(1,14)-0.02;
	
	pointPtr[54].x = _workWfm->Ggridx(2,14);
	pointPtr[54].y = _workWfm->Ggridy(2,14);
	pointPtr[54].z = _workWfm->Ggridz(2,14);
	pointPtr[26].x = _workWfm->Ggridx(2,14)+0.01;
	pointPtr[26].y = _workWfm->Ggridy(2,14)+0.01;
	pointPtr[26].z = _workWfm->Ggridz(2,14)-0.02;
	
	pointPtr[53].x = _workWfm->Ggridx(1,33);
	pointPtr[53].y = _workWfm->Ggridy(1,33);
	pointPtr[53].z = _workWfm->Ggridz(1,33);
	pointPtr[23].x = _workWfm->Ggridx(1,33)-0.01;
	pointPtr[23].y = _workWfm->Ggridy(1,33)+0.01;
	pointPtr[23].z = _workWfm->Ggridz(1,33)-0.02;
	
	pointPtr[29].x = _workWfm->Ggridx(2,33);
	pointPtr[29].y = _workWfm->Ggridy(2,33);
	pointPtr[29].z = _workWfm->Ggridz(2,33);
	pointPtr[60].x = _workWfm->Ggridx(2,33)+0.01;
	pointPtr[60].y = _workWfm->Ggridy(2,33)+0.01;
	pointPtr[60].z = _workWfm->Ggridz(2,33)-0.02;
	
	pointPtr[16].x = _workWfm->Ggridx(1,18);
	pointPtr[16].y = _workWfm->Ggridy(1,18);
	pointPtr[16].z = _workWfm->Ggridz(1,18);
	pointPtr[62].x = _workWfm->Ggridx(1,18);
	pointPtr[62].y = _workWfm->Ggridy(1,18);
	pointPtr[62].z = _workWfm->Ggridz(1,18)-0.02;
	
	pointPtr[50].x = _workWfm->Ggridx(2,18);
	pointPtr[50].y = _workWfm->Ggridy(2,18);
	pointPtr[50].z = _workWfm->Ggridz(2,18);
	pointPtr[19].x = _workWfm->Ggridx(2,18);
	pointPtr[19].y = _workWfm->Ggridy(2,18);
	pointPtr[19].z = _workWfm->Ggridz(2,18)-0.02;
	
	pointPtr[11].x = _workWfm->Ggridx(1,17);
	pointPtr[11].y = _workWfm->Ggridy(1,17);
	pointPtr[11].z = _workWfm->Ggridz(1,17);
	pointPtr[10].x = _workWfm->Ggridx(1,17);
	pointPtr[10].y = _workWfm->Ggridy(1,17);
	pointPtr[10].z = _workWfm->Ggridz(1,17)-0.02;
	
	pointPtr[17].x = _workWfm->Ggridx(2,17);
	pointPtr[17].y = _workWfm->Ggridy(2,17);
	pointPtr[17].z = _workWfm->Ggridz(2,17);
	pointPtr[57].x = _workWfm->Ggridx(2,17);
	pointPtr[57].y = _workWfm->Ggridy(2,17);
	pointPtr[57].z = _workWfm->Ggridz(2,17)-0.02;
	
	pointPtr[4].x = _workWfm->Ggridx(1,16);
	pointPtr[4].y = _workWfm->Ggridy(1,16);
	pointPtr[4].z = _workWfm->Ggridz(1,16);
	pointPtr[3].x = _workWfm->Ggridx(1,16);
	pointPtr[3].y = _workWfm->Ggridy(1,16);
	pointPtr[3].z = _workWfm->Ggridz(1,16)-0.02;
	
	pointPtr[13].x = _workWfm->Ggridx(2,16);
	pointPtr[13].y = _workWfm->Ggridy(2,16);
	pointPtr[13].z = _workWfm->Ggridz(2,16);
	pointPtr[12].x = _workWfm->Ggridx(2,16);
	pointPtr[12].y = _workWfm->Ggridy(2,16);
	pointPtr[12].z = _workWfm->Ggridz(2,16)-0.02;
	
	pointPtr[52].x = _workWfm->Ggridx(1,15);
	pointPtr[52].y = _workWfm->Ggridy(1,15);
	pointPtr[52].z = _workWfm->Ggridz(1,15);
	pointPtr[64].x = _workWfm->Ggridx(1,15);
	pointPtr[64].y = _workWfm->Ggridy(1,15);
	pointPtr[64].z = _workWfm->Ggridz(1,15)-0.02;
	
	pointPtr[55].x = _workWfm->Ggridx(2,15);
	pointPtr[55].y = _workWfm->Ggridy(2,15);
	pointPtr[55].z = _workWfm->Ggridz(2,15);
	pointPtr[63].x = _workWfm->Ggridx(2,15);
	pointPtr[63].y = _workWfm->Ggridy(2,15);
	pointPtr[63].z = _workWfm->Ggridz(2,15)-0.02;
	
	pointPtr[5].x = _workWfm->Ggridx(1,37);
	pointPtr[5].y = _workWfm->Ggridy(1,37);
	pointPtr[5].z = _workWfm->Ggridz(1,37);
	pointPtr[1].x = _workWfm->Ggridx(1,37);
	pointPtr[1].y = _workWfm->Ggridy(1,37);
	pointPtr[1].z = _workWfm->Ggridz(1,37)-0.02;
}

void ObjectModel::calcTexturePoint()
{
	TRACE("ObjectModel::calcTexturePoint()");
	
	int i;
	double xmin, xmax , ymin , ymax, zmin , zmax;
	double x , y , z , p , q ;
	double xcenter , ycenter , zcenter;
	int pointNum = getPointNum();
	Point3f *pointPtr = getPoint();
	Point2f *texturePointPtr = getTexturePoint();
	xmin = ymin = zmin = MAXDOUBLE;
	xmax = ymax = zmax = -MAXDOUBLE;
	
	for(i = 0 ; i < pointNum ; i++){
		x = pointPtr->x;
		y = pointPtr->y;
		z = pointPtr->z;
		
		if(x < xmin) xmin = x;
		if(y < ymin) ymin = y;
		if(z < zmin) zmin = z;
		
		if(x > xmax) xmax = x;
		if(y > ymax) ymax = y;
		if(z > zmax) zmax = z;
		
		pointPtr++;
	}
	
	pointPtr = getPoint();
	
	xcenter =(xmax + xmin) / 2.0;
	ycenter =(ymax + ymin) / 2.0;
	zcenter =(zmax + zmin) / 2.0;
	
	for(i = 0 ; i < pointNum ; i++){
		pointPtr->x -= xcenter;
		pointPtr->y -= ycenter;
		pointPtr->z -= zcenter;
		
		pointPtr++;
	}
	pointPtr = getPoint();
	
	for(i = 0 ; i < pointNum ; i++){
		x = pointPtr->x;
		y = pointPtr->y;
		z = pointPtr->z;
		/*	First Devision	*/
		if(x >= 0.0 && y >= 0.0 && z >=0.0){
			p = acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = acos((-z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		if(x >= 0.0 && y >= 0.0 && z <=0.0){
			p = acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = acos((z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		
		/*	Second Devision	*/
		if(x <= 0.0 && y >= 0.0 && z >=0.0){
			p = acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = -acos((-z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		if(x <= 0.0 && y >= 0.0 && z <=0.0){
			p = acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = -acos((z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		
		/*	Third Devision	*/
		if(x <= 0.0 && y <= 0.0 && z >=0.0){
			p = -acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = -acos((-z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		if(x <= 0.0 && y <= 0.0 && z <=0.0){
			p = -acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = -acos((z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		
		/*	Fourth Devision	*/
		if(x >= 0.0 && y <= 0.0 && z >=0.0){
			p = -acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = acos((-z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		if(x >= 0.0 && y <= 0.0 && z <=0.0){
			p = -acos((x*x + z*z)/((sqrt(x*x + y*y + z*z)
				*sqrt(x*x + z*z))))/3.14159*2.0;
			q = acos((z*z)/(sqrt(z*z) * sqrt(x*x + z*z)))/3.14159;
		}
		texturePointPtr->x = q;
		texturePointPtr->y = p;
		pointPtr++;
		texturePointPtr++;
	}
	pointPtr = getPoint();
	texturePointPtr = getTexturePoint();
}

void ObjectModel::drawMouthwallModel()
{
	TRACE("ObjectModel::drawMouthwallModel()");
	
	int polygonNum = getPolygonNum();
	int pointVectorNum = getPointVectorNum();
	Point3f *pointPtr = getPoint();
	Polygon3 *polygonPtr = getPolygon();
	Vector3f *pointVectorPtr = getPointVector();
	Point2f *texturePointPtr = getTexturePoint();
	
	moveMouthwall();
	
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < polygonNum; i++){
		glTexCoord2d(texturePointPtr[polygonPtr->p0].x, texturePointPtr[polygonPtr->p0].y);
		glVertex3dv((GLdouble *)(pointPtr + polygonPtr->p0));
		
		glTexCoord2d(texturePointPtr[polygonPtr->p1].x, texturePointPtr[polygonPtr->p1].y);
		glVertex3dv((GLdouble *)(pointPtr + polygonPtr->p1));
		
		glTexCoord2d(texturePointPtr[polygonPtr->p2].x, texturePointPtr[polygonPtr->p2].y);
		glVertex3dv((GLdouble *)(pointPtr + polygonPtr->p2));
		polygonPtr++;
	}
	glEnd();
}

void ObjectModel::drawTeethModel(int mode)
{
	TRACE("ObjectModel::drawTeethModel()");
	
	int i;
	int pointNum = getPointNum();
	int polygonNum = getPolygonNum();
	int pointVectorNum = getPointVectorNum();
	double x,y,z,y1,z1,xmax,ymax,zmax,xmin,ymin,zmin,xcenter;
	double calc = moveTeeth(mode);
	double jawAngle = getJawAngle();
	Point3f tmp[600];
	Point3f *pointPtr = getPoint();
	Polygon3 *polygonPtr = getPolygon();
	Vector3f *polygonVectorPtr = getPolygonVector();
	Vector3f *pointVectorPtr = getPointVector();
	glPushMatrix();
	
	switch(mode){
	case TEETH_UPPER:
		for(i = 0 ; i < pointNum ; i++){
			tmp[i].x = pointPtr->x;
			tmp[i].y = pointPtr->y;
			tmp[i].z = pointPtr->z;
			pointPtr++;
		}
		pointPtr = getPoint();
		break;
	case TEETH_LOWER:
		xmin = ymin = zmin = MAXDOUBLE;
		xmax = ymax = zmax = -MAXDOUBLE;
		for(i = 0 ; i < pointNum ; i++){
			x = pointPtr->x;
			y = pointPtr->y;
			z = pointPtr->z;
			if(x < xmin) xmin = x;
			if(y < ymin) ymin = y;
			if(z < zmin) zmin = z;
			
			if(x > xmax) xmax = x;
			if(y > ymax) ymax = y;
			if(z > zmax) zmax = z;
			pointPtr++;
		}
		pointPtr = getPoint();
		for(i = 0 ; i < pointNum ; i++){
			tmp[i].x = pointPtr->x;
			tmp[i].y = pointPtr->y;
			tmp[i].z = pointPtr->z;
			pointPtr++;
		}
		pointPtr = getPoint();
		xcenter =(xmax + xmin) / 2.0;
		y1 = 0.00;
		z1 = zmin;
		glTranslated(-xcenter, -y1, -z1);
		glRotated(jawAngle / 2.0, 1.0, 0.0, 0.0);
		glTranslated(xcenter, y1, z1);
		break;
	}
	glBegin(GL_TRIANGLES);
	for(i = 0; i < polygonNum; i++){
		glNormal3dv((GLdouble *)(pointVectorPtr + polygonPtr->tp0));
		glVertex3d(tmp[polygonPtr->p0].x,tmp[polygonPtr->p0].y, tmp[polygonPtr->p0].z);
		
		glNormal3dv((GLdouble *)(pointVectorPtr + polygonPtr->tp1));
		glVertex3d(tmp[polygonPtr->p1].x,tmp[polygonPtr->p1].y, tmp[polygonPtr->p1].z);
		
		glNormal3dv((GLdouble *)(pointVectorPtr + polygonPtr->tp2));
		glVertex3d(tmp[polygonPtr->p2].x,tmp[polygonPtr->p2].y, tmp[polygonPtr->p2].z);
		polygonPtr++;
	}
	glEnd();
	glPopMatrix();
	
}

