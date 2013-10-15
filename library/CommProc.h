#pragma once

#include "pipeline_api.h"

#define MAX_SIZE 2

#define CAM_TO_MGR	"/mnt/f0/pipe/cam_to_mgr"
#define ROT_TO_MGR	"/mnt/f0/pipe/rot_to_mgr"
#define MGR_TO_CAM  "/mnt/f0/pipe/mgr_to_cam"
#define MGR_TO_ROT  "/mnt/f0/pipe/mgr_to_rot"
#define MID_LINE 80
#define MESSAGE_SIZE_ROT_TO_MGR 3


enum COMM_TYPE{
	COMM_TYPE_NONE = -1,
	COMM_TYPE_CAM_TO_MGR,
	COMM_TYPE_ROT_TO_MGR,
	COMM_TYPE_MGR_TO_CAM,
	COMM_TYPE_MGR_TO_ROT,
	COMM_TYPE_MAX,
};


enum M_STATE{
	M_BARI1,
	M_HURDLE,
	M_TURNEL1,
	M_STAIR,
	M_TURNEL2,
	M_GOLF,
	M_RED,
	M_BARI2,
	M_EDGE,
	M_ALIGN_CENTER,
};

enum R_STATE{
	R_WAIT,
	R_GO,			// 앞으로 걸어가는 일반 동작
	R_GO_BARI,		// 바리게이트의 앞으로 걸어가는 동작 + 숙이기
	R_GO_TURNEL,	// 빨간색 터널의 앞으로 걸어가는 동작 + 숙이기
	R_LEFT,
	R_RIGHT,
	R_TURNLEFT,
	R_TURNRIGHT,
	R_TURNLEFT90,
	R_TURNRIGHT90,
	R_SITDOWN,
	R_SITGO,
	R_HEAD_LEFT,
	R_HEAD_RIGHT,
	R_HEAD_CENTER,
	R_LINE,
	R_LINE_END,
	R_CREEP,
};

enum MV_STEP{
	MV_0,
	MV_1,
	MV_2,
	MV_3,
	MV_4,
	MV_5,
	MV_6,
	MV_7,
	MV_8,
	MV_9,
};

enum MI_COMP{
	MI_PREV = -1,
	MI_NOW,
	MI_NEXT,
	MI_LINE,
	MI_END
};

enum MI_LINE_COMP {
	MI_LINE_NOW = 0,
	MI_LINE_NEXT,
	MI_LINE_END,
};

typedef struct _tagSend{
	R_STATE state;
	MV_STEP step;
	MI_COMP now;
	int size;
}TSEND, PTSEND;

enum PIPE_PROTOCOL {
	PP_NONE,
	PP_ROBOT_STATE_CHANGE,		// M -> R, 행동 명령
	PP_ROBOT_STATE_CHANGE_ACK,	// R -> M, ACK 신호
	PP_ROBOT_STATE_END,			// R -> M, 행동 종료 
	PP_CAM_CAPTURE,				// M -> C, 영상 찍기 명령
	PP_CAM_RESULT,				// C -> M, 영상 결과
	PP_MAX,
};

const float c_fStateTime[] = 
{
	1.5,		// MOTION_STAY
	3.4,		// MOTION_WALK_ONE
	0.5,		// MOTION_WALK_BARI
	0.5,		// MOTION_WALK_TURNEL
	2.6,		// MOTION_TURN_LEFT
	2.4,		// MOTION_TURN_RIGHT
	0.7,		// MOTION_LINE
	0.7,		// MOTION_LINE_END
	0.7,		// MOTION_LEFT
	0.7,		// MOTION_RIGHT
	0.7,		// MOTION_HEAD_LEFT
	3.6,		// MOTION_HEAD_RIGHT
	3.6,		// MOTION_HEAD_CENTER
	1.5,		// MOTION_TURN_LEFT_90
	0.0,		// MOTION_TURN_RIGHT_90
	0.0,		// MOTION_SIT_DOWN
	0.0,		// MOTION_CREEP

	0.8,		//STATE_HEAD_RIGHT_THIRTY
	0.8,		//STATE_HEAD_LEFT_SIXTY
	0.8,		//STATE_HEAD_RIGHT_SIXTY
	0.8,		//STATE_HEAD_LEFT_NINETY
	0.8,		//STATE_HEAD_RIGHT_NINETY
	3,			//STATE_STEP_LEFT
	3,			//STATE_STEP_RIGHT
	3,			//STATE_TURN_LEFT
	3,			//STATE_TURN_RIGHT
	4.7,		//STATE_TURN_LEFT_RIGHT_ANGLE
	3,			//STATE_LINE_CHECK
};
enum MOTION {
	MOTION_NONE = -1,
	MOTION_STAY,			// 0
	MOTION_WALK_ONE,		// 1
	MOTION_WALK_BARI,		// 2
	MOTION_WALK_TURNEL,		// 3
	MOTION_TURN_LEFT,		// 4
	MOTION_TURN_RIGHT,		// 5
	MOTION_LINE,			// 6
	MOTION_LINE_END,		// 7
	MOTION_LEFT,			// 8
	MOTION_RIGHT,			// 9
	MOTION_HEAD_LEFT,		// 10
	MOTION_HEAD_RIGHT,		// 11
	MOTION_HEAD_CENTER,		// 12
	MOTION_TURN_LEFT_90,	// 13
	MOTION_TURN_RIGHT_90,	// 14
	MOTION_SIT_DOWN,		// 15
	MOTION_CREEP,			// 16

	MOTION_UP_STAIR,
	MOTION_WALK_THREE,
	MOTION_WALK_TWO,
	STATE_MAX
};


//robot에서 manager로 보내는 메세지
enum REQ_STATE_CHANGE_ACK {
	REQ_STATE_CHANGE_ACK_OK,
	REQ_STATE_CHANGE_ACK_FAIL,
	REQ_STATE_CHANGE_MAX,
};

class CCommProc
{
public:
	CCommProc();
	~CCommProc();

	void Init();
	void Destroy();
	int SendMessage(char *data, int size);
	int ReceiveMessage(COMM_TYPE commType, char *msg, int size);
	void Connect(COMM_TYPE commType);
	COMM_TYPE ProtocolToCommType(PIPE_PROTOCOL protocol);
	const char* GetProtocolString(PIPE_PROTOCOL protocol);
	const char* GetTypeString(COMM_TYPE commType);

	const char* GetMotionString(MOTION motion);
	const char* GetMStateString(M_STATE state);
	const char* GetRStateString(R_STATE state);


private:
	int m_fileDes[COMM_TYPE_MAX];
};
