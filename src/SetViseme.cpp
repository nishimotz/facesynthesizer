/*+++
SetViseme.c++
---*/
#include <stdlib.h>

#include "SetViseme.h"

#include "trace.h"
#include "Resources.h"
#include "costable.h"

SetViseme::SetViseme(int mouthScale, string &visemeFile)
{
	TRACE("SetViseme::SetViseme()");
	
	m_iMouthScale = mouthScale;
	m_sVisemeFile = visemeFile;

	if( !ReadViseme() ) exit(1);
	
	m_viRstNo.reserve( 4000 );
	m_viRstDur.reserve( 4000 );
	
	
	m_vdTable.resize( MAX_VM_NUMBER );
	
	for( int i = 0; i < MAX_VM_NUMBER; i++ ) {  // TableÇæÇØÇÃà◊ñÒÇRÇlÇ∆ÇÈÅ@Ç±ÅAÇ±ÇÌÇ¢
		m_vdTable[i].reserve( 60000 );
	}
	
	m_vdTemp1.resize( MAX_VM_NUMBER );
	m_vdTemp2.resize( MAX_VM_NUMBER );
	
	m_dTotalTime = 0;
	
}

SetViseme::~SetViseme()
{
	TRACE("SetViseme::~SetViseme()");
}

bool SetViseme::ReadViseme()
{
	TRACE("bool SetViseme::ReadViseme()");
	
	int temp_int;
	char temp_str[256];
	
	FILE *fp;
	
	if(( fp = fopen(m_sVisemeFile.c_str(), "r" )) == NULL ) {
		cout << "Cannot open viseme file: " << m_sVisemeFile << endl;
		return false;
	}
	
	while(!feof(fp)) {
		
		fscanf(fp, "%s\t%d", temp_str, &temp_int );
		
		m_vsSrcName.push_back( temp_str );
		m_viSrcNo.push_back( temp_int );
		
#ifdef DEBUG_PHONEME_TO_VISEME
		fprintf(stderr, "DEBUG: %s, %d\n", temp_str, temp_int );
#endif
		
	}
	
	fclose(fp);
	return true;
}


void SetViseme::PhonemeToViseme( vector<string> phoneme, vector<int> duration, int speakOffset )
{
	TRACE("SetViseme::PhonemeToViseme( vector<string> &phoneme, vector<int> &duration )");
	
	m_viRstNo.clear();
	m_viRstDur.clear();
	
	int k = 0; //key-frame number
	
	// First step: 1ms ÇÃñ≥âπãÊä‘Çê∂ê¨
	m_viRstNo.push_back(0);
	//m_viRstDur.push_back(int(1000.0 / 30) * 3 + 1);
	m_viRstDur.push_back(1 + speakOffset);
	
#ifdef DEBUG_PHONEME_TO_VISEME
	fprintf(stderr, "Line No.00: --- First Silence ---  ...Viseme No.00\t%d [ms]\n", m_viRstDur[0]);
#endif
	
	k++;
	// VISEME No.Ç∆DulationÇÃåàíËÅ@ëSåüçı:óvâ¸ëP
	unsigned int i, j;
	
	for( i = 0; i < phoneme.size(); i++ ) {   
		for( j = 0; j < m_vsSrcName.size(); j++ ) {
			if( phoneme[i] == m_vsSrcName[j] ) {   	// íTçıê¨å˜
				m_viRstNo.push_back( m_viSrcNo[j] );
				m_viRstDur.push_back( duration[i] );
#ifdef DEBUG_PHONEME_TO_VISEME
				fprintf( stderr, "Line No.%02d: [%s] ... Viseme No.%d:\t%d[ms]\n", 
					i+1, phoneme[i].c_str(), m_viRstNo[k], m_viRstDur[k] );
#endif
				k++;
				break;
			} 
		}
		if( j == m_vsSrcName.size() ) {           // íTçıé∏îs    
			m_viRstNo.push_back(0);
			m_viRstDur.push_back( duration[i] );
#ifdef DEBUG_PHONEME_TO_VISEME
			fprintf(stderr, "Line No.%02d : [%s] ... ?No Match? ... Viseme No.00\t%d[ms]\n",
				i+1, m_viRstNo[k], m_viRstDur[k]);
#endif
			k++;
		}
		
	}
	
	// for Final Step: 1ms ÇÃñ≥âπãÊä‘Çê∂ê¨
	m_viRstNo.push_back(0);
	m_viRstDur.push_back(1);
#ifdef DEBUG_PHONEME_TO_VISEME
	fprintf(stderr, "Line No.%02d: --- Final Silence ---  ...Viseme No.00\t%d [ms]\n",
		m_viRstDur.size() - 1, m_viRstDur[m_viRstDur.size() - 1]);
#endif
	
	AddKeyFrame();
	MakeTable();
}

void SetViseme::AddKeyFrame() 
{
	int i, j;
	int num = m_viRstNo.size();
	j = 0;
	for( i = 0; i < num; i++ ) {
		vector<int>::iterator pRstNo = m_viRstNo.begin();
		vector<int>::iterator pRstDur = m_viRstDur.begin();
		if( m_viRstDur[j + i] >= 200 ) {
			m_viRstDur[j + i] = m_viRstDur[j + i] - 92;
			pRstNo = pRstNo + j + i + 1;
			pRstDur = pRstDur + j + i + 1;
			
			m_viRstNo.insert( pRstNo, m_viRstNo[j + i] );
			m_viRstDur.insert( pRstDur, 92 );
			
#ifdef DEBUG_PHONEME_TO_VISEME
			fprintf( stderr, "insert Line No.%02d: ... Viseme No.%d:\t%d[ms]\n", 
				i+j+1, m_viRstNo[i+j +1], m_viRstDur[i+j+1] );
#endif
			j++;
		}
	}
}

void SetViseme::MakeTable() 
{
	int i;
	
	// Clear Table
	for( i = 0; i < MAX_VM_NUMBER; i++ ) {
		m_vdTable[i].clear();
	}  
	m_vdTable.resize( MAX_VM_NUMBER );
	m_dTotalTime = 0;
	
#ifdef LINEAR
	LinearMove();
#else
	SinMove();
#endif
	
	m_dTotalTime = m_vdTable[0].size();
}

void SetViseme::SinMove() 
{
	unsigned int i;
	int j, k;
	double value;
	int table;
	
	for( i = 0; i < m_viRstNo.size(); i++ ) {
		// keyFrameÇÃê∂ê¨
		for( j = 0; j < MAX_VM_NUMBER; j++ ) {
			m_vdTemp1[j] = 0.0;
			m_vdTemp2[j] = 0.0;
		}
		
		if( m_viRstNo[i] != 0 )
			m_vdTemp1[m_viRstNo[i] - 1] = (float)(m_iMouthScale / 100.0);
		if( i != (m_viRstNo.size() - 1) ) {
			if( m_viRstNo[i + 1] != 0 )
				m_vdTemp2[m_viRstNo[i + 1] - 1] = (float)(m_iMouthScale / 100.0);
		}

		// Ç±Ç±Ç©ÇÁTableÇÇ¬Ç≠ÇÈ
		for( j = 0; j < MAX_VM_NUMBER; j++ ) {
			for( k = 0; k < m_viRstDur[i]; k++ ) {
				if( m_vdTemp1[j] < m_vdTemp2[j] ) {
					table = (int)(k * 1000 / m_viRstDur[i]);
					value = m_vdTemp1[j] + (m_vdTemp2[j] - m_vdTemp1[j]) * costable[table];

				} else if( m_vdTemp1[j] > m_vdTemp2[j] ) {
					table = 1000 - (int)(k * 1000/ m_viRstDur[i]);
					value = m_vdTemp2[j] + (m_vdTemp1[j] - m_vdTemp2[j]) * costable[table];
					
				} else {
					value = m_vdTemp1[j];
				}
				m_vdTable[j].push_back((float)value);
			}
		}
	}
	// Final Frame
	for( i = 0; i < MAX_VM_NUMBER; i++ ) {
		m_vdTable[i].push_back(0.0);
	}
	
}

void SetViseme::LinearMove() 
{
	unsigned int i;
	int j, k;
	double value;
	
	for( i = 0; i < m_viRstNo.size(); i++ ) {
		
		// keyFrameÇÃê∂ê¨
		for( j = 0; j < MAX_VM_NUMBER; j++ ) {
			m_vdTemp1[j] = 0.0;
			m_vdTemp2[j] = 0.0;
		}
		
		if( m_viRstNo[i] != 0 )
			m_vdTemp1[m_viRstNo[i] - 1] = (float)(m_iMouthScale / 100.0);
		if( i != (m_viRstNo.size() - 1)) {
			if( m_viRstNo[i + 1] != 0 )
				m_vdTemp2[m_viRstNo[i + 1] - 1] = (float)(m_iMouthScale / 100.0);
		}
		// Ç±Ç±Ç©ÇÁTableÇÇ¬Ç≠ÇÈ
		for( j = 0; j < MAX_VM_NUMBER; j++ ) {
			for( k = 0; k < m_viRstDur[i]; k++ ) {
				value = m_vdTemp1[j] + (m_vdTemp2[j] - m_vdTemp1[j]) / m_viRstDur[i] * k;
				m_vdTable[j].push_back((float)value);
			}
		}
	}
	// Final Frame
	for( i = 0; i < MAX_VM_NUMBER; i++ ) {
		m_vdTable[i].push_back(0.0);
	}
	
}

void SetViseme::GetFrame( unsigned int time, float *vm_number )
{
//	printf("total time[%d] = %d\n", time,m_vdTable[0].size());
	
	for( int i = 0; i < MAX_VM_NUMBER; i++ ) {
		vm_number[i] = m_vdTable[i][time];
	}
//	printf("%0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f\n",
//		vm_number[7], vm_number[8], vm_number[9], vm_number[10], vm_number[11],
//		vm_number[12], vm_number[13], vm_number[14], vm_number[15], vm_number[16]);
	
}


