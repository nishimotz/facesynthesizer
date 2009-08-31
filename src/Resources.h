#ifndef __RESOURCES_H__
#define __RESOURCES_H__

//////////////////////////////
// General Resorces  ... start
const int MAX_FACE_NUMBER = 10;

const int MAX_AUTONOMOUS_NUMER = 10;

const int MAX_AU_NUMBER = 65; // max amount of AU
const int MAX_MU_NUMBER = 17; // max amount of MU
const int MAX_VM_NUMBER = 27; // max amount of VM

// Object name
const char *const U_TEETH_OBJ_NAME = "../data/teethUpper.obj";
const char *const L_TEETH_OBJ_NAME = "../data/teethLower.obj";
const char *const M_WALL_OBJ_NAME = "../data/mouthwall.obj";

// Object name
const char *const EYE_OBJ_NAME = "../data/eye.obj";

// Texture name
const char *const M_WALL_TEXTURE = "../data/mouthwall.bmp";

// Version
const char *const WINDOW_TITLE = "Face Synthesis Module v2.35";
const char *const MODULE_VERSION_NAME = "Module Ver. 2.35";

// Define files
const char *const COMMAND_SET_FILE = "../data/command.txt";
const char *const VISEME_JP_FILE = "../data/viseme_jp.txt";
const char *const VISEME_EN_FILE = "../data/viseme_en.txt";

const char *const CONFIG_FILE = "../data/config.txt";

// Face 
const int DEF_WINDOW_WIDTH = 256;
const int DEF_WINDOW_HEIGHT = 256;
const int DEF_WINDOW_POS_X = 40;
const int DEF_WINDOW_POS_Y = 100;


const char *const DEF_WFM_PNT = "../data/fheadv2.pnt";
const char *const DEF_WFM_LNK = "../data/fheadv2.lnk";

const int EXPRESSION_DURATION = 100;

const int EYEMOVE_DURATION = 70;

enum SELECT_EYE {
	EYE_RIGHT = 0,
		EYE_LEFT
};


// General Resorces  ... end
////////////////////////////



/////////////////////////////////
// Command Set Resorces ... start

// "Run" command 
const int MAX_RUN_NUMBER = 2;
const char *const RUN_NAME[] = {
	"LIVE",
		"DEAD" 
};
enum RUN_ID {
	LIVE = 0,
		DEAD
};

// "set Run" command
const char *const SET_RUN_NAME[] = {
	"INIT",
		"EXIT"
};
enum SET_RUN_ID {
	INIT = 0,
		EXIT,
		MAX_SET_RUN_NUMBER
};

// "FaceMot" Command
const int MAX_FACE_MOT_NUMBER = 6;
const char *const FACE_MOT_NAME[] = {
	"NOMOVE", "BLINK", "NOD", "REFUSE", "LISTEN", "USERDEF"
};
enum FACE_MOT_ID {
	NOMOVE = 0,
		BLINK,
		NOD,
		REFUSE,
		LISTEN,
		USERDEF_FACEMOT,
		FACE_MOT_ID_NUM
};

// "FaceExp" Command
const int MAX_FACE_EXP_NUMBER = 8;
const char *const FACE_EXP_NAME[] = {
	"HAPPY", "ANGRY", "DISGUSTED", "SAD", "SURPRISED",
		"FEARED", "USERDEF", "NEUTRAL"
};
enum FACE_EXP_ID {
	HAPPY = 0,
		ANGRY,
		DISGUSTED,
		SAD,
		SURPRISE,
		FEARED,
		USERDEF_EXP,
		NEUTRAL
};

// "ViewMode"
const char *const VIEW_MODE_NAME[] = {
	"TEXTURE",
		"TEXTURE_WITH_WIREFRAME",
		"WIREFRAME"
};
enum VIEW_MODE_ID {
	VIEW_TEXTURE = 0,
		VIEW_TEXTURE_WITH_WIREFRAME,
		VIEW_WIREFRAME,
		MAX_VIEW_MODE_NUMBER
};

// "ViewBackground"
const char *const VIEW_BACKGROUND_NAME[] = {
	"ENABLE",
		"DISABLE",
};
enum VIEW_BACKGROUND_ID {
	VIEW_ENABLE = 0,
		VIEW_DISABLE,
		MAX_VIEW_BACKGROUND_NUMBER
};


// "Speak" "LipSync" Command
const int MAX_SPEAK_NUMBER = 4;
const char *const SPEAK_NAME[] = {
	"IDLE", "PROCESSING", "READY", "SPEAKING"
};
enum SPEAK_ID {
	IDLE = 0,
		PROCESSING,
		READY,
		SPEAKING
};
enum SPEAK_INQ_ID {
	NOW = 0,
		STOP
};

// "Autnomous" Command
const char *const AUTONOMOUS_NAME[] = {
	"BLINK", "MOVE", "USERDEF"
};

enum AUTONOMOUS_ID {
	BLINKING = 0,
		MOVE,
		USERDEF_AUTONOMOUS,
		AUTONOMOUS_ID_NUM
};

// "AgentEnable Command"
const char *const AGENT_ENABLE_NAME[] = {
	"ENABLE",
		"DISABLE"
};

const char *const CAPTURE_NAME[] = {
	"ENABLE", "DISABLE"
};

// Command Set Resorces ... end
///////////////////////////////

#endif
