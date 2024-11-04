#ifndef MAIN_H
#define MIAN_H
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


#define FAILURE				-1
#define SUCCESS			 	 1 

#define RED				"\033[31m"
#define CYAN				"\033[36m"
#define WHITE				"\033[97m"

#define NORMAL_MODE			'N'
#define OCTET_MODE			'O'
#define NET_ASCII			'A'

#define DISCONNECT		'D'
#define CLIENT_READY		'C'
#define  PUT			 	 3
#define  GET			 	 4
#define SERVER_READY		 0
#define END					'E'

struct packet
{
    char errn;
    char buffer[512];
    int byte;
    int packno;
    char mode;
    char oper;
};


int connect_the_server();
int send_file(struct packet *data_pac,int sock_fd);
int receive_file(struct packet *data_pac,int data_fd);
#endif

