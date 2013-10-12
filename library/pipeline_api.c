#include "pipeline_api.h"

int pipe_connect(char* pipe_name)
{
	//printf("pipe_connect \n");

	int filedes;
	if( mkfifo(pipe_name, 0666) == -1 )
	{
		//printf("Fail (pipeline) : call mkfifo \n");
	}
	
	if( (filedes = open(pipe_name, O_RDWR)) < 0 )
	{
		//printf("Fail (pipeline) : call open \n");
	}
	
	return filedes;
}

void pipe_disconnect(char* pipe_name)
{
	unlink(pipe_name);
}

int pipe_write(int filedes, char* msg, int msg_size)
{
	if( write(filedes, msg, msg_size) == -1)
	{
		return -1;
	}
	
	return 0;
}

int pipe_read(int filedes, char* msg, int msg_size)
{
	int nread;	
	if( (nread = read(filedes, msg, msg_size)) < 0 )
	{
		return -1;
	}
	
	return 0;
}
