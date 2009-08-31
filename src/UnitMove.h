#ifndef __UNITMOVE_H__
#define __UNITMOVE_H__

#include <iostream>

#include "Wfm.h"

#define MAX_NUM 100

using namespace std;

class UnitMove {
private:
	Wfm *m_pUnitData[MAX_NUM];
	float m_UnitParam[MAX_NUM];

	Wfm *m_pStdWfm;		// Standard Wireframe Model ... Before fitting
	Wfm *m_pOrgWfm;		// Original Wireframe Model ... After fitting
	Wfm *m_pWorkWfm;	// for Move ... copy of _orgWfm
	
	int m_MaxNum;

	char *m_pDirName;
	char **m_pFileName;

	void OpenUnitData();

public:
	// public constructor
	UnitMove(int max_num, char *dirname, char **filename);
	
	// public destructor
	~UnitMove();
	
	void SetWfm(Wfm *stdWfm, Wfm *orgWfm, Wfm *workWfm);
	void ResetWfm();

	void UnitMoving(float *muParam);
	
	// Process function

};

#endif

