/*++
Configuration.c++
--*/

#include <stdlib.h>
#include "Configuration.h"
#include "trace.h"


Configuration::Configuration( string configFile )
{   
	TRACE("Configuration::~Configuration()");
	
	FILE *fp;
	char buf[256];

	// initialization
	m_iAutonomousMove = 0;
	m_iAutonomousUserDef = 0;
	m_iFaceMotNod = 0;
	m_iFaceMotListen = 0;
	m_iFaceMotRefuse = 0;
	m_iFaceMotUserDef = 0;
	m_iFaceMotBlink = 0;

	m_iMaxFrameRate = DEFALUT_MAX_FPS;
	m_sVisemeFile = VISEME_JP_FILE;

	m_fFaceMotAngle[0] = m_fFaceMotAngle[1] = m_fFaceMotAngle[2] = 0.0;
	m_fAutonomousMoveAngle[0] = m_fAutonomousMoveAngle[1] = m_fAutonomousMoveAngle[2] = 0.0;

	// Resources.h に書かれている内容をコピー
	m_sUpperTeethObjName = U_TEETH_OBJ_NAME;
	m_sLowerTeethObjName = L_TEETH_OBJ_NAME;
	m_sMouthWallObjName = M_WALL_OBJ_NAME;
	m_sEyeObjName = EYE_OBJ_NAME;
	m_sMouthWallTexture = M_WALL_TEXTURE;
	m_sDefWfmPnt = DEF_WFM_PNT;
	m_sDefWfmLnk = DEF_WFM_LNK;
	m_dDefWindowWidth = DEF_WINDOW_WIDTH;
	m_dDefWindowHeight = DEF_WINDOW_HEIGHT;
	m_dDefWindowPosX = DEF_WINDOW_POS_X;
	m_dDefWindowPosY = DEF_WINDOW_POS_Y;
	m_dExpressionDuration = EXPRESSION_DURATION;
	m_dEyeMoveDuration = EYEMOVE_DURATION;

	// Open Configuration File  
	if(( fp = fopen( configFile.c_str(), "r" )) == NULL ) {
		cout << "Cannot open configration file: " 
			<< configFile.c_str() << endl;
		exit(1);
	}
	
	// 一行読み込み
	while( fgets( buf, 256, fp ) != NULL ) {
		string s_buf = buf;
		SplitToken( s_buf );
		
		if( m_vsToken[0] == "#" ) {
			// cout << "Comment out \n" << endl;
		}
		else if( m_vsToken[0] == "U_TEETH_OBJ_NAME") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sUpperTeethObjName = m_vsToken[1];
		}
		else if( m_vsToken[0] == "L_TEETH_OBJ_NAME") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sLowerTeethObjName = m_vsToken[1];
		}
		else if( m_vsToken[0] == "M_WALL_OBJ_NAME") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sMouthWallObjName = m_vsToken[1];
		}
		else if( m_vsToken[0] == "EYE_OBJ_NAME") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sEyeObjName = m_vsToken[1];
		}
		else if( m_vsToken[0] == "M_WALL_TEXTURE") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sMouthWallTexture = m_vsToken[1];
		}
		else if( m_vsToken[0] == "DEF_WFM_PNT") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sDefWfmPnt = m_vsToken[1];
		}
		else if( m_vsToken[0] == "DEF_WFM_LNK") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_sDefWfmLnk = m_vsToken[1];
		}
		else if( m_vsToken[0] == "DEF_WINDOW_WIDTH") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_dDefWindowWidth = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == "DEF_WINDOW_HEIGHT") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_dDefWindowHeight = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == "DEF_WINDOW_POS_X") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_dDefWindowPosX = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == "DEF_WINDOW_POS_Y") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_dDefWindowPosY = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == "EXPRESSION_DURATION") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_dExpressionDuration = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == "EYEMOVE_DURATION") {
			cout << m_vsToken[0] << " " << m_vsToken[1] << endl;
			m_dEyeMoveDuration = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == DEF_MASKFILE ) {
			m_vsMaskName.push_back( m_vsToken[1] );
			m_vsMaskRgbFile.push_back( m_vsToken[2] );
			m_vsMaskPntFile.push_back( m_vsToken[3] );
		}
		else if( m_vsToken[0] == DEF_MASKEYE ) {
			m_vsMaskEyeName.push_back( m_vsToken[1] );
			m_vsMaskEyeImageFile.push_back( m_vsToken[2] );
			m_vsMaskEyeDefFile.push_back( m_vsToken[3] );
		}
		else if( m_vsToken[0] == DEF_MASKTEETH ) {
			m_vsMaskTeethName.push_back( m_vsToken[1] );
			m_vsMaskTeethDefFile.push_back( m_vsToken[2] );
		}
		else if( m_vsToken[0] == DEF_BGFILE ) {
			m_vsBGName.push_back( m_vsToken[1] );
			m_vsBGRgbFile.push_back( m_vsToken[2] );
		}
		else if( m_vsToken[0] == DEF_AUTONOMOUS_BLINK ) {
			m_viAutonomousBlink.push_back( atoi(m_vsToken[1].c_str()) );
			m_viAutonomousBlinkPattern.push_back( atoi(m_vsToken[2].c_str()) );
		}
		else if( m_vsToken[0] == DEF_AUTONOMOUS_MOVE ) {
			OpenAutonomousMoveFile( m_vsToken[1], MOVE );
		}
		else if( m_vsToken[0] == DEF_AUTONOMOUS_USERDEF ) {
			OpenAutonomousMoveFile( m_vsToken[1], USERDEF_AUTONOMOUS );
		}
		else if( m_vsToken[0] == DEF_FACEMOT_NOD ) {
			OpenFaceMotFile( m_vsToken[1], NOD );
		}
		else if( m_vsToken[0] == DEF_FACEMOT_LISTEN ) {
			OpenFaceMotFile( m_vsToken[1], LISTEN );
		}
		else if( m_vsToken[0] == DEF_FACEMOT_REFUSE ) {
			OpenFaceMotFile( m_vsToken[1], REFUSE );
		}
		else if( m_vsToken[0] == DEF_FACEMOT_USERDEF ) {
			OpenFaceMotFile( m_vsToken[1], USERDEF_FACEMOT);
		}
		else if( m_vsToken[0] == DEF_FACEMOT_BLINK ) {
			OpenFaceMotBlinkFile(atoi(m_vsToken[1].c_str()));
		}
		else if( m_vsToken[0] == DEF_MAX_FRAME_RATE ) {
			m_iMaxFrameRate = atoi(m_vsToken[1].c_str());
		}
		else if( m_vsToken[0] == DEF_SET_VISEME ) {
			if( m_vsToken[1] == "JP" ) {
				m_sVisemeFile = VISEME_JP_FILE;
				cout << "config: Japanese viseme mode" << endl;
			}
			else if( m_vsToken[1] == "EN" ) {
				m_sVisemeFile = VISEME_EN_FILE;
				cout << "config: English viseme mode" << endl;
			}
			else {
				cout << "error: unknown viseme mode - " << m_vsToken[1] << endl;
			}
		}
	}
	fclose(fp);
}

Configuration::~Configuration()
{
	TRACE("Configuration::~Configuration()");
}


bool Configuration::OpenFaceMotBlinkFile( int frame_num )
{
	TRACE("Configuration::OpenFaceMotBlinkFile( int frame_num )");
	// FaceMoveのIDの個数を増やす
	m_iFaceMotBlink++;
	m_viFaceMotBlink.resize(m_iFaceMotBlink);

	// even and odd check
	int evenORodd = frame_num % 2;
	int value = 100;

	int i;
	int count = (frame_num - evenORodd) / 2;
	float div = (float)value / (float)count;

	for( i = 0; i < count; i++ ) {
		// 強度は0 - 100迄
		m_viFaceMotBlink[m_iFaceMotBlink - 1].push_back((int)(div * (i + 1)));
	}
	if( evenORodd == 1 ) {
		m_viFaceMotBlink[m_iFaceMotBlink - 1].push_back(value);
	}
	for( i = 0; i < count; i++ ) {
		m_viFaceMotBlink[m_iFaceMotBlink - 1].push_back((int)(100.0 - div * (i + 1)));
	}
	// 最後の値は必ず0にする．
	m_viFaceMotBlink[m_iFaceMotBlink - 1][m_viFaceMotBlink[m_iFaceMotBlink - 1].size() - 1] = 0;

	return true;
}

bool Configuration::OpenAutonomousMoveFile(string buffer, int num)
{
	TRACE("Configuration::OpenAutonomousMovFile(string buffer, int num)");
	
	FILE *fp;
	char buf[512];
	
	// Open Autonomous Move File  
	if(( fp = fopen( buffer.c_str(), "r" )) == NULL ) {
		cout << "Cannot open configration file: " 
			<< buffer.c_str() << endl;
		return false;
	}
	
	// Move File の個数を増やす
	switch( num ) {
	case MOVE: // MOVE
		m_iAutonomousMove++;
		m_viAutonomousMove_frame.resize(m_iAutonomousMove);
		m_vfAutonomousMove_x.resize(m_iAutonomousMove);
		m_vfAutonomousMove_y.resize(m_iAutonomousMove);
		m_vfAutonomousMove_z.resize(m_iAutonomousMove);
		break;
	case USERDEF_AUTONOMOUS: // USERDEF
		m_iAutonomousUserDef++;
		m_viAutonomousUserDef_frame.resize(m_iAutonomousUserDef);
		m_vfAutonomousUserDef_x.resize(m_iAutonomousUserDef);
		m_vfAutonomousUserDef_y.resize(m_iAutonomousUserDef);
		m_vfAutonomousUserDef_z.resize(m_iAutonomousUserDef);
		break;
	}

	int i;
	int total_frame_num, frame;

	float rot_x, rot_y, rot_z;
	// 1行目 comment
	fgets( buf, 512, fp );
	// 2行目
	fgets( buf, 512, fp );
	sscanf( buf, "%d", &total_frame_num );
	// 3行目 comment
	// 4行目 comment
	fgets( buf, 512, fp );
	fgets( buf, 512, fp );

	// 5行目以降
	for( i = 0; i < total_frame_num; i++ ) {
		if( fgets( buf, 512, fp ) == NULL ) {
			cout << "cannot find parameter!!" << endl;
			return false;
		}
		sscanf( buf, "%d %f %f %f", &frame, &rot_x, &rot_y, &rot_z );

		switch( num ) {
		case MOVE:
			m_viAutonomousMove_frame[m_iAutonomousMove - 1].push_back(frame);
			m_vfAutonomousMove_x[m_iAutonomousMove - 1].push_back(rot_x);
			m_vfAutonomousMove_y[m_iAutonomousMove - 1].push_back(rot_y);
			m_vfAutonomousMove_z[m_iAutonomousMove - 1].push_back(rot_z);
			break;
		case USERDEF_AUTONOMOUS:
			m_viAutonomousUserDef_frame[m_iAutonomousUserDef - 1].push_back(frame);
			m_vfAutonomousUserDef_x[m_iAutonomousUserDef - 1].push_back(rot_x);
			m_vfAutonomousUserDef_y[m_iAutonomousUserDef - 1].push_back(rot_y);
			m_vfAutonomousUserDef_z[m_iAutonomousUserDef - 1].push_back(rot_z);
			break;
		}
	}
	
	fclose(fp);
	return true;
}

bool Configuration::OpenFaceMotFile(string buffer, int num)
{
	TRACE("Configuration::OpenFaceMotFile(string buffer, int num)");
	
	FILE *fp;
	char buf[512];
	
	// Open Autonomous Move File  
	if(( fp = fopen( buffer.c_str(), "r" )) == NULL ) {
		cout << "Cannot open configration file: " 
			<< buffer.c_str() << endl;
		return false;
	}
	
	// Move File の個数を増やす
	switch( num ) {
	case NOD: // NOD
		m_iFaceMotNod++;
		m_viFaceMotNod_frame.resize(m_iFaceMotNod);
		m_vfFaceMotNod_x.resize(m_iFaceMotNod);
		m_vfFaceMotNod_y.resize(m_iFaceMotNod);
		m_vfFaceMotNod_z.resize(m_iFaceMotNod);
		break;
	case LISTEN: // LISTEN
		m_iFaceMotListen++;
		m_viFaceMotListen_frame.resize(m_iFaceMotListen);
		m_vfFaceMotListen_x.resize(m_iFaceMotListen);
		m_vfFaceMotListen_y.resize(m_iFaceMotListen);
		m_vfFaceMotListen_z.resize(m_iFaceMotListen);
		break;
	case REFUSE: // REDUSE
		m_iFaceMotRefuse++;
		m_viFaceMotRefuse_frame.resize(m_iFaceMotRefuse);
		m_vfFaceMotRefuse_x.resize(m_iFaceMotRefuse);
		m_vfFaceMotRefuse_y.resize(m_iFaceMotRefuse);
		m_vfFaceMotRefuse_z.resize(m_iFaceMotRefuse);
		break;
	case USERDEF_FACEMOT:	// USERDEF
		m_iFaceMotUserDef++;
		m_viFaceMotUserDef_frame.resize(m_iFaceMotUserDef);
		m_vfFaceMotUserDef_x.resize(m_iFaceMotUserDef);
		m_vfFaceMotUserDef_y.resize(m_iFaceMotUserDef);
		m_vfFaceMotUserDef_z.resize(m_iFaceMotUserDef);
		break;
	}

	int i;
	int total_frame_num, frame;

	float rot_x, rot_y, rot_z;
	// 1行目 comment
	fgets( buf, 512, fp );
	// 2行目
	fgets( buf, 512, fp );
	sscanf( buf, "%d", &total_frame_num );
	// 3行目 comment
	// 4行目 comment
	fgets( buf, 512, fp );
	fgets( buf, 512, fp );

	// 5行目以降
	for( i = 0; i < total_frame_num; i++ ) {
		if( fgets( buf, 512, fp ) == NULL ) {
			cout << "cannot find parameter!!" << endl;
			return false;
		}
		sscanf( buf, "%d %f %f %f", &frame, &rot_x, &rot_y, &rot_z );

		switch( num ) {
		case NOD:
			m_viFaceMotNod_frame[m_iFaceMotNod - 1].push_back(frame);
			m_vfFaceMotNod_x[m_iFaceMotNod - 1].push_back(rot_x);
			m_vfFaceMotNod_y[m_iFaceMotNod - 1].push_back(rot_y);
			m_vfFaceMotNod_z[m_iFaceMotNod - 1].push_back(rot_z);
			break;
		case LISTEN:
			m_viFaceMotListen_frame[m_iFaceMotListen - 1].push_back(frame);
			m_vfFaceMotListen_x[m_iFaceMotListen - 1].push_back(rot_x);
			m_vfFaceMotListen_y[m_iFaceMotListen - 1].push_back(rot_y);
			m_vfFaceMotListen_z[m_iFaceMotListen - 1].push_back(rot_z);
			break;
		case REFUSE:
			m_viFaceMotRefuse_frame[m_iFaceMotRefuse - 1].push_back(frame);
			m_vfFaceMotRefuse_x[m_iFaceMotRefuse - 1].push_back(rot_x);
			m_vfFaceMotRefuse_y[m_iFaceMotRefuse - 1].push_back(rot_y);
			m_vfFaceMotRefuse_z[m_iFaceMotRefuse - 1].push_back(rot_z);
			break;
		case USERDEF_FACEMOT:
			m_viFaceMotUserDef_frame[m_iFaceMotUserDef - 1].push_back(frame);
			m_vfFaceMotUserDef_x[m_iFaceMotUserDef - 1].push_back(rot_x);
			m_vfFaceMotUserDef_y[m_iFaceMotUserDef - 1].push_back(rot_y);
			m_vfFaceMotUserDef_z[m_iFaceMotUserDef - 1].push_back(rot_z);
			break;
		}
	}
	
	fclose(fp);
	return true;
}

void Configuration::SplitToken(string buffer)
{
	TRACE( "void Configuration::SplitToken(string buffer)" );
	
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
	
	m_vsToken[m_vsToken.size() - 1].erase(m_vsToken[m_vsToken.size() - 1].size() - 1 );
	
#if 0
	// 結果を出力する
	
	for( int i = 0; i < m_vsToken.size() ; i++ ) {
		printf( "Split Token. %02d = %s \n", i, m_vsToken[i].c_str() );
	}
#endif
}

void Configuration::GetAutonomousMoveCurrentFrame(int mode, int id, int frame)
{
	if( frame == 0 ) frame = 1;
	unsigned int i;
	float diff_x, diff_y, diff_z;
	int diff_time, now_frame;

	switch(mode) {
	case MOVE:
		for( i = 0; i < m_viAutonomousMove_frame[id].size(); i++ ) {
			if( frame < m_viAutonomousMove_frame[id][i] ) {
				if( i == 0 ) { // 一回目
					m_fAutonomousMoveAngle[0] = m_vfAutonomousMove_x[id][i] / m_viAutonomousMove_frame[id][i] * frame;
					m_fAutonomousMoveAngle[1] = m_vfAutonomousMove_y[id][i] / m_viAutonomousMove_frame[id][i] * frame;
					m_fAutonomousMoveAngle[2] = m_vfAutonomousMove_z[id][i] / m_viAutonomousMove_frame[id][i] * frame;
				} else {
					diff_x = m_vfAutonomousMove_x[id][i] - m_vfAutonomousMove_x[id][i-1];
					diff_y = m_vfAutonomousMove_y[id][i] - m_vfAutonomousMove_y[id][i-1];
					diff_z = m_vfAutonomousMove_z[id][i] - m_vfAutonomousMove_z[id][i-1];
					diff_time = m_viAutonomousMove_frame[id][i] - m_viAutonomousMove_frame[id][i-1];
					now_frame = frame - m_viAutonomousMove_frame[id][i-1];

					m_fAutonomousMoveAngle[0] =  diff_x / diff_time * now_frame + m_vfAutonomousMove_x[id][i-1];
					m_fAutonomousMoveAngle[1] =  diff_y / diff_time * now_frame + m_vfAutonomousMove_y[id][i-1];
					m_fAutonomousMoveAngle[2] =  diff_z / diff_time * now_frame + m_vfAutonomousMove_z[id][i-1];
				}
				return;
			}
		}
		m_fAutonomousMoveAngle[0] = m_fAutonomousMoveAngle[0] = m_fAutonomousMoveAngle[0] = 0.0;
		break;
	case USERDEF_AUTONOMOUS:
		for( i = 0; i < m_viAutonomousUserDef_frame[id].size(); i++ ) {
			if( frame < m_viAutonomousUserDef_frame[id][i] ) {
				if( i == 0 ) { // 一回目
					m_fAutonomousMoveAngle[0] = m_vfAutonomousUserDef_x[id][i] / m_viAutonomousUserDef_frame[id][i] * frame;
					m_fAutonomousMoveAngle[1] = m_vfAutonomousUserDef_y[id][i] / m_viAutonomousUserDef_frame[id][i] * frame;
					m_fAutonomousMoveAngle[2] = m_vfAutonomousUserDef_z[id][i] / m_viAutonomousUserDef_frame[id][i] * frame;
				} else {
					diff_x = m_vfAutonomousUserDef_x[id][i] - m_vfAutonomousUserDef_x[id][i-1];
					diff_y = m_vfAutonomousUserDef_y[id][i] - m_vfAutonomousUserDef_y[id][i-1];
					diff_z = m_vfAutonomousUserDef_z[id][i] - m_vfAutonomousUserDef_z[id][i-1];
					diff_time = m_viAutonomousUserDef_frame[id][i] - m_viAutonomousUserDef_frame[id][i-1];
					now_frame = frame - m_viAutonomousUserDef_frame[id][i-1];

					m_fAutonomousMoveAngle[0] =  diff_x / diff_time * now_frame + m_vfAutonomousUserDef_x[id][i-1];
					m_fAutonomousMoveAngle[1] =  diff_y / diff_time * now_frame + m_vfAutonomousUserDef_y[id][i-1];
					m_fAutonomousMoveAngle[2] =  diff_z / diff_time * now_frame + m_vfAutonomousUserDef_z[id][i-1];
				}
				return;
			}
		}
		m_fAutonomousMoveAngle[0] = m_fAutonomousMoveAngle[0] = m_fAutonomousMoveAngle[0] = 0.0;
		break;
	}
}

void Configuration::GetFaceMotCurrentFrame(int mode, int id, int frame)
{
	if( frame == 0 ) frame = 1;
	unsigned int i;
	float diff_x, diff_y, diff_z;
	int diff_time, now_frame;

	switch(mode) {
	case NOD:
		for( i = 0; i < m_viFaceMotNod_frame[id].size(); i++ ) {
			if( frame < m_viFaceMotNod_frame[id][i] ) {
				if( i == 0 ) { // 一回目
					m_fFaceMotAngle[0] = m_vfFaceMotNod_x[id][i] / m_viFaceMotNod_frame[id][i] * frame;
					m_fFaceMotAngle[1] = m_vfFaceMotNod_y[id][i] / m_viFaceMotNod_frame[id][i] * frame;
					m_fFaceMotAngle[2] = m_vfFaceMotNod_z[id][i] / m_viFaceMotNod_frame[id][i] * frame;
				} else {
					diff_x = m_vfFaceMotNod_x[id][i] - m_vfFaceMotNod_x[id][i-1];
					diff_y = m_vfFaceMotNod_y[id][i] - m_vfFaceMotNod_y[id][i-1];
					diff_z = m_vfFaceMotNod_z[id][i] - m_vfFaceMotNod_z[id][i-1];
					diff_time = m_viFaceMotNod_frame[id][i] - m_viFaceMotNod_frame[id][i-1];
					now_frame = frame - m_viFaceMotNod_frame[id][i-1];

					m_fFaceMotAngle[0] =  diff_x / diff_time * now_frame + m_vfFaceMotNod_x[id][i-1];
					m_fFaceMotAngle[1] =  diff_y / diff_time * now_frame + m_vfFaceMotNod_y[id][i-1];
					m_fFaceMotAngle[2] =  diff_z / diff_time * now_frame + m_vfFaceMotNod_z[id][i-1];
				}
				return;
			}
		}
		m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = 0.0;
		break;
	case LISTEN:
		for( i = 0; i < m_viFaceMotListen_frame[id].size(); i++ ) {
			if( frame < m_viFaceMotListen_frame[id][i] ) {
				if( i == 0 ) { // 一回目
					m_fFaceMotAngle[0] = m_vfFaceMotListen_x[id][i] / m_viFaceMotListen_frame[id][i] * frame;
					m_fFaceMotAngle[1] = m_vfFaceMotListen_y[id][i] / m_viFaceMotListen_frame[id][i] * frame;
					m_fFaceMotAngle[2] = m_vfFaceMotListen_z[id][i] / m_viFaceMotListen_frame[id][i] * frame;
				} else {
					diff_x = m_vfFaceMotListen_x[id][i] - m_vfFaceMotListen_x[id][i-1];
					diff_y = m_vfFaceMotListen_y[id][i] - m_vfFaceMotListen_y[id][i-1];
					diff_z = m_vfFaceMotListen_z[id][i] - m_vfFaceMotListen_z[id][i-1];
					diff_time = m_viFaceMotListen_frame[id][i] - m_viFaceMotListen_frame[id][i-1];
					now_frame = frame - m_viFaceMotListen_frame[id][i-1];

					m_fFaceMotAngle[0] =  diff_x / diff_time * now_frame + m_vfFaceMotListen_x[id][i-1];
					m_fFaceMotAngle[1] =  diff_y / diff_time * now_frame + m_vfFaceMotListen_y[id][i-1];
					m_fFaceMotAngle[2] =  diff_z / diff_time * now_frame + m_vfFaceMotListen_z[id][i-1];
				}
				return;
			}
		}
		m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = 0.0;
		break;
	case REFUSE:
		for( i = 0; i < m_viFaceMotRefuse_frame[id].size(); i++ ) {
			if( frame < m_viFaceMotRefuse_frame[id][i] ) {
				if( i == 0 ) { // 一回目
					m_fFaceMotAngle[0] = m_vfFaceMotRefuse_x[id][i] / m_viFaceMotRefuse_frame[id][i] * frame;
					m_fFaceMotAngle[1] = m_vfFaceMotRefuse_y[id][i] / m_viFaceMotRefuse_frame[id][i] * frame;
					m_fFaceMotAngle[2] = m_vfFaceMotRefuse_z[id][i] / m_viFaceMotRefuse_frame[id][i] * frame;
				} else {
					diff_x = m_vfFaceMotRefuse_x[id][i] - m_vfFaceMotRefuse_x[id][i-1];
					diff_y = m_vfFaceMotRefuse_y[id][i] - m_vfFaceMotRefuse_y[id][i-1];
					diff_z = m_vfFaceMotRefuse_z[id][i] - m_vfFaceMotRefuse_z[id][i-1];
					diff_time = m_viFaceMotRefuse_frame[id][i] - m_viFaceMotRefuse_frame[id][i-1];
					now_frame = frame - m_viFaceMotRefuse_frame[id][i-1];

					m_fFaceMotAngle[0] =  diff_x / diff_time * now_frame + m_vfFaceMotRefuse_x[id][i-1];
					m_fFaceMotAngle[1] =  diff_y / diff_time * now_frame + m_vfFaceMotRefuse_y[id][i-1];
					m_fFaceMotAngle[2] =  diff_z / diff_time * now_frame + m_vfFaceMotRefuse_z[id][i-1];
				}
				return;
			}
		}
		m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = 0.0;
		break;
	case USERDEF_FACEMOT:
		for( i = 0; i < m_viFaceMotUserDef_frame[id].size(); i++ ) {
			if( frame < m_viFaceMotUserDef_frame[id][i] ) {
				if( i == 0 ) { // 一回目
					m_fFaceMotAngle[0] = m_vfFaceMotUserDef_x[id][i] / m_viFaceMotUserDef_frame[id][i] * frame;
					m_fFaceMotAngle[1] = m_vfFaceMotUserDef_y[id][i] / m_viFaceMotUserDef_frame[id][i] * frame;
					m_fFaceMotAngle[2] = m_vfFaceMotUserDef_z[id][i] / m_viFaceMotUserDef_frame[id][i] * frame;
				} else {
					diff_x = m_vfFaceMotUserDef_x[id][i] - m_vfFaceMotUserDef_x[id][i-1];
					diff_y = m_vfFaceMotUserDef_y[id][i] - m_vfFaceMotUserDef_y[id][i-1];
					diff_z = m_vfFaceMotUserDef_z[id][i] - m_vfFaceMotUserDef_z[id][i-1];
					diff_time = m_viFaceMotUserDef_frame[id][i] - m_viFaceMotUserDef_frame[id][i-1];
					now_frame = frame - m_viFaceMotUserDef_frame[id][i-1];

					m_fFaceMotAngle[0] =  diff_x / diff_time * now_frame + m_vfFaceMotUserDef_x[id][i-1];
					m_fFaceMotAngle[1] =  diff_y / diff_time * now_frame + m_vfFaceMotUserDef_y[id][i-1];
					m_fFaceMotAngle[2] =  diff_z / diff_time * now_frame + m_vfFaceMotUserDef_z[id][i-1];
				}
				return;
			}
		}
		m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = m_fFaceMotAngle[0] = 0.0;
		break;
	}
}

int Configuration::GetAutonomousMoveSize(int mode)
{
	int ans;
	switch(mode) {
	case MOVE:
		ans = m_iAutonomousMove;
		break;
	case USERDEF_AUTONOMOUS:
		ans = m_iAutonomousUserDef;
		break;
	}
	return ans;
}

int Configuration::GetAutonomousMoveFrameSize(int mode, int id)
{
	int ans;
	switch(mode) {
	case MOVE:
		ans = m_viAutonomousMove_frame[id][m_viAutonomousMove_frame[id].size() - 1];
		break;
	case USERDEF_AUTONOMOUS:
		ans = m_viAutonomousUserDef_frame[id][m_viAutonomousUserDef_frame[id].size() - 1];
		break;
	}
	return ans;
}

int Configuration::GetFaceMotSize(int mode)
{
	int ans;
	switch(mode) {
	case NOD:
		ans = m_iFaceMotNod;
		break;
	case REFUSE:
		ans = m_iFaceMotRefuse;
		break;
	case LISTEN:
		ans = m_iFaceMotListen;
		break;
	case USERDEF_FACEMOT:
		ans = m_iFaceMotUserDef;
		break;
	}
	return ans;
}

int Configuration::GetFaceMotFrameSize(int mode, int id)
{
	int ans;
	switch(mode) {
	case NOD:
		ans = m_viFaceMotNod_frame[id][m_viFaceMotNod_frame[id].size() - 1];
		break;
	case REFUSE:
		ans = m_viFaceMotRefuse_frame[id][m_viFaceMotRefuse_frame[id].size() - 1];
		break;
	case LISTEN:
		ans = m_viFaceMotListen_frame[id][m_viFaceMotListen_frame[id].size() - 1];
		break;
	case USERDEF_FACEMOT:
		ans = m_viFaceMotUserDef_frame[id][m_viFaceMotUserDef_frame[id].size() - 1];
		break;
	}
	return ans;
}



