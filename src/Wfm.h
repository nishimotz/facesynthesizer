
#ifndef __WFM_H__
#define __WFM_H__

/*+++
Wfm.h
Copyright (c) 2000, Junichi Muto.
Modified 2000, Takafumi Misawa.
Modified by Shin Ogata. 2001/11/23
- addition for "M_PI" at Values.h and "SQR", from "MFunction.h"
Faculty of Engineering, Seikei University. Morishima Laboratory.
ALL RIGHTS RESERVED.
---*/

#include <iostream>
#include <math.h>

using namespace std;

// function SQR definition
#ifndef SQR
#define SQR(x) pow((x), (2.0))
#endif

// M_PI definition
#ifndef M_PI
#define M_PI 3.141592
#endif

// Wfm Group Number
#define		GroupMax	40

/* Face Group Number Define */
#define		G_LIP_L			1
#define		G_LIP_R			2
#define		G_NOSE			3
#define		G_EYE_L			4
#define		G_EYE_R			5
#define		G_BROW_L		6
#define		G_BROW_R		7
#define		G_FOREHEAD_L	8
#define		G_FOREHEAD_R	9
#define		G_CHEEK_L		10
#define		G_CHEEK_R		11
#define		G_MOUTH_L		12
#define		G_MOUTH_R		13
#define		G_JAW_L			14
#define		G_JAW_R			15
#define		G_NECK_L		16
#define		G_NECK_R		17
#define		G_CONTOUR_L		18
#define		G_CONTOUR_R		19
#define		G_EYEBALL_L		20
#define		G_EYEBALL_R		21
#define		G_HEAD_L		25
#define		G_HEAD_R		26

class PointNumber	{ public:	short gn, pn; };

class Wfm {
public:
	class XYZ {
	public:
		double	x, y, z;
		short	gn, pn;
		XYZ operator-(XYZ &xyz) { XYZ temp = {x - xyz.x, y - xyz.y, z - xyz.z}; return temp; }
		XYZ operator/(double d) { XYZ temp = {x / d, y / d, z / d}; return temp; }
		XYZ &operator=(double d) { x = y = z = d; return *this; }
		XYZ &operator+=(XYZ &xyz) { x += xyz.x; y += xyz.y; z += xyz.z; return *this; }
		double length() { return sqrt(x * x + y * y + z * z); }
		XYZ operator*(XYZ &xyz)	{ XYZ temp = {y * xyz.z - z * xyz.y, z * xyz.x - x * xyz.z, x * xyz.y - y * xyz.x}; return temp; }	// outer product
		double operator,(XYZ &xyz) { return (x * xyz.x + y * xyz.y + z * xyz.z); }		// inner product
	};
	class Link1			{ public:	short g, p, sn; };
	class LINK			{ public:	Link1 g1, g2, g3;	short gn, pn; };
	class EYE			{ public:	double x, y; };
	
	private:
		double m_dRadius;
		bool m_bCylinder;
		bool m_bSuccess;
		
	protected:
		XYZ		*_grid;
		short	*_gindex[GroupMax];
		short	_grn;
		short	_gindexmax[GroupMax];
		
		short	*_linkindex[GroupMax];
		LINK		*_link;
		int		_trn;
		
		// Protected Data input function
		
	public:
		// Public data members
		static const PointNumber EyeHole[40][3];
		
		// Public constructors
		Wfm();
		Wfm(const char *filename);
		
		// Copy constructor
		Wfm(Wfm& obj);
		
		// Destructor
		virtual ~Wfm();
		
		// Process function
		int setLink(void);
		void deleteLink();
		void MoveToZero();
		
		// Data input function
		int load(const char *filename);
		int loadLink(const char *filename);
		
		void copyGrn(Wfm& obj);
		
		// Data output function
		Wfm& operator=(Wfm& obj);
		Wfm& cylinder(double dR = 0.0);	// transfrom 3D model to cylinder model
		Wfm& flat();					// transform cylinder model to 2D model
		Wfm& normal();					// calculate normal vector of vertex
		int save(const char *filaneme);
		int saveDiff(const char *filename, Wfm *target_wfm);
		int saveLink(const char *filename);
		
		short grn() { return _grn; }
		int trn() { return _trn; }
		
		double& Ggridx(int group, int number) { return _grid[_gindex[group][number]].x; }
		double& Ggridy(int group, int number) { return _grid[_gindex[group][number]].y; }
		double& Ggridz(int group, int number) { return _grid[_gindex[group][number]].z; }
		double* Ggridv(int group, int number) { return (double *)&_grid[_gindex[group][number]]; }
		
		short S2Ggridgn(int number) { return _grid[number].gn; }
		short S2Ggridpn(int number) { return _grid[number].pn; }
		
		short G2Sgrid(int group, int number) { return _gindex[group][number]; }
		
		double& Sgridx(int number) { return _grid[number].x; }
		double& Sgridy(int number) { return _grid[number].y; }
		double& Sgridz(int number) { return _grid[number].z; }
		double* Sgridv(int number) { return (double *)&_grid[number]; }
		
		short S2Glinkgn(int number) { return _link[number].gn; }
		short S2Glinkpn(int number) { return _link[number].pn; }
		
		// group, number in group -> link serial number
		short G2Slink(int group, int number) { return _linkindex[group][number]; }
		
		short GL2G1gn(int group, int number) { return _link[_linkindex[group][number]].g1.g; }
		short GL2G1pn(int group, int number) { return _link[_linkindex[group][number]].g1.p; }
		short GL2G1sn(int group, int number) { return _link[_linkindex[group][number]].g1.sn; }
		
		short GL2G2gn(int group, int number) { return _link[_linkindex[group][number]].g2.g; }
		short GL2G2pn(int group, int number) { return _link[_linkindex[group][number]].g2.p; }
		short GL2G2sn(int group, int number) { return _link[_linkindex[group][number]].g2.sn; }
		
		short GL2G3gn(int group, int number) { return _link[_linkindex[group][number]].g3.g; }
		short GL2G3pn(int group, int number) { return _link[_linkindex[group][number]].g3.p; }
		short GL2G3sn(int group, int number) { return _link[_linkindex[group][number]].g3.sn; }
		
		short SL2G1gn(int number) { return _link[number].g1.g; }
		short SL2G1pn(int number) { return _link[number].g1.p; }
		short SL2G1sn(int number) { return _link[number].g1.sn; }
		
		short SL2G2gn(int number) { return _link[number].g2.g; }
		short SL2G2pn(int number) { return _link[number].g2.p; }
		short SL2G2sn(int number) { return _link[number].g2.sn; }
		
		short SL2G3gn(int number) { return _link[number].g3.g; }
		short SL2G3pn(int number) { return _link[number].g3.p; }
		short SL2G3sn(int number) { return _link[number].g3.sn; }
		
		double* GL2G1v(int group, int number) { return (double*)&_grid[_link[_linkindex[group][number]].g1.sn]; }
		double* GL2G2v(int group, int number) { return (double*)&_grid[_link[_linkindex[group][number]].g2.sn]; }
		double* GL2G3v(int group, int number) { return (double*)&_grid[_link[_linkindex[group][number]].g3.sn]; }
		
		double* SL2G1v(int number) { return (double*)&_grid[_link[number].g1.sn]; }
		double* SL2G2v(int number) { return (double*)&_grid[_link[number].g2.sn]; }
		double* SL2G3v(int number) { return (double*)&_grid[_link[number].g3.sn]; }
		
		double& GL2G1x(int group, int number) { return _grid[_link[_linkindex[group][number]].g1.sn].x; }
		double& GL2G2x(int group, int number) { return _grid[_link[_linkindex[group][number]].g2.sn].x; }
		double& GL2G3x(int group, int number) { return _grid[_link[_linkindex[group][number]].g3.sn].x; }
		
		double& SL2G1x(int number) { return _grid[_link[number].g1.sn].x; }
		double& SL2G2x(int number) { return _grid[_link[number].g2.sn].x; }
		double& SL2G3x(int number) { return _grid[_link[number].g3.sn].x; }
		
		double& GL2G1y(int group, int number) { return _grid[_link[_linkindex[group][number]].g1.sn].y; }
		double& GL2G2y(int group, int number) { return _grid[_link[_linkindex[group][number]].g2.sn].y; }
		double& GL2G3y(int group, int number) { return _grid[_link[_linkindex[group][number]].g3.sn].y; }
		
		double& SL2G1y(int number) { return _grid[_link[number].g1.sn].y; }
		double& SL2G2y(int number) { return _grid[_link[number].g2.sn].y; }
		double& SL2G3y(int number) { return _grid[_link[number].g3.sn].y; }
		
		double& GL2G1z(int group, int number) { return _grid[_link[_linkindex[group][number]].g1.sn].z; }
		double& GL2G2z(int group, int number) { return _grid[_link[_linkindex[group][number]].g2.sn].z; }
		double& GL2G3z(int group, int number) { return _grid[_link[_linkindex[group][number]].g3.sn].z; }
		
		double& SL2G1z(int number) { return _grid[_link[number].g1.sn].z; }
		double& SL2G2z(int number) { return _grid[_link[number].g2.sn].z; }
		double& SL2G3z(int number) { return _grid[_link[number].g3.sn].z; }
		
		bool success() { return m_bSuccess; }
};

#endif	// __WFM_H__
