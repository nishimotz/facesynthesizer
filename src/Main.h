#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>

#ifndef WIN32_GALATEA
unsigned __stdcall thread(LPVOID pParam);
#endif

#endif

#include <stdio.h>
#include <stdlib.h>

#include "FaceEditApp.h"
#include "FaceModel.h"
#include "SetViseme.h"
#include "ReadCommand.h"
#include "UnitMove.h"
#include "Configuration.h"


#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ïWèÄì¸óÕä÷òAÇÃInclude
#ifndef WIN32
#include <termio.h>            /* System V */
#if 0
#include <sgtty.h>             /* 4.2/3 BSD */
#endif
#endif

#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "ControlTime.h"
#include "FaceView.h"
#include "Resources.h"
#include "Resources_AU.h"
#include "Resources_MU.h"
#include "Resources_VM.h"

class FaceEditApp;
class FaceView;
class UnitMove;
class FaceModel;
class Texture;
class SetViseme;
class ReadCommand;
class Configuration;

/***** original values ******/
float _framerate;
int _main_width, _main_height;

/********** Terminal I/O variables **************/
#ifndef WIN32
struct termio tty,oldtty;             /* System V */
#if 0
struct sgttyb ttyb,oldttyb;           /* 4.2/3 BSD */
#endif
#endif

#ifdef WIN32
	SOCKET dstSocket;
	char rec_buf[2048];
#endif

float g_AUNumber[AU_MOVE_NUM];
float g_MUNumber[MU_MOVE_NUM];
float g_VMNumber[VM_MOVE_NUM];

// éûä‘ä«óù
#ifndef WIN32
struct timezone m_tz; 
#endif

/********** Face Move Class **********/
FaceEditApp *m_pFaceEditApp[MAX_FACE_NUMBER];
FaceView *m_pFaceView[MAX_FACE_NUMBER];
UnitMove *m_pAuMove[MAX_FACE_NUMBER];
UnitMove *m_pMuMove[MAX_FACE_NUMBER];
UnitMove *m_pVmMove[MAX_FACE_NUMBER];
FaceModel *m_pFaceModel[MAX_FACE_NUMBER];
Texture *m_pBGTexture[MAX_FACE_NUMBER];
SetViseme *m_pSetViseme[MAX_FACE_NUMBER];
ReadCommand *m_pReadCommand;
Configuration *m_pConfig;



/********** For Time Control **************/
ControlTime m_FPSTime;
ControlTime m_FPSCtrlTime;
ControlTime m_MouthTime;
ControlTime m_ExpTime;
ControlTime m_HeadTime;
ControlTime m_BlinkTime;
ControlTime m_AutonomousBlinkTime;
ControlTime m_AutonomousMoveTime;
ControlTime m_EyeTime[2];


bool checkArg(int &argc, char **argv, int pos[2], int size[2]);

/********** method ************/
void animation(bool flag);
void getFrameRate();

void myGlutIdle(void);
void myGlutDisplay(void);
void myGlutReshape(int x, int y);
void myGlutTimer(int value);
void myMousefn(int button, int updown, int x, int y);
void myMotionfn(int x, int y);
void myKeyfn ( unsigned char key, int, int );

bool checkFaceExpression(void);
bool checkMouthMotion(void);
bool checkEyeBlink(void);
bool checkHeadMove(void);
bool checkEyeMove(int num);
bool checkAutonomousBlink(void);
bool checkAutonomousMove(void);

void makeLinearFunc( double x1, double y1, double x2, double y2, double &a, double &b);

int _autonomousBlinkPattern;
bool _autonomousBlinkMode;

#endif

