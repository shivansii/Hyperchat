#include <stdio.h>
#include <stdlib.h>
#include <standards.h>
#include <string.h>
#include <os.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <list.h>
#include <sys/time.h>

#define UNUSED(x) (void)(x)
#define MAX_MSGS 10000
PID receive_messages_pid, display_received_messages_pid, keyboard_input_pid, send_messages_pid, server_pid;

LIST *input_list;
LIST *output_list;
enum messageType { MSG_IO, MSG_RECIEVED};
typedef struct ThreadArgs {
        int sockfd;
        struct sockaddr_in serv_addr;
        struct sockaddr_in client_addr;
        int cli_len;
} ThreadArgs;

typedef struct userMsg{
	char msg[255];
	struct timeval tv;
} userMsg;

typedef struct serverMsg{
	enum messageType type;
	userMsg *u_msg;
} serverMsg;


char *msgs[MAX_MSGS];
void error(char *msg){
        perror(msg);
        exit(1);
}

PROCESS receive_messages(void *arg) {
        ThreadArgs *threadArgs; 
        int sock_fd;
        struct sockaddr_in client_addr;
        int cli_len;
        userMsg client_msg;
        struct hostent *hostp;
        char *hostaddrp;
        int len;

        threadArgs = (ThreadArgs *) arg;
        sock_fd = threadArgs->sockfd;
        client_addr = threadArgs->client_addr;
        cli_len = threadArgs->cli_len;
        len = sizeof(serverMsg);
        
        while (1) {
                serverMsg *server_msg;
		int n;

                n = recvfrom(sock_fd, &client_msg, sizeof(userMsg), 0, (struct sockaddr *)&client_addr, &cli_len);
		if(n < 0 ){
			if(n == EAGAIN || n == EWOULDBLOCK)
				continue;
			if(n == EBADF)
				continue;
			continue;
			error("Error receiving message from client with recvfrom()");
		}
		hostp = gethostbyaddr((const char *)&client_addr.sin_addr.s_addr, sizeof(client_addr.sin_addr.s_addr), AF_INET);
		if (hostp == NULL) {
			perror("gethostbyaddr");
			exit(1);
		}
		hostaddrp = inet_ntoa(client_addr.sin_addr);
		if (hostaddrp == NULL)
		error("ERROR on inet_ntoa\n");
		
		server_msg = malloc(sizeof(serverMsg));
		server_msg->type = MSG_RECIEVED;
		server_msg->u_msg = &client_msg;

		if(Send(server_pid, (void*) server_msg, &len) != 1){
		      printf("Error sending message to server");	
		}

        }

}

PROCESS display_received_messages(void *arg) {

	userMsg curr_msg;
	while(1){
		if(ListCount(output_list)>0){
			curr_msg = *((userMsg*)ListFirst(output_list));
		       	printf("Recieved at %ld::%ld\n",curr_msg.tv.tv_sec, curr_msg.tv.tv_usec);
			printf("Message : %s\n", curr_msg.msg);
			ListRemove(output_list);
		}
	}
	UNUSED(arg);
}

PROCESS keyboard_input(void *arg) {
        char usr_message[255];
        int len = sizeof(serverMsg);
        UNUSED(arg);

        while (1){
                int success;
		bzero(usr_message, 255);
                success = read(0, usr_message, 255);

                if(success > 0){
			serverMsg *server_msg;
			userMsg *user_msg;
			user_msg = malloc(sizeof(userMsg));
			strcpy(user_msg->msg,usr_message); 
			server_msg = malloc(sizeof(serverMsg));
			server_msg->type = MSG_IO;
			server_msg->u_msg = user_msg;

			if(Send(server_pid, (void*) server_msg, &len) != 1){
			      printf("Error sending message to server");	
			}

                }
        }      
}


PROCESS send_messages(void *arg) {
        ThreadArgs *threadArgs;
        int sock_fd;
        struct sockaddr_in client_addr;
        int cli_len;
        char *msg_to_send;
	userMsg *user_msg;
	struct timeval tv;
        

        threadArgs = (ThreadArgs *) arg;
        sock_fd = threadArgs->sockfd;
        client_addr = threadArgs->client_addr;
        cli_len = threadArgs->cli_len;


        while (1) {
		if(ListCount(input_list)>0){
                        int succ;

			msg_to_send = ListFirst(input_list);
			user_msg = malloc(sizeof(userMsg));
			strcpy(user_msg->msg, msg_to_send);
			user_msg->tv = tv;	
			
			succ = sendto(sock_fd, user_msg, sizeof(userMsg), 0, 
				(struct sockaddr *)&client_addr, cli_len);
			if(succ == EAGAIN){
				continue;
			}

			if(succ < 0){
				error("Error sending message");
			}

			ListRemove(input_list);
		}
        }
}

PROCESS server(void *arg) {

        int len;
	PID pid;
        serverMsg *msg;
        UNUSED(arg);
        while (1){
                msg = Receive(&pid, &len);
		switch(msg->type){
			case MSG_IO:
				ListAppend(input_list, msg->u_msg->msg); 
                	break;
			case MSG_RECIEVED:
				ListAppend(output_list, msg->u_msg); 
			break;
		}

		Reply(pid,1,sizeof(int));
        }
}

int mainp(int argc, char *argv[]){

        int sock_fd, myport, cli_port, cli_len;
        struct sockaddr_in serv_addr, client_addr;
        ThreadArgs *threadArgs;
        char *cli_name;
        int i;

        char server_message[255];
        char client_message[255];
	struct hostent *remote_info;
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));
        input_list = ListCreate();
        output_list = ListCreate();
	for(i = 0; i < MAX_MSGS; i++){
		msgs[i] = malloc( sizeof(char) * 255);
		msgs[i] = NULL;
	}

        if(argc < 4){
                error(" Usage: ./s-chat <server-port> <client-device-name> <client-port>");
        }

        myport = atoi(argv[1]);
        cli_name = argv[2];
        cli_port = atoi(argv[3]);
 
        if ( (myport < 30001) || (myport > 40000) ) {
                fprintf(stderr, "Invalid port number argument - should be between 30001 and 40000 %d.\n", myport);
                exit(1);
        }

        if ( (cli_port < 30001) || (cli_port > 40000) ) {
                fprintf(stderr, "Invalid port number argument - should be between 30001 and 40000 %d.\n", cli_port);
                exit(1);
        }

	threadArgs = malloc(sizeof(ThreadArgs));
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock_fd < 0){
                perror("Error opening socket with socket()");
		exit(-1);
        }
        printf("Socket created successfully\n");

        bzero((char *) &serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(myport);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
                perror("Error binding socket with bind()");
        	exit(-1);
	}
        printf("Socket binded successfully\n");

        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(cli_port);
        remote_info = gethostbyname(cli_name);
        if (remote_info == NULL) {
                perror("gethostbyname");
                exit(1);
                }
        client_addr.sin_addr = *((struct in_addr *)remote_info->h_addr);


        cli_len = sizeof(client_addr);

        threadArgs->sockfd = sock_fd;
        threadArgs->serv_addr = serv_addr;
        threadArgs->client_addr = client_addr;
        threadArgs->cli_len = cli_len;

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	fcntl(sock_fd, F_SETFL,fcntl(sock_fd, F_GETFL) | O_NONBLOCK);
	
	send_messages_pid = Create((void(*)())send_messages,16000,"sm", (void *) threadArgs, NORM,USR );
        if (send_messages_pid == PNUL){
                printf("Error creating pthread message_sending_thread");
                exit(1);
        }

	receive_messages_pid = Create((void(*)())receive_messages,16000,"rm", (void *) threadArgs, NORM, USR ); 
        if (receive_messages_pid == PNUL){
                printf("Error creating pthread receive_messages");
                exit(1);
        }

        display_received_messages_pid = Create((void(*)())display_received_messages,16000,"drm", (void *) threadArgs, NORM, USR );
        if (display_received_messages_pid == PNUL){
                printf("Error creating pthread display_received_messages");
                exit(1);
        }

        keyboard_input_pid = Create((void(*)())keyboard_input,16000,"ki", (void *) threadArgs, NORM, USR );
        if (keyboard_input_pid == PNUL){
                printf("Error creating pthread keyboard_input_thread");
                exit(1);
        }      

        server_pid = Create((void(*)())server,16000,"s", (void *) threadArgs, NORM, USR );
        if (server_pid == PNUL){
                printf("Error creating pthread server_thread");
                exit(1);
        }

        return 0;

}




