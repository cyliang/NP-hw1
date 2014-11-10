#include "header.h"

void send_client(int fd, const char *msg) {
	char buf[CMD_MAX];
	sprintf(buf, "[Server] %s\n", msg);
	write(fd, buf, strlen(buf));
}

void proc_cmd(int clients[], int maxi, int user, char *cmd) {
	char *pos = strtok(cmd, " \n");
	if(!pos);
	else if(strcmp(pos, "who") == 0) {
		cmd_who(clients, maxi, user);
	} else if(strcmp(pos, "name") == 0) {
		pos = strtok(NULL, "\n");
		cmd_name(clients, maxi, user, pos);
	} else if(strcmp(pos, "yell") == 0) {
		pos = strtok(NULL, "\n");
		cmd_yell(clients, maxi, user, pos);
	} else if(strcmp(pos, "tell") == 0) {
		pos = strtok(NULL, " \n");
		char *m = strtok(NULL, "\n");
		cmd_tell(clients, maxi, user, pos, m);
	} else {
		send_client(clients[user], "ERROR: Error command.");
	}
}
