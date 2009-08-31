/*+++
Wfm.cpp
Copyright (c) 2000, Junichi MUTO.
Modified 2000, Takafumi MISAWA.
Faculty of Engineering, Seikei University. Morishima Laboratory.
ALL RIGHTS RESERVED.
---*/

#include "Wfm.h"
#include <stdio.h>
#include <string.h>

Wfm::Wfm()
{
	m_bCylinder = false;
	m_bSuccess = true;
	_grid = NULL;
	_link = NULL;
	for(int i = 0; i < GroupMax; i++){
		_gindex[i] = NULL;
		_gindexmax[i] = 0;
	}
	_grn = 0;
	_trn = 0;
}

Wfm::Wfm(const char *filename)
{
	m_bCylinder = false;
	m_bSuccess = true;
	_grid = NULL;
	for(int i = 0; i < GroupMax; i++){
		_gindex[i] = NULL;
		_gindexmax[i] = 0;
	}
	_grn = 0;
	
	if(!load(filename)) { m_bSuccess = false; }
}

Wfm::~Wfm()
{
	if( _grid != NULL ) delete [] _grid;
	if( _link != NULL ) delete [] _link;

	for(int i = 0; i < GroupMax; i++) {
		if( _gindex[i] != NULL ) delete [] _gindex[i];
		if( _linkindex[i] != NULL ) delete [] _linkindex[i];
	}

	
}

int Wfm::save(const char *filename)
{
	if(!_grn) return false;
	
	FILE *fp;
	
	if(NULL == (fp = fopen(filename , "w"))) return false;
	fprintf(fp, "#%d\n", _grn + 1);
	
	for(int i = 1 ; i <= _grn ; i++)
		fprintf(fp , "%3d %3d %3d %14.10lf %14.10lf %14.10lf\n" ,
		i , _grid[i].gn , _grid[i].pn , _grid[i].x , _grid[i].y , _grid[i].z);
	
	fclose(fp);
	return true;
}

int Wfm::saveDiff(const char *filename, Wfm *target_wfm)
{
	if(!_grn) return false;
	
	FILE *fp;
	
	if(NULL == (fp = fopen(filename , "w"))) return false;
	fprintf(fp, "#%d\n", _grn + 1);
	
	for(int i = 1 ; i <= _grn ; i++)
		fprintf(fp , "%3d %3d %3d %14.10lf %14.10lf %14.10lf\n" ,
		i, _grid[i].gn, _grid[i].pn,
		target_wfm->_grid[i].x - _grid[i].x,
		target_wfm->_grid[i].y - _grid[i].y,
		target_wfm->_grid[i].z - _grid[i].z);
	
	fclose(fp);
	return true;
}

int Wfm::saveLink(const char *filename)
{
	if(!_trn) return false;
	
	FILE		*fp;
	if(NULL == (fp = fopen(filename, "w"))) return false;
	fprintf(fp, "#%d\n", _trn + 1);
	
	for(int i = 1 ; i <= _trn ; i++ ){
		fprintf(fp , "%3d %3d %3d %3d %3d %3d %3d %3d %3d\n" ,
			i , _link[i].gn , _link[i].pn , 
			_link[i].g1.g , _link[i].g1.p ,
			_link[i].g2.g , _link[i].g2.p , 
			_link[i].g3.g , _link[i].g3.p);
	}
	fclose(fp);
	return true;
}

int Wfm::loadLink(const char *filename)
{
	if(_trn) return true;
	
	FILE *fp;
	char dummy;
	int i, j;
	
	if(NULL == (fp = fopen(filename, "r"))) return false;
	fscanf(fp, "%c%d", &dummy, &_trn);
	
	if(_trn == 0) return false;
	_link = new LINK[_trn];
	int tn, gn, gn_now = 1, pn, l1g, l1p, l2g, l2p, l3g, l3p;
	
	for(i = 1; i < _trn; i++){
		fscanf(fp , "%d %d %d %d %d %d %d %d %d" , &tn , &gn , &pn , 
			&l1g , &l1p , &l2g , &l2p , &l3g , &l3p);
		_link[i].g1.g = l1g;
		_link[i].g1.p = l1p;
		_link[i].g2.g = l2g;
		_link[i].g2.p = l2p;
		_link[i].g3.g = l3g;
		_link[i].g3.p = l3p;
		_link[i].gn = gn;
		_link[i].pn = pn;
		if(gn != gn_now){
			_linkindex[gn_now] = new short[_link[i-1].pn + 1];
			for(j = 1; j <= _link[i-1].pn; j++)
				_linkindex[gn_now][j] = (i - 1) - _link[i-1].pn + j;
			gn_now = gn;
		}
	}
	_linkindex[gn_now] = new short[_link[i-1].pn + 1];
	for(j = 1; j <= _link[i-1].pn; j++)
		_linkindex[gn_now][j] = (i - 1) - _link[i-1].pn + j;
	fclose(fp);
	_trn--;
	
	return true;
}

int Wfm::setLink(void)
{
	if(!_trn || !_grn) return false;
	
	for(int i = 1 ; i <= _trn ; i++){
		_link[i].g1.sn = _gindex[_link[i].g1.g][_link[i].g1.p];
		_link[i].g2.sn = _gindex[_link[i].g2.g][_link[i].g2.p];
		_link[i].g3.sn = _gindex[_link[i].g3.g][_link[i].g3.p];
	}
	return true;
}

void Wfm::deleteLink()
{
	delete [] _link;
	for(int i = 0; i < GroupMax; i++) {
		delete [] _linkindex[i];
		_linkindex[i] = NULL;
	}
	_link = NULL;
	_trn = 0;
}

void Wfm::MoveToZero()
{
	int i;
	
	double xmax = -100.0, ymax = -100.0, zmax = -100.0;
	double xmin  = 100.0, ymin = 100.0, zmin = 100.0;
	double xcenter, ycenter, zcenter;
	for(i = 1; i < _grn; i++) {
		if(xmax < _grid[i].x) xmax = _grid[i].x;
		if(ymax < _grid[i].y) ymax = _grid[i].y;
		if(zmax < _grid[i].z) zmax = _grid[i].z;
		if(xmin > _grid[i].x) xmin = _grid[i].x;
		if(ymin > _grid[i].y) ymin = _grid[i].y;
		if(zmin > _grid[i].z) zmin = _grid[i].z;
	}
	xcenter = (xmax + xmin) / 2.0;
	ycenter = (ymax + ymin) / 2.0;
	zcenter = (zmax + zmin) / 2.0;
	
	for(i = 1; i < _grn; i++) {
		_grid[i].x -= xcenter;
		_grid[i].y -= ycenter;
		_grid[i].z -= zcenter;
	}
}

int Wfm::load(const char *filename)
{
	FILE *fp;
	int i, j;
	
	if(NULL == (fp = fopen(filename, "r"))) { m_bSuccess = false;	return false; }
	
	int tn, gn, gn_now = 1, pn;
	double x, y, z;
	char dummy;
	
	fscanf(fp, "%c%hd", &dummy, &_grn);
	if(!_grn) return false;
	_grid = new XYZ[_grn];
	
	for(i = 1; i < _grn; i++){
		fscanf( fp , "%d %d %d %lf %lf %lf" , &tn , &gn , &pn , &x , &y , &z );
		_grid[i].x = x;
		_grid[i].y = y;
		_grid[i].z = z;
		_grid[i].gn = gn;
		_grid[i].pn = pn;
		if(gn != gn_now){
			_gindexmax[gn_now] = _grid[i-1].pn;
			_gindex[gn_now] = new short[_grid[i-1].pn + 1];
			for(j = 1; j <= _grid[i-1].pn; j++){
				_gindex[gn_now][j] = (i - 1) - _grid[i-1].pn + j;
			}
			gn_now = gn;
		}
	}
	_gindexmax[gn] = _grid[i-1].pn;
	_gindex[gn] = new short[_grid[i-1].pn + 1];
	for(j = 1; j <= _grid[i-1].pn; j++)
		_gindex[gn][j] = (i - 1) - _grid[i-1].pn + j;
	fclose(fp);
	_grn--;
	
	return true;
}

Wfm::Wfm(Wfm& obj)
{
	_grn = obj._grn;
	_grid = new XYZ[_grn+1];
	memcpy(_grid, obj._grid, sizeof(XYZ)*(_grn+1));
	
	for(int i = 1; i <= GroupMax; i++){
		_gindexmax[i] = obj._gindexmax[i];
		if(_gindexmax[i]){
			_gindex[i] = new short[_gindexmax[i]];
			for(short j = 0; j < _gindexmax[i]; j++) _gindex[i][j] = obj._gindex[i][j];
		}
		else _gindex[i] = NULL;
	}
}

Wfm& Wfm::operator=(Wfm& obj)
{
	_grn = obj._grn;
	if(!_grid) _grid = new XYZ[_grn+1];
	memcpy(_grid, obj._grid, sizeof(XYZ)*(_grn+1));
	
	for(int i = 1; i < GroupMax; i++) {
		if(_gindexmax[i] != obj._gindexmax[i]) {
			delete [] _gindex[i];
			if(obj._gindexmax[i]){
				_gindexmax[i] = obj._gindexmax[i];
				_gindex[i] = new short[_gindexmax[i]];
				for(short j = 0; j < _gindexmax[i]; j++) _gindex[i][j] = obj._gindex[i][j];
			}
			else _gindex[i] = NULL;
		}
	}
	return *this;
}

void Wfm::copyGrn(Wfm& obj)
{
	memcpy(_grid, obj._grid, sizeof(XYZ)*(_grn+1));
}


Wfm& Wfm::cylinder(double dR)
{
	double x, y, z;
	if(dR == 0.0) {	// Decide dR with calcuration.
		double xmax = 0.0, ymax = 0.0, zmax = 0.0, xmin = 100.0, ymin = 100.0, zmin = 100.0;
		for(int i = 1; i <= _grn; i++) {
			x = _grid[i].x;	if(x > xmax) xmax = x;	else if(x < xmin) xmin = x;
			y = _grid[i].y;	if(y > ymax) ymax = y;	else if(y < ymin) ymin = y;
			z = _grid[i].z;	if(z > zmax) zmax = z;	else if(z < zmin) zmin = z;
		}
		x = xmax - xmin;	y = ymax - ymin;	z = zmax - zmin;
		if(x > z) dR = x / 2.0 + 0.10;	else dR = z / 2.0 + 0.10;	// ratio of horizontal and vertical = arbitary
		dR = y / (2.0 * M_PI);	// ratio of horizontal and vertical = 1 : 1
	}
	m_dRadius = dR;
	
	for(int i = 1; i <= _grn; i++) {
		x = _grid[i].x;
		z = _grid[i].z;
		if(x >= 0.0 && z >= 0.0) {			// Area No.1
			_grid[i].x = dR / sqrt(1.0 + SQR(z / x));
			_grid[i].z = dR / sqrt(1.0 + SQR(x / z));
		} else if(x < 0.0 && z >= 0.0) {	// Area No.2
			_grid[i].x = -dR / sqrt(1.0 + SQR(z / x));
			_grid[i].z = dR / sqrt(1.0 + SQR(x / z));
		} else if(x < 0.0 && z < 0.0) {		// Area No.3
			_grid[i].x = -dR / sqrt(1.0 + SQR(z / x));
			_grid[i].z = -dR / sqrt(1.0 + SQR(x / z));
		} else if(x >= 0.0 && z < 0.0) {	// Area No.4
			_grid[i].x = dR / sqrt(1.0 + SQR(z / x));
			_grid[i].z = -dR / sqrt(1.0 + SQR(x / z));
		}
	}
	
	m_bCylinder = true;
	return *this;
}

Wfm& Wfm::flat()
{
	int i;
	if(m_bCylinder) {
		double x, z, dist1, dist2, angle, dist;
		for(i = 1; i <= _grn; i++) {
			x = _grid[i].x;
			z = _grid[i].z;
			dist1 = sqrt(SQR(x) + SQR(z - m_dRadius)) / 2.0;
			dist2 = sqrt(SQR(m_dRadius) - SQR(dist1));
			angle = 2.0 * acos(dist2 / m_dRadius);	// radian
			angle *= 360.0 / (2.0 * M_PI);			// degree
			dist = 2.0 * M_PI * m_dRadius * angle / 360.0;
			if(x >= 0.0 && z >= 0.0) _grid[i].x = dist;			// Area No.1
			else if(x < 0.0 && z >= 0.0) _grid[i].x = -dist;	// Area No.2
			else if(x < 0.0 && z < 0.0) _grid[i].x = -dist;		// Area No.3
			else if(x >= 0.0 && z < 0.0) _grid[i].x = dist;		// Area No.4
			_grid[i].z = 0.0;
		}
	}
	
	return *this;
}

Wfm& Wfm::normal()
{
	int i;
	
	XYZ *xyz = new XYZ[_grn + 1];
	for(i = 0; i <= _grn; i++) xyz[i].x = xyz[i].y = xyz[i].z = 0.0;
	int from, to1, to2;
	XYZ v1, v2, v;
	for(i = 1; i <= _trn; i++) {
		from = _link[i].g1.sn;	// serial number of vertex 1 using serial number of link. = SL2G1sn(i)
		to1 = _link[i].g2.sn;
		to2 = _link[i].g3.sn;
		v1 = _grid[to1] - _grid[from];
		v2 = _grid[to2] - _grid[from];
		v = v1 * v2;	// calculate outer product
		xyz[from] += v;
		xyz[to1] += v;
		xyz[to2] += v;
	}
	double len;
	for(i = 1; i <= _grn; i++) {
		len = xyz[i].length();
		if(len != 0.0) _grid[i] = xyz[i] / len;
		else _grid[i] = 0.0;
	}
	
	delete [] xyz;
	return *this;
}

const PointNumber Wfm::EyeHole[40][3] = {
	// Left eye top
	{{4, 23}, {20, 24}, {20, 25}},
	{{20, 25}, {4, 22}, {4, 23}},
	{{20, 25}, {4, 21}, {4, 22}},
	{{4, 21}, {20, 25}, {20, 18}},
	{{4, 21}, {20, 18}, {4, 20}},
	{{20, 18}, {4, 19}, {4, 20}},
	{{4, 19}, {20, 18}, {20, 19}},
	{{20, 19}, {4, 18}, {4, 19}},
	{{4, 18}, {20, 19}, {20, 20}},
	{{4, 17}, {4, 18}, {20, 20}},
	// Left eye bottom
	{{20, 24}, {4, 23}, {20, 23}},
	{{20, 23}, {4, 23}, {4, 28}},
	{{20, 23}, {4, 28}, {4, 27}},
	{{4, 27}, {20, 22}, {20, 23}},
	{{20, 22}, {4, 27}, {4, 26}},
	{{20, 22}, {4, 26}, {4, 25}},
	{{4, 25}, {20, 21}, {20, 22}},
	{{20, 21}, {4, 25}, {4, 24}},
	{{20, 21}, {4, 24}, {4, 17}},
	{{4, 17}, {20, 20}, {20, 21}},
	// Right eye top
	{{5, 17}, {21, 24}, {21, 25}},
	{{21, 25}, {5, 18}, {5, 17}},
	{{21, 25}, {5, 19}, {5, 18}},
	{{5, 19}, {21, 25}, {21, 18}},
	{{5, 19}, {21, 18}, {5, 20}},
	{{21, 18}, {5, 21}, {5, 20}},
	{{5, 21}, {21, 18}, {21, 19}},
	{{21, 19}, {5, 22}, {5, 21}},
	{{5, 22}, {21, 19}, {21, 20}},
	{{5, 23}, {5, 22}, {21, 20}},
	// Right eye bottom
	{{21, 24}, {5, 17}, {21, 23}},
	{{21, 23}, {5, 17}, {5, 24}},
	{{21, 23}, {5, 24}, {5, 25}},
	{{5, 25}, {21, 22}, {21, 23}},
	{{21, 22}, {5, 25}, {5, 26}},
	{{21, 22}, {5, 26}, {5, 27}},
	{{5, 27}, {21, 21}, {21, 22}},
	{{21, 21}, {5, 27}, {5, 28}},
	{{21, 21}, {5, 28}, {5, 23}},
	{{5, 23}, {21, 20}, {21, 21}}
};
