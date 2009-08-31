#include "UnitMove.h"

#include <string>
#include <stdlib.h>

UnitMove::UnitMove(int max_num, char *dirname, char **filename)
{
	m_MaxNum = max_num;
	m_pDirName = dirname;
	m_pFileName = filename;

	m_pStdWfm = NULL;
	m_pOrgWfm = NULL;
	m_pWorkWfm = NULL;

	for( int i = 0; i < MAX_NUM; i++ ) {
		m_pUnitData[i] = NULL;
	}
	OpenUnitData();
}

UnitMove::~UnitMove()
{
	for(int i = 0; i < m_MaxNum; i++) delete m_pUnitData[i];
}

void UnitMove::OpenUnitData()
{
	string s_dirName = m_pDirName;
	string s_fullFileName;

	for( int i = 0; i < m_MaxNum; i++ ) {
		m_pUnitData[i] = new Wfm;

		s_fullFileName  = s_dirName + m_pFileName[i];

		if(!m_pUnitData[i]->load(s_fullFileName.c_str())) {
			cout << "### Can't Read " << s_fullFileName << " File. ###" << endl;
			exit(1);
		}
	}
}

void UnitMove::SetWfm(Wfm *stdWfm, Wfm *orgWfm, Wfm *workWfm)
{
	m_pStdWfm = stdWfm;
	m_pOrgWfm = orgWfm;
	m_pWorkWfm = workWfm;
}

void UnitMove::ResetWfm()
{
	*m_pWorkWfm = *m_pOrgWfm;
}

void UnitMove::UnitMoving(float *unitParam)
{
	int i;
	
	for(i = 0; i < m_MaxNum; i++) m_UnitParam[i] = unitParam[i];
	
	for(i = 0; i < m_MaxNum; i++) {
		if(m_UnitParam[i] != 0.0) {
			for(int j = 1; j <= m_pWorkWfm->grn(); j++) {
				m_pWorkWfm->Sgridx(j) += m_pUnitData[i]->Sgridx(j) * (float)m_UnitParam[i];
				m_pWorkWfm->Sgridy(j) += m_pUnitData[i]->Sgridy(j) * (float)m_UnitParam[i];
				m_pWorkWfm->Sgridz(j) += m_pUnitData[i]->Sgridz(j) * (float)m_UnitParam[i];
			}
		}
	}
}

