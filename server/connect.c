/*
NAME:
DATE:
DESCRIPTION:
SAMPLE I/P:
SAMPLE O/P:
*/
#include"main.h"

int connect_the_system(int *sock_fd)
{
    //Creating the socket
	*sock_fd = socket( AF_INET, SOCK_STREAM, 0 );

	/* Validating if socket was created */
	if( *sock_fd < 0 )
	{
		printf( RED "ERROR: ");
		printf( WHITE "Socket creation failed\n");
		return FAILURE;
	}

	/* Structure to initialize the server socket */
	struct sockaddr_in server_address;

	/* Loading the structure with server address details */
	server_address.sin_family = AF_INET; //Setting the type as TCP
	server_address.sin_addr.s_addr = inet_addr( SERVER_IP ); //Storing the converted IP address
	server_address.sin_port = htons( SERVER_PORT ); //Storing the converted Port number

	//Binding the socket with the adrress and the por number */
	if( bind( *sock_fd, (struct sockaddr*)&server_address, sizeof( server_address ) ) != 0 )
	{
		return FAILURE;
	}

	return SUCCESS;
}

