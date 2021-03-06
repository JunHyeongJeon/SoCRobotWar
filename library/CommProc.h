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
	M_TURN
};

enum R_STATE{
	R_WAIT,
	R_STAND,
	R_GO,			// 앞으로 걸어가는 일반 동작
	R_GO_BARI,		// 걸어가는 동작 + 숙이기
	R_GO_HURDLE,
	R_GO_RUN,		// 잰발로 빨리 걸어가기 
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
	R_LINE_FRONT0,
	R_LINE_FRONT1,
	R_LINE_FRONT2,
	R_LINE_HURDLE,
	R_HURDLE,
	R_STAIR_UP,
	R_STAIR_DOWN,
	R_SHORT_WALK
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
	1.5,		// 0.  MOTION_STAY
	4.0,		// 1.  MOTION_WALK_ONE
	7.2,		// 2.  MOTION_WALK_TWO
	9.4,		// 3.  MOTION_WALK_THREE
	4.8,		// 4.  MOTION_WALK_BARI
	0.4,		// 5.  MOTION_TURN_LEFT
	0.4,		// 6.  MOTION_TURN_RIGHT
	2.6,		// 7.  MOTION_LINE
	2.4,		// 8.  MOTION_LINE_END
	0.5,		// 9.  MOTION_LEFT
	0.5,		// 10. MOTION_RIGHT
	0.7,		// 11. MOTION_HEAD_LEFT
	0.7,		// 12. MOTION_HEAD_RIGHT
	0.7,		// 13. MOTION_HEAD_CENTER
	3.6,		// 14. MOTION_TURN_LEFT_90
	3.6,		// 15. MOTION_TURN_RIGHT_90
	1.5,		// 16. MOTION_SIT_DOWN
	24.5,		// 17. MOTION_CREEP
	0.8,		// 18. MOTION_LINE_FRONT_1
	4.7,		// 19. MOTION_RUN
	11.9,		// 20. MOTION_HURDLE
	19.0,		// 21. MOTION_STAIR_UP
	10.7,		// 22. MOTION_STAIR_DOWN
	1.6,		// 23. MOTION_SHORT_WALK
	4.8,		// 24. MOTION_WALK_HURDLE
	0.8,		// 25. MOTION_LINE_FRONT_0
	0.8,		// 26. MOTION_LINE_FRONT_2
	0.8			// 27

};

enum MOTION {
	MOTION_NONE = -1,
	MOTION_STAY,			// 0
	MOTION_WALK_ONE,		// 1
	MOTION_WALK_TWO,		// 2
	MOTION_WALK_THREE,		// 3
	MOTION_WALK_BARI,		// 4
	MOTION_TURN_LEFT,		// 5
	MOTION_TURN_RIGHT,		// 6
	MOTION_LINE,			// 7
	MOTION_LINE_END,		// 8
	MOTION_LEFT,			// 9
	MOTION_RIGHT,			// 10
	MOTION_HEAD_LEFT,		// 11
	MOTION_HEAD_RIGHT,		// 12
	MOTION_HEAD_CENTER,		// 13
	MOTION_TURN_LEFT_90,	// 14
	MOTION_TURN_RIGHT_90,	// 15
	MOTION_SIT_DOWN,		// 16
	MOTION_CREEP,			// 17
	MOTION_LINE_FRONT_1,	// 18
	MOTION_RUN,				// 19
	MOTION_HURDLE,			// 20
	MOTION_STAIR_UP,		// 21
	MOTION_STAIR_DOWN,		// 22
	MOTION_SHORT_WALK,		// 23
	MOTION_WALK_HURDLE,		// 24
	MOTION_LINE_FRONT_0,	// 25
	MOTION_LINE_FRONT_2,	// 26
	MOTION_SEE_HURDLE,		// 27

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
