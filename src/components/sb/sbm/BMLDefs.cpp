#include "sbm/BMLDefs.h"

namespace BML
{

XMLCh* BMLDefs::TAG_BML = NULL;
XMLCh* BMLDefs::ATTR_ID = NULL;
XMLCh* BMLDefs::ATTR_TYPE = NULL;
XMLCh* BMLDefs::ATTR_NAME = NULL;
XMLCh* BMLDefs::ATTR_LEVEL = NULL;
XMLCh* BMLDefs::ATTR_HANDLE = NULL;
XMLCh* BMLDefs::ATTR_LEXEME = NULL;
XMLCh* BMLDefs::ATTR_FILTERED = NULL;

XMLCh* BMLDefs::ATTR_START = NULL;
XMLCh* BMLDefs::ATTR_READY = NULL;
XMLCh* BMLDefs::ATTR_STROKE_START = NULL;
XMLCh* BMLDefs::ATTR_STROKE = NULL;
XMLCh* BMLDefs::ATTR_STROKE_END = NULL;
XMLCh* BMLDefs::ATTR_RELAX = NULL;
XMLCh* BMLDefs::ATTR_END = NULL;

XMLCh* BMLDefs::ATTR_PRESTROKE_HOLD = NULL;
XMLCh* BMLDefs::ATTR_POSTSTROKE_HOLD = NULL;
XMLCh* BMLDefs::ATTR_PRESTROKE_HOLD_IDLEMOTION = NULL;
XMLCh* BMLDefs::ATTR_POSTSTROKE_HOLD_IDLEMOTION = NULL;
XMLCh* BMLDefs::ATTR_SCALE = NULL;
XMLCh* BMLDefs::ATTR_FREQUENCY = NULL;
XMLCh* BMLDefs::ATTR_PRIORITY = NULL;
XMLCh* BMLDefs::ATTR_GROUP = NULL;
XMLCh* BMLDefs::ATTR_EMOTION = NULL;

XMLCh* BMLDefs::TM_START = NULL;
XMLCh* BMLDefs::TM_READY = NULL;
XMLCh* BMLDefs::TM_STROKE_START = NULL;
XMLCh* BMLDefs::TM_STROKE = NULL;
XMLCh* BMLDefs::TM_STROKE_END = NULL;
XMLCh* BMLDefs::TM_RELAX = NULL;
XMLCh* BMLDefs::TM_END = NULL;

XMLCh* BMLDefs::TAG_SBM_ANIMATION = NULL;
XMLCh* BMLDefs::ATTR_SPEED = NULL;
XMLCh* BMLDefs::ATTR_PVALUE = NULL;

XMLCh* BMLDefs::TAG_REF 				= NULL;
XMLCh* BMLDefs::TAG_CURVE			= NULL;
XMLCh* BMLDefs::TAG_NUM_KEYS			= NULL;
XMLCh* BMLDefs::TAG_LIPS				= NULL;
XMLCh* BMLDefs::TAG_ARTICULATION		= NULL;
XMLCh* BMLDefs::TAG_SYNC 			= NULL;
XMLCh* BMLDefs::TAG_TIME 			= NULL;

XMLCh* BMLDefs::TAG_BODYREACH = NULL;

XMLCh* BMLDefs::TAG_CONSTRAINT = NULL;

XMLCh* BMLDefs::TAG_SBM_EVENT = NULL;
XMLCh* BMLDefs::ATTR_MESSAGE = NULL;

XMLCh* BMLDefs::TAG_FACE = NULL;
	
XMLCh* BMLDefs::TAG_GAZE = NULL;

XMLCh* BMLDefs::TAG_PARAM = NULL;

XMLCh* BMLDefs::TAG_INTERRUPT = NULL;

XMLCh* BMLDefs::TAG_LOCOTMOTION = NULL;

XMLCh* BMLDefs::TAG_QUICKDRAW = NULL;
XMLCh* BMLDefs::TAG_REACH = NULL;
XMLCh* BMLDefs::TAG_NOISE = NULL;

XMLCh* BMLDefs::TAG_SPEECH = NULL;
XMLCh* BMLDefs::TAG_SBM_SPEECH = NULL;
XMLCh* BMLDefs::ATTR_TARGET	= NULL;


	// XMLStrings (utf-16 character arrays) for parsing vrSpeak's XML
XMLCh* BMLDefs::TAG_ACT = NULL;
XMLCh* BMLDefs::TAG_BODY  = NULL;
XMLCh* BMLDefs::TAG_GESTURE = NULL;
XMLCh* BMLDefs::TAG_TORSO  = NULL;
XMLCh* BMLDefs::TAG_REQUIRED = NULL;
XMLCh* BMLDefs::TAG_HEAD = NULL;
XMLCh* BMLDefs::TAG_TM  = NULL;
XMLCh* BMLDefs::TAG_MARK = NULL;


XMLCh* BMLDefs::TAG_SBM_COMMAND = NULL;

	// Deprecated behavior tags
XMLCh* BMLDefs::TAG_ANIMATION = NULL;
XMLCh* BMLDefs::TAG_EVENT = NULL;

XMLCh* BMLDefs::TAG_PANIMATION = NULL;

	// XMLStrings (utf-16 character arrays) for parsing vrSpeak's XML
XMLCh* BMLDefs::ATTR_SPEAKER = NULL;
XMLCh* BMLDefs::ATTR_ADDRESSEE = NULL;
XMLCh* BMLDefs::ATTR_CONTENTTYPE = NULL;
XMLCh* BMLDefs::ATTR_LANG  = NULL;
XMLCh* BMLDefs::ATTR_TID = NULL;
XMLCh* BMLDefs::ATTR_POSTURE = NULL;
XMLCh* BMLDefs::ATTR_REPEATS = NULL;
XMLCh* BMLDefs::ATTR_AMOUNT  = NULL;
XMLCh* BMLDefs::ATTR_VELOCITY  = NULL;
XMLCh* BMLDefs::ATTR_ANGLE  = NULL;
XMLCh* BMLDefs::ATTR_DIRECTION = NULL;
XMLCh* BMLDefs::ATTR_ROLL = NULL;
XMLCh* BMLDefs::ATTR_SMOOTH = NULL;

	////// XML Direction constants
	// Angular (gaze) and orienting (head)
XMLCh* BMLDefs::DIR_RIGHT = NULL;
XMLCh* BMLDefs::DIR_LEFT = NULL;
XMLCh* BMLDefs::DIR_UP = NULL;
XMLCh* BMLDefs::DIR_DOWN = NULL;
	// Angular only
XMLCh* BMLDefs::DIR_UPRIGHT = NULL;
XMLCh* BMLDefs::DIR_UPLEFT = NULL;
XMLCh* BMLDefs::DIR_DOWNRIGHT = NULL;
XMLCh* BMLDefs::DIR_DOWNLEFT = NULL;
XMLCh* BMLDefs::DIR_POLAR = NULL;
	// Orienting only
XMLCh* BMLDefs::DIR_ROLLRIGHT = NULL;
XMLCh* BMLDefs::DIR_ROLLLEFT = NULL;

////// XML Tags
XMLCh* BMLDefs::TAG_DESCRIPTION = NULL;

////// BML Description Type
XMLCh* BMLDefs::DTYPE_SBM = NULL;

////// XML ATTRIBUTES
XMLCh* BMLDefs::ATTR_ROOT = NULL;
XMLCh* BMLDefs::ATTR_ROOTJOINT = NULL;
XMLCh* BMLDefs::ATTR_EFFECTOR = NULL;
XMLCh* BMLDefs::ATTR_CONSTRAINT_TYPE = NULL;
XMLCh* BMLDefs::ATTR_EFFECTOR_ROOT = NULL;
XMLCh* BMLDefs::ATTR_FADE_OUT = NULL;
XMLCh* BMLDefs::ATTR_FADE_IN = NULL;

XMLCh* BMLDefs::ATTR_OFFSET_ROTX = NULL;
XMLCh* BMLDefs::ATTR_OFFSET_ROTY = NULL;
XMLCh* BMLDefs::ATTR_OFFSET_ROTZ = NULL;
//XMLCh* BMLDefs::ATTR_OFFSET_POS[]         = L"offset-pos";

XMLCh* BMLDefs::ATTR_RAMPUP = NULL;
XMLCh* BMLDefs::ATTR_RAMPDOWN = NULL;
XMLCh* BMLDefs::ATTR_DURATION = NULL;

XMLCh* BMLDefs::ATTR_X = NULL;
XMLCh* BMLDefs::ATTR_Y = NULL;
XMLCh* BMLDefs::ATTR_Z = NULL;

XMLCh* BMLDefs::ATTR_POSX = NULL;
XMLCh* BMLDefs::ATTR_POSY = NULL;
XMLCh* BMLDefs::ATTR_POSZ = NULL;

XMLCh* BMLDefs::ATTR_ROTX = NULL;
XMLCh* BMLDefs::ATTR_ROTY = NULL;
XMLCh* BMLDefs::ATTR_ROTZ = NULL;

////// XML ATTRIBUTES
XMLCh* BMLDefs::ATTR_TARGET_POS = NULL;
XMLCh* BMLDefs::ATTR_REACH_VELOCITY = NULL;
XMLCh* BMLDefs::ATTR_REACH_FINISH = NULL;
XMLCh* BMLDefs::ATTR_REACH_TYPE = NULL;
//XMLCh* BMLDefs::ATTR_APEX_DURATION[] = L"sbm:apex-duration";

XMLCh* BMLDefs::ATTR_ROTATION = NULL;
XMLCh* BMLDefs::ATTR_ENABLE = NULL;

XMLCh* BMLDefs::TAG_VISEME = NULL;
XMLCh* BMLDefs::TAG_SOUND  = NULL;
XMLCh* BMLDefs::ATTR_AU = NULL;
XMLCh* BMLDefs::ATTR_SIDE = NULL;

////// XML ATTRIBUTES
XMLCh* BMLDefs::ATTR_SBM_ROLL = NULL;
XMLCh* BMLDefs::ATTR_JOINT_RANGE = NULL;
XMLCh* BMLDefs::ATTR_JOINT_SPEED = NULL;
XMLCh* BMLDefs::ATTR_TIME_HINT = NULL;
XMLCh* BMLDefs::ATTR_JOINT_SMOOTH = NULL;
XMLCh* BMLDefs::ATTR_PITCH = NULL;
XMLCh* BMLDefs::ATTR_HEADING = NULL;
XMLCh* BMLDefs::ATTR_BLEND = NULL;
XMLCh* BMLDefs::ATTR_INTERPOLATE_BIAS = NULL;

XMLCh* BMLDefs::ATTR_PRIORITY_JOINT = NULL;
XMLCh* BMLDefs::ATTR_PITCH_MIN = NULL;
XMLCh* BMLDefs::ATTR_PITCH_MAX = NULL;

XMLCh* BMLDefs::VALUE_TEXT_PLAIN = NULL;
XMLCh* BMLDefs::VALUE_SSML = NULL;

XMLCh*  BMLDefs::start_id = NULL;
XMLCh*  BMLDefs::end_id   = NULL;

XMLCh* BMLDefs::ATTR_REACH_ARM = NULL;
XMLCh* BMLDefs::ATTR_USE_EXAMPLE = NULL;
XMLCh* BMLDefs::ATTR_BUILD_EXAMPLE = NULL;
XMLCh* BMLDefs::ATTR_EXAMPLE_DIST = NULL;
XMLCh* BMLDefs::ATTR_RESAMPLE_SIZE = NULL;

XMLCh* BMLDefs::ATTR_ANIM = NULL;
XMLCh* BMLDefs::ATTR_TRACK_DUR = NULL;
XMLCh* BMLDefs::ATTR_GUNDRAW_DUR = NULL;
XMLCh* BMLDefs::ATTR_HOLSTER_DUR = NULL;
XMLCh* BMLDefs::ATTR_AIM_OFFSET = NULL;


XMLCh* BMLDefs::ATTR_ANIM1 = NULL;
XMLCh* BMLDefs::ATTR_ANIM2 = NULL;
XMLCh* BMLDefs::ATTR_LOOP = NULL;
XMLCh* BMLDefs::ATTR_WRAPMODE = NULL;
XMLCh* BMLDefs::ATTR_SCHEDULEMODE = NULL;
XMLCh* BMLDefs::ATTR_BLENDMODE = NULL;
XMLCh* BMLDefs::ATTR_TRANSITION_LENGTH = NULL;
XMLCh* BMLDefs::ATTR_DIRECTPLAY = NULL;

////// XML ATTRIBUTES
XMLCh* BMLDefs::ATTR_VALUE = NULL;

XMLCh* BMLDefs::ATTR_TWARP = NULL;

XMLCh* BMLDefs::ATTR_TRUE = NULL;
XMLCh* BMLDefs::ATTR_FALSE = NULL;
XMLCh* BMLDefs::ATTR_RESET = NULL;

XMLCh* BMLDefs::ATTR_SHAKE  = NULL;
XMLCh* BMLDefs::ATTR_TOSS   = NULL;
XMLCh* BMLDefs::ATTR_ORIENT   = NULL;
XMLCh* BMLDefs::ATTR_NOD   = NULL;


XMLCh* BMLDefs::ATTR_ARMLEFT = NULL;
XMLCh* BMLDefs::ATTR_ARMRIGHT = NULL;
XMLCh* BMLDefs::ATTR_FACS = NULL;

XMLCh* BMLDefs::ATTR_RPS = NULL;
XMLCh* BMLDefs::ATTR_GRPS = NULL;
XMLCh* BMLDefs::ATTR_LRPS = NULL;

XMLCh* BMLDefs::ATTR_EYEBROWS = NULL;
XMLCh* BMLDefs::ATTR_EYELIDS = NULL;
XMLCh* BMLDefs::ATTR_MOUTH = NULL;

XMLCh* BMLDefs::TAG_GRAB = NULL;
XMLCh* BMLDefs::ATTR_CONS_JOINT = NULL;
XMLCh* BMLDefs::ATTR_CONS_TARGET = NULL;

XMLCh* BMLDefs::ATTR_WRIST = NULL;
XMLCh* BMLDefs::ATTR_GRAB_SPEED = NULL;
XMLCh* BMLDefs::ATTR_GRAB_FINISH = NULL;
XMLCh* BMLDefs::ATTR_GRAB_TYPE = NULL;
XMLCh* BMLDefs::ATTR_APEX_DURATION = NULL;
XMLCh* BMLDefs::ATTR_OBSTACLE = NULL;
XMLCh* BMLDefs::ATTR_GRAB_STATE = NULL;
XMLCh* BMLDefs::ATTR_REACH_ACTION = NULL;
XMLCh* BMLDefs::ATTR_REACH_DURATION = NULL;

XMLCh* BMLDefs::ATTR_PROXIMITY = NULL;
XMLCh* BMLDefs::ATTR_MANNER = NULL;
XMLCh* BMLDefs::ATTR_FACING = NULL;
XMLCh* BMLDefs::ATTR_FOLLOW = NULL;
XMLCh* BMLDefs::ATTR_NUM_STEPS = NULL;
XMLCh* BMLDefs::ATTR_SPD = NULL;
XMLCh* BMLDefs::ATTR_SOURCE_JOINT = NULL;
XMLCh* BMLDefs::ATTR_ATTACH_PAWN = NULL;
XMLCh* BMLDefs::ATTR_RELEASE_PAWN = NULL;
XMLCh* BMLDefs::ATTR_FOOT_IK = NULL;
XMLCh* BMLDefs::ATTR_USE_LOCOMOTION = NULL;
XMLCh* BMLDefs::TAG_SACCADE = NULL;
XMLCh* BMLDefs::ATTR_MAGNITUDE = NULL;
XMLCh* BMLDefs::ATTR_MODE = NULL;
XMLCh* BMLDefs::ATTR_ANGLELIMIT = NULL;
XMLCh* BMLDefs::ATTR_FINISH = NULL;
XMLCh* BMLDefs::ATTR_STEERACCEL = NULL;
XMLCh* BMLDefs::ATTR_STEERANGLEACCEL = NULL;
XMLCh* BMLDefs::ATTR_STEERSCOOTACCEL = NULL;
XMLCh* BMLDefs::ATTR_NODAXIS = NULL;
XMLCh* BMLDefs::ATTR_NODACCEL = NULL;
XMLCh* BMLDefs::ATTR_NODPERIOD = NULL;
XMLCh* BMLDefs::ATTR_NODWARP = NULL;
XMLCh* BMLDefs::ATTR_NODPITCH = NULL;
XMLCh* BMLDefs::ATTR_NODDECAY = NULL;
XMLCh* BMLDefs::ATTR_WIGGLE = NULL;
XMLCh* BMLDefs::ATTR_WAGGLE = NULL;
XMLCh* BMLDefs::ATTR_PARAMETERIZED = NULL;

XMLCh* BMLDefs::TAG_STATES = NULL;
XMLCh* BMLDefs::TAG_BLEND = NULL;
XMLCh* BMLDefs::ATTR_STARTINGNOW = NULL;
XMLCh* BMLDefs::ATTR_ADDITIVE = NULL;
XMLCh* BMLDefs::ATTR_PARTIALJOINT = NULL;

XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN0 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN45 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN90 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN135 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN180 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN225 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN270 = NULL;
XMLCh* BMLDefs::ATTR_PERCENTAGE_BIN315 = NULL;
XMLCh* BMLDefs::ATTR_SACCADE_INTERVAL_MEAN = NULL;
XMLCh* BMLDefs::ATTR_SACCADE_INTERVAL_VARIANT = NULL;

// OpenCollada
XMLCh* BMLDefs::ATTR_SID = NULL;
XMLCh* BMLDefs::ATTR_STRIDE = NULL;
XMLCh* BMLDefs::ATTR_COUNT = NULL;
XMLCh* BMLDefs::ATTR_INPUT = NULL;
XMLCh* BMLDefs::ATTR_SEMANTIC = NULL;
XMLCh* BMLDefs::ATTR_VCOUNT = NULL;
XMLCh* BMLDefs::ATTR_SOURCE = NULL;
XMLCh* BMLDefs::ATTR_URL = NULL;
XMLCh* BMLDefs::ATTR_OFFSET = NULL;
XMLCh* BMLDefs::ATTR_TRIM = NULL;
XMLCh* BMLDefs::ATTR_SYMBOL = NULL;
XMLCh* BMLDefs::ATTR_MATERIAL = NULL;
XMLCh* BMLDefs::ATTR_OPAQUE = NULL;
XMLCh* BMLDefs::ATTR_TEXTURE = NULL;



// locomotion
XMLCh* BMLDefs::ATTR_BRAKING = NULL;

XMLCh* BMLDefs::ATTR_STYLE = NULL;

XMLCh* BMLDefs::ATTR_STATENAME = NULL;

XMLCh* BMLDefs::ATTR_POLICY = NULL;
XMLCh* BMLDefs::ATTR_PROCID = NULL;

XMLCh* BMLDefs::OGRE_BONE = NULL;
XMLCh* BMLDefs::OGRE_PARENT = NULL;
XMLCh* BMLDefs::OGRE_ID = NULL;
XMLCh* BMLDefs::OGRE_VERTEX_COUNT = NULL;
XMLCh* BMLDefs::OGRE_NAME = NULL;
XMLCh* BMLDefs::OGRE_MATERIAL = NULL;
XMLCh* BMLDefs::OGRE_LENGTH = NULL;
XMLCh* BMLDefs::OGRE_U = NULL;
XMLCh* BMLDefs::OGRE_V = NULL;
XMLCh* BMLDefs::OGRE_V1 = NULL;
XMLCh* BMLDefs::OGRE_V2 = NULL;
XMLCh* BMLDefs::OGRE_V3 = NULL;
XMLCh* BMLDefs::OGRE_T1 = NULL;
XMLCh* BMLDefs::OGRE_T2 = NULL;
XMLCh* BMLDefs::OGRE_T3 = NULL;
XMLCh* BMLDefs::OGRE_X = NULL;
XMLCh* BMLDefs::OGRE_Y = NULL;
XMLCh* BMLDefs::OGRE_Z = NULL;
XMLCh* BMLDefs::OGRE_ANGLE = NULL;
XMLCh* BMLDefs::OGRE_AXIS = NULL;
XMLCh* BMLDefs::OGRE_ANIMATION = NULL;
XMLCh* BMLDefs::OGRE_TRACKS = NULL;
XMLCh* BMLDefs::OGRE_TRACK = NULL;
XMLCh* BMLDefs::OGRE_KEYFRAMES = NULL;
XMLCh* BMLDefs::OGRE_KEYFRAME = NULL;
XMLCh* BMLDefs::OGRE_TRANSLATE = NULL;
XMLCh* BMLDefs::OGRE_ROTATE = NULL;
XMLCh* BMLDefs::OGRE_TIME = NULL;
XMLCh* BMLDefs::OGRE_VERTEX_INDEX = NULL;
XMLCh* BMLDefs::OGRE_BONE_INDEX = NULL;
XMLCh* BMLDefs::OGRE_WEIGHT = NULL;


BMLDefs::BMLDefs()
{
	ATTR_ID = XMLString::transcode("id");
	ATTR_TYPE = XMLString::transcode("type");
	ATTR_NAME = XMLString::transcode("name");
	ATTR_LEVEL = XMLString::transcode("level");
	ATTR_HANDLE = XMLString::transcode("sbm:handle");
	ATTR_LEXEME = XMLString::transcode("lexeme");
	ATTR_FILTERED = XMLString::transcode("filtered");

	ATTR_START = XMLString::transcode("start");
	ATTR_READY = XMLString::transcode("ready");
	ATTR_STROKE_START = XMLString::transcode("stroke_start");
	ATTR_STROKE = XMLString::transcode("stroke");
	ATTR_STROKE_END = XMLString::transcode("stroke_end");
	ATTR_RELAX = XMLString::transcode("relax");
	ATTR_END = XMLString::transcode("end");

	ATTR_PRESTROKE_HOLD =  XMLString::transcode("prestroke_hold");
	ATTR_PRESTROKE_HOLD_IDLEMOTION = XMLString::transcode("prestroke_hold_idlemotion");
	ATTR_POSTSTROKE_HOLD =  XMLString::transcode("poststroke_hold");
	ATTR_POSTSTROKE_HOLD_IDLEMOTION = XMLString::transcode("poststroke_hold_idlemotion");
	ATTR_SCALE = XMLString::transcode("sbm:scale");
	ATTR_FREQUENCY = XMLString::transcode("sbm:frequency");
	ATTR_PRIORITY = XMLString::transcode("priority");
	ATTR_GROUP = XMLString::transcode("group");
	ATTR_EMOTION = XMLString::transcode("emotion");

	TM_START = XMLString::transcode("start");
	TM_READY = XMLString::transcode("ready");
	TM_STROKE_START = XMLString::transcode("stroke_start");
	TM_STROKE = XMLString::transcode("stroke");
	TM_STROKE_END = XMLString::transcode("stroke_end");
	TM_RELAX = XMLString::transcode("relax");
	TM_END = XMLString::transcode("end");

	TAG_SBM_ANIMATION		= XMLString::transcode("sbm:animation");
	ATTR_TWARP       		= XMLString::transcode("ME:twarp");
	ATTR_PVALUE				= XMLString::transcode("sbm:value");

	TAG_REF 				= XMLString::transcode("ref");
	TAG_CURVE				= XMLString::transcode("curve");
	TAG_NUM_KEYS			= XMLString::transcode("num_keys");
	TAG_LIPS				= XMLString::transcode("lips");
	TAG_ARTICULATION		= XMLString::transcode("articulation");
	TAG_SYNC				= XMLString::transcode("sync");
	TAG_TIME				= XMLString::transcode("time");

	TAG_BODYREACH			= XMLString::transcode("sbm:reach");

	TAG_CONSTRAINT			= XMLString::transcode("sbm:constraint");

	TAG_SBM_EVENT			= XMLString::transcode("sbm:event");
	ATTR_MESSAGE			= XMLString::transcode("message");

	TAG_FACE				= XMLString::transcode("face");
	
	TAG_GAZE				= XMLString::transcode("gaze");

	TAG_PARAM				= XMLString::transcode("param");

	TAG_INTERRUPT			= XMLString::transcode("sbm:interrupt");

	TAG_LOCOTMOTION			= XMLString::transcode("locomotion");

	TAG_QUICKDRAW			= XMLString::transcode("sbm:quickdraw");

	TAG_SACCADE				= XMLString::transcode("saccade");
	TAG_REACH				= XMLString::transcode("sbm:reach");
	TAG_NOISE    			= XMLString::transcode("sbm:noise");

	TAG_SPEECH				= XMLString::transcode("speech");     // Original tag, here for backward compatibility
	TAG_SBM_SPEECH			= XMLString::transcode("sbm:speech");
	ATTR_TARGET				= XMLString::transcode("target");

	TAG_SBM_EVENT			= XMLString::transcode("sbm:event");
	ATTR_MESSAGE			= XMLString::transcode("message");

	// XMLStrings (utf-16 character arrays) for parsing vrSpeak's XML
	TAG_ACT		= XMLString::transcode("act");
	TAG_BML       = XMLString::transcode("bml");
	TAG_BODY      = XMLString::transcode("body");
	TAG_TORSO      = XMLString::transcode("torso");
	TAG_REQUIRED  = XMLString::transcode("required");
	TAG_GESTURE = XMLString::transcode("gesture");
	TAG_HEAD      = XMLString::transcode("head");
	TAG_TM        = XMLString::transcode("tm");
	TAG_MARK      = XMLString::transcode("mark");


	TAG_SBM_COMMAND = XMLString::transcode("sbm:command");

	// Deprecated behavior tags
	TAG_ANIMATION = XMLString::transcode("animation");
	TAG_EVENT     = XMLString::transcode("event");

	TAG_PANIMATION = XMLString::transcode("panimation");
	TAG_REACH = XMLString::transcode("sbm:reach_old");

	// XMLStrings (utf-16 character arrays) for parsing vrSpeak's XML
	ATTR_SPEAKER      = XMLString::transcode("speaker");
	ATTR_ADDRESSEE    = XMLString::transcode("addressee");
	ATTR_CONTENTTYPE  = XMLString::transcode("contenttype");
	ATTR_LANG         = XMLString::transcode("lang");
	ATTR_TID          = XMLString::transcode("tid");
	ATTR_POSTURE      = XMLString::transcode("posture");
	ATTR_REPEATS      = XMLString::transcode("repeats");
	ATTR_AMOUNT       = XMLString::transcode("amount");
	ATTR_VELOCITY     = XMLString::transcode("velocity");
	ATTR_ANGLE        = XMLString::transcode("angle");
	ATTR_DIRECTION    = XMLString::transcode("direction");
	ATTR_ROLL         = XMLString::transcode("sbm:roll");
	ATTR_SMOOTH       = XMLString::transcode("sbm:smooth");

	////// XML Direction constants
	// Angular (gaze) and orienting (head)
	DIR_RIGHT        = XMLString::transcode("RIGHT");
	DIR_LEFT         = XMLString::transcode("LEFT");
	DIR_UP           = XMLString::transcode("UP");
	DIR_DOWN         = XMLString::transcode("DOWN");
	// Angular only
	DIR_UPRIGHT      = XMLString::transcode("UPRIGHT");
	DIR_UPLEFT       = XMLString::transcode("UPLEFT");
	DIR_DOWNRIGHT    = XMLString::transcode("DOWNRIGHT");
	DIR_DOWNLEFT     = XMLString::transcode("DOWNLEFT");
	DIR_POLAR        = XMLString::transcode("POLAR");
	// Orienting only
	DIR_ROLLRIGHT    = XMLString::transcode("ROLLRIGHT");
	DIR_ROLLLEFT     = XMLString::transcode("ROLLLEFT");


	////// XML Tags
	TAG_DESCRIPTION  = XMLString::transcode("description");

	////// BML Description Type
	DTYPE_SBM   = XMLString::transcode("ICT.SBM");

	////// XML ATTRIBUTES
	ATTR_ROOTJOINT  = XMLString::transcode("sbm:root-joint");
	ATTR_EFFECTOR  = XMLString::transcode("effector");
	ATTR_CONSTRAINT_TYPE  = XMLString::transcode("sbm:constraint-type");
	ATTR_EFFECTOR_ROOT  = XMLString::transcode("sbm:effector-root");
	ATTR_FADE_OUT		 = XMLString::transcode("sbm:fade-out");
	ATTR_FADE_IN		 = XMLString::transcode("sbm:fade-in");

	ATTR_OFFSET_ROTX         = XMLString::transcode("rot-x");
	ATTR_OFFSET_ROTY       = XMLString::transcode("rot-y");
	ATTR_OFFSET_ROTZ          = XMLString::transcode("rot-z");
	//ATTR_OFFSET_POS          = XMLString::transcode("offset-pos";
	
	ATTR_RAMPUP 	= XMLString::transcode("sbm:rampup");
	ATTR_RAMPDOWN	= XMLString::transcode("sbm:rampdown");
	ATTR_DURATION	= XMLString::transcode("sbm:duration");

	ATTR_X      = XMLString::transcode("pos-x");
	ATTR_Y      = XMLString::transcode("pos-y");
	ATTR_Z      = XMLString::transcode("pos-z");

	////// XML ATTRIBUTES
	ATTR_TARGET_POS  = XMLString::transcode("sbm:target-pos");
	ATTR_REACH_VELOCITY  = XMLString::transcode("sbm:reach-velocity");
	ATTR_REACH_DURATION  = XMLString::transcode("sbm:reach-duration");
	ATTR_REACH_FINISH  = XMLString::transcode("sbm:reach-finish");
	ATTR_REACH_TYPE  = XMLString::transcode("sbm:reach-type");
	//ATTR_APEX_DURATION  = XMLString::transcode("sbm:apex-duration";

	ATTR_ROTATION      = XMLString::transcode("rotation");
	ATTR_ENABLE      = XMLString::transcode("enable");
	ATTR_SPEED      = XMLString::transcode("speed");

	ATTR_X      = XMLString::transcode("x");
	ATTR_Y      = XMLString::transcode("y");
	ATTR_Z      = XMLString::transcode("z");

	ATTR_POSX      = XMLString::transcode("pos-x");
	ATTR_POSY      = XMLString::transcode("pos-y");
	ATTR_POSZ      = XMLString::transcode("pos-z");

	ATTR_ROTX      = XMLString::transcode("rot-x");
	ATTR_ROTY      = XMLString::transcode("rot-y");
	ATTR_ROTZ      = XMLString::transcode("rot-z");


	TAG_VISEME  = XMLString::transcode("viseme");
	TAG_SOUND  = XMLString::transcode("soundFile"); //this tag is used to rename the soundFile by Remote speech process
	ATTR_AU      = XMLString::transcode("au");
	ATTR_SIDE    = XMLString::transcode("side");	

	////// XML ATTRIBUTES
	ATTR_SBM_ROLL      = XMLString::transcode("sbm:roll");
	ATTR_JOINT_RANGE   = XMLString::transcode("sbm:joint-range");
	ATTR_JOINT_SPEED   = XMLString::transcode("sbm:joint-speed");
	ATTR_TIME_HINT	 = XMLString::transcode("sbm:time-hint");
	ATTR_JOINT_SMOOTH  = XMLString::transcode("sbm:speed-smoothing");
	ATTR_PITCH         = XMLString::transcode("pitch");
	ATTR_HEADING       = XMLString::transcode("heading");
	ATTR_ROLL          = XMLString::transcode("roll");
	ATTR_BLEND         = XMLString::transcode("blend");
	ATTR_INTERPOLATE_BIAS  = XMLString::transcode("interpolate-bias");

	ATTR_PRIORITY_JOINT  = XMLString::transcode("sbm:priority-joint");
	ATTR_PITCH_MIN	 = XMLString::transcode("pitch-min");
	ATTR_PITCH_MAX	 = XMLString::transcode("pitch-max");

//	TAG_MARK       = XMLString::transcode("mark");

	VALUE_TEXT_PLAIN  = XMLString::transcode("text/plain");
	VALUE_SSML        = XMLString::transcode("application/ssml+xml");

	start_id  = XMLString::transcode("bml:start");
	end_id    = XMLString::transcode("bml:end");

	////// XML ATTRIBUTES
	ATTR_REACH_ARM  = XMLString::transcode("reach-arm");
	ATTR_USE_EXAMPLE  = XMLString::transcode("use-example");
	ATTR_BUILD_EXAMPLE  = XMLString::transcode("build-example");
	ATTR_EXAMPLE_DIST  = XMLString::transcode("example-dist"); // minimal distances between pose examples
	ATTR_RESAMPLE_SIZE  = XMLString::transcode("resample-size"); // minimal distances between pose examples
	////// XML ATTRIBUTES
	ATTR_ANIM          = XMLString::transcode("anim");
	ATTR_TRACK_DUR     = XMLString::transcode("track-duration");
	ATTR_GUNDRAW_DUR    = XMLString::transcode("gundraw-duration");
	ATTR_HOLSTER_DUR    = XMLString::transcode("holster-duration");
	ATTR_AIM_OFFSET     = XMLString::transcode("aim-offset");


	ATTR_ANIM1  = XMLString::transcode("anim1");
	ATTR_ANIM2  = XMLString::transcode("anim2");
	ATTR_LOOP  = XMLString::transcode("loop");
	ATTR_WRAPMODE = XMLString::transcode("sbm:wrap-mode");
	ATTR_BLENDMODE = XMLString::transcode("sbm:blend-mode");
	ATTR_SCHEDULEMODE = XMLString::transcode("sbm:schedule-mode");
	ATTR_TRANSITION_LENGTH = XMLString::transcode("sbm:transition-length");
	ATTR_DIRECTPLAY = XMLString::transcode("sbm:direct-play");

	////// XML ATTRIBUTES
	ATTR_VALUE   = XMLString::transcode("value");

	ATTR_TRUE   = XMLString::transcode("true");
	ATTR_FALSE   = XMLString::transcode("false");
	ATTR_RESET   = XMLString::transcode("reset");

	ATTR_SHAKE  = XMLString::transcode("shake");
	ATTR_TOSS   = XMLString::transcode("toss");
	ATTR_ORIENT   = XMLString::transcode("orient");
	ATTR_NOD   = XMLString::transcode("nod");
	ATTR_WIGGLE   = XMLString::transcode("wiggle");
	ATTR_WAGGLE   = XMLString::transcode("waggle");
	ATTR_PARAMETERIZED = XMLString::transcode("parameterized");

	ATTR_ARMLEFT   = XMLString::transcode("left");
	ATTR_ARMRIGHT   = XMLString::transcode("right");

	ATTR_FACS   = XMLString::transcode("facs");
	ATTR_RPS = XMLString::transcode("rps");
	ATTR_GRPS = XMLString::transcode("grps");
	ATTR_LRPS= XMLString::transcode("lrps");

	ATTR_EYEBROWS = XMLString::transcode("eyebrows");
	ATTR_EYELIDS = XMLString::transcode("eyelids");
	ATTR_MOUTH = XMLString::transcode("mouth");

	ATTR_ROOT = XMLString::transcode("sbm:root");

	TAG_GRAB = XMLString::transcode("sbm:grab");
	ATTR_REACH_FINISH = XMLString::transcode("sbm:reach-finish");
	ATTR_CONS_JOINT = XMLString::transcode("sbm:cons-joint");
	ATTR_CONS_TARGET = XMLString::transcode("sbm:cons-target");
	ATTR_APEX_DURATION = XMLString::transcode("sbm:apex-duration");
	ATTR_WRIST = XMLString::transcode("sbm:wrist");
	ATTR_GRAB_SPEED = XMLString::transcode("sbm:grab-speed");
	ATTR_GRAB_FINISH = XMLString::transcode("sbm:grab-finish");
	ATTR_GRAB_TYPE = XMLString::transcode("sbm:grab-type");
	ATTR_OBSTACLE = XMLString::transcode("sbm:obstacle");
	ATTR_GRAB_STATE = XMLString::transcode("sbm:grab-state");
	ATTR_REACH_ACTION = XMLString::transcode("sbm:action");
	ATTR_PROXIMITY = XMLString::transcode("proximity");
	ATTR_MANNER = XMLString::transcode("manner");
	ATTR_FACING= XMLString::transcode("facing");
	ATTR_FOLLOW= XMLString::transcode("sbm:follow");
	ATTR_NUM_STEPS = XMLString::transcode("sbm:numsteps");
	ATTR_SPD = XMLString::transcode("spd");
	ATTR_SOURCE_JOINT = XMLString::transcode("sbm:source-joint");
	ATTR_ATTACH_PAWN = XMLString::transcode("sbm:attach-pawn");
	ATTR_RELEASE_PAWN = XMLString::transcode("sbm:release-pawn");
	ATTR_FOOT_IK = XMLString::transcode("sbm:foot-ik");
	ATTR_USE_LOCOMOTION = XMLString::transcode("sbm:use-locomotion");

	ATTR_MAGNITUDE = XMLString::transcode("magnitude");
	ATTR_MODE = XMLString::transcode("mode");
	ATTR_ANGLELIMIT = XMLString::transcode("angle-limit");
	ATTR_FINISH = XMLString::transcode("finish");

	ATTR_STEERACCEL = XMLString::transcode("sbm:accel");
	ATTR_STEERANGLEACCEL = XMLString::transcode("sbm:angleaccel");
	ATTR_STEERSCOOTACCEL = XMLString::transcode("sbm:scootaccel");

	ATTR_NODAXIS = XMLString::transcode("sbm:axis");
	ATTR_NODACCEL = XMLString::transcode("sbm:accel");
	ATTR_NODPERIOD = XMLString::transcode("sbm:period");
	ATTR_NODWARP = XMLString::transcode("sbm:warp");
	ATTR_NODPITCH = XMLString::transcode("sbm:pitch");
	ATTR_NODDECAY = XMLString::transcode("sbm:decay");

	TAG_STATES = XMLString::transcode("sbm:states");
	TAG_BLEND = XMLString::transcode("blend");
	ATTR_STARTINGNOW = XMLString::transcode("sbm:startnow");
	ATTR_ADDITIVE = XMLString::transcode("sbm:additive");
	ATTR_PARTIALJOINT = XMLString::transcode("sbm:partial-joint");

	ATTR_PERCENTAGE_BIN0 = XMLString::transcode("sbm:bin0");
	ATTR_PERCENTAGE_BIN45 = XMLString::transcode("sbm:bin45");
	ATTR_PERCENTAGE_BIN90 = XMLString::transcode("sbm:bin90");
	ATTR_PERCENTAGE_BIN135 = XMLString::transcode("sbm:bin135");
	ATTR_PERCENTAGE_BIN180 = XMLString::transcode("sbm:bin180");
	ATTR_PERCENTAGE_BIN225 = XMLString::transcode("sbm:bin225");
	ATTR_PERCENTAGE_BIN270 = XMLString::transcode("sbm:bin270");
	ATTR_PERCENTAGE_BIN315 = XMLString::transcode("sbm:bin315");
	ATTR_SACCADE_INTERVAL_MEAN = XMLString::transcode("sbm:mean");
	ATTR_SACCADE_INTERVAL_VARIANT = XMLString::transcode("sbm:variant");

	// OpenCollada
	ATTR_SID = XMLString::transcode("sid");
	ATTR_STRIDE = XMLString::transcode("stride");
	ATTR_COUNT = XMLString::transcode("count");
	ATTR_INPUT = XMLString::transcode("input");
	ATTR_SEMANTIC = XMLString::transcode("semantic");
	ATTR_VCOUNT = XMLString::transcode("vcount");
	ATTR_SOURCE = XMLString::transcode("source");
	ATTR_URL = XMLString::transcode("url");
	ATTR_OFFSET = XMLString::transcode("offset");
	ATTR_TRIM = XMLString::transcode("trim");
	ATTR_SYMBOL = XMLString::transcode("symbol");
	ATTR_MATERIAL = XMLString::transcode("material");
	ATTR_OPAQUE = XMLString::transcode("opaque");
	ATTR_TEXTURE = XMLString::transcode("texture");

	// locomotion
	ATTR_BRAKING = XMLString::transcode("sbm:braking");

	ATTR_STYLE = XMLString::transcode("sbm:style");

	ATTR_STATENAME = XMLString::transcode("sbm:state-name");

	ATTR_POLICY = XMLString::transcode("policy");
	ATTR_PROCID = XMLString::transcode("procid");

	OGRE_BONE = XMLString::transcode("bone");
	OGRE_PARENT = XMLString::transcode("parent");
	OGRE_ID = XMLString::transcode("id");
	OGRE_VERTEX_COUNT = XMLString::transcode("vertexcount");
	OGRE_NAME = XMLString::transcode("name");
	OGRE_MATERIAL = XMLString::transcode("material");
	OGRE_LENGTH = XMLString::transcode("length");
	OGRE_X = XMLString::transcode("x");
	OGRE_Y = XMLString::transcode("y");
	OGRE_Z = XMLString::transcode("z");
	OGRE_U = XMLString::transcode("u");
	OGRE_V = XMLString::transcode("v");
	OGRE_V1 = XMLString::transcode("v1");
	OGRE_V2 = XMLString::transcode("v2");
	OGRE_V3 = XMLString::transcode("v3");
	OGRE_T1 = XMLString::transcode("t1");
	OGRE_T2 = XMLString::transcode("t2");
	OGRE_T3 = XMLString::transcode("t3");
	OGRE_ANGLE = XMLString::transcode("angle");
	OGRE_AXIS = XMLString::transcode("axis");
	OGRE_ANIMATION = XMLString::transcode("animation");
	OGRE_TRACKS = XMLString::transcode("tracks");
	OGRE_TRACK = XMLString::transcode("track");
	OGRE_KEYFRAMES = XMLString::transcode("keyframes");
	OGRE_KEYFRAME = XMLString::transcode("keyframe");
	OGRE_TRANSLATE = XMLString::transcode("translate");
	OGRE_ROTATE = XMLString::transcode("rotate");
	OGRE_TIME = XMLString::transcode("time");
	OGRE_VERTEX_INDEX = XMLString::transcode("vertexindex");
	OGRE_BONE_INDEX = XMLString::transcode("boneindex");
	OGRE_WEIGHT = XMLString::transcode("weight");

}

BMLDefs::~BMLDefs()
{

}

}
