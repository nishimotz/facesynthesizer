/*++
CommandSet.c++
--*/

#include "CommandSet.h"

#include "trace.h"

CommandSet::CommandSet()
{
	TRACE("CommandSet::~CommandSet()");
	int i;
	
	// All Data Initialize
	
	// Init. of Run and Versions. 
	m_iRun = LIVE;
	m_sProtocolVersion = PROTOCOL_VERSION_NAME;
	m_sModuleVersion = MODULE_VERSION_NAME;
	
	m_vdAUNumber.resize(MAX_AU_NUMBER);
	m_vdMUNumber.resize(MAX_MU_NUMBER);
	m_vdVMNumber.resize(MAX_VM_NUMBER);
	
	// Init. of AU, MU, VM
	for( i = 0; i < MAX_AU_NUMBER; i++ )
		m_vdAUNumber[i] = 0.0;
	for( i = 0; i < MAX_MU_NUMBER; i++ )
		m_vdMUNumber[i] = 0.0;
	for( i = 0; i < MAX_VM_NUMBER; i++ )
		m_vdVMNumber[i] = 0.0;
	
	// Init. of HeadRotRel & HeadRotAbs
	m_HeadRotRel.id = 0;
	m_HeadRotAbs.id = 0;
	for( i = 0; i < 3; i++ ) {
		m_HeadRotRel.axis[i] = 0.0;
		m_HeadRotAbs.axis[i] = 0.0;
	}
	
	m_FaceMot.motion = NOMOVE;
	m_FaceMot.pattern = 0;
	
	// Init. of FaceExp
	m_FaceExp.exp = NEUTRAL;
	m_FaceExp.pattern = 0;
	m_FaceExp.value = 100;
	m_FaceExp.duration = 0;
	
	// Init of SpeakValue
	// あらかじめメモり確保なり 4KB
	m_Speak.lipSync.pho.reserve(4096);
	m_Speak.lipSyncOld.pho.reserve(4096);
	m_Speak.lipSync.dur.reserve(4096);
	m_Speak.lipSyncOld.dur.reserve(4096);
	
	// そして何でもいいから初期化
	m_Speak.time = -1;
	m_Speak.current_time[0] = 0;
	m_Speak.current_time[1] = 0;
	m_Speak.current_time[2] = 0;
	m_Speak.stat = IDLE;
	m_Speak.text = "non";
	m_Speak.lipSync.total_dur = 0;
	m_Speak.lipSync.total_pho = "non";
	m_Speak.lipSyncOld.total_dur = 0;
	m_Speak.lipSyncOld.total_pho = "non";
	m_Speak.syncOffset = 0;
	m_Speak.syncTime = 0;
	
	// Init of MaskSet
	m_MaskSet.nowMask = -1; // あり得ない数を代入

	// Init of BGSet
	m_BGSet.nowBG = -1; // あり得ない数を代入

	// Init of Autonomous
	for( i = 0; i < AUTONOMOUS_ID_NUM; i++ ) {
		m_Autonomous.motion[i] = 0;
	}
	
	// Init of MouthScale
	m_iMouthScale = 30;
	
	// Init of SpeakProp
	m_SpeakProp.text = false;
	m_SpeakProp.pho = false;
	m_SpeakProp.dur = false;
	m_SpeakProp.utt = false;
	m_SpeakProp.len = false;
	m_SpeakProp.stat = false;
	
	// Init of SaveImage
	m_SaveImage.flag = false;
	m_SaveImage.file = "defalut.rgb";

	// Init of EyeMot;
	m_EyeRotLeft.angle[0] = 0.0;
	m_EyeRotLeft.angle[0] = 0.0;
	m_EyeRotLeft.duration = 0;
	m_EyeRotLeft.flag = false;
	m_EyeRotRight.angle[0] = 0.0;
	m_EyeRotRight.angle[1] = 0.0;
	m_EyeRotRight.duration = 0;
	m_EyeRotRight.flag = false;

	// Init of FaceView
	m_iViewMode = VIEW_TEXTURE;
	m_iViewBackground = VIEW_ENABLE;
	
	// Init of AgentLocation
	ResetAgentLocation();
	m_AgentLocation.enable = true;

	// Init of AgentAlpha
	m_dAgentAlpha = 1.0;
	
	// Init of Capture
	m_Capture.dir_name = "./";
	m_Capture.file_name = "sample";
	m_Capture.fps = 0;
	m_Capture.enable = false;

	// Init of Offset
	m_fOffset = 0.0;

}

CommandSet::~CommandSet()
{
	TRACE("CommandSet::~CommandSet()");
}

void CommandSet::SendToLipSyncOld()
{
	TRACE("CommandSet::SendToLipSyncOld()");
	// lipSyncOldの要素をクリアする
	m_Speak.lipSyncOld.pho.clear();
	m_Speak.lipSyncOld.dur.clear();
	
	// コピーする（たぶん書き方はあっていると思う）
	m_Speak.lipSyncOld.pho = m_Speak.lipSync.pho;
	m_Speak.lipSyncOld.dur = m_Speak.lipSync.dur;
	m_Speak.lipSyncOld.total_pho = m_Speak.lipSync.total_pho;
	m_Speak.lipSyncOld.total_dur = m_Speak.lipSync.total_dur;
	
	// lipSyncの要素をクリアする
	m_Speak.lipSync.pho.clear();
	m_Speak.lipSync.dur.clear();
	m_Speak.lipSync.total_pho = "Non";
	m_Speak.lipSync.total_dur = 0;
}

string CommandSet::GetToChar(int i) 
{ 
	char c[256];
	sprintf( c, "%d", i);  
	string s = c;  
	return s;
}

string CommandSet::GetToChar(float f)
{ 
	char c[256];
	sprintf( c, "%f", f);  
	string s = c;  
	return s;
}


string CommandSet::GetRunStr() { 
	string s = RUN_NAME[m_iRun];  
	return s; 
}

string CommandSet::GetFaceMotMotionStr() {
	string s = FACE_MOT_NAME[m_FaceMot.motion];  
	return s;
}

string CommandSet::GetFaceMotMotionStr(int i) { 
	string s = FACE_MOT_NAME[i]; 
	return s;
}

string CommandSet::GetFaceExpExpStr() 
{
	string s = FACE_EXP_NAME[m_FaceExp.exp]; 
	return s;
}

string CommandSet::GetFaceExpExpStr(int i) { 
	string s = FACE_EXP_NAME[i]; 
	return s;
}

string CommandSet::GetSpeakStatStr() {
	string s = SPEAK_NAME[m_Speak.stat];
	return s;
}

string CommandSet::GetSpeakStatStr(int i) {
	string s = SPEAK_NAME[i];
	return s;
}

string CommandSet::GetViewModeStr()
{
	string s = VIEW_MODE_NAME[m_iViewMode];
	return s;
}

string CommandSet::GetViewBackgroundStr()
{
	string s = VIEW_BACKGROUND_NAME[m_iViewBackground];
	return s;
}

int CommandSet::GetViewMode(string s)
{
	for( int i = 0; i < MAX_VIEW_MODE_NUMBER; i++ ) {
		if( s == VIEW_MODE_NAME[i] ) {
			return i;
		}
	}
	return -1;
}

int CommandSet::GetViewBackground(string s)
{
	for( int i = 0; i < MAX_VIEW_BACKGROUND_NUMBER; i++ ) {
		if( s == VIEW_BACKGROUND_NAME[i] ) {
			return i;
		}
	}
	return -1;
}

int CommandSet::GetAutonomousPattern(string motion) {
	for( int i = 0; i < AUTONOMOUS_ID_NUM; i++ ) {
		if( motion == AUTONOMOUS_NAME[i]) {
			return m_Autonomous.motion[i];
		}
	}
	// err ... 0 returen
	return 0;
}

bool CommandSet::SetAutonomousPattern(string motion, int pattern) {
	for( int i = 0; i < AUTONOMOUS_ID_NUM; i++ ) {
		if( motion == AUTONOMOUS_NAME[i] ) {
			m_Autonomous.motion[i] = pattern;
			return true;
		}
	}
	return false;
}


void CommandSet::SetTotalLipSync()
{
	TRACE("CommandSet::SetTotalLipSync();");
	
	string s, ss;
	int j = 0;
	
	for( unsigned int i = 0; i < m_Speak.lipSync.pho.size(); i++ ) {
		s += m_Speak.lipSync.pho[i];
		s += " ";
		ss += m_Speak.lipSync.pho[i];
		ss += " [";
		ss += GetToChar(m_Speak.lipSync.dur[i]);
		ss += "] ";
		j += m_Speak.lipSync.dur[i];
	}
	m_Speak.text = s;
	m_Speak.lipSync.total_pho = ss;
	m_Speak.lipSync.total_dur = j;
}

bool CommandSet::SetNowMaskName(string name) 
{
	TRACE("CommandSet::SetNowMask(string str)");
	for( unsigned int i = 0; i < m_MaskSet.maskName.size(); i++ ) {
		if( name == m_MaskSet.maskName[i] ) {
			m_MaskSet.nowMask = i;
			return true;
		}
	}
	return false;
}

bool CommandSet::SetNowBGName(string name) 
{
	TRACE("CommandSet::SetNowBG(string str)");
	for( unsigned int i = 0; i < m_BGSet.BGName.size(); i++ ) {
		if( name == m_BGSet.BGName[i] ) {
			m_BGSet.nowBG = i;
			return true;
		}
	}
	return false;
}

void CommandSet::SetDefalut()
{
	int i;
	m_iRun = LIVE;
	m_sProtocolVersion = PROTOCOL_VERSION_NAME;
	m_sModuleVersion = MODULE_VERSION_NAME;
	
	// Init. of AU, MU, VM
	for( i = 0; i < MAX_AU_NUMBER; i++ )
		m_vdAUNumber[i] = 0.0;
	for( i = 0; i < MAX_MU_NUMBER; i++ )
		m_vdMUNumber[i] = 0.0;
	for( i = 0; i < MAX_VM_NUMBER; i++ )
		m_vdVMNumber[i] = 0.0;
	
	// Init. of HeadRotRel & HeadRotAbs
	m_HeadRotRel.id = 0;
	m_HeadRotAbs.id = 0;
	for( i = 0; i < 3; i++ ) {
		m_HeadRotRel.axis[i] = 0.0;
		m_HeadRotAbs.axis[i] = 0.0;
	}
	
	m_FaceMot.motion = NOMOVE;
	m_FaceMot.pattern = 0;
	
	// Init. of FaceExp
	m_FaceExp.exp = NEUTRAL;
	m_FaceExp.pattern = 0;
	m_FaceExp.value = 100;
	m_FaceExp.duration = 0;
	
	// Init of SpeakValue
	// あらかじめメモり確保なり 4KB
	m_Speak.lipSync.pho.reserve(4096);
	m_Speak.lipSyncOld.pho.reserve(4096);
	m_Speak.lipSync.dur.reserve(4096);
	m_Speak.lipSyncOld.dur.reserve(4096);
	
	// そして何でもいいから初期化
	m_Speak.time = -1;
	m_Speak.current_time[0] = 0;
	m_Speak.current_time[1] = 0;
	m_Speak.current_time[2] = 0;
	m_Speak.stat = IDLE;
	m_Speak.text = "non";
	m_Speak.lipSync.total_dur = 0;
	m_Speak.lipSync.total_pho = "non";
	m_Speak.lipSyncOld.total_dur = 0;
	m_Speak.lipSyncOld.total_pho = "non";
	m_Speak.syncOffset = 0;
	m_Speak.syncTime = 0;
	
	// Init of MaskSet
	m_MaskSet.nowMask = 0;

	// Init of BGSet
	m_BGSet.nowBG = 0;
	
	// Init of Autonomous
	for( i = 0; i < AUTONOMOUS_ID_NUM; i++ ) {
		m_Autonomous.motion[i] = 0;
	}
	
	// Init of MouthScale
	m_iMouthScale = 30;
	
	// Init of SpeakProp
	m_SpeakProp.text = false;
	m_SpeakProp.pho = false;
	m_SpeakProp.dur = false;
	m_SpeakProp.utt = false;
	m_SpeakProp.len = false;
	m_SpeakProp.stat = false;
	
	// Init of SaveImage
	m_SaveImage.flag = false;
	m_SaveImage.file = "defalut.rgb";

	// Init of EyeMot;
	m_EyeRotLeft.angle[0] = 0.0;
	m_EyeRotLeft.angle[0] = 0.0;
	m_EyeRotLeft.duration = 0;
	m_EyeRotLeft.flag = false;
	m_EyeRotRight.angle[0] = 0.0;
	m_EyeRotRight.angle[1] = 0.0;
	m_EyeRotRight.duration = 0;
	m_EyeRotRight.flag = false;

	// Init of FaceView
	m_iViewMode = VIEW_TEXTURE;
	m_iViewBackground = VIEW_ENABLE;
}


