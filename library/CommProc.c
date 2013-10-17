#include "CommProc.h"

CCommProc::CCommProc()
{
	Init();
}

CCommProc::~CCommProc()
{
	Destroy();
}

void CCommProc::Init()
{
}


const char* CCommProc::GetProtocolString(PIPE_PROTOCOL protocol)
{
	switch(protocol)
	{
		case PP_NONE: return "PP_NONE";
		case PP_ROBOT_STATE_CHANGE: return "PP_ROBOT_STATE_CHANGE";
		case PP_ROBOT_STATE_CHANGE_ACK: return "PP_ROBOT_STATE_CHANGE_ACK";
		case PP_ROBOT_STATE_END: return "PP_ROBOT_STATE_END";
		case PP_CAM_CAPTURE: return "PP_CAM_CAPTURE";
		case PP_CAM_RESULT: return "PP_CAM_RESULT"; 
		default: return "NULL";
	}
}

const char* CCommProc::GetTypeString(COMM_TYPE commType)
{
	switch(commType)
	{
		case COMM_TYPE_NONE: return "COMM_TYPE_NONE";
		case COMM_TYPE_CAM_TO_MGR: return "COMM_TYPE_CAM_TO_MGR";
		case COMM_TYPE_ROT_TO_MGR: return "COMM_TYPE_ROT_TO_MGR";
		case COMM_TYPE_MGR_TO_CAM: return "COMM_TYPE_MGR_TO_CAM";
		case COMM_TYPE_MGR_TO_ROT: return "COMM_TYPE_MGR_TO_ROT";
		default : return "NULL"; 
	}
}

const char* CCommProc::GetMStateString(M_STATE state)
{
	switch(state)
	{
	case M_BARI1:			return "M_BARI1";
	case M_BARI2:			return "M_BARI2";
	case M_TURNEL1:			return "M_TURNEL1";
	case M_TURNEL2:			return "M_TURNEL2";
	case M_STAIR:			return "M_STAIR";
	case M_GOLF:			return "M_GOLF";
	case M_RED:				return "M_RED";
	case M_EDGE:			return "M_EDGE";
	case M_ALIGN_CENTER:	return "M_ALIGN_CENTER";
	default:				return "NULL";
	}
}

const char* CCommProc::GetRStateString(R_STATE state)
{
	switch(state)
	{
	case R_WAIT:		return "R_WAIT";
	case R_GO:			return "R_GO";
	case R_LEFT:		return "R_LEFT";
	case R_RIGHT:		return "R_RIGHT";
	case R_TURNLEFT:	return "R_TURNLEFT";
	case R_TURNRIGHT:	return "R_TURNRIGHT";
	case R_SITDOWN:		return "R_SITDOWN";
	case R_LINE:		return "R_LINE";
	default:			return "NULL";
	}
}


const char* CCommProc::GetMotionString(MOTION motion)
{
	switch(motion)
	{
	case MOTION_NONE :		return "MOTION_NONE";
	case MOTION_STAY:		return "MOTION_STAY";
	case MOTION_WALK_THREE: return "MOTION_WALK_THREE";
	case MOTION_WALK_TWO:	return "MOTION_WALK_TWO";
	case MOTION_WALK_ONE:	return "MOTION_WALK_ONE";
	case MOTION_CREEP:		return "MOTION_CREEP";
	case MOTION_SIT_DOWN:	return "MOTION_SIT_DOWN";
	case MOTION_STAIR_UP:	return "MOTION_STAIR_UP";		
	default:				return "NULL";
	}

	return NULL;
}

void CCommProc::Connect(COMM_TYPE commType)
{
	switch(commType)
	{
	case COMM_TYPE_CAM_TO_MGR:
		m_fileDes[commType] = pipe_connect(CAM_TO_MGR);
		break;
	case COMM_TYPE_ROT_TO_MGR:
		m_fileDes[commType] = pipe_connect(ROT_TO_MGR);
		break;
	case COMM_TYPE_MGR_TO_CAM:
		m_fileDes[commType] = pipe_connect(MGR_TO_CAM);
		break;
	case COMM_TYPE_MGR_TO_ROT:
		m_fileDes[commType] = pipe_connect(MGR_TO_ROT);
		break;

	default:
		break;
	}
}

void CCommProc::Destroy()
{

}

COMM_TYPE CCommProc::ProtocolToCommType(PIPE_PROTOCOL protocol)
{
	switch(protocol)
	{
	case PP_ROBOT_STATE_CHANGE: 
		return COMM_TYPE_MGR_TO_ROT;
		break;
	case PP_ROBOT_STATE_CHANGE_ACK:
	case PP_ROBOT_STATE_END:
		return COMM_TYPE_ROT_TO_MGR;
		break;
	case PP_CAM_CAPTURE:
		return COMM_TYPE_MGR_TO_CAM;
		break;
	case PP_CAM_RESULT:
		return COMM_TYPE_CAM_TO_MGR;
		break;
	default:
		return COMM_TYPE_NONE;
	}
}

int CCommProc::SendMessage(char *data, int size)
{
	COMM_TYPE type = ProtocolToCommType((PIPE_PROTOCOL)data[0]);
	if(type == COMM_TYPE_NONE){
		printf("CCommProc::SendMessage() : Send Fail\n");
		return -1;
	}
	return pipe_write(m_fileDes[type], data, size);
}

int CCommProc::ReceiveMessage(COMM_TYPE commType, char *msg, int size)
{
	int result = pipe_read(m_fileDes[commType], msg, size);
	return result;
}
