/*++
	ObjModel.h
	Copyright (c) 2000, Takafumi MISAWA & Daisuke ITOH.
	Copyright (c) 2001, teleboy
	Faculty of Engineering, Seikei University. Morishima Laboratory.
	All Rights Reserved.

	Last Modified: 2001/11/12

	last modified 2002 for muscle project.
--*/

#ifndef __OBJECT_MODEL_H__WRITTEN_BY_TAKAHASY__INCLUDED__
#define __OBJECT_MODEL_H__WRITTEN_BY_TAKAHASY__INCLUDED__

#ifdef _WIN32
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif // _WINDOWS

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <math.h>

using namespace std;

#ifndef BOOL
typedef int BOOL;
#endif // BOOL

#ifndef TRUE
#define TRUE ( 1 == 1 )
#endif // TRUE

#ifndef FALSE
#define FALSE !TRUE
#endif // FALSE

// Use these values when you call OpenObjectFile().
#define OM_VERTEX			0x0001
#define OM_LINK				0x0002
#define OM_NORMAL			0x0004
#define OM_TEXTURE			0x0008
#define OM_READALL			0xffff
#define OM_VERTEX_ONLY		OM_VERTEX
#define OM_VERTEX_AND_LINK	( OM_VERTEX | OM_LINK )

class VERTEX3D
{
public:
	double x, y, z;
	double length(){ return sqrt( x * x + y * y + z * z ); }
	// operator function for vertex culculation
	VERTEX3D operator + (VERTEX3D& v){ VERTEX3D temp = { x + v.x, y + v.y, z + v.z }; return temp; }
	VERTEX3D operator - (VERTEX3D& v){ VERTEX3D temp = { x - v.x, y - v.y, z - v.z }; return temp; }
	VERTEX3D& operator += (VERTEX3D& v){ x += v.x; y += v.y; z += v.z; return *this; }
	VERTEX3D& operator -= (VERTEX3D& v){ x -= v.x; y -= v.y; z -= v.z; return *this; }
	VERTEX3D& operator /= (double d){ x /= d; y /= d; z /= d; return *this; }
	VERTEX3D operator * (double d){ VERTEX3D temp = { x * d, y * d, z * d }; return temp; }
	VERTEX3D operator / (double d){ VERTEX3D temp = { x / d, y / d, z / d }; return temp; }
	VERTEX3D operator * (VERTEX3D& v){ VERTEX3D temp = { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; return temp; }	// outer product
	double operator , (VERTEX3D& v){ return ( x * v.x + y * v.y + z * v.z ); }		// inner product
};
typedef VERTEX3D NORMAL3D;
typedef VERTEX3D TEXTURE3D;
class LINK_ELEMENT { public: int v, n, t; };
class LINK { public: LINK_ELEMENT* element; };

class ObjModel
{
private:
	BOOL m_eolFlag;

protected:
	// protected member variance
	int m_number_v;			// number of "v" lines
	int m_number_vn;		// number of "vn" lines
	int m_number_f;			// number of "f" lines
	int m_number_tv;		// number of "tv"("vt") lines
	int *m_number_f_ele;	// number of "f" elements of a line. (v, n) is one.

	double m_xmax, m_xmin, m_ymax, m_ymin, m_zmax, m_zmin;

	VERTEX3D* m_vertex;
	NORMAL3D* m_normal;
	LINK* m_link;
	TEXTURE3D* m_texture_ver;

	// protected process function
	BOOL GetFileData(FILE* fp, char* buf);
	BOOL LoadObjectFile(const char* filename, int iFlag = OM_READALL);

	// protected data output function
	VERTEX3D* GetVertexPtr() { return m_vertex; }			// for get vertex pointer
	NORMAL3D* GetNormalPtr() { return m_normal; }			// for get normal pointer
	LINK* GetLinkPtr() { return m_link; }					// for get link pointer
	TEXTURE3D* GetTextureVerPtr() { return m_texture_ver; }	// for get texture pointer

public:
	// constructor
	ObjModel();
	ObjModel(const char* filename);

	// destructor
	virtual ~ObjModel();

	// public process function
	virtual BOOL OpenObjectFile(const char* filename, int iFlag = OM_READALL){ return LoadObjectFile(filename, iFlag); }
	virtual void DrawObjModel();
	virtual void DrawWFM();
	virtual BOOL CalcNormalVector();
	virtual BOOL NormalizeObjModel();

	// compatibility function with old versions
	BOOL open(const char* filename, int iFlag = OM_READALL) { return OpenObjectFile(filename, iFlag); }
	void draw() { DrawObjModel(); }
	void drawWFM() { DrawWFM(); }

	// public data output function
	int GetVertexNum() { return m_number_v; }						// for get vertex(v) line number
	int GetNormalNum() { return m_number_vn; }						// for get normal(vn) line number
	int GetLinkNum() { return m_number_f; }							// for get link(f) line number
	int GetLinkEleNum(int index) { return m_number_f_ele[index]; }	// for get element number of each link(f) line
	int GetTextureNum() { return m_number_tv; } 					// for get texture(tv) line number
	VERTEX3D& GetVertex(int index = 0) { return m_vertex[index]; }
	NORMAL3D& GetNormal(int index = 0) { return m_normal[index]; }
	LINK& GetLink(int index = 0) { return m_link[index]; }
	TEXTURE3D& GetTextureVer(int index = 0) { return m_texture_ver[index]; }


	// for muscle project
	void SetVertex(int index, VERTEX3D vertex){ m_vertex[index] = vertex; }
	void SaveObjModel(const char* filename);
	ObjModel(const char* vertexfile, const char* linkfile);
	ObjModel(ObjModel* pObj);

	// for IPA
	void ReCalcTexture(double scale);
	double GetXMax() { return m_xmax; }
	double GetYMax() { return m_ymax; }
	double GetZMax() { return m_zmax; }
	double GetXMin() { return m_xmin; }
	double GetYMin() { return m_ymin; }
	double GetZMin() { return m_zmin; }
	

protected:
	BOOL LoadUTFModel(const char* vertexfile, const char* linkfile);
	int* m_vertex_index;
};

#endif	// __OBJECT_MODEL_H__WRITTEN_BY_TAKAHASY__INCLUDED__
