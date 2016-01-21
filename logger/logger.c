#include <stdio.h>
#include <inttypes.h>
#include <lcm/lcm.h>
#include "../lcmtypes/vicon_state_t.h"
#include  <time.h>
#include <stdlib.h>

#define OBJ_NUM 2

time_t rawtime;
struct tm* info;
char state_fname[50];


static void state_handler(const lcm_recv_buf_t *rbuf, const char * channel, 
        const vicon_state_t * msg, char* obj_name)
{


	char fname[50];
	strcpy(fname, obj_name);
	strcat(fname,".txt");

    FILE* logger;
	logger = fopen(fname, "a");
	if(logger == NULL) {
		perror("File open error! \n");
		exit(0);
	}
	
	//timestamp, accel[0]~[2], angle[0]~[2], angular_vel[0]~[2]
	fprintf(logger, "%" PRId64 ",%f, %f, %f, %f, %f, %f\n", 
		msg->timestamp, msg->position[0], msg->position[1], msg->position[2], msg->attitude[0], msg->attitude[1], msg->attitude[2]);
	fclose(logger);
}

int main(int argc, char ** argv)
{

	time(&rawtime);
	info = localtime(&rawtime);

	char** obj_name = (char**)malloc(OBJ_NUM*sizeof(char*));
	for(int i = 0; i < OBJ_NUM; i ++)
		obj_name[i] = (char*)malloc(50*sizeof(char));

	strcpy(obj_name[0], "abc");
	strcpy(obj_name[1], "abc_1");


    lcm_t * lcm = lcm_create(NULL);
    if(!lcm)
        return 1;

    for (int i = 0; i < OBJ_NUM; i ++)
    	vicon_state_t_subscribe(lcm, obj_name[i], &state_handler, obj_name[i]);

    while(1)
        lcm_handle(lcm);

    lcm_destroy(lcm);
    return 0;
}