#include "header.h"

char names[FD_SETSIZE][20];
char ips[FD_SETSIZE][20];
short ports[FD_SETSIZE];

void user_init(int clients[], int id, const struct sockaddr_in *cli_addr) {
	char welcome_msg[CMD_MAX];

	strcpy(names[id], "anonymous");
	inet_ntop(AF_INET, &cli_addr->sin_addr, ips[id], sizeof(ips[id]));
	ports[id] = ntohs(cli_addr->sin_port);

#ifdef FLAVOR
	sprintf(welcome_msg, "\033[1;33mHello, anonymous! \033[m\033[33mFrom: %s/%hu\033[m", ips[id], ports[id]);
#else
	sprintf(welcome_msg, "Hello, anonymous! From: %s/%hu", ips[id], ports[id]);
#endif
	send_client(clients[id], welcome_msg);
}

void user_down(int clients[], int maxi, int user) {
	char msg[CMD_MAX];
#ifdef FLAVOR
	sprintf(msg, "\033[1;33m%s\033[m\033[33m is offline.\033[m", names[user]);
#else
	sprintf(msg, "%s is offline.", names[user]);
#endif
	boardcast(clients, maxi, msg);
}

void cmd_who(int clients[], int maxi, int user) {
	int i;
	for(i=0; i<=maxi; i++) {
		if(clients[i] < 0)
			continue;

		char msg[CMD_MAX];
#ifdef FLAVOR
		sprintf(msg, "\033[1;33m%s\033[m\033[33m %s/%hu\033[m", names[i], ips[i], ports[i]);
#else
		sprintf(msg, "%s %s/%hu", names[i], ips[i], ports[i]);
#endif
		if(i == user) {
#ifdef FLAVOR
			strcat(msg, " \033[1;33m->me\033[m");
#else
			strcat(msg, " ->me");
#endif
		}

		send_client(clients[user], msg);
	}
}

void cmd_name(int clients[], int maxi, int user, const char *name) {
	if(strcmp(name, "anonymous") == 0) {
#ifdef FLAVOR
		send_client(clients[user], "\033[1;31mERROR:\033[m\033[31m Username cannot be anonymous.\033[m");
#else
		send_client(clients[user], "ERROR: Username cannot be anonymous.");
#endif
		return;
	}

	int len = strlen(name);
	if(len < 2 || len > 12 || strspn(name, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != len) {
#ifdef FLAVOR
		send_client(clients[user], "\033[1;31mERROR:\033[m\033[31m Username can only consists of 2~12 English letters.\033[m");
#else
		send_client(clients[user], "ERROR: Username can only consists of 2~12 English letters.");
#endif
		return;
	}

	int i;
	for(i=0; i<=maxi; i++) {
		if(clients[i] >= 0 && i != user && strcmp(name, names[i]) == 0) {
			char msg[CMD_MAX];
#ifdef FLAVOR
			sprintf(msg, "\033[1;31mERROR:\033[m\033[31m %s has been used by others.\033[m", name);
#else
			sprintf(msg, "ERROR: %s has been used by others.", name);
#endif
			send_client(clients[user], msg);
			return;
		}
	}

	char msg[CMD_MAX];
	int u_fd = clients[user];
	clients[user] = -1;
#ifdef FLAVOR
	sprintf(msg, "\033[1;33m%s\033[m\033[33m is now known as \033[1m%s.\033[m", names[user], name);
#else
	sprintf(msg, "%s is now known as %s.", names[user], name);
#endif
	strcpy(names[user], name);

	boardcast(clients, maxi, msg);
	clients[user] = u_fd;

#ifdef FLAVOR
	sprintf(msg, "\033[32mYou're now known as \033[1m%s.\033[m", name);
#else
	sprintf(msg, "You're now known as %s.", name);
#endif
	send_client(u_fd, msg);
}

void cmd_tell(int clients[], int maxi, int user, const char *name, const char *msg) {
	if(strcmp(names[user], "anonymous") == 0) {
#ifdef FLAVOR
		send_client(clients[user], "\033[1;31mERROR:\033[m\033[31m You are anonymous.\033[m");
#else
		send_client(clients[user], "ERROR: You are anonymous.");
#endif
		return;
	}
	if(strcmp(name, "anonymous") == 0) {
#ifdef FLAVOR
		send_client(clients[user], "\033[1;31mERROR:\033[m\033[31m The client to which you sent is anonymous.\033[m");
#else
		send_client(clients[user], "ERROR: The client to which you sent is anonymous.");
#endif
		return;
	}

	int i;
	for(i=0; i<=maxi; i++) {
		if(clients[i] >= 0 && strcmp(names[i], name) == 0) {
			char buf[CMD_MAX];
#ifdef FLAVOR
			sprintf(buf, "\033[1;33m%s\033[m\033[33m tell you\033[m %s", names[user], msg);
#else
			sprintf(buf, "%s tell you %s", names[user], msg);
#endif

			send_client(clients[i], buf);
#ifdef FLAVOR
			send_client(clients[user], "\033[1;32mSUCCESS:\033[m\033[32m Your message has been sent.\033[m");
#else
			send_client(clients[user], "SUCCESS: Your message has been sent.");
#endif
			return;
		}
	}

#ifdef FLAVOR
	send_client(clients[user], "\033[1;31mERROR:\033[m\033[31m The receiver doesn't exist.\033[m");
#else
	send_client(clients[user], "ERROR: The receiver doesn't exist.");
#endif
}

void cmd_yell(int clients[], int maxi, int user, const char *msg) {
	char buf[CMD_MAX];
#ifdef FLAVOR
	sprintf(buf, "\033[1;33m%s\033[m\033[33m yell\033[m %s", names[user], msg);
#else
	sprintf(buf, "%s yell %s", names[user], msg);
#endif
	boardcast(clients, maxi, buf);
}

void boardcast(int clients[], int maxi, const char *msg) {
	int i;
	for(i=0; i<=maxi; i++) {
		if(clients[i] >= 0) {
			send_client(clients[i], msg);
		}
	}
}
