/*++
ObjModel.cpp
Copyright (c) 2000, Takafumi MISAWA & Daisuke ITOH.
Copyright (c) 2001, teleboy
Faculty of Engineering, Seikei University. Morishima Laboratory.
All Rights Reserved.

  Last Modified: 2001/11/12
--*/

//	If your environment is VC++, your file will include 'stdafx.h'.

#ifdef WIN32
#include <windows.h>
#endif

#include "ObjModel.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <GL/gl.h>

//#ifndef _WINDOWS
//#include "trace.h"
//#endif // _WINDOWS

// constructor
ObjModel::ObjModel()
{
	// initialize
	m_number_v = m_number_vn = m_number_f = m_number_tv = 0;
	m_vertex = NULL;
	m_normal = NULL;
	m_number_f_ele = NULL;
	m_link = NULL;
	m_texture_ver = NULL;
	
	m_eolFlag = FALSE;
	
	m_vertex_index = NULL;
}

ObjModel::ObjModel(const char* filename)
{
	m_number_v = m_number_vn = m_number_f = m_number_tv = 0;
	m_vertex = NULL;
	m_normal = NULL;
	m_number_f_ele = NULL;
	m_link = NULL;
	m_texture_ver = NULL;
	
	m_eolFlag = FALSE;
	
	m_vertex_index = NULL;
	
	OpenObjectFile(filename);
}

// destructor
ObjModel::~ObjModel()
{
	if(m_vertex) delete [] m_vertex;
	if(m_normal) delete [] m_normal;
	if(m_texture_ver) delete [] m_texture_ver;
	if(m_number_f_ele) delete [] m_number_f_ele;
	if(m_link){
		for(int i = 0; i < m_number_f; i++)
			delete [] m_link[i].element;
		delete [] m_link;
	}
	if(m_vertex_index) delete [] m_vertex_index;
}

// ++ protected process function ++
BOOL ObjModel::GetFileData(FILE* fp, char* buf)
{
	int ch;
	char chOld = '\0';
	char* ptr = buf;
	char temp[1024];
	*ptr = '\0';
	
	while( (ch = fgetc(fp)) != EOF ){
		//		TRACE("%c", ch);
		if( isspace(ch) || strchr("#/\\", ch) ){
			if( (char)ch == '#' ){
				fgets(temp, 256, fp);
				//				TRACE("# detected! : %s", temp);
				continue;
			}
			if( (char)ch == '\n' && chOld != '\\' ) m_eolFlag = TRUE;
			else m_eolFlag = FALSE;
			
			*ptr = '\0';
			if( strcmp(buf, "\0") ) return TRUE;
			
			if( (char)ch == '/' ){
				strcpy(buf, "1");
				return TRUE;
			}
			
			ptr = buf;
		}
		else *ptr++ = ch;
		chOld = (char)ch;
	}
	
	return FALSE;
}

BOOL ObjModel::LoadObjectFile(const char* filename, int iFlag)
{
	// file check
	// underconstruction
	int i;
	//	TRACE("ObjModel::LoadObjectFile(), FileOpening\n");
	
	// file open
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL) return FALSE;
	
	//	TRACE("ObjModel::LoadObjectFile(), CountingComponent\n");
	
	// count the number of "v", "vn", "f" & "tv" lines
	char buf[1024];
	while(GetFileData(fp, buf)){
		//		TRACE("%s\n", buf);
		if(!strcmp(buf, "v")) m_number_v++;
		else if(!strcmp(buf, "f")) m_number_f++;
		else if(!strcmp(buf, "vn")) m_number_vn++;
		else if(!strcmp(buf, "tv") || !strcmp(buf, "vt")) m_number_tv++;
	}
	
	//	TRACE("ObjModel::LoadObjectFile(), MemoryAllocation\n");
	
	// memory allocation
	if(m_number_v){
		if((m_vertex = new VERTEX3D[m_number_v]) == NULL){
			fclose(fp);	return FALSE;
		}
	}
	if(m_number_f){
		if((m_link = new LINK[m_number_f]) == NULL){
			fclose(fp);	return FALSE;
		}
		m_link->element = NULL;
		if((m_number_f_ele = new int[m_number_f]) == NULL){
			fclose(fp);	return FALSE;
		}
	}
	if(m_number_vn){
		if((m_normal = new NORMAL3D[m_number_vn]) == NULL){
			fclose(fp);	return FALSE;
		}
	}
	if(m_number_tv){
		if((m_texture_ver = new TEXTURE3D[m_number_tv]) == NULL){
			fclose(fp);	return FALSE;
		}
	}
	
	//	TRACE("ObjModel::LoadObjectFile(), CountingLinkElements\n");
	
	// count the number of "f" elements
	BOOL f_flag = FALSE;
	int linkElementNum;
	int f_count = -1;
	fseek(fp, 0L, SEEK_SET);
	while(GetFileData(fp, buf)){
		if(!strcmp(buf, "f")){
			if(f_count >= 0) m_number_f_ele[f_count] = linkElementNum / 3;
			f_count++;
			f_flag = TRUE;
			linkElementNum = 0;
		} else if(f_flag) linkElementNum++;
		else f_flag = FALSE;
	}
	if((f_count + 1) == m_number_f) m_number_f_ele[f_count] = linkElementNum / 3;	// last line of "f"
	
	//	TRACE("ObjModel::LoadObjectFile(), LinkElementsMemoryAllocation\n");
	
	// link element memory allocation
	for(i = 0; i < m_number_f; i++){
		if((m_link[i].element = new LINK_ELEMENT[m_number_f_ele[i]]) == NULL){
			fclose(fp); return FALSE;
		}
	}
	
	//	TRACE("ObjModel::LoadObjectFile(), ReadingValuesFromFile\n");
	
	// input values from file to variance
	fseek(fp, 0L, SEEK_SET);
	f_count = 0;
	VERTEX3D* vertex = m_vertex;
	NORMAL3D* normal = m_normal;
	TEXTURE3D* texture = m_texture_ver;
	while(GetFileData(fp, buf)){
		if(!strcmp(buf, "v")){
			GetFileData(fp, buf);	m_vertex->x = (double)atof(buf);
			GetFileData(fp, buf);	m_vertex->y = (double)atof(buf);
			GetFileData(fp, buf);	m_vertex->z = (double)atof(buf);
			m_vertex++;
		} else if(!strcmp(buf, "vn")){
			GetFileData(fp, buf);	m_normal->x = (double)atof(buf);
			GetFileData(fp, buf);	m_normal->y = (double)atof(buf);
			GetFileData(fp, buf);	m_normal->z = (double)atof(buf);
			m_normal++;
		} else if(!strcmp(buf, "tv") || !strcmp(buf, "vt")){
			GetFileData(fp, buf);	m_texture_ver->x = (double)atof(buf);
			GetFileData(fp, buf);	m_texture_ver->y = (double)atof(buf);
			m_texture_ver++;
		} else if(!strcmp(buf, "f")){
			for(i = 0; i < m_number_f_ele[f_count]; i++){
				GetFileData(fp, buf);	m_link[f_count].element[i].v = atoi(buf) - 1;
				GetFileData(fp, buf);	m_link[f_count].element[i].t = atoi(buf) - 1;
				GetFileData(fp, buf);	m_link[f_count].element[i].n = atoi(buf) - 1;
			}
			f_count++;
		}
	}
	m_vertex = vertex;
	m_normal = normal;
	m_texture_ver = texture;
	
	fclose(fp);
	
	//	TRACE("ObjModel::LoadObjectFile(), FileClosed\n");
	
	return TRUE;
}
// -- protected process function --

// ++ public process function ++
void ObjModel::DrawObjModel()
{
	for(int i = 0; i < m_number_f; i++){
		glBegin(GL_POLYGON);
		for(int j = 0; j < m_number_f_ele[i]; j++){
			if( m_texture_ver ) 
				//glTexCoord3dv((GLdouble *)&m_texture_ver[m_link[i].element[j].v]);
				glTexCoord2d(m_texture_ver[m_link[i].element[j].v].x, m_texture_ver[m_link[i].element[j].v].y);
			if( m_normal ) 
				glNormal3dv((GLdouble *)&m_normal[m_link[i].element[j].n]);
			glVertex3dv((GLdouble *)&m_vertex[m_link[i].element[j].v]);
		}
		glEnd();
	}
}

void ObjModel::DrawWFM()
{
	for(int i = 0; i < m_number_f; i++){
		glBegin(GL_POLYGON);
		for(int j = 0; j < m_number_f_ele[i]; j++){
			glVertex3dv((GLdouble *)&m_vertex[m_link[i].element[j].v]);
		}
		glEnd();
	}
}

void ObjModel::ReCalcTexture(double scale)
{
	int i;
	double x, y, z;
	m_xmin = m_ymin = m_zmin = 1000.0;
	m_xmax = m_ymax = m_zmin = -1000.0;

	for(i = 0; i < m_number_v; i++ ) {
		x = m_vertex[i].x; 
		y = m_vertex[i].y;
		z = m_vertex[i].z;

		if(x < m_xmin) m_xmin = x; if(y < m_ymin) m_ymin = y; if(z < m_zmin) m_zmin = z;
		if(x > m_xmax) m_xmax = x; if(y > m_ymax) m_ymax = y; if(z > m_zmax) m_zmax = z;
	}

	for( i = 0; i < m_number_v; i++ ) {
		x = m_vertex[i].x; 
		y = m_vertex[i].y;
	
		m_texture_ver[i].x = ((scale * 2.0) / (m_xmax - m_xmin)) * x + (scale - (scale * 2.0) * m_xmax / (m_xmax - m_xmin));
		m_texture_ver[i].y = ((scale * 2.0) / (m_ymax - m_ymin)) * y + (scale - (scale * 2.0) * m_ymax / (m_ymax - m_ymin));
	}
}


BOOL ObjModel::CalcNormalVector()
{
	int i;
	if( m_normal == NULL ){
		m_number_vn = m_number_v;
		if((m_normal = new NORMAL3D[m_number_vn]) == NULL) return FALSE;
		else {
			for(int i = 0; i < m_number_vn; i++) m_normal[i].x = m_normal[i].y = m_normal[i].z = 0.0;
		}
	}
	
	VERTEX3D v1, v2, temp;
	int from, end;
	for(i = 0; i < m_number_f; i++) {
		from = m_link[i].element[0].v;
		end  = m_link[i].element[1].v;
		v1 = m_vertex[end] - m_vertex[from];
		end = m_link[i].element[2].v;
		v2 = m_vertex[end] - m_vertex[from];
		temp = v1 * v2;	// calculate outer product
		for(int j = 0; j < m_number_f_ele[i]; j++) m_normal[m_link[i].element[j].v] += temp;
	}
	for(i = 0; i < m_number_vn; i++){
		if(m_normal[i].length() != 0.0) m_normal[i] /= m_normal[i].length();
		else m_normal[i].x = m_normal[i].y = m_normal[i].z = 0.0;
	}
	
	return TRUE;
}

BOOL ObjModel::NormalizeObjModel()
{
	int i;
	if(m_vertex == NULL) return FALSE;
	
	// move to the object's center
	VERTEX3D center = {0.0, 0.0, 0.0};
	int counter = 0;
	for(i = 0; i < m_number_v; i++) {
		center += m_vertex[i];
		counter++;
	}
	center /= (double)counter;
	
	for(i = 0; i < m_number_v; i++) m_vertex[i] -= center;
	
	// normalize vertex
	double max = 0.0;
	for(i = 0; i < m_number_v; i++)
		if(m_vertex[i].length() > max) max = m_vertex[i].length();
		for(i = 0; i < m_number_v; i++) m_vertex[i] /= max * 0.8;
		
		return TRUE;
}
// -- public process function --


// for muscle project


void ObjModel::SaveObjModel(const char* filename)
{
	FILE *fp = fopen(filename, "w");
	
	if( m_normal == NULL ) CalcNormalVector();
	
	int i, j;
	for(i = 0; i < m_number_v; i++){
		fprintf(fp, "v %.6f %.6f %.6f\n", m_vertex[i].x, m_vertex[i].y, m_vertex[i].z);
	}
	fprintf(fp, "# %d vertices\n", m_number_v);
	fprintf(fp, "\n");
	
	for(i = 0; i < m_number_vn; i++){
		fprintf(fp, "vn %.6f %.6f %.6f\n", m_normal[i].x, m_normal[i].y, m_normal[i].z);
	}
	fprintf(fp, "# %d normals\n", m_number_vn);
	fprintf(fp, "\n");
	
#define SAVETEX	0
#if SAVETEX
	if( m_texture_ver ){
		for(i = 0; i < m_number_tv; i++){
			fprintf(fp, "tv %.6f %.6f\n", m_texture_ver[i].x, m_texture_ver[i].y);
		}
		fprintf(fp, "# %d texture vertices\n", m_number_tv);
		fprintf(fp, "\n");
	}
#endif
	
	for(i = 0; i < m_number_f; i++){
		fprintf(fp, "f");
		for(j = 0; j < m_number_f_ele[i]; j++){
#if SAVETEX
			if( m_texture_ver )
				fprintf(fp, " %d/%d/%d", m_link[i].element[j].v+1, m_link[i].element[j].t+1, m_link[i].element[j].v+1);
			else
#endif
				fprintf(fp, " %d//%d", m_link[i].element[j].v+1, m_link[i].element[j].v+1);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "# %d elements\n", m_number_f);
	fprintf(fp, "\n");
	
	fclose(fp);
}

ObjModel::ObjModel(const char* vertexfile, const char* linkfile)
{
	m_number_v = m_number_vn = m_number_f = m_number_tv = 0;
	m_vertex = NULL;
	m_normal = NULL;
	m_number_f_ele = NULL;
	m_link = NULL;
	m_texture_ver = NULL;
	
	m_eolFlag = FALSE;
	
	m_vertex_index = NULL;
	
	LoadUTFModel(vertexfile, linkfile);
}

ObjModel::ObjModel(ObjModel* pObj)
{
	m_number_v = pObj->GetVertexNum();
	m_number_vn = pObj->GetNormalNum();
	m_number_f = pObj->GetLinkNum();
	m_number_tv = pObj->GetTextureNum();
	m_number_f_ele = new int [ m_number_f ];
	for(int i = 0; i < m_number_f; i++ )
		m_number_f_ele[i] = pObj->GetLinkEleNum(i);
	
	if( m_number_v ){
		m_vertex = new VERTEX3D[m_number_v];
		for(int i = 0; i < m_number_v; i++)
			m_vertex[i] = pObj->GetVertex(i);
	}
	if( m_number_vn ){
		m_normal = new NORMAL3D[m_number_vn];
		for(int i = 0; i < m_number_vn; i++)
			m_normal[i] = pObj->GetNormal(i);
	}
	if( m_number_f ){
		m_link = new LINK[m_number_f];
		m_link->element = NULL;
		for(int i = 0; i < m_number_f; i++){
			m_link[i].element = new LINK_ELEMENT[m_number_f_ele[i]];
			LINK lnk = pObj->GetLink(i);
			for(int j = 0; j < m_number_f_ele[i]; j++){
				m_link[i].element[j].v = lnk.element[j].v;
				m_link[i].element[j].n = lnk.element[j].n;
				m_link[i].element[j].t = lnk.element[j].t;
			}
		}
	}
	if( m_number_tv ){
		m_texture_ver = new TEXTURE3D[m_number_tv];
		for(int i = 0; i < m_number_tv; i++)
			m_texture_ver[i] = pObj->GetTextureVer(i);
	}
}

BOOL ObjModel::LoadUTFModel(const char* vertexfile, const char* linkfile)
{
	//	TRACE("ObjModel::LoadUTFModel(), FileOpening\n");
	int i;
	ifstream fv(vertexfile);
	
	fv >> m_number_v;
	
	if(m_number_v){
		if((m_vertex = new VERTEX3D[m_number_v]) == NULL){
			return FALSE;
		}
	}
	
	m_vertex_index = new int [ m_number_v ];
	
	VERTEX3D* vertex = m_vertex;
	for(i = 0; i < m_number_v; i++){
		fv >> m_vertex_index[i];
		fv >> m_vertex->x;
		fv >> m_vertex->y;
		fv >> m_vertex->z;
		m_vertex++;
	}
	m_vertex = vertex;
	
	fv.close();
	
	ifstream fl(linkfile);
	
	int flag_num;
	fl >> m_number_f;
	fl >> flag_num;
	
	if(m_number_f){
		if((m_link = new LINK[m_number_f]) == NULL){
			return FALSE;
		}
		m_link->element = NULL;
		if((m_number_f_ele = new int[m_number_f]) == NULL){
			return FALSE;
		}
	}
	int j;
	for(i = 0; i < m_number_f; i++){
		m_number_f_ele[i] = 3;
		if((m_link[i].element = new LINK_ELEMENT[m_number_f_ele[i]]) == NULL){
			return FALSE;
		}
		int index;
		for(j = 0; j < m_number_f_ele[i]; j++){
			fl >> index;
			for(int k = 0; k < m_number_v; k++){
				if( m_vertex_index[k] == index ){
					m_link[i].element[j].v = k;
					break;
				}
			}
		}
		for(j = 0; j < flag_num; j++) fl >> index;
	}
	
	fl.close();
	
	//	TRACE("ObjModel::LoadUTFModel(), FileClosed\n");
	
	return TRUE;
}

