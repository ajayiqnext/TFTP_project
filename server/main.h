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

/* Change SERVER_IP to appropriate values */
#define SERVER_IP               "172.30.234.188"
#define SERVER_PORT     5039
#define SERVER_LENGTH   10

#define FAILURE				-1
#define SUCCESS			 	 1 

#define  PUT			 	 3
#define  GET			 	 4

#define NORMAL_MODE			'N'
#define OCTET_MODE			'O'
#define NET_ASCII			'A'

#define SERVER_READY		 0
#define END					'E'
#define DISCONNECT			'D'
#define CLIENT_READY		'C'


#define RED                 "\033[31m"
#define CYAN                "\033[36m"
#define GREEN               "\033[32m"
#define WHITE               "\033[97m"

#define ORANGE				"\033[38;5;208m"

struct packet
{
    char errn;
    char buffer[512];
    int byte;
    int packno;
    char mode;
    char oper;

};

int connect_the_system(int *sock_fd);
int receive_file(int data_fd,struct packet *data_pac);
int send_file(int sock_fd,struct packet *data_pac) ;
#endif
