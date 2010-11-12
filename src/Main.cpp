#include "Main.h"

#include <stdio.h>
#include <stdlib.h>

#include "FaceEditApp.h"
#include "FaceModel.h"
#include "SetViseme.h"
#include "ReadCommand.h"
#include "UnitMove.h"
#include "Configuration.h"
#include "trace.h"

// additional: for galatea-mmi system
#ifdef WIN32
#ifdef WIN32_GALATEA
#include "Sock.h"
CSock g_Socket;

void RecvThread(void *pParam)
{
	int nRecv;								// 受信したサイズを保持
	std::string strMsg = "";				// 受信データを格納する
	char szBuff[RECV_BUFFER_SIZE+1];		// 受信用バッファ

	while(1)
	{
		szBuff[0] = '\0';
		nRecv = g_Socket.Recv( szBuff );
		if( nRecv == SOCKET_ERROR )
		{
			int ret = WSAGetLastError();
			exit(ret);
		}

		strMsg.append( szBuff, nRecv );	
		int nIndex = strMsg.find("./\n");
		while(nIndex != -1)
		{
			std::string strCommand = strMsg.substr(0,nIndex);
			strMsg.erase(0,nIndex+3);

			if(strCommand.compare("CloseSocket") == 0)
			{
				std::cout << "接続切断" << std::endl;
				g_Socket.Close();
				WSACleanup();
				m_pReadCommand->AnalysisCmd( "set Run = EXIT" );
				_endthread();
			}
			std::cout << "rec_buf: " << strCommand.c_str() << std::endl;
			m_pReadCommand->AnalysisCmd( strCommand.c_str() );
			nIndex = strMsg.find("./\n",0);
		}
	}
}

void WinSockInit()
{
	// WinSockの初期化
	int		nResult;
	WORD	wRequireVersion;	// 使用するWinSockのバージョン
	WSADATA	lpWSAData;			// WinSock初期化の結果

	// WinSock2を使用するWinSockのバージョンとして設定
	wRequireVersion = MAKEWORD( 2, 0 );

	// WinSockの初期化を行なう
	nResult = WSAStartup( wRequireVersion, &lpWSAData );
	if( nResult != 0  )
	{
//		cerr << "WinSockの初期化に失敗" << endl;
	}

	// 初期化したWinSockのバージョンが要求したものか確認
	if( lpWSAData.wVersion != wRequireVersion )
	{
//		cerr << "要求したWinSockのバージョンが取得できませんでした" << endl;
	}
}

bool init_sock()
{
	int nPort = 10700;

	if(!g_Socket.Socket())
	{
		cerr << "ソケット作成失敗\n" << endl;
		return false;
	}

	if(!g_Socket.Bind(nPort))
	{
		cerr << "バインド失敗\n" << endl;
		return false;
	}
	return true;
}

bool SendMsg( const char* lpszMsg )
{
	// メッセージのサイズを調べて、全部送信できるまでひたすらsendする
	std::string msg = lpszMsg;
	size_t nSize = msg.length();
	size_t nRet = 0;
	g_Socket.Lock();
	while( 1 )
	{
		if( nRet >= nSize )
			break;
		nRet += g_Socket.Send( &msg[nRet], nSize - nRet );
	}
	g_Socket.Unlock();
	return TRUE;
}
#endif
#endif
// additional: end


bool checkArg(int &argc, char **argv, string &config_name, int pos[2], int size[2])
{
	if( argc == 1 ) return true;

	for( int i = 1; i < argc; i++) {
		if( '-' == argv[i][0] ) {
			switch( argv[i][1] ) {
			case 'C':
				i++; 
				config_name = argv[i];
				printf("tell detected: -C flag: %s\n", config_name.c_str() );
				break;
			case 'p':
				i++; pos[0] = atoi(argv[i]);
				i++; pos[1] = atoi(argv[i]); 
				break;
			case 's':
				i++; size[0] = atoi(argv[i]);
				i++; size[1] = atoi(argv[i]); 
				break;
			default:
				cerr << "bad argument(s) !\n";
				return false;
				break;
			}
		} 
		else {
			cerr << "bad argument(s) !\n";
			return false;
		}
	}
	return true;
}



/**************************************** main() ********************/


int main(int argc, char* argv[])
{
	// additional:  for galatea-mmi system
#ifdef WIN32
#ifdef WIN32_GALATEA
	WinSockInit();
	init_sock();
	g_Socket.Listen();
	g_Socket.Accept();
	_beginthread(RecvThread,0,NULL);
#endif
#endif
	// Create Configuration Class
	string config = CONFIG_FILE;

	int init_window_pos[2] = { -1, -1 };
	int init_window_size[2] = { -1, -1 };

	if( !checkArg(argc, argv, config, init_window_pos, init_window_size) ) {
		cerr << "fsm (-C config.file)" << endl;
		return false;
	}

	m_pConfig = new Configuration( config );

	if( init_window_pos[0] == -1 && init_window_pos[0] == -1 && init_window_size[0] == -1 && init_window_size[0] == -1 ) {
		init_window_pos[0] = m_pConfig->GetDefWindowPosX();
		init_window_pos[1] = m_pConfig->GetDefWindowPosY();
		init_window_size[0] = m_pConfig->GetDefWindowWidth();
		init_window_size[1] = m_pConfig->GetDefWindowHeight();
	}

	// additional end

	int   main_window;

	char m_wfm_pnt[256], m_wfm_lnk[256]; 
	char m_face_sgi[256], m_face_pnt[256];
	char m_eye_image[256], m_eye_def[256];
	char m_teeth_def[256];

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/
	glutInit(&argc, argv);

	//glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition( init_window_pos[0], init_window_pos[1] );
	glutInitWindowSize( init_window_size[0], init_window_size[1] );
	
	main_window = glutCreateWindow( WINDOW_TITLE );
	glutDisplayFunc( myGlutDisplay );
	glutReshapeFunc( myGlutReshape ); 
	glutTimerFunc( 10, myGlutTimer, 0 );
	glutIdleFunc( myGlutIdle );
	glutMouseFunc( myMousefn );
	glutMotionFunc( myMotionfn );
	glutKeyboardFunc( myKeyfn );
	
	_main_width = init_window_size[0];
	_main_height = init_window_size[1];

	_framerate = 0.0;

	// for autonomous blink
	_autonomousBlinkPattern = 0;
	_autonomousBlinkMode = false;


	int i, j;
	
	// Create ReadCommand (CommandSet) Class
	m_pReadCommand = new ReadCommand();
	
	// Cofigurationに入っている値を代入

	// Background Texture の読み出し
	int bgNum = m_pConfig->GetBGNum();
	for( i = 0; i < bgNum; i++ ) {
		m_pBGTexture[i] = new Texture((m_pConfig->GetBGRgbFile(i)).c_str());
		m_pReadCommand->AddBGName( m_pConfig->GetBGName(i) );
	}

	// Mask
	int maskNum = m_pConfig->GetMaskNum();
	for( i = 0; i < maskNum; i++ ) {
		// Create FaceEditApp Class
		// badly codes
		strcpy(m_wfm_pnt, m_pConfig->GetDefWfmPnt().c_str());
		strcpy(m_wfm_lnk, m_pConfig->GetDefWfmLnk().c_str());
		m_pFaceEditApp[i] = new FaceEditApp( m_wfm_pnt, m_wfm_lnk, i, 
			m_pReadCommand->GetMouthScale(),
			m_pConfig->GetVisemeFile(),
			m_pConfig);
		
		// MaskNameを入れる
		m_pReadCommand->AddMaskName( m_pConfig->GetMaskName(i) );
		
		strcpy(m_face_sgi, (m_pConfig->GetMaskRgbFile(i)).c_str() );
		strcpy(m_face_pnt, (m_pConfig->GetMaskPntFile(i)).c_str() );

		for( j = 0; j < m_pConfig->GetMaskEyeNum(); j++ ) {
			if( (m_pConfig->GetMaskName(i)) == (m_pConfig->GetMaskEyeName(j)) ) {
				strcpy( m_eye_image, (m_pConfig->GetMaskEyeImageFile(j)).c_str() );
				strcpy( m_eye_def, (m_pConfig->GetMaskEyeDefFile(j)).c_str() );
				break;
			}
		}
		if( j == m_pConfig->GetMaskEyeNum() ) {
			printf( "cannnot find eye file\n" );
			exit(1);
		}

		for( j = 0; j < m_pConfig->GetMaskTeethNum(); j++ ) {
			if( m_pConfig->GetMaskName(i) == m_pConfig->GetMaskTeethName(j) ) {
				strcpy( m_teeth_def, (m_pConfig->GetMaskTeethDefFile(j)).c_str() );
				break;
			}
		}
		if( j == m_pConfig->GetMaskTeethNum() ) {
			printf( "cannnot find eye file\n" );
			exit(1);
		}

		m_pFaceEditApp[i]->OpenDocument( m_face_sgi, m_face_pnt,
			m_eye_image, m_eye_def, m_teeth_def );
		
		m_pAuMove[i] = m_pFaceEditApp[i]->GetAUMove();
		m_pMuMove[i] = m_pFaceEditApp[i]->GetMUMove();
		m_pVmMove[i] = m_pFaceEditApp[i]->GetVMMove();
		m_pFaceModel[i] = m_pFaceEditApp[i]->GetFaceModel();
		m_pSetViseme[i] = m_pFaceEditApp[i]->GetSetViseme();
		
		m_pFaceView[i] = m_pFaceEditApp[i]->GetView();
	}
	
	// Config.txtの最初にかかれているMaskをDefaultとする
	m_pReadCommand->SetNowMaskName( m_pConfig->GetMaskName(0) );
	// Config.txtの最初にかかれているBGをDefaultとする
	m_pReadCommand->SetNowBGName( m_pConfig->GetBGName(0) );

	// Init AU, MU, VM Num
	for( i = 0; i < AU_MOVE_NUM; i++ ) g_AUNumber[i] = 0.0;
	for( i = 0; i < MU_MOVE_NUM; i++ ) g_MUNumber[i] = 0.0;
	for( i = 0; i < VM_MOVE_NUM; i++ ) g_VMNumber[i] = 0.0;

	// 時間制御のためのInit.(一回のみ）
	m_ExpTime.Initialize();

	// Windows Thread initialize
#ifdef WIN32
#ifndef WIN32_GALATEA
	unsigned int id;
	HANDLE threadHandle;

	// スレッドの開始
	threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, &thread, 0, 0, &id));
#endif
#endif
	
	// Initialize GLDrawingArea        	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	
	// using alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	
	glutMainLoop();

	return 0;
}

#ifdef WIN32
#ifndef WIN32_GALATEA
unsigned __stdcall thread(LPVOID pParam)
{
    const signed short PORT = 11234;
	static int initflag = 0;
	
    struct sockaddr_in srcAddr;
    struct sockaddr_in dstAddr;
	int dstAddrSize = sizeof(dstAddr);
	
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data); 
	
	// sockaddr_in設定
	memset(&srcAddr, 0, sizeof(srcAddr));
	srcAddr.sin_port = htons(PORT);
	srcAddr.sin_family = AF_INET;
	srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	SOCKET srcSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	bind(srcSocket, (struct sockaddr *) &srcAddr, sizeof(srcAddr));
	
	listen(srcSocket, 1);
	
	std::cout << "I will accept " << std::endl;
	dstSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
	std::cout << "connected from " << inet_ntoa(dstAddr.sin_addr) << std::endl;
	
	while(true) {
		
		int rcv_bytes = recv(dstSocket, rec_buf, 2048, 0); 
		if(rcv_bytes == 0 || rcv_bytes == -1) {
			closesocket(dstSocket); 
			WSACleanup();
			return(0);
		}
		//std::cout << "rec_buf: " << rec_buf << std::endl;
		m_pReadCommand->AnalysisCmd( rec_buf );
	}	
	WSACleanup();
	return 0;
}
#endif
#endif

/********** method ************/
void animation( bool flag )
{
	TRACE("animation");
	int nowMask = m_pReadCommand->GetNowMask();
	int i;

	for( i = 0; i < AU_MOVE_NUM; i++ ) g_AUNumber[i] = m_pReadCommand->GetFace(i);
	for( i = 0; i < MU_MOVE_NUM; i++ ) g_MUNumber[i] = m_pReadCommand->GetMouth(i);
	for( i = 0; i < VM_MOVE_NUM; i++ ) g_VMNumber[i] = m_pReadCommand->GetViseme(i);

	if( flag == true ) {
		TRACE("animation()");
		m_pAuMove[nowMask]->SetWfm(m_pFaceModel[nowMask]->GetWfm(WFM_STD),
			m_pFaceModel[nowMask]->GetWfm(WFM_ORG),
			m_pFaceModel[nowMask]->GetWfm(WFM_WORK));
		m_pAuMove[nowMask]->ResetWfm();

		m_pFaceModel[nowMask]->SetWireframeModifyFlag();

		m_pAuMove[nowMask]->UnitMoving(g_AUNumber);
		
		m_pMuMove[nowMask]->SetWfm(m_pFaceModel[nowMask]->GetWfm(WFM_STD),
			m_pFaceModel[nowMask]->GetWfm(WFM_ORG),
			m_pFaceModel[nowMask]->GetWfm(WFM_WORK));
		m_pMuMove[nowMask]->UnitMoving(g_MUNumber);

		m_pVmMove[nowMask]->SetWfm(m_pFaceModel[nowMask]->GetWfm(WFM_STD),
			m_pFaceModel[nowMask]->GetWfm(WFM_ORG),
			m_pFaceModel[nowMask]->GetWfm(WFM_WORK));
		m_pVmMove[nowMask]->UnitMoving(g_VMNumber);
	}
}

void getFrameRate() 
{
	TRACE("getFrameRate");

	// 描画フレームレート制御
	static bool ctlFirstFrame = true;
	static int ctlMaxTime = 1000 / m_pConfig->GetFrameRate();

	if( ctlFirstFrame == true ) {
		ctlFirstFrame = false;
		m_FPSCtrlTime.SetStartTime();
	} else {
		m_FPSCtrlTime.GetDurTimeWithProcess();
			
		if( m_FPSCtrlTime.GetDurTime() < ctlMaxTime ) {
			m_FPSCtrlTime.SleepTime(ctlMaxTime - m_FPSCtrlTime.GetDurTime());
		}
		m_FPSCtrlTime.SetStartTime();
	}

	static int f_countTime = 0;
	static bool first_frame = true;
	// 描画フレームレート描画
	if(f_countTime >= 60) {
		m_FPSTime.GetDurTimeWithProcess();

		if( m_FPSTime.GetDurTime() != 0.0 ) {
			_framerate = (float)(60000.0 / (float)m_FPSTime.GetDurTime());
		}
		f_countTime = 0;
		m_FPSTime.SetStartTime();
	}
	else {
		if( first_frame == true ) {
			m_FPSTime.SetStartTime();
			first_frame = false;
		}
		f_countTime++;
	}
}

/***************************************** myGlutTimer() **************/
void myGlutTimer(int value)
{
	TRACE("myGlutTimer");
#ifndef WIN32
	static int initflag = 0;
	static int count = 0;
	int i;
	int local = value;
	fd_set readfds, writefds, exceptfds; /* define bitmask */
	struct timeval timeout;
	static char buforig[1024*1024];
	static char *buf;
	char c;
	int on,off;
	buf = &buforig[1024];
	
	if(!initflag){
		initflag = 1;	
#ifndef MACOSX		
		/* echo off (System V) */
		ioctl(0,TCGETA,&tty);
		oldtty=tty;
		//tty.c_lflag &= ~(ICANON|ECHO);
		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 1;
		//tty.c_iflag &= ~ICRNL;
		//tty.c_oflag &= ~ONLCR;
		ioctl(0,TCSETAW,&tty);
#else
		/* echo off (4.2/3 BSD) */
		ioctl(0,TIOCGETP,&ttyb);
		oldttyb=ttyb;
		ttyb.sg_flags |= CBREAK;
		ttyb.sg_flags &= ~(ECHO|CRMOD);
		ioctl(0,TIOCSETN,&ttyb);
#endif	
		
		setbuf(stdin, NULL);          /* stdin buffering off */
		on=off=0;
		//printf("myGlutTimer(INIT)\n");
	}
	for(i=0;i<128;i++){
		timeout.tv_sec  = 0;          /* seconds */
		timeout.tv_usec = 1000*1000/100; /* and microseconds */
		FD_ZERO(&readfds);          /* clear bitmask */
		FD_SET(0, &readfds);        /* set stdin(read) */
		
		if(select(1,&readfds,NULL,NULL,&timeout)<0){ /* check readable fd */
			fprintf(stderr,"ERROR: select()\n");
			abort();
			// break;
			// exit(-1);
		}
		if(FD_ISSET(0,&readfds)){   /* stdin readable */
			read(0,&c,1);
			if(count>1024*1022)
				abort();
			buf[count++] = c;
			if(c=='\n' || c=='\r'){	// read 1 line.
				buf[count-1] = (char)0;
				
				//printf("STDIN : [%s] length : [%d] \n",buf, strlen(buf));
				
				// Analysis Command
				m_pReadCommand->AnalysisCmd( buf );
				//printf("STDIN : [%s]\n",buf);
				count = 0;
			}
		}
		else{
			//printf(".");
			i=128;
			break;
		}
	}
	fflush(stdout);

	glutTimerFunc( 10, myGlutTimer, 0 );
#endif
}

/***************************************** myGlutIdle() ***************/

void myGlutIdle( void )
{
	glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
	TRACE("myGlutReshape");
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if( x <= y ) {
		glOrtho(-(GLfloat)x/(GLfloat)y, (GLfloat)x/(GLfloat)y, -1.0, 1.0, 0.0, 10.0);
	} else {
		glOrtho(-1.0, 1.0, -(GLfloat)y/(GLfloat)x, (GLfloat)y/(GLfloat)x, 0.0, 10.0);
	}
	glMatrixMode(GL_MODELVIEW);
	
	glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
	TRACE("myGlutDisplay");
	static int	bg_tex_num = -1;
	static int	mask_tex_num = -1;

	int nowMask = m_pReadCommand->GetNowMask();
	int nowBG = m_pReadCommand->GetNowBG();

	getFrameRate();
	
	// Mouth Animation
	checkMouthMotion();
	
	// EyeBlinking
	checkEyeBlink();

	// Head Rotation
	checkHeadMove();
	
	// Autonomous
	checkAutonomousBlink();
	checkAutonomousMove();

	// Eye Rotation
	checkEyeMove(EYE_RIGHT);
	checkEyeMove(EYE_LEFT);

	// Facial Expression
	checkFaceExpression();
	
	animation( true );
	
	// 画面をセーブするかどうかチェック
	if( m_pReadCommand->GetSaveImageFlag() == true ) {
		char filename[1024];
		sprintf( filename, "%s", m_pReadCommand->GetSaveImageFile().c_str());
		m_pFaceView[nowMask]->saveImage( filename, nowMask, _main_width, _main_height );
		m_pReadCommand->SetSaveImageFlag( false );
	}

	static int img_counter = 0;
	if( m_pReadCommand->GetFSMCaptureEnable() == true && 
		m_pReadCommand->GetSpeakStat() == SPEAKING ) {
			char counter_file[100];
			sprintf(counter_file, "%s/%s%05d.bmp", 
				m_pReadCommand->GetFSMCaptureDirName().c_str(), 
				m_pReadCommand->GetFSMCaptureFileName().c_str(), img_counter);
			m_pFaceView[nowMask]->saveImage( counter_file, nowMask, _main_width, _main_height );
			img_counter++;
	}
	else {
		img_counter = 0;
	}
	
	// 背景のイメージを獲得
	if( bg_tex_num != nowBG || mask_tex_num != nowMask ) {
		m_pFaceModel[nowMask]->SetBGTexture( m_pBGTexture[nowBG] );
		bg_tex_num = nowBG;
		mask_tex_num = nowMask;
	}

	// 背景のイメージの有無
	if( m_pReadCommand->GetViewBackground() == VIEW_ENABLE ) {
		m_pFaceView[nowMask]->setBackgroundVisible(1);
	} else {
		m_pFaceView[nowMask]->setBackgroundVisible(0);
	}

	// モデルの透明度を指定
	m_pFaceView[nowMask]->setAgentAlpha(m_pReadCommand->GetAgentAlpha());


	// モデルの見せ方を決定
	m_pFaceView[nowMask]->setModelView(m_pReadCommand->GetViewMode());

	m_pFaceView[nowMask]->redraw(0);
	glFlush();


	// output framerate
	static int count = 0;

	if( count >= 100 ) {
		char frame_title[256];

		if( m_pReadCommand->GetFSMCaptureEnable() == true ) {
			sprintf(frame_title, "%s - CAPTURE MODE", WINDOW_TITLE );
		}
		else {
			sprintf(frame_title, "%s - %4.1f [fps]", WINDOW_TITLE, _framerate );
		}
		glutSetWindowTitle(frame_title);
		count = 0;
	}

	// agent location
	double agent_rot[3], agent_trans[2];
	m_pReadCommand->GetAgentRot(agent_rot);
	m_pFaceView[nowMask]->setRotate(agent_rot[0], agent_rot[1], agent_rot[2]);
	m_pReadCommand->GetAgentTrans(agent_trans);
	m_pFaceView[nowMask]->setTranslate(agent_trans[0], agent_trans[1]);
	m_pFaceView[nowMask]->setScale(m_pReadCommand->GetAgentScale());
	m_pFaceView[nowMask]->setEnable(m_pReadCommand->GetAgentEnable());
	
	count++;

	glutSwapBuffers();
}

void myMousefn(int button, int updown, int x, int y)
{
	int nowMask = m_pReadCommand->GetNowMask();
	int status = 0;
	
	if( button == GLUT_LEFT_BUTTON && updown == GLUT_DOWN) {
		m_pFaceView[nowMask]->LeftMousePushEvent(x, y, status);
	} 
	else if( button == GLUT_RIGHT_BUTTON && updown == GLUT_DOWN ) {
		m_pFaceView[nowMask]->RightMousePushEvent(x, y, status);
	}
	else if( button == GLUT_MIDDLE_BUTTON && updown == GLUT_DOWN ) {
		m_pFaceView[nowMask]->MidMousePushEvent(x, y, status);
	}
	else if( button == GLUT_LEFT_BUTTON && updown == GLUT_UP) {
		m_pFaceView[nowMask]->LeftMouseReleaseEvent(x, y, status);
	}
	else if( button == GLUT_RIGHT_BUTTON && updown == GLUT_UP ) {
		m_pFaceView[nowMask]->RightMouseReleaseEvent(x, y, status);
	}
	else if( button == GLUT_MIDDLE_BUTTON && updown == GLUT_UP ) {
		m_pFaceView[nowMask]->MidMouseReleaseEvent(x, y, status);
	}
	glutPostRedisplay();
}

void myMotionfn(int x, int y)
{
	int nowMask = m_pReadCommand->GetNowMask();
	int status = 0;
	
	double trans[2], rot[3], scale;

	// tatsuo work
	m_pReadCommand->GetAgentRot(rot);
	m_pReadCommand->GetAgentTrans(trans);
	m_pReadCommand->GetAgentScale(scale);
	m_pFaceView[nowMask]->MouseDragEvent(x, y, status, _main_width, _main_height,
		rot, trans, scale);
	m_pReadCommand->SetAgentRot(rot);
	m_pReadCommand->SetAgentTrans(trans);
	m_pReadCommand->SetAgentScale(scale);

	glutPostRedisplay();
}

void myKeyfn ( unsigned char key, int, int )
{
	int nowMask = m_pReadCommand->GetNowMask();
	HeadRot *headRot = m_pReadCommand->GetHeadRotAbs();

	switch( key ) {
	case 'r':
	case 'R':
		m_pReadCommand->ResetAgentLocation();
		break;
	case 'f':
	case 'F':
		glutFullScreen();
		break;
	case 27:
		exit(1);
		break;
#if 0
	case '1':
		headRot->axis[0] -= 1.0;
		break;
	case '2':
		headRot->axis[0] += 1.0;
		break;
	case '3':
		headRot->axis[1] -= 1.0;
		break;
	case '4':
		headRot->axis[1] += 1.0;
		break;
	case '5':
		headRot->axis[2] -= 1.0;
		break;
	case '6':
		headRot->axis[2] += 1.0;
		break;
#endif
	}
	glutPostRedisplay();
}


/****************************************** check motion ******************/
bool checkMouthMotion(void) 
{
	int nowMask = m_pReadCommand->GetNowMask();
	
	int speak_time;
	static bool first_speak_flag = true;
	
	static int start_time_ms, start_time_ss, start_time_mm;
	static int start_time_hr, start_time_day;

	// sync 調整用
	static int sync_time = 0;

	int i;

	// Status = Processing
	if( m_pReadCommand->GetSpeakStat() == PROCESSING ) {
		// 口の大きさをSetVisemeクラスへ送る
		m_pSetViseme[nowMask]->SetMouthScale( m_pReadCommand->GetMouthScale() );
		
		// 音素からVisemeへ分割 + Offset値も追加
		m_pSetViseme[nowMask]->PhonemeToViseme( m_pReadCommand->GetPhoneme(), 
			m_pReadCommand->GetDuration(), m_pReadCommand->GetSpeakSyncOffset() + m_pReadCommand->GetOffset());
		// StatusをReadyにする
		m_pReadCommand->SetSpeakStat(READY);

		// check Speak prop ( stat )
		if( m_pReadCommand->GetSpeakStatProp() == true )
			m_pReadCommand->AnalysisCmd( "inq Speak.stat" );
		
		fflush(stdout);
	}
	
	// Status = Ready
	if( m_pReadCommand->GetSpeakStat() == READY ) {
		// Speak Status = NOW
		if( m_pReadCommand->GetSpeakTime() == 0 ) {
			// 直ちに処理開始：StatusをSpeakingに変化
			m_pReadCommand->SetSpeakStat(SPEAKING);
			sync_time = 0;
			
			// check Speak prop ( stat )
			if( m_pReadCommand->GetSpeakStatProp() == true )
				m_pReadCommand->AnalysisCmd( "inq Speak.stat" );
			first_speak_flag = true;
		} 
		
		// Speak Statusの中に開始相対時間が入っているとき
		else if( m_pReadCommand->GetSpeakTime() != -1 ) {
			m_MouthTime.SetStartTime();
			m_MouthTime.GetStartTime(start_time_ms, start_time_ss, start_time_mm, start_time_hr, start_time_day);

			// 相対時間と比べる
			speak_time = ((start_time_day - m_pReadCommand->GetSpeakCurrentTime(0)) * 24 * 60 * 60 * 1000) +
				((start_time_hr - m_pReadCommand->GetSpeakCurrentTime(1)) * 60 * 60 * 1000) + 
				((start_time_mm - m_pReadCommand->GetSpeakCurrentTime(2)) * 60 * 1000) +
				((start_time_ss - m_pReadCommand->GetSpeakCurrentTime(3)) * 1000) +
				((start_time_ms - m_pReadCommand->GetSpeakCurrentTime(4)));

			//printf("target: %d:%d:%d:%d:%d\n", m_pReadCommand->GetSpeakCurrentTime(0), m_pReadCommand->GetSpeakCurrentTime(1), m_pReadCommand->GetSpeakCurrentTime(2), m_pReadCommand->GetSpeakCurrentTime(3), m_pReadCommand->GetSpeakCurrentTime(4) );
			//printf("now:    %d:%d:%d:%d:%d\n", start_time_day, start_time_hr, start_time_mm, start_time_ss, start_time_ms );

			//printf("speak_time = %d target_time = %d\n", speak_time, m_pReadCommand->GetSpeakTime());

			// もし開始相対時間より上回った場合
			if( speak_time >= m_pReadCommand->GetSpeakTime() ) {
				// StatusをSpeakingにする
				m_pReadCommand->SetSpeakStat(SPEAKING);
				sync_time = 0;
				
				// check Speak prop ( stat )
				if( m_pReadCommand->GetSpeakStatProp() == true )
					m_pReadCommand->AnalysisCmd( "inq Speak.stat" );
				
				first_speak_flag = true;
			}
		}
	}
	
	// Capture用
	if( m_pReadCommand->GetFSMCaptureEnable() == false || 
		(m_pReadCommand->GetSpeakStat() == SPEAKING && m_pReadCommand->GetSpeakTime() == -1) ) {
		m_MouthTime.SetRT(true);
		m_ExpTime.SetRT(true);
		m_HeadTime.SetRT(true);
		m_BlinkTime.SetRT(true);
		m_AutonomousMoveTime.SetRT(true);
		m_EyeTime[0].SetRT(true);
		m_EyeTime[1].SetRT(true);
	}
	else {
		m_MouthTime.SetStepTime(1000.0 / (double) m_pReadCommand->GetFSMCaptureFPS());
		m_MouthTime.SetRT(false);
		m_ExpTime.SetRT(false);
		m_HeadTime.SetRT(false);
		m_BlinkTime.SetRT(false);
		m_AutonomousMoveTime.SetRT(false);
		m_EyeTime[0].SetRT(false);
		m_EyeTime[1].SetRT(false);
	}


	// Status = Speaking
	if( m_pReadCommand->GetSpeakStat() == SPEAKING ) {
		// Speak Status = STOP
		if( m_pReadCommand->GetSpeakTime() == -1 ) {
			// 処理をストップさせる：StatusをIDLEに変化
			m_pReadCommand->SetSpeakStat(IDLE);
			
			// check Speak prop ( stat )
			if( m_pReadCommand->GetSpeakStatProp() == true )
				m_pReadCommand->AnalysisCmd( "inq Speak.stat" );

			// 口を閉じる
			for( int ii = 0; ii < VM_MOVE_NUM; ii++ ) m_pReadCommand->SetViseme(ii, 0.0);
			
			// Speak開始直後のフラグをTrueにもどす
			first_speak_flag = true;

			// 音声合成とのSyncをリセット
			sync_time = 0;

			// CaptureMode をDisableにする
			m_pReadCommand->SetFSMCaptureEnable(false);

		} else {
			// 発話開始直後の処理
			if( first_speak_flag == true ) {
				m_MouthTime.SetStartTime();
				first_speak_flag = false;
			}

			if( m_pReadCommand->GetSpeakSyncTime() != 0 ) {
				sync_time = m_pReadCommand->GetSpeakSyncTime() - m_MouthTime.GetDurTimeWithProcess();
				m_pReadCommand->SetSpeakSyncTime(0);
			}
			// 現在の時間を獲得 + 音声合成との同期
			speak_time = m_MouthTime.GetDurTimeWithProcess() + sync_time;
			
			
			// 発話時間を超えていない場合
			// + 2ms している理由として無音区間で1ms前後に追加している為
			// + Offset値
			if( (m_pReadCommand->GetSpeakDur() + 2 + m_pReadCommand->GetSpeakSyncOffset()) > speak_time ) {
				m_pSetViseme[m_pReadCommand->GetNowMask()]->GetFrame( speak_time, g_VMNumber);
				for( i = 0; i < VM_MOVE_NUM; i++ ) m_pReadCommand->SetViseme(i, g_VMNumber[i]);
				// 越えたならばIDLE状態にしSpeakステータスを-1にする
			} else {
				m_pReadCommand->SetSpeakStat(IDLE);
				m_pReadCommand->SetSpeak( -1, 0, 0, 0, 0, 0 );
				
				// check Speak prop ( stat )
				if( m_pReadCommand->GetSpeakStatProp() == true )
					m_pReadCommand->AnalysisCmd( "inq Speak.stat" );
	
				// 口を閉じる
				for( int ii = 0; ii < VM_MOVE_NUM; ii++ ) m_pReadCommand->SetViseme(ii, 0.0);

				first_speak_flag = true;
				
				// CaptureMode をDisableにする
				m_pReadCommand->SetFSMCaptureEnable(false);
			}
		}
	}
	return true;
}

bool checkEyeMove(int num)
{
	int nowMask = m_pReadCommand->GetNowMask();

	static bool eyeRotAnimFlag[2] = {false, false};
	static bool eyeRotFirstFlag[2] = {false, false};
	static bool eyeRotEndFlag[2] = {false, false};

	static double oldEyeRot[2][2] = { 0.0, 0.0, 0.0, 0.0 };
	static double a_func[2][2], b_func[2][2];
	static double eyeTotalRot[2][2];

	static int totalDur[2];
	static double *eyeRot[2];

	int durtime;

	if( num == EYE_RIGHT ) {
		eyeRot[num] = m_pFaceModel[nowMask]->GetAppEyeRotR();
		if( m_pReadCommand->GetEyeRotRightFlag() == true ) {
			m_pReadCommand->SetEyeRotRightFlag(false);
			eyeRotAnimFlag[num] = eyeRotFirstFlag[num] = eyeRotEndFlag[num] = true;
		}
	}
	else {
		eyeRot[num] = m_pFaceModel[nowMask]->GetAppEyeRotL();
		if( m_pReadCommand->GetEyeRotLeftFlag() == true ) {
			m_pReadCommand->SetEyeRotLeftFlag(false);
			eyeRotAnimFlag[num] = eyeRotFirstFlag[num] = eyeRotEndFlag[num] = true;
		}
	}

	if( eyeRotAnimFlag[num] == true ) {
		if( eyeRotFirstFlag[num] == true ) {

			m_EyeTime[num].SetStartTime();
			
			if( num == EYE_RIGHT ) {
				eyeTotalRot[num][0] = m_pReadCommand->GetEyeRotRightAngleX();
				eyeTotalRot[num][1] = m_pReadCommand->GetEyeRotRightAngleY();
				totalDur[num] = (int)m_pReadCommand->GetEyeRotRightDuration();
			} else {
				eyeTotalRot[num][0] = m_pReadCommand->GetEyeRotLeftAngleX();
				eyeTotalRot[num][1] = m_pReadCommand->GetEyeRotLeftAngleY();
				totalDur[num] = (int)m_pReadCommand->GetEyeRotLeftDuration();
			}
			makeLinearFunc( 0, oldEyeRot[num][0], m_pConfig->GetEyeMoveDuration(), eyeTotalRot[num][0], a_func[num][0], b_func[num][0]);
			makeLinearFunc( 0, oldEyeRot[num][1], m_pConfig->GetEyeMoveDuration(), eyeTotalRot[num][1], a_func[num][1], b_func[num][1]);			

			eyeRotFirstFlag[num] = false;
		}

		m_EyeTime[num].GetDurTimeWithProcess();

		if( m_EyeTime[num].GetDurTime() < m_pConfig->GetEyeMoveDuration() ) {
			oldEyeRot[num][0] = eyeRot[num][0] = a_func[num][0] * m_EyeTime[num].GetDurTime() + b_func[num][0];
			oldEyeRot[num][1] = eyeRot[num][1] = a_func[num][1] * m_EyeTime[num].GetDurTime() + b_func[num][1];
		}
		else if( totalDur[num] == 0 ) {
			oldEyeRot[num][0] = eyeRot[num][0] = eyeTotalRot[num][0];
			oldEyeRot[num][1] = eyeRot[num][1] = eyeTotalRot[num][1];
			eyeRotAnimFlag[num] = false;
		}
		else if( m_EyeTime[num].GetDurTime() < (totalDur[num] - m_pConfig->GetEyeMoveDuration()) ) {
			oldEyeRot[num][0] = eyeRot[num][0] = eyeTotalRot[num][0];
			oldEyeRot[num][1] = eyeRot[num][1] = eyeTotalRot[num][1];
		}
		else if( m_EyeTime[num].GetDurTime() < totalDur[num] ) {
			if( eyeRotEndFlag[num] == true ) {
				makeLinearFunc( 0, oldEyeRot[num][0], m_pConfig->GetEyeMoveDuration(), 0, a_func[num][0], b_func[num][0]);
				makeLinearFunc( 0, oldEyeRot[num][1], m_pConfig->GetEyeMoveDuration(), 0, a_func[num][1], b_func[num][1]);
				eyeRotEndFlag[num] = false;
			}
			durtime = m_EyeTime[num].GetDurTime() - (totalDur[num] - m_pConfig->GetEyeMoveDuration());
			oldEyeRot[num][0] = eyeRot[num][0] = a_func[num][0] * durtime + b_func[num][0];
			oldEyeRot[num][1] = eyeRot[num][1] = a_func[num][1] * durtime + b_func[num][1];
		}
		else {
			oldEyeRot[num][0] = eyeRot[num][0] = 0.0;
			oldEyeRot[num][1] = eyeRot[num][1] = 0.0;
			eyeRotAnimFlag[num]= false;
		}
	}

	return true;
}

bool checkFaceExpression() {
	static bool faceexp_flag = false;
	
	static bool first_time = true;  // 最初だけ読み込むためのフラグ
	static bool time_flag = true;   // 最初の時間を読み込む
	static bool time_flag2 = true;
	static int nowfaceexp = 0;
	static int oldfaceexp = 0;
	
	static float oldvalue;
	static int oldvalueexp;
	
	static int faceexp_duration;
	static float value;
	static int config_duration;
	
	if( first_time == true ) {
		oldfaceexp = m_pReadCommand->GetFaceExpExp();
		first_time = false;
	}
	
	nowfaceexp = m_pReadCommand->GetFaceExpExp();
	faceexp_duration = m_pReadCommand->GetFaceExpDuration();
	
	if( (nowfaceexp != oldfaceexp && oldfaceexp == NEUTRAL) || 
		(nowfaceexp == oldfaceexp && oldfaceexp != NEUTRAL)) {
		if( time_flag == true ) {

			m_ExpTime.SetStartTime();

			if( faceexp_duration == 0 ) {
				config_duration = m_pConfig->GetExpressionDuration();
			}
			else if( faceexp_duration < m_pConfig->GetExpressionDuration() * 2) {
				config_duration = faceexp_duration / 2;
			} 
			else {
				config_duration = m_pConfig->GetExpressionDuration();
			}
			
			time_flag = false;
		}

		m_ExpTime.GetDurTimeWithProcess();

		if( m_ExpTime.GetDurTime() < config_duration ) {
			
			value = (float)m_pReadCommand->GetFaceExpValue() / 100.0 
				/ config_duration * m_ExpTime.GetDurTime();
		} 
		else if(  m_ExpTime.GetDurTime()  >= m_ExpTime.GetDurTime() && 
			 m_ExpTime.GetDurTime()  <= faceexp_duration - config_duration ) {
			value = (float)m_pReadCommand->GetFaceExpValue() / 100.0;
		}
		else if(  m_ExpTime.GetDurTime()  > faceexp_duration - config_duration &&
			 m_ExpTime.GetDurTime()  < faceexp_duration ) {
			value = (float)m_pReadCommand->GetFaceExpValue() / 100.0
				/ config_duration * (faceexp_duration -  m_ExpTime.GetDurTime() );
		}
		else if(  m_ExpTime.GetDurTime()  >= faceexp_duration ) {
			if( faceexp_duration == 0 ) {
				value = (float)m_pReadCommand->GetFaceExpValue() / 100.0;
			} else {
				value = 0;
				m_pReadCommand->SetFaceExp( NEUTRAL, 0, 0, 0 );
				nowfaceexp = NEUTRAL;
				time_flag = true;
			}
		}
		
		for(int i = 30; i < 65; i++ ) {
			m_pReadCommand->SetFace( i, 0 );
		}
		int pattern = m_pReadCommand->GetFaceExpPattern();


		switch( nowfaceexp ) {
		case HAPPY:
			if( pattern == 1 ) m_pReadCommand->SetFace( 30, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 36, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 37, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 38, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 39, value );
			break;
		case ANGRY:
			if( pattern == 1 ) m_pReadCommand->SetFace( 31, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 40, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 41, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 42, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 43, value );
			break;
		case DISGUSTED:
			if( pattern == 1 ) m_pReadCommand->SetFace( 32, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 44, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 45, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 46, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 47, value );
			break;
		case SAD:
			if( pattern == 1 ) m_pReadCommand->SetFace( 33, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 48, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 49, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 50, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 51, value );
			break;
		case SURPRISE:
			if( pattern == 1 ) m_pReadCommand->SetFace( 34, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 52, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 53, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 54, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 55, value );
			break;
		case FEARED:
			if( pattern == 1 ) m_pReadCommand->SetFace( 35, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 56, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 57, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 58, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 59, value );
			break;
		case USERDEF_EXP:
			if( pattern == 1 ) m_pReadCommand->SetFace( 60, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 61, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 62, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 63, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 64, value );
			break;
		}
	}
	else if(((nowfaceexp != oldfaceexp ) && (nowfaceexp == NEUTRAL)) ||
		((nowfaceexp == NEUTRAL) && (time_flag2 == false))) {
		
		if( time_flag2 == true ) {
			oldvalue = value;
			oldvalueexp = oldfaceexp;
		
			m_ExpTime.SetStartTime();
			
			time_flag2 = false;
		}
		
		m_ExpTime.GetDurTimeWithProcess();
		
		if( m_ExpTime.GetDurTime() < m_pConfig->GetExpressionDuration() ) {
			value = (float)oldvalue / m_pConfig->GetExpressionDuration() * (m_pConfig->GetExpressionDuration() -  m_ExpTime.GetDurTime() );
		} 
		else if(  m_ExpTime.GetDurTime()  >= m_pConfig->GetExpressionDuration() ) {
			value = 0;
			m_pReadCommand->SetFaceExp( NEUTRAL, 0, 0, 0 );
			nowfaceexp = NEUTRAL;
			time_flag = true;
			time_flag2 = true;
		}
		
		for(int i = 30; i < 65; i++ ) {
			m_pReadCommand->SetFace( i, 0 );
		}
		int pattern = m_pReadCommand->GetFaceExpPattern();
		
		switch( oldvalueexp ) {
		case HAPPY:
			if( pattern == 1 ) m_pReadCommand->SetFace( 30, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 36, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 37, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 38, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 39, value );
			break;
		case ANGRY:
			if( pattern == 1 ) m_pReadCommand->SetFace( 31, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 40, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 41, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 42, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 43, value );
			break;
		case DISGUSTED:
			if( pattern == 1 ) m_pReadCommand->SetFace( 32, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 44, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 45, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 46, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 47, value );
			break;
		case SAD:
			if( pattern == 1 ) m_pReadCommand->SetFace( 33, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 48, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 49, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 50, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 51, value );
			break;
		case SURPRISE:
			if( pattern == 1 ) m_pReadCommand->SetFace( 34, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 52, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 53, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 54, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 55, value );
			break;
		case FEARED:
			if( pattern == 1 ) m_pReadCommand->SetFace( 35, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 56, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 57, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 58, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 59, value );
			break;
		case USERDEF_EXP:
			if( pattern == 1 ) m_pReadCommand->SetFace( 60, value );
			else if( pattern == 2 )  m_pReadCommand->SetFace( 61, value );
			else if( pattern == 3 )  m_pReadCommand->SetFace( 62, value );
			else if( pattern == 4 )  m_pReadCommand->SetFace( 63, value );
			else if( pattern == 5 )  m_pReadCommand->SetFace( 64, value );
			break;
		}
	}   
	oldfaceexp = nowfaceexp;
	return true;
}

bool checkEyeBlink()
{
	TRACE("checkEyeBlink");
	int motion = m_pReadCommand->GetAutonomousPattern(BLINKING);
	int r;
	
	if( motion != 0 ) {
		if( motion > m_pConfig->GetAutonomousBlinkNum() ) {
			m_pReadCommand->SetAutonomousPattern(BLINKING, 0);
			return false;
		}
		r = rand();
		if( ((1 + (int)( 20.0 * m_pConfig->GetFrameRate() * rand() / (RAND_MAX + 1.0 ))) % 
			(int)(20 * m_pConfig->GetFrameRate() / m_pConfig->GetAutonomousBlink( motion - 1 ))) == 0 ) {
			m_AutonomousBlinkTime.SetDurTime(0);
			_autonomousBlinkMode = true;
			_autonomousBlinkPattern = m_pConfig->GetAutonomousBlinkPattern( motion - 1 );
		}
	}
	return true;
}

bool checkHeadMove()
{
	TRACE("checkHeadMove");
	int FaceMotMotion = m_pReadCommand->GetFaceMotMotion();
	int FaceMotPattern = m_pReadCommand->GetFaceMotPattern();

	int nowMask = m_pReadCommand->GetNowMask();

	double *headRot = m_pFaceView[nowMask]->GetHeadRot();

	// for FaceMot
	switch( FaceMotMotion ) {
	case NOD:
	case REFUSE:
	case LISTEN:
	case USERDEF_FACEMOT:
		//cout << "checkFaceMot " << m_HeadTime.GetDurTime() << endl; 
		// IDが存在しないとき
		if( FaceMotPattern > m_pConfig->GetFaceMotSize(FaceMotMotion) || FaceMotPattern < 0) {
			m_HeadTime.SetDurTime(0);
			m_pReadCommand->SetFaceMot(NOMOVE, 0);
			m_pConfig->ResetFaceMotFrame();
		}
		// 時間を超えたとき
		else if( m_HeadTime.GetDurTime() >= m_pConfig->GetFaceMotFrameSize(FaceMotMotion, FaceMotPattern - 1) ) {
			m_HeadTime.SetDurTime(0);
			m_pReadCommand->SetFaceMot(NOMOVE, 0);
			m_pConfig->ResetFaceMotFrame();
		} else {
			// 最初の処理
			if( m_HeadTime.GetDurTime() == 0 ) { 
				m_HeadTime.SetStartTime();
				m_HeadTime.SetDurTime(1);
			} else {
				m_HeadTime.GetDurTimeWithProcess();
			}
			if( m_HeadTime.GetDurTime() < m_pConfig->GetFaceMotFrameSize(FaceMotMotion, FaceMotPattern - 1))
				m_pConfig->GetFaceMotCurrentFrame(FaceMotMotion, FaceMotPattern - 1, m_HeadTime.GetDurTime());
		}
		break;

	case BLINK:
		//cout << "checkHead " << m_BlinkTime.GetDurTime() << endl; 
		// IDが存在しないとき
		if( FaceMotPattern > m_pConfig->GetFaceMotBlinkSize() || FaceMotPattern < 0) {
			m_BlinkTime.SetDurTime(0);
			m_pReadCommand->SetFace( 30 - 1, 0.0 );
			m_pReadCommand->SetFaceMot(NOMOVE, 0);
		}
		// 時間を超えたとき
		else if( m_BlinkTime.GetDurTime() >= m_pConfig->GetFaceMotBlinkStrSize(FaceMotPattern - 1) ) {
			m_BlinkTime.SetDurTime(0);
			m_pReadCommand->SetFace( 30 - 1, 0.0 );

			m_pReadCommand->SetFaceMot(NOMOVE, 0);
		} else {
			// 最初の処理
			if( m_BlinkTime.GetDurTime() == 0 ) { 
				m_BlinkTime.SetStartTime();
				m_BlinkTime.SetDurTime(1);
			} else {
				m_BlinkTime.GetDurTimeWithProcess();
			}
			if( m_BlinkTime.GetDurTime() < m_pConfig->GetFaceMotBlinkStrSize(FaceMotPattern - 1) ) 
				m_pReadCommand->AnalysisCmd( m_pConfig->GetFaceMotBlinkStr(FaceMotPattern - 1,  m_BlinkTime.GetDurTime() ));
		}
		break;
	}	

	headRot[0] = m_pReadCommand->GetHeadRotAbsAxis(0) + m_pConfig->GetFaceMotAngle(0);
	headRot[1] = m_pReadCommand->GetHeadRotAbsAxis(1) + m_pConfig->GetFaceMotAngle(1);
	headRot[2] = m_pReadCommand->GetHeadRotAbsAxis(2) + m_pConfig->GetFaceMotAngle(2);
	return true;
}

bool checkAutonomousBlink()
{
	if( _autonomousBlinkMode == true ) {
		//cout << "checkHead " << m_AutonomousBlinkTime.GetDurTime() << endl;

		// IDが存在しないとき
		if( _autonomousBlinkPattern > m_pConfig->GetFaceMotBlinkSize() || _autonomousBlinkPattern < 0) {
			m_AutonomousBlinkTime.SetDurTime(0);
			m_pReadCommand->SetFace( 30 - 1, 0.0 );
			_autonomousBlinkMode = false;
		}
		// 時間を超えたとき
		else if( m_AutonomousBlinkTime.GetDurTime() >= m_pConfig->GetFaceMotBlinkStrSize(_autonomousBlinkPattern - 1) ) {
			m_AutonomousBlinkTime.SetDurTime(0);
			m_pReadCommand->SetFace( 30 - 1, 0.0 );
			_autonomousBlinkMode = false;
		} else {
			// 最初の処理
			if( m_AutonomousBlinkTime.GetDurTime() == 0 ) { 
				m_AutonomousBlinkTime.SetStartTime();
				m_AutonomousBlinkTime.SetDurTime(1);
			} else {
				m_AutonomousBlinkTime.GetDurTimeWithProcess();
			}
			if( m_AutonomousBlinkTime.GetDurTime() < m_pConfig->GetFaceMotBlinkStrSize(_autonomousBlinkPattern - 1) ) 
				m_pReadCommand->AnalysisCmd( m_pConfig->GetFaceMotBlinkStr(_autonomousBlinkPattern - 1,  m_AutonomousBlinkTime.GetDurTime() ));
		}
	}
	return true;
}

bool checkAutonomousMove(void)
{
	TRACE("Check AutonomousMove");

	int AutonomousMotion = m_pReadCommand->GetAutonomousPattern(MOVE);
	int AutonomousUserdef = m_pReadCommand->GetAutonomousPattern(USERDEF_AUTONOMOUS);
	int nowMask = m_pReadCommand->GetNowMask();

	static bool autonomousFlag = true;

	double *headRot = m_pFaceView[nowMask]->GetHeadRot();


	// for Autonomous HeadMove
	if( AutonomousMotion == 0 && AutonomousUserdef == 0) {
		if( autonomousFlag == true ) {
			m_AutonomousMoveTime.SetDurTime(0);
			autonomousFlag = false;
			m_pConfig->ResetAutonomousMoveFrame();
		}
	}
	else if( AutonomousMotion != 0 ) {
		//cout << "checkAutoMotion " << m_AutonomousMoveTime.GetDurTime() << endl; 
		if( AutonomousMotion > m_pConfig->GetAutonomousMoveSize(MOVE) ) {
			m_pConfig->ResetAutonomousMoveFrame();
			return false;
		}
		if( m_AutonomousMoveTime.GetDurTime() >= m_pConfig->GetAutonomousMoveFrameSize(MOVE, AutonomousMotion - 1) ) {
			m_AutonomousMoveTime.SetDurTime(0);
		}
		// 最初の処理
		if( m_AutonomousMoveTime.GetDurTime() == 0 ) { 
			m_AutonomousMoveTime.SetStartTime();
			m_AutonomousMoveTime.SetDurTime(1);
		} else {
			m_AutonomousMoveTime.GetDurTimeWithProcess();
		}

		m_pConfig->GetAutonomousMoveCurrentFrame(MOVE, AutonomousMotion - 1, m_AutonomousMoveTime.GetDurTime());
		autonomousFlag = true;
	}
	else if( AutonomousUserdef != 0 ) {
		//cout << "checkAutoUserDef " << m_AutonomousMoveTime.GetDurTime() << endl; 
		if( AutonomousUserdef > m_pConfig->GetAutonomousMoveSize(USERDEF_AUTONOMOUS) ) {
			m_pConfig->ResetAutonomousMoveFrame();
			return false;
		}
		if( m_AutonomousMoveTime.GetDurTime() >= m_pConfig->GetAutonomousMoveFrameSize(USERDEF_AUTONOMOUS, AutonomousUserdef - 1) ) {
			m_AutonomousMoveTime.SetDurTime(0);
		}
		// 最初の処理
		if( m_AutonomousMoveTime.GetDurTime() == 0 ) { 
			m_AutonomousMoveTime.SetStartTime();
			m_AutonomousMoveTime.SetDurTime(1);
		} else {
			m_AutonomousMoveTime.GetDurTimeWithProcess();
		}
		m_pConfig->GetAutonomousMoveCurrentFrame(USERDEF_AUTONOMOUS, AutonomousUserdef - 1, m_AutonomousMoveTime.GetDurTime());
		autonomousFlag = true;
	}

	// += になっていることに注意
	headRot[0] += m_pConfig->GetAutonomousMoveAngle(0);
	headRot[1] += m_pConfig->GetAutonomousMoveAngle(1);
	headRot[2] += m_pConfig->GetAutonomousMoveAngle(2);

	return true;
}

void makeLinearFunc( double x1, double y1, double x2, double y2, double &a, double &b)
{
	a = (y1 -y2) / (x1 -x2);
	b = y1 - a * x1;
}

