#include "header.h"

void send_client(int fd, const char *msg) {
	char buf[CMD_MAX];
#ifdef FLAVOR
	sprintf(buf, "\033[44m[Server]\033[m %s\n", msg);
#else
	sprintf(buf, "[Server] %s\n", msg);
#endif
	write(fd, buf, strlen(buf));
}

void proc_cmd(int clients[], int maxi, int user, char *cmd) {
	char *pos = strtok(cmd, " \n");
	if(!pos);
	else if(strcmp(pos, "who") == 0) {
		pos = strtok(NULL, "\n");
		if(!pos) {
			cmd_who(clients, maxi, user);
			return;
		}
	} else if(strcmp(pos, "name") == 0) {
		pos = strtok(NULL, "\n");
		if(pos) {
			cmd_name(clients, maxi, user, pos);
			return;
		}
	} else if(strcmp(pos, "yell") == 0) {
		pos = strtok(NULL, "\n");
		if(pos) {
			cmd_yell(clients, maxi, user, pos);
			return;
		}
	} else if(strcmp(pos, "tell") == 0) {
		pos = strtok(NULL, " \n");
		if(pos) {
			char *m = strtok(NULL, "\n");
			if(m) {
				cmd_tell(clients, maxi, user, pos, m);
				return;
			}
		}
	}

#ifdef FLAVOR
	send_client(clients[user], "\033[1;31mERROR:\033[m\033[31m Error command.\033[m");
#else
	send_client(clients[user], "ERROR: Error command.");
#endif
}
