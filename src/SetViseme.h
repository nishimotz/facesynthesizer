/*+++
SetViseme.h
音素からVisemeへ変換 - Only Japanese Version
---*/

#ifndef __SET_VISEME_H__
#define __SET_VISEME_H__


#ifdef WIN32
#pragma warning (disable: 4786)
#endif


// #define DEBUG_PHONEME_TO_VISEME 1

#include <iostream>
#include <vector>
#include <string>

//#define LINEAR 1

using namespace std;

class SetViseme {
	
private:
	vector<string>  m_vsSrcName;  // 検索元: PhonemeName
	vector<int> m_viSrcNo;        //         VisemeNo.
	
	vector<int> m_viRstNo;        // 結果：VisemeNo.
	vector<int> m_viRstDur;       //       Duration
	
	vector< vector<float> >  m_vdTable; // Table生成 1freme/msec
	vector<float> m_vdTemp1;
	vector<float> m_vdTemp2;
	
	int m_dTotalTime;
	
	int m_iMouthScale;           // MouthScale 口の大きさの定義
	
	bool ReadViseme();
	void AddKeyFrame();
	void MakeTable();
	void LinearMove();
	void SinMove();
	
	string m_sVisemeFile;

public:
	// public constructor
	SetViseme(int mouthScale, string &visemeFile);
	
	// public destructor
	~SetViseme();
	
	// process function
	void PhonemeToViseme( vector<string> phoneme, vector<int> duration, int speakOffset );
	void GetFrame( unsigned int time, float *vm_number );
	
	void SetMouthScale(int i) { m_iMouthScale = i; }
	
	
};

#endif	//	__SET_VISEME_H__


