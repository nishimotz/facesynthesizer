/*++
ReadCommand.c++
--*/
#include "ReadCommand.h"

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "trace.h"
#include "CommandSet.h"

#include "Resources.h"

#ifdef WIN32
#ifdef WIN32_GALATEA
extern bool SendMsg( const char* lpszMsg );
#endif
#endif

ReadCommand::ReadCommand() : CommandSet()
{
	TRACE("ReadCommand::~ReadCommand()");
	
	// コマンドセット読み込み
	if( ReadCommandSet() != true ) {
		exit(1);
	}
}

ReadCommand::~ReadCommand()
{
	TRACE("ReadCommand::~ReadCommand()");
}

bool ReadCommand::AnalysisCmd( string command ) {
	
	bool (ReadCommand::*pFunc[])(int) = {
		
		&ReadCommand::FuncCommentOut,                     // Unknown Command
			&ReadCommand::FuncInqProtocolVersion,             // inq ProtocolVersion
			&ReadCommand::FuncInqModuleVersion,               // inq ModuleVersion
			&ReadCommand::FuncInqRun,                         // inq Run
			&ReadCommand::FuncInqLipSync,                     // inq LipSync
			&ReadCommand::FuncInqSpeak,                       // inq Speak
			&ReadCommand::FuncInqMaskSet,                     // inq MaskSet
			&ReadCommand::FuncSetFace,                        // set Face
			&ReadCommand::FuncSetMouth,                       // set Mouse
			&ReadCommand::FuncSetHeadRotRel,                  // set HeadRotRel
			&ReadCommand::FuncSetHeadRotAbs,                  // set HeadRotAbs
			&ReadCommand::FuncSetFaceMot,                     // set FaceMot
			&ReadCommand::FuncSetFaceExp,                     // set FaceExp
			&ReadCommand::FuncSetLipSync,                     // set LipSync
			&ReadCommand::FuncSetSpeak,                       // set Speak
			&ReadCommand::FuncSetMask,                        // set Mask
			&ReadCommand::FuncSetAutonomous,                  // set Autonomous
			&ReadCommand::FuncSetMouthScale,                  // set MouthScale
			&ReadCommand::FuncInqMouthScale,                  // inq MouthScale
			&ReadCommand::FuncPropSpeak,                      // prop Speak
			&ReadCommand::FuncSetSaveImage,                   // set ImageSave
			&ReadCommand::FuncSetBackground,				  // set Background
			&ReadCommand::FuncInqBackgroundSet,				  // inq BackgroundSet
			&ReadCommand::FuncInqNowMask,					  // inq NowMask
			&ReadCommand::FuncInqNowBackground,				  // inq NowBackground		
			&ReadCommand::FuncSetEyeRot,					// set EyeRot
			&ReadCommand::FuncSetEyeRotLeft,				// set EyeRotLeft
			&ReadCommand::FuncSetEyeRotRight,				// set EyeRotRight
			&ReadCommand::FuncSetViewMode,					// set ViewMode
			&ReadCommand::FuncInqViewMode,					// inq ViewMode
			&ReadCommand::FuncSetViewBackground,			// set ViewBackground
			&ReadCommand::FuncInqViewBackground,			// inq ViewBackground
			&ReadCommand::FuncSetRun,						// set Run
			&ReadCommand::FuncSetAgentRot,					// set AgentRot
			&ReadCommand::FuncSetAgentTrans,				// set AgentTrans
			&ReadCommand::FuncSetAgentScale,				// set AgentScale
			&ReadCommand::FuncSetAgentEnable,				// set AgentEnable
			&ReadCommand::FuncSetAgentAlpha,				// set AgentAlpha
			&ReadCommand::FuncSetCapture,					// set Capture
			&ReadCommand::FuncInqCapture,					// inq Capture
			&ReadCommand::FuncSetOffset,					// set Offset
			&ReadCommand::FuncInqOffset						// inq Offset
	};
	
	unsigned int i, dummy = 0;
	// トークンに分割
	SplitToken( command );
	
	// コマンド検索
	// コメント行の検索
	if( m_vsToken.size() < 2 )
		return false;

	if( m_vsToken[0] == m_vsCmdParamName[0] ) {
		(this->*pFunc[0])(0);
		return true;
	}
	
	// 非コメント行の検索
	for( i = 1; i < m_vsCmdParamName.size(); i++ ) {
		// Slot, Parameterの検索
		if( m_vsToken[0] == m_vsCmdParamName[i] &&
			m_vsToken[1] == m_vsCmdSlotName[i] ) {
			// コマンドの実行
			if ((this->*pFunc[m_vsCmdMethodNum[i]])( m_vsCmdMethodSubNum[i] ) == false ) {
				return false;
			}
			return true;
		}
		//TRACE2("NO");
	}
	// 見つからなかった場合
	if( i == m_vsCmdParamName.size() ) {
		string msg = "ERROR COMMANDS: " + command;
		TRACE2( msg.c_str() );
		return false;
	}
	return true;
}


bool ReadCommand::ReadCommandSet()
{
	TRACE( "bool ReadCommand::ReadCommandSet()" );
	FILE *fp;
	
	char temp_cmd_param[256];
	char temp_cmd_slot[256];
	char temp_cmd_method_name[256];
	int temp_cmd_method_num;
	int temp_cmd_method_sub_num;
	
	if(( fp = fopen(COMMAND_SET_FILE, "r" )) == NULL ) {
		cout << "Cannot open command set file: " << COMMAND_SET_FILE << endl;
		return false;
	}
	
	while(!feof(fp)) {
		
		fscanf(fp, "%s\t%s\t%s\t%d\t%d", 
			temp_cmd_param, temp_cmd_slot, temp_cmd_method_name,
			&temp_cmd_method_num, &temp_cmd_method_sub_num );
		
		m_vsCmdParamName.push_back( temp_cmd_param );
		m_vsCmdSlotName.push_back( temp_cmd_slot );
		m_vsCmdMethodName.push_back( temp_cmd_method_name );
		m_vsCmdMethodNum.push_back( temp_cmd_method_num );
		m_vsCmdMethodSubNum.push_back( temp_cmd_method_sub_num );
		
#if 0
		fprintf(stderr, "DEBUG2: %s, %s, %s, %d, %d\n", 
			temp_cmd_param, temp_cmd_slot, temp_cmd_method_name,
			temp_cmd_method_num, temp_cmd_method_sub_num );
#endif
	}
	fclose(fp);
	
	return true;
}

void ReadCommand::SplitToken(string buffer)
{
	TRACE( "void ReadCommand::SplitToken(string buffer)" );
	
	// すべての有無を言わさず要素をすべて削除する (^^;
	m_vsToken.clear();
	
	int pos = 0, prepos = 0;
	
	buffer.insert( buffer.size(), " " );
	
	// 文字列の最後までトークンを検索する。
	while( buffer.npos != ( pos = buffer.find_first_of( " ", prepos ) ))  {
		if( prepos != pos  ) { // 文字が空でなかったら配列に登録
			// 文字列を切り出す
			string str = buffer.substr( prepos,  pos - prepos );
			// 配列に追加
			m_vsToken.push_back( str );
			// 検索開始位置を変更
			prepos = pos + 1 ;
		} 
		else {
			// 検索位置を１文字進める
			prepos++ ;
		}
	}
	
#if 0
	// 結果を出力する
	for( int i = 0; i < m_vsToken.size() ; i++ ) {
		printf( "Split Token. %02d = %s \n", i, m_vsToken[i].c_str() );
	}
#endif
}

int ReadCommand::CheckToken() {
	if(m_vsToken.size() < 4) {
		return 0;
	}
	
	if(m_vsToken[2] == "=") {
		return 1;
	} 
	else if( m_vsToken[2] == "<" ) {
		return 2;
	}
	else if( m_vsToken[2] == "<<" ) {
		return 3;
	}
	return 0;
}

bool ReadCommand::CheckTokenNum( int num ) {
	if(m_vsToken.size() != num) {
		return false;
	}
	return true;
}

void ReadCommand::SendMessageCommand( string message )
{
	string sendMessage = "rep " + message + "\n";
	printf( sendMessage.c_str() );
#ifdef WIN32
#ifdef WIN32_GALATEA
	sendMessage = "rep " + message + "./\n";
	SendMsg(sendMessage.c_str());
#endif
#endif
}

void ReadCommand::NowTime( int &day, int &hr, int &mm, int &ss, int &ms )
{
#ifdef WIN32
	int nowtime = timeGetTime();	
	ms =          nowtime % 1000;
	ss =        ((nowtime - ms) / 1000) % 60;
	mm =      ((((nowtime - ms) / 1000) - ss) / 60) % 60;
	hr =    ((((((nowtime - ms) / 1000) - ss) / 60) - mm ) / 60) % 24;
	day = ((((((((nowtime - ms) / 1000) - ss) / 60) - mm ) / 60) - hr ) / 24);
#else
	timeval nowtime;
	struct timezone nowtime_zone;
	
	gettimeofday(&nowtime, &nowtime_zone);
	
	ms = nowtime.tv_usec / 1000;
	ss = nowtime.tv_sec % 60;
	mm =      ((nowtime.tv_sec - ss) / 60) % 60;
	hr =    ((((nowtime.tv_sec - ss) / 60) - mm ) / 60 ) % 24;
	day = ((((((nowtime.tv_sec - ss) / 60) - mm ) / 60 ) - hr ) / 24 );
#endif
}


bool ReadCommand::FuncCommentOut( int dummy )
{
	TRACE2("> \"#\" Commnent Out");
	return true;
}

bool ReadCommand::FuncInqProtocolVersion( int dummy )
{
	string message = "ProtocolVersion = " + GetProtocolVersionStr();
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncInqModuleVersion( int dummy )
{
	string message = "ModuleVersion = " + GetModuleVersionStr();
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncInqRun( int dummy )
{
	string message = "Run = " + GetRunStr();
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncInqLipSync( int num )
{
	string message;
	
	switch( num ) {
	case 0:
		message = "LipSync.pho = " + GetLipSyncPhoStr();
		SendMessageCommand( message );
		message = "LipSync.dur = " + GetLipSyncDurStr();
		SendMessageCommand( message );
		break;
	case 1:
		message = "LipSync.pho = " + GetLipSyncPhoStr();
		SendMessageCommand( message );
		break;
	case 2:
		message = "LipSync.dur = " + GetLipSyncDurStr();
		SendMessageCommand( message );
		break;
	default:
		break;
	}
	return true;
}

bool ReadCommand::FuncInqSpeak( int num )
{
	string message;
	
	switch( num ) {
	case 0:		// slot name: Speak
		message = "Speak.text = " + GetSpeakTextStr();
		SendMessageCommand( message );
		message = "Speak.pho = " + GetSpeakPhoStr();
		SendMessageCommand( message );
		message = "Speak.dur = " + GetSpeakDurStr();
		SendMessageCommand( message );
		message = "Speak.utt = " + GetSpeakUttStr();
		SendMessageCommand( message );
		message = "Speak.len = " + GetSpeakLenStr();
		SendMessageCommand( message );
		message = "Speak.stat = " + GetSpeakStatStr();
		SendMessageCommand( message );
		message = "Speak.syncoffset = " +  GetSpeakSyncOffsetStr();
		SendMessageCommand( message );
		break;
	case 1:		// slot name: Speak.text
		message = "Speak.text = " + GetSpeakTextStr();
		SendMessageCommand( message );
		break;
	case 2:		// slot name: Speak.pho
		message = "Speak.pho = " + GetSpeakPhoStr();
		SendMessageCommand( message );
		break;
	case 3:		// slot name: Speak.dur
		message = "Speak.dur = " + GetSpeakDurStr();
		SendMessageCommand( message );
		break;
	case 4:		// slot name: Speak.utt
		message = "Speak.utt = " + GetSpeakUttStr();
		SendMessageCommand( message );
		break;
	case 5:		// slot name: Speak.len
		message = "Speak.len = " + GetSpeakLenStr();
		SendMessageCommand( message );
		break;
	case 6:		// slot name: Speak.stat
		message = "Speak.stat = " + GetSpeakStatStr();
		SendMessageCommand( message );
		break;
	case 7:		// slot name: Speak.syncoffset
		message = "Speak.syncoffset = " +  GetSpeakSyncOffsetStr();
		SendMessageCommand( message );
		break;
	default:
		break;
	}
	return true;
}

bool ReadCommand::FuncInqMaskSet( int dummy )
{
	string message = "MaskSet = \"";
	
	for( int i = 0; i < GetMaskNum(); i++ ) {
		message = message + GetMaskName(i) + ",";
	}
	message.erase( message.size() - 1 );
	message = message + "\"";
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncInqNowMask( int dummy )
{
	string message = "NowMask = \"";
	
	message = message + GetNowMaskName();
	message = message + "\"";
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncInqNowBackground( int dummy )
{
	string message = "NowBackground = \"";
	
	message = message + GetNowBGName();
	message = message + "\"";
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncInqBackgroundSet( int dummy )
{
	string message = "BackgroundSet = \"";
	
	for( int i = 0; i < GetBGNum(); i++ ) {
		message = message + GetBGName(i) + ",";
	}
	message.erase( message.size() - 1 );
	message = message + "\"";
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncSetFace( int num )
{
	double value;
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		TRACE2("Not Support... sorry");
		return false;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( num < 0 || num > MAX_AU_NUMBER ) return false;
		value = atof( m_vsToken[3].c_str() );
		SetFace( num - 1, (float)(value / 100.0) );
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetMouth( int num )
{
	double value;
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( num < 0 || num > MAX_MU_NUMBER ) return false;
		value = atof( m_vsToken[3].c_str() );
		SetMouth( num - 1, (float)(value / 100.0) );
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetHeadRotRel( int num )
{
	float value[3];
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		value[0] = (float)atof( m_vsToken[3].c_str() );
		value[1] = (float)atof( m_vsToken[4].c_str() );
		value[2] = (float)atof( m_vsToken[5].c_str() );
		SetHeadRotRel( num, value[0], value[1], value[2] );
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetHeadRotAbs( int num )
{
	float value[3];
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		value[0] = (float)atof( m_vsToken[3].c_str() );
		value[1] = (float)atof( m_vsToken[4].c_str() );
		value[2] = (float)atof( m_vsToken[5].c_str() );
		SetHeadRotAbs( num, value[0], value[1], value[2] );
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetFaceMot( int dummy )
{
	int value;
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( m_vsToken[3] == GetFaceMotMotionStr(NOMOVE) ) {
			SetFaceMot( NOMOVE, 0 );
		}
		else {
			if( CheckTokenNum(5) == false ) return false;
			value = atoi( m_vsToken[4].c_str() );
			for( int i = 0; i < MAX_FACE_MOT_NUMBER; i++ ) {
				if( m_vsToken[3] == GetFaceMotMotionStr(i) ) {
					SetFaceMot( i, value );
					break;
				}
				if( i ==  MAX_FACE_MOT_NUMBER ) {
					return false;
				}
			}
		}
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetFaceExp( int dummy )
{
	int value[3];
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( m_vsToken[3] == GetFaceExpExpStr(NEUTRAL) ) {
			SetFaceExp( NEUTRAL, 0, 0, 0 );
		}
		else {
			if( CheckTokenNum(7) == false ) return false;
			value[0] = atoi( m_vsToken[4].c_str() );
			value[1] = atoi( m_vsToken[5].c_str() );
			value[2] = atoi( m_vsToken[6].c_str() );
			for( int i = 0; i < MAX_FACE_EXP_NUMBER; i++ ) {
				if( m_vsToken[3] == GetFaceExpExpStr(i) ) {
					SetFaceExp( i, value[0], value[1], value[2] );
					break;
				}
				if( i == MAX_FACE_EXP_NUMBER ) {
					return false;
				}
			}
		}
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetLipSync( int num ) 
{
	int token_size;
	int value;
	string s;
	
	int flag = CheckToken();
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( num == 0 ) {        // slot name: LipSync
			return false; 
		}
		else if( num == 1 ) {   // slot name: LipSync.pho
			if( GetSpeakStat() == SPEAKING ) {
				return false;
			}
			else {
				token_size = m_vsToken.size() - 3;
				if( token_size % 2 == 1 || token_size <= 0 ) return false;
				SendToLipSyncOld();   // 入れ替え
				for( int i = 3; i < token_size + 3; i += 2 ) {
					value = atoi( m_vsToken[i + 1].c_str() );
					PushBackLipSync( m_vsToken[i], value );
				}
				SetTotalLipSync();
				SetSpeakStat( PROCESSING );
				
				// check Speak prop ( text, pho, dur, utt, len, stat )
				if( GetSpeakTextProp() == true )
					AnalysisCmd( "inq Speak.text" );
				if( GetSpeakPhoProp() == true )
					AnalysisCmd( "inq Speak.pho" );
				if( GetSpeakDurProp() == true )
					AnalysisCmd( "inq Speak.dur" );
				if( GetSpeakUttProp() == true )
					AnalysisCmd( "inq Speak.utt" );
				if( GetSpeakLenProp() == true )
					AnalysisCmd( "inq Speak.len" );
				if( GetSpeakStatProp() == true )
					AnalysisCmd( "inq Speak.stat" );
			} 
		}
		else if( num == 2 ) {   // slot name: LipSync.dur
			return false;
		} 
		else {
			return false;
		}
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetSpeak( int num )
{
	int value;
	int flag = CheckToken();
	int time[5];
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( num == 0 ) {        // slot name: Speak	
			if( m_vsToken[3] == "NOW" ) {
				SetSpeak( 0, 0, 0, 0, 0, 0 );
			} 
			else if( m_vsToken[3] == "STOP" ) {
				SetSpeak( -1, 0, 0, 0, 0, 0 );
			}
			else {
				if( CheckTokenNum(5) == false ) return false;
				if( m_vsToken[3] != "+" ) return false;
				if( GetSpeakStat() != IDLE || GetSpeakStat() != SPEAKING ) {
					value = atoi( m_vsToken[4].c_str() );
					NowTime( time[0], time[1], time[2], time[3], time[4] );
					SetSpeak( value, time[0], time[1], time[2], time[3], time[4] );
				}
				else {
					return false;
				}
			}
		}
		else if( num == 1 ) {		// slot name: Speak.syncoffset
			SetSpeakSyncOffset(	atoi(m_vsToken[3].c_str()) );
		}
		else if( num == 2 ) {		// slot name: Speak.sync
			SetSpeakSyncTime( atoi(m_vsToken[3].c_str()) );
		}
		else {
			return false;
		}
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetMask( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( SetNowMaskName( m_vsToken[3] ) != true ) return false;
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetBackground( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( SetNowBGName( m_vsToken[3] ) != true ) return false;
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetAutonomous( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(5) == false ) return false;
		SetAutonomousPattern( m_vsToken[3], atoi(m_vsToken[4].c_str()));
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
	
	return true;
}

bool ReadCommand::FuncSetMouthScale( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		SetMouthScale( atoi( m_vsToken[3].c_str()));
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncInqMouthScale( int dummy ) 
{
	string message = "MouthScale = " + GetMouthScaleStr();
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncSetViewBackground( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		SetViewBackground( GetViewBackground(m_vsToken[3]));
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncInqViewBackground( int dummy ) 
{
	string message = "ViewBackground = " + GetViewBackgroundStr();
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncSetViewMode( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		SetViewMode( GetViewMode(m_vsToken[3]));
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncInqViewMode( int dummy ) 
{
	string message = "ViewMode = " + GetViewModeStr();
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncPropSpeak( int num )
{
	int flag = CheckToken();
	bool bool_flag;
	if( m_vsToken[3] == "AutoOutput" ) {
		bool_flag = true;
	} 
	else if( m_vsToken[3] == "NoAutoOutput" ) {
		bool_flag = false;
	}
	else {
		return false;
	}
	
	switch( flag ) {
	case 0:
		return false;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( num < 0 || num > 6 ) return false;
		
		switch( num ) {
		case 0:
			SetSpeakProp( bool_flag );
			break;
		case 1:
			SetSpeakTextProp( bool_flag );
			break;
		case 2:
			SetSpeakPhoProp( bool_flag );
			break;
		case 3:
			SetSpeakDurProp( bool_flag );
			break;
		case 4:
			SetSpeakUttProp( bool_flag );
			break;
		case 5:
			SetSpeakLenProp( bool_flag );
			break;
		case 6:
			SetSpeakStatProp( bool_flag );
			break;
		default:
			return false;
			break;
		}
		break;
		
		case 2:
		case 3:
			return false;
			break;
			
		default:
			return false;
			break;
	}
	return true;
}

bool ReadCommand::FuncSetSaveImage( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		SetSaveImageFlag( true );
		SetSaveImageFile( m_vsToken[3] );
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetEyeRotLeft( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		SetEyeRotLeft( atof( m_vsToken[3].c_str()), atof(m_vsToken[4].c_str()), atoi(m_vsToken[5].c_str()));
		SetEyeRotLeftFlag(true);
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetEyeRotRight( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		SetEyeRotRight( atof( m_vsToken[3].c_str()), atof(m_vsToken[4].c_str()), atoi(m_vsToken[5].c_str()));
		SetEyeRotRightFlag(true);
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetEyeRot( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		SetEyeRotRight( atof( m_vsToken[3].c_str()), atof(m_vsToken[4].c_str()), atoi(m_vsToken[5].c_str()));
		SetEyeRotLeft( atof( m_vsToken[3].c_str()), atof(m_vsToken[4].c_str()), atoi(m_vsToken[5].c_str()));
		SetEyeRotRightFlag(true);
		SetEyeRotLeftFlag(true);
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetRun( int dummy )
{
	int flag = CheckToken();
	string s_init = SET_RUN_NAME[0];
	string s_exit = SET_RUN_NAME[1];
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( s_init == m_vsToken[3] ) {
			SetDefalut();
		}
		if( s_exit == m_vsToken[3] ) {
			exit(1);
		}
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}


bool ReadCommand::FuncSetAgentRot( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		SetAgentRot( atof( m_vsToken[3].c_str()), atof(m_vsToken[4].c_str()), atoi(m_vsToken[5].c_str()));
		break;
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetAgentTrans( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(5) == false ) return false;
		SetAgentTrans( atof( m_vsToken[3].c_str()), atof(m_vsToken[4].c_str()));
		break;
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetAgentScale( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		SetAgentScale( atof( m_vsToken[3].c_str()));
		break;
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetAgentEnable( int dummy )
{
	int flag = CheckToken();
	string s_enable = AGENT_ENABLE_NAME[0];
	string s_disable = AGENT_ENABLE_NAME[1];
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		if( s_enable == m_vsToken[3] ) {
			SetAgentEnable(true);
		}
		if( s_disable == m_vsToken[3] ) {
			SetAgentEnable(false);
		}
		break;
		
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncSetAgentAlpha( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		SetAgentAlpha((float)atof( m_vsToken[3].c_str()));
		break;
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncInqCapture( int dummy )
{
	string message = "Capture = ";
	if( this->GetFSMCaptureEnable() == true ) {
		message = message + "ENABLE";
	}
	else {
		message = message + "DISABLE";
	}
	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncSetCapture( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(6) == false ) return false;
		this->SetFSMCapture(m_vsToken[3], m_vsToken[4], atoi(m_vsToken[5].c_str()));
		break;
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}

bool ReadCommand::FuncInqOffset( int dummy )
{
	string message = "Offset = ";
	message = message + this->GetOffsetStr();

	SendMessageCommand( message );
	return true;
}

bool ReadCommand::FuncSetOffset( int dummy )
{
	int flag = CheckToken();
	
	switch( flag ) {
	case 0:
		return false;
		break;
		
	case 1:
		if( CheckTokenNum(4) == false ) return false;
		this->SetOffset((float)atof( m_vsToken[3].c_str()));
		break;
	case 2:
	case 3:
		return false;
		break;
		
	default:
		return false;
		break;
	}
	return true;
}



// EOF (ReadCommand.cpp)
