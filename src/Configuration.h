/*++
Cofiguration.h
--*/
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>

#include "Resources.h"

#define DEF_MASKFILE	"MaskFile"
#define DEF_MASKEYE		"MaskEye"
#define DEF_MASKTEETH	"MaskTeeth"
#define DEF_BGFILE		"BackgroundFile"
#define DEF_AUTONOMOUS_BLINK	"AutonomousBlink"
#define DEF_AUTONOMOUS_MOVE		"AutonomousMove"
#define DEF_AUTONOMOUS_USERDEF	"AutonomousUserdef"
#define DEF_FACEMOT_NOD		"FaceMotNod"
#define DEF_FACEMOT_LISTEN	"FaceMotListen"
#define DEF_FACEMOT_REFUSE	"FaceMotRefuse"
#define DEF_FACEMOT_USERDEF	"FaceMotUserDef"
#define DEF_FACEMOT_BLINK	"FaceMotBlink"
#define DEF_MAX_FRAME_RATE	"MaxFrameRate"
#define DEF_SET_VISEME		"SetViseme"

#define BLINK_STR		"set Face.30 = "

using namespace std;

const int DEFALUT_MAX_FPS = 30;

class Configuration { 
private:
	// ��摜
	vector<string> m_vsMaskName;    // ��摜�̖��O
	vector<string> m_vsMaskRgbFile; // ��摜��Image�t�@�C��
	vector<string> m_vsMaskPntFile; // ��摜��pnt�t�@�C��
	vector<string> m_vsMaskEyeName;			// ��摜�̖��O
	vector<string> m_vsMaskEyeImageFile;	// ��摜�̖�Image�t�@�C��
	vector<string> m_vsMaskEyeDefFile;		// ��摜�̖ڒ�`�t�@�C��
	vector<string> m_vsMaskTeethName;		// ��摜�̖��O
	vector<string> m_vsMaskTeethDefFile;	// ��摜�̎���`�t�@�C��

	// �w�i
	vector<string> m_vsBGName;    // �w�i�̖��O
	vector<string> m_vsBGRgbFile; // �w�i��Image�t�@�C��

	vector<string> m_vsToken;       // �g�[�N���Ǘ�
	
	// Autonomous�֘A
	// Autonomous MOVE ���̓���
	vector< vector<int> > m_viAutonomousMove_frame;
	vector< vector<float> > m_vfAutonomousMove_x;
	vector< vector<float> > m_vfAutonomousMove_y;
	vector< vector<float> > m_vfAutonomousMove_z;
	int m_iAutonomousMove;

	// Autonomous MOVE ���[�U��`�i�����̓����j
	vector< vector<int> > m_viAutonomousUserDef_frame;
	vector< vector<float> > m_vfAutonomousUserDef_x;
	vector< vector<float> > m_vfAutonomousUserDef_y;
	vector< vector<float> > m_vfAutonomousUserDef_z;
	int m_iAutonomousUserDef;

	// Autonomous Blink �u��
	vector<int> m_viAutonomousBlink;
	vector<int> m_viAutonomousBlinkPattern;

	// FaceMot�֘A
	vector< vector<int> > m_viFaceMotNod_frame;
	vector< vector<float> > m_vfFaceMotNod_x;
	vector< vector<float> > m_vfFaceMotNod_y;
	vector< vector<float> > m_vfFaceMotNod_z;
	int m_iFaceMotNod;
	vector< vector<int> > m_viFaceMotRefuse_frame;
	vector< vector<float> > m_vfFaceMotRefuse_x;
	vector< vector<float> > m_vfFaceMotRefuse_y;
	vector< vector<float> > m_vfFaceMotRefuse_z;
	int m_iFaceMotRefuse;
	vector< vector<int> > m_viFaceMotListen_frame;
	vector< vector<float> > m_vfFaceMotListen_x;
	vector< vector<float> > m_vfFaceMotListen_y;
	vector< vector<float> > m_vfFaceMotListen_z;
	int m_iFaceMotListen;
	vector< vector<int> > m_viFaceMotUserDef_frame;
	vector< vector<float> > m_vfFaceMotUserDef_x;
	vector< vector<float> > m_vfFaceMotUserDef_y;
	vector< vector<float> > m_vfFaceMotUserDef_z;
	int m_iFaceMotUserDef;
	
	float m_fFaceMotAngle[3];
	float m_fAutonomousMoveAngle[3];
	
	vector< vector<int> > m_viFaceMotBlink;
	int m_iFaceMotBlink;

	// Frame Rate�֘A
	int m_iMaxFrameRate;

	// Viseme
	string m_sVisemeFile;

	bool OpenAutonomousMoveFile(string buffer, int num);
	bool OpenFaceMotFile(string buffer, int num);
	bool OpenFaceMotBlinkFile( int frame_num );
	void SplitToken(string buffer);
	
	// Resources.h�ŊǗ����Ă���e��p�����[�^
	string m_sUpperTeethObjName;
	string m_sLowerTeethObjName;
	string m_sMouthWallObjName;
	string m_sEyeObjName;
	string m_sMouthWallTexture;
	string m_sDefWfmPnt;
	string m_sDefWfmLnk;
	int m_dDefWindowWidth;
	int m_dDefWindowHeight;
	int m_dExpressionDuration;
	int m_dEyeMoveDuration;
	int m_dDefWindowPosX;
	int m_dDefWindowPosY;

public:
	// public constructor
	Configuration(string configFile);
	
	// public destructor
	~Configuration();
	
	int GetMaskNum() { return m_vsMaskName.size(); }
	string GetMaskName(int i) { return m_vsMaskName[i]; }
	string GetMaskRgbFile(int i) { return m_vsMaskRgbFile[i]; }
	string GetMaskPntFile(int i) { return m_vsMaskPntFile[i]; }

	int GetMaskEyeNum() { return m_vsMaskEyeName.size(); }
	string GetMaskEyeName(int i) { return m_vsMaskEyeName[i]; }
	string GetMaskEyeImageFile(int i) { return m_vsMaskEyeImageFile[i]; }
	string GetMaskEyeDefFile(int i) { return m_vsMaskEyeDefFile[i]; }
	
	int GetMaskTeethNum() { return m_vsMaskTeethName.size(); }
	string GetMaskTeethName(int i) { return m_vsMaskTeethName[i]; }
	string GetMaskTeethDefFile(int i) { return m_vsMaskTeethDefFile[i]; }

	int GetBGNum() { return m_vsBGName.size(); }
	string GetBGName(int i) { return m_vsBGName[i]; }
	string GetBGRgbFile(int i) { return m_vsBGRgbFile[i]; }

	// Autonomous Blink
	int GetAutonomousBlinkNum() { return m_viAutonomousBlink.size(); }
	int GetAutonomousBlink(int i) { return m_viAutonomousBlink[i]; }
	int GetAutonomousBlinkPattern(int i) { return m_viAutonomousBlinkPattern[i]; }

	// AutonomousMov
	int GetAutonomousMoveSize(int mode);
	int GetAutonomousMoveFrameSize(int mode, int id);
	void GetAutonomousMoveCurrentFrame(int mode, int id, int frame);

	float GetAutonomousMoveAngle(int i) { return m_fAutonomousMoveAngle[i]; }
	void ResetAutonomousMoveFrame() {
		m_fAutonomousMoveAngle[0] = 0.0;
		m_fAutonomousMoveAngle[1] = 0.0;
		m_fAutonomousMoveAngle[2] = 0.0;
	}

	// FaceMot
	int GetFaceMotSize(int mode);
	int GetFaceMotFrameSize(int mode, int id);
	void GetFaceMotCurrentFrame(int mode, int id, int frame);

	float GetFaceMotAngle(int i) { return m_fFaceMotAngle[i]; }
	void ResetFaceMotFrame() {
		m_fFaceMotAngle[0] = 0.0;
		m_fFaceMotAngle[1] = 0.0;
		m_fFaceMotAngle[2] = 0.0;
	}

	int GetFaceMotBlinkSize() { return m_iFaceMotBlink; }
	int GetFaceMotBlinkStrSize(int i) {
		return m_viFaceMotBlink[i].size();
	}
	string GetFaceMotBlinkStr(int i, int j) {
		char temp[256];
		sprintf( temp, "%d", m_viFaceMotBlink[i][j] );
		string out_str(BLINK_STR);
		out_str += temp;
		return out_str;
	}

	int GetFrameRate() { return m_iMaxFrameRate; }
	void SetFrameRate(int i) { m_iMaxFrameRate = i; }

	string& GetVisemeFile() { return m_sVisemeFile; }
	void SetVisemeFile(string s) { m_sVisemeFile = s; }


	string GetUpperTeethObjName() { return m_sUpperTeethObjName; }
	string GetLowerTeethObjName() { return m_sLowerTeethObjName; }
	string GetMouthWallObjName() { return m_sMouthWallObjName; }
	string GetEyeObjName() { return m_sEyeObjName; }
	string GetMouthWallTexture() { return m_sMouthWallTexture; }
	string GetDefWfmPnt() { return m_sDefWfmPnt; }
	string GetDefWfmLnk() { return m_sDefWfmLnk; }
	int GetDefWindowWidth() { return m_dDefWindowWidth; }
	int GetDefWindowHeight() { return m_dDefWindowHeight; }
	int GetDefWindowPosX() { return m_dDefWindowPosX; }
	int GetDefWindowPosY() { return m_dDefWindowPosY; }
	int GetExpressionDuration() { return m_dExpressionDuration; }
	int GetEyeMoveDuration() { return m_dEyeMoveDuration; }


	void SetUpperTeethObjName(string value) { m_sUpperTeethObjName = value; }
	void SetLowerTeethObjName(string value) { m_sLowerTeethObjName = value; }
	void SetMouthWallObjName(string value) { m_sMouthWallObjName = value; }
	void SetEyeObjName(string value) { m_sEyeObjName = value; }
	void SetMouthWallTexture(string value) { m_sMouthWallTexture = value; }
	void SetDefWfmPnt(string value) { m_sDefWfmPnt = value; }
	void SetDefWfmLnk(string value) { m_sDefWfmLnk = value; }
	void SetDefWindowWidth(int value) { m_dDefWindowWidth = value; }
	void SetDefWindowHeight(int value) { m_dDefWindowHeight = value; }
	void SetDefWindowPosX(int value) { m_dDefWindowPosX = value; }
	void SetDefWindowPosY(int value) { m_dDefWindowPosY = value; }
	void SetExpressionDuration(int value) { m_dExpressionDuration = value; }
	void SetEyeMoveDuration(int value) { m_dEyeMoveDuration = value; }

};

#endif

