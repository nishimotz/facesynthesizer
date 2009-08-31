/*++
CommandSet.h
--*/


#ifndef __COMMAND_SET_H__
#define __COMMAND_SET_H__

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <iostream>
#include <vector>
#include <string>

#include "Resources.h"

using namespace std;

// Protocol Vresion 管理
const char PROTOCOL_VERSION_NAME[] = "Protocol Ver. 2.06";

#ifndef EOF
#define EOF -1
#endif

struct HeadRot {
	int id;
	float axis[3];
};

struct FaceMotValue {
	int motion;
	int pattern;
};

struct FaceExpValue {
	int exp;
	int pattern;
	int value;
	int duration;
};

struct LipSyncValue {
	vector<string> pho;
	string total_pho;
	vector<int> dur;
	int total_dur;
};

struct SpeakValue {
	int time;
	int current_time[5];
	LipSyncValue lipSync;
	LipSyncValue lipSyncOld;
	int stat;
	string text;
	int syncOffset;
	int syncTime;
};

struct SpeakProp {
	bool text;
	bool pho;
	bool dur;
	bool utt;
	bool len;
	bool stat;
};

struct MaskSet {
	vector<string> maskName; // 現在の保有するマスクの数
	int nowMask;             // 現在表示されているマスクのID
};

struct BGSet {
	vector<string> BGName;
	int nowBG;
};

struct EyeRotValue {
	bool flag;
	double angle[2];
	int duration;
};

struct AutonomousValue {
	int motion[AUTONOMOUS_ID_NUM];
};

struct SaveImageValue {
	bool flag;
	string file;
};

struct AgentLocation {
	double rot[3];		// rotate x, y, z
	double trans[2];	// translate x, y
	double scale;		// scale
	bool enable;
};

struct FSMCapture {
	string dir_name;
	string file_name;
	int fps;
	bool enable;
};



class CommandSet { 
private:
	int m_iRun;						// Run
	string m_sProtocolVersion;		// Protocol Vresion
	string m_sModuleVersion;		// Module Version
	vector<float> m_vdAUNumber;     // AU Number
	vector<float> m_vdMUNumber;     // MU Number
	vector<float> m_vdVMNumber;     // VM Number
	HeadRot m_HeadRotRel;			// HeadRotRel
	HeadRot m_HeadRotAbs;			// HeadRotAbs
	FaceMotValue m_FaceMot;			// FaceMot
	FaceExpValue m_FaceExp;			// FaceExp
	SpeakValue m_Speak;				// Speak (also LipSyc)
	AutonomousValue m_Autonomous;	// Autonomous
	MaskSet m_MaskSet;				// MaskSet
	BGSet m_BGSet;					// BGSet
	int m_iMouthScale;				// MouthScale
	float m_dAgentAlpha;			// AgentAlpha
	SpeakProp m_SpeakProp;			// SpeakProp 
	SaveImageValue m_SaveImage;		// SaveImage
	EyeRotValue m_EyeRotLeft;		// Eye Rot (Left)
	EyeRotValue m_EyeRotRight;		// Eye Rot (Right)
	int m_iViewMode;				// ViewMode
	int m_iViewBackground;			// ViewBackground
	AgentLocation m_AgentLocation;	// AgentLocation
	FSMCapture m_Capture;			// Capture
	float m_fOffset;				// Offset

	vector<string> m_vsToken;
	
	// 汎用
	string GetToChar(int i);
	string GetToChar(float f);
	
public:
	// public constructor
	CommandSet();
	
	// public destructor
	~CommandSet();
	
	// init
	void SetDefalut();

	// command set function (Get or Set)
	// Run
	int    GetRun() { return m_iRun; }
	string GetRunStr();
	
	// ProtocolVersion & ModuleVersion
	string &GetProtocolVersionStr() { return m_sProtocolVersion; }
	string &GetModuleVersionStr() { return m_sModuleVersion; }
	
	// Face
	void  SetFace(int i, float d) { m_vdAUNumber[i] = d; }
	float GetFace(int i) { return m_vdAUNumber[i]; }
	vector<float> &GetFace() { return m_vdAUNumber; }
	
	// Mouth
	void  SetMouth(int i, float d) { m_vdMUNumber[i] = d; }
	float GetMouth(int i) { return m_vdMUNumber[i]; }
	vector<float> &GetMouth() { return m_vdMUNumber; }
	
	// Viseme
	void  SetViseme(int i, float d) { m_vdVMNumber[i] = d; }
	float GetViseme(int i) { return m_vdVMNumber[i]; }
	vector<float> &GetViseme() { return m_vdVMNumber; }
	
	// HeadRolRel
	void SetHeadRotRel(int id, float x, float y, float z) {
		m_HeadRotRel.id = id;
		m_HeadRotRel.axis[0] = x; 
		m_HeadRotRel.axis[1] = y;
		m_HeadRotRel.axis[2] = z;
		m_HeadRotAbs.id = id;
		m_HeadRotAbs.axis[0] += m_HeadRotRel.axis[0]; 
		m_HeadRotAbs.axis[1] += m_HeadRotRel.axis[1]; 
		m_HeadRotAbs.axis[2] += m_HeadRotRel.axis[2];
	}
	float &GetHeadRotRelAxis(int i) { return m_HeadRotRel.axis[i]; }
	int    GetHeadRotRelId() { return m_HeadRotRel.id; }
	HeadRot *GetHeadRotRel() { return &m_HeadRotRel; }
	
	// HeadRolAbs
	void SetHeadRotAbs(int id, float x, float y, float z) {
		m_HeadRotAbs.id = id;
		m_HeadRotAbs.axis[0] = x; 
		m_HeadRotAbs.axis[1] = y; 
		m_HeadRotAbs.axis[2] = z;
	}
	void SetHeadRotAbs( float x, float y, float z) {
		m_HeadRotAbs.axis[0] = x; 
		m_HeadRotAbs.axis[1] = y; 
		m_HeadRotAbs.axis[2] = z;
	}
	float &GetHeadRotAbsAxis(int i) { return m_HeadRotAbs.axis[i]; }
	int    GetHeadRotAbsId() { return m_HeadRotAbs.id; }
	HeadRot *GetHeadRotAbs() { return &m_HeadRotAbs; }
	
	// FaceMot
	void SetFaceMot(int motion, int pattern) {
		m_FaceMot.motion = motion;
		m_FaceMot.pattern = pattern;
	}
	FaceMotValue *GetFaceMot() { return &m_FaceMot; }
	int GetFaceMotMotion() { return m_FaceMot.motion; }
	int GetFaceMotPattern() { return m_FaceMot.pattern; }
	string GetFaceMotMotionStr();
	string GetFaceMotMotionStr(int i);
	
	// EyeMot
	void SetEyeRotLeft(double angle_x, double angle_y, int duration) {
		m_EyeRotLeft.angle[0] = angle_x;
		m_EyeRotLeft.angle[1] = angle_y;
		m_EyeRotLeft.duration = duration;
	}
	void SetEyeRotRight(double angle_x, double angle_y, int duration) {
		m_EyeRotRight.angle[0] = angle_x;
		m_EyeRotRight.angle[1] = angle_y;
		m_EyeRotRight.duration = duration;
	}
	void SetEyeRot(double angle_x, double angle_y, int duration) {
		SetEyeRotLeft(angle_x, angle_y, duration);
		SetEyeRotRight(angle_x, angle_y, duration);
	}
	void SetEyeRotLeftFlag(bool flag) {
		m_EyeRotLeft.flag = flag;
	}
	void SetEyeRotRightFlag(bool flag) {
		m_EyeRotRight.flag = flag;
	}
	void SetEyeRotFlag(bool flag) {
		SetEyeRotLeftFlag(flag);
		SetEyeRotRightFlag(flag);
	}
	EyeRotValue *GetEyeRotLeft() { return &m_EyeRotLeft; }
	EyeRotValue *GetEyeRotRight() { return &m_EyeRotRight; }
	double GetEyeRotLeftAngleX() { return m_EyeRotLeft.angle[0]; }
	double GetEyeRotLeftAngleY() { return m_EyeRotLeft.angle[1]; }
	double GetEyeRotLeftDuration() { return m_EyeRotLeft.duration; }
	double GetEyeRotRightAngleX() { return m_EyeRotRight.angle[0]; }
	double GetEyeRotRightAngleY() { return m_EyeRotRight.angle[1]; }
	double GetEyeRotRightDuration() { return m_EyeRotRight.duration; }
	bool GetEyeRotLeftFlag() { return m_EyeRotLeft.flag; }
	bool GetEyeRotRightFlag() { return m_EyeRotRight.flag; }

	// FaceExp
	void SetFaceExp(int exp, int pattern, int value, int duration) {
		m_FaceExp.exp = exp;
		m_FaceExp.pattern = pattern;
		m_FaceExp.value = value;
		m_FaceExp.duration = duration;
	}
	FaceExpValue *GetFaceExp() { return &m_FaceExp; }
	string GetFaceExpExpStr();
	string GetFaceExpExpStr(int i);
	int GetFaceExpExp() { return m_FaceExp.exp; }
	int GetFaceExpPattern() { return m_FaceExp.pattern; }
	int GetFaceExpValue() { return m_FaceExp.value; }
	int GetFaceExpDuration() { return m_FaceExp.duration; }
	
	// LipSync
	void SendToLipSyncOld();  // 新しいのと古いのを入れ替える.
	void PushBackLipSync(string pho, int dur) { 
		m_Speak.lipSync.pho.push_back(pho);
		m_Speak.lipSync.dur.push_back(dur);
	}
	void   SetTotalLipSync();      // setting text, total_dur, total_pho
	string GetLipSyncPhoStr() { return m_Speak.lipSync.total_pho; }
	int    GetLipSyncDur() { return m_Speak.lipSync.total_dur; }
	string GetLipSyncDurStr() { return GetToChar( m_Speak.lipSync.total_dur ); }
	
	// Speak
	// time = 0: now    time = -1: stop 発話開始相対時間
	void SetSpeak(int time, int day, int hr, int mm, int ss, int ms) {
		m_Speak.time = time;
		m_Speak.current_time[0] = day;
		m_Speak.current_time[1] = hr;
		m_Speak.current_time[2] = mm;
		m_Speak.current_time[3] = ss;
		m_Speak.current_time[4] = ms;
	}
	void SetSpeakStat(int i) { m_Speak.stat = i; }
	string GetSpeakTextStr() { return m_Speak.text; }
	string GetSpeakPhoStr() { return m_Speak.lipSync.total_pho; }
	int    GetSpeakDur() { return m_Speak.lipSync.total_dur; }
	string GetSpeakDurStr() { return GetToChar(m_Speak.lipSync.total_dur); }
	string GetSpeakUttStr() { return m_Speak.lipSyncOld.total_pho; }
	int    GetSpeakLen() { return m_Speak.lipSyncOld.total_dur; }
	string GetSpeakLenStr() { return GetToChar(m_Speak.lipSyncOld.total_dur); }
	int    GetSpeakStat() { return m_Speak.stat; }
	string GetSpeakStatStr();
	string GetSpeakStatStr(int i);
	SpeakValue *GetSpeak() { return &m_Speak; }
	int GetSpeakCurrentTime(int i) { return m_Speak.current_time[i]; }
	int GetSpeakTime() { return m_Speak.time; }
	void SetSpeakSyncOffset(int i) { m_Speak.syncOffset = i; }
	int GetSpeakSyncOffset() { return m_Speak.syncOffset; }
	string GetSpeakSyncOffsetStr() { return GetToChar(m_Speak.syncOffset); }
	void SetSpeakSyncTime(int i) { m_Speak.syncTime = i; }
	int GetSpeakSyncTime() { return m_Speak.syncTime; }
	
	// MaskSet
	int    GetNowMask() { return m_MaskSet.nowMask; }
	string GetNowMaskName() { return m_MaskSet.maskName[m_MaskSet.nowMask]; }
	string GetMaskName(int i) { return m_MaskSet.maskName[i]; }
	int  GetMaskNum() { return m_MaskSet.maskName.size(); }
	
	bool   SetNowMaskName(string name);
	void   SetNowMask(int i) { m_MaskSet.nowMask = i; }
	void   AddMaskName(string name) { m_MaskSet.maskName.push_back( name ); }
	
	MaskSet *GetMaskSet() { return &m_MaskSet; }

	// BGSet
	int    GetNowBG() { return m_BGSet.nowBG; }
	string GetNowBGName() { return m_BGSet.BGName[m_BGSet.nowBG]; }
	string GetBGName(int i) { return m_BGSet.BGName[i]; }
	int  GetBGNum() { return m_BGSet.BGName.size(); }
	
	bool   SetNowBGName(string name);
	void   SetNowBG(int i) { m_BGSet.nowBG = i; }
	void   AddBGName(string name) { m_BGSet.BGName.push_back( name ); }
	
	BGSet *GetBGSet() { return &m_BGSet; }

	// Autnomous
	void SetAutonomousPattern(int motion, int pattern) { 
		m_Autonomous.motion[motion] = pattern; 
	}
	bool SetAutonomousPattern(string motion, int pettern);
	
	int GetAutonomousPattern(int motion) { return m_Autonomous.motion[motion]; }
	int GetAutonomousPattern(string motion);
	
	// Viseme検索用  
	vector<string> &GetPhoneme() { return m_Speak.lipSync.pho; }
	vector<int> &GetDuration() { return m_Speak.lipSync.dur; }
	
	// MouthScale
	int GetMouthScale() { return m_iMouthScale; }
	string GetMouthScaleStr() { return GetToChar(m_iMouthScale); }
	void SetMouthScale(int i) { m_iMouthScale = i; }
	

	// ViewMode
	int GetViewMode(string s);
	int GetViewMode() { return m_iViewMode; }
	string GetViewModeStr();
	void SetViewMode(int i) { m_iViewMode = i; }

	// ViewBackground
	int GetViewBackground(string s);
	int GetViewBackground() { return m_iViewBackground; }
	string GetViewBackgroundStr();
	void SetViewBackground(int i) { m_iViewBackground = i; }

	// SpeakProp
	void SetSpeakTextProp(bool b) { m_SpeakProp.text = b; }
	void SetSpeakPhoProp(bool b) { m_SpeakProp.pho = b; }
	void SetSpeakDurProp(bool b) { m_SpeakProp.dur = b; }
	void SetSpeakUttProp(bool b) { m_SpeakProp.utt = b; }
	void SetSpeakLenProp(bool b) { m_SpeakProp.len = b; }
	void SetSpeakStatProp(bool b) { m_SpeakProp.stat = b; }
	void SetSpeakProp(bool b) {
		SetSpeakTextProp(b);
		SetSpeakPhoProp(b);
		SetSpeakDurProp(b);
		SetSpeakUttProp(b);
		SetSpeakLenProp(b);
		SetSpeakStatProp(b);
	}
	bool GetSpeakPhoProp() { return m_SpeakProp.pho; }
	bool GetSpeakDurProp() { return m_SpeakProp.dur; }
	bool GetSpeakTextProp() { return m_SpeakProp.text; }
	bool GetSpeakUttProp() { return m_SpeakProp.utt; }
	bool GetSpeakLenProp() { return m_SpeakProp.len; }
	bool GetSpeakStatProp() { return m_SpeakProp.stat; }
	
	// SaveImage
	void SetSaveImageFlag(bool b) { m_SaveImage.flag = b; }
	void SetSaveImageFile(string s) { m_SaveImage.file = s; }
	bool GetSaveImageFlag() { return m_SaveImage.flag; }
	string GetSaveImageFile() { return m_SaveImage.file; }
	

	// AgentLocation
	void SetAgentRot(double x, double y, double z) {
		m_AgentLocation.rot[0] = x;
		m_AgentLocation.rot[1] = y;
		m_AgentLocation.rot[2] = z;
	}
	void SetAgentRot(double rotation[3]) {
		m_AgentLocation.rot[0] = rotation[0];
		m_AgentLocation.rot[1] = rotation[1];
		m_AgentLocation.rot[2] = rotation[2];
	}
	void GetAgentRot(double &x, double &y, double &z) {
		x = m_AgentLocation.rot[0];
		y = m_AgentLocation.rot[1];
		z = m_AgentLocation.rot[2];
	}
	void GetAgentRot( double rotation[3] ) {
		rotation[0] = m_AgentLocation.rot[0];
		rotation[1] = m_AgentLocation.rot[1];
		rotation[2] = m_AgentLocation.rot[2];
	}
	void SetAgentTrans(double x, double y) {
		m_AgentLocation.trans[0] = x;
		m_AgentLocation.trans[1] = y;
	}
	void SetAgentTrans(double translate[2]) {
		m_AgentLocation.trans[0] = translate[0];
		m_AgentLocation.trans[1] = translate[1];
	}
	void GetAgentTrans(double &x, double &y) {
		x = m_AgentLocation.trans[0];
		y = m_AgentLocation.trans[1];
	}
	void GetAgentTrans(double translate[2]) {
		translate[0] = m_AgentLocation.trans[0];
		translate[1] = m_AgentLocation.trans[1];
	}
	
	void SetAgentScale(double scale) { m_AgentLocation.scale = scale; }
	double GetAgentScale() { return m_AgentLocation.scale; }
	void GetAgentScale(double &scale) { scale = m_AgentLocation.scale; }
	
	void SetAgentEnable(bool yesno) { m_AgentLocation.enable = yesno; }
	bool GetAgentEnable() { return m_AgentLocation.enable; }
	void GetAgentEnable(bool &yesno) { yesno = m_AgentLocation.enable; }
	
	void ResetAgentLocation() {
		m_AgentLocation.rot[0] = m_AgentLocation.rot[1] = m_AgentLocation.rot[2] = 0.0;
		m_AgentLocation.trans[0] = m_AgentLocation.trans[1] = 0.0;
		m_AgentLocation.scale = 1.0;
	}

	void SetFSMCapture(string &dir, string &file, int framerate) {
		m_Capture.dir_name = dir;
		m_Capture.file_name = file;
		m_Capture.fps = framerate;
		m_Capture.enable = true;
	}
	string GetFSMCaptureDirName() { return  m_Capture.dir_name; }
	string GetFSMCaptureFileName() { return m_Capture.file_name; }
	int GetFSMCaptureFPS() { return m_Capture.fps; }
	bool GetFSMCaptureEnable() { return m_Capture.enable; }
	void SetFSMCaptureEnable(bool flag) { m_Capture.enable = flag; }

	// AgentAlpha
	double GetAgentAlpha() { return m_dAgentAlpha; }
	string GetAgentAlphaStr() { return GetToChar(m_dAgentAlpha); }
	void SetAgentAlpha(float f) { m_dAgentAlpha = f; }

	// Offset
	void SetOffset(float f) { m_fOffset = f; }
	float GetOffset() { return m_fOffset; }
	string GetOffsetStr() { return GetToChar(m_fOffset); }

};

#endif

