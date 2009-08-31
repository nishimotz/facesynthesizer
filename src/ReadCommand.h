/*++
ReadCommand.h
--*/

#ifndef __READ_COMMAND_H__
#define __READ_COMMAND_H__

#ifdef WIN32
#pragma warning (disable: 4786)
#endif


#include <iostream>

#include "CommandSet.h"

using namespace std;

class ReadCommand : public CommandSet {
	
public:
	// public constructor
	ReadCommand();
	
	// public destructor
	~ReadCommand();
	
private:
	vector<string> m_vsToken;
	vector<string> m_vsCmdParamName;
	vector<string> m_vsCmdSlotName;
	vector<string> m_vsCmdMethodName;
	vector<int> m_vsCmdMethodNum;
	vector<int> m_vsCmdMethodSubNum;
	
	bool FuncCommentOut(int dummy);          // Unknown Command
	bool FuncInqProtocolVersion(int dummy);  // inq ProtocolVersion
	bool FuncInqModuleVersion(int dummy);    // inq ModuleVersion
	bool FuncInqRun(int dummy);              // inq Run
	bool FuncInqLipSync(int num);            // inq LipSync
	bool FuncInqSpeak(int num);              // inq Speak
	bool FuncInqMaskSet(int dummy);          // inq MaskSet
	bool FuncSetFace(int num);               // set Face
	bool FuncSetMouth(int num);              // set Mouse
	bool FuncSetHeadRotRel(int num);         // set HeadRotRel
	bool FuncSetHeadRotAbs(int num);         // set HeadRotAbs
	bool FuncSetFaceMot(int dummy);          // set FaceMot
	bool FuncSetFaceExp(int dummy);          // set FaceExp
	bool FuncSetLipSync(int num);            // set LipSync
	bool FuncSetSpeak(int num);              // set Speak
	bool FuncSetMask(int dummy);             // set Mask
	bool FuncSetAutonomous(int dummy);       // set Autonomous
	bool FuncSetMouthScale(int dummy);       // set MouthScale
	bool FuncInqMouthScale(int dummy);       // inq MouthScale
	bool FuncPropSpeak(int num);             // prop Speak
	bool FuncSetSaveImage(int dummy);        // set ImageSave
	bool FuncSetBackground(int dummy);		 // set Background
	bool FuncInqBackgroundSet(int dummy);	 // inq Background
	bool FuncInqNowMask(int dummy);			 // inq NowMask
	bool FuncInqNowBackground(int dummy);    // inq NowBackground
	bool FuncSetEyeRot(int dummy);			// set EyeRot
	bool FuncSetEyeRotLeft(int dummy);		// set EyeRotLeft
	bool FuncSetEyeRotRight(int dummy);		// set EyeRotRight
	bool FuncSetViewMode(int dummy);		// set ViewMode
	bool FuncInqViewMode(int dummy);		// inq ViewMode
	bool FuncSetViewBackground(int dummy);	// set ViewBackground
	bool FuncInqViewBackground(int dummy);	// inq ViewBackground
	bool FuncSetRun(int dummy);				// set Run
	bool FuncSetAgentRot(int dummy);		// set AgentRot
	bool FuncSetAgentTrans(int dummy);		// set AgentTrans
	bool FuncSetAgentScale(int dummy);		// set AgentScale
	bool FuncSetAgentEnable(int dummy);		// set AgentEnable
	bool FuncSetAgentAlpha(int dummy);		// set AgentAlpha
	bool FuncSetCapture(int dummy);			// set Capture
	bool FuncInqCapture(int dummy);			// inq Capture
	bool FuncSetOffset(int dummy);			// set Offset
	bool FuncInqOffset(int dummy);			// inq Offset

	// メッセージを送る（単にreqを加える）
	void SendMessageCommand( string message );
	
	// コマンド群の読み出し
	bool ReadCommandSet();
	
	// Tokenに分割
	void SplitToken(string buffer);
	
	// Tokenのチェック 
	// 戻り値 0:false  1:'='  2:'<'  3:"<<"
	int CheckToken();
	
	// Tokenの数のチェック
	// 引数 Tokenの数
	bool CheckTokenNum( int num );
	
	// 現在の時刻を獲得
	void NowTime( int &day, int &hr, int &mm, int &ss, int &ms );
	
public:
	bool AnalysisCmd( string command );
};

#endif

