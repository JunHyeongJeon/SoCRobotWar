#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	pipe_connect(char* pipe_name);
void	pipe_disconnect(char* pipe_name);
int 	pipe_write(int filedes, char* msg, int msg_size);
int 	pipe_read(int filedes, char* msg, int msg_size);
