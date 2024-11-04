/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~TFTP - SERVER ~~~~~~~~~~~~~~~~~~~~~~
Name : G.AJAY REDDY
Name of Project : Implementing TFPT( Trivial File Transfer Protocol )
Description : TFTP is a file transfer protocol that allows the user to send or
			  receive a file from a remote host/server.
			  This project is the simple implementation of this file transfer protocol

			  This is the client part of the program and allows the user to connect to
			  the host based on the host address and port number input by the user

			  Type of socket used : TCP
*/
#include"main.h"

int main()
{
    int server_sock_fd,data_fd;
    int operation = -1;
    struct packet data_pac;

    //CONNECT TO THE CLIENT
    if(connect_the_system(&server_sock_fd) == SUCCESS)
    {
	printf(WHITE "connetcted to the client successful\n");
    }
    else
    {
	printf( RED "ERROR: ");
	printf( WHITE "Unable to create server socket\n");
	_exit(-1);
    }
    printf("____________ HYDRA SERVER ____________ \n");
    printf("SERVER IS ");
    printf( GREEN "ACTIVE\n");
    printf( WHITE "IP ADDRESS : %s\n",SERVER_IP );
    printf("PORT NUMBER : %d\n\n", SERVER_PORT );

    listen(server_sock_fd,SERVER_LENGTH);


    while(1)
    {
	data_fd = accept(server_sock_fd,NULL,NULL);

	//CREATE A CHILD PROCESS
	int pid = fork();


	if(pid == 0)
	{
	    //Server operations
	    printf( CYAN "INFO: ");
	    printf( WHITE "Connected with THE CLIENT\n");

	    while(1)
	    {
		//RECEIVE ACK FROM CLIENT UNTIL IT GET DISCONNECT

		recv(data_fd,(void *)&data_pac,sizeof(data_pac),0);

		if(data_pac.errn == 1)
		{
		    printf( CYAN "INFO: ");
		    printf( WHITE "Server Acknowledges client request\n");
		    data_pac.errn = SERVER_READY;
		    operation = data_pac.oper;

		    send(data_fd,&data_pac,sizeof(data_pac),0);
		    if(operation == PUT)//TO GET A FILE FROM CLIENT
		    {
			if(receive_file(data_fd,&data_pac) == SUCCESS)
			{
			    printf( CYAN "INFO: ");
			    printf( WHITE "Sucessfully received the file\n");
			}
			else
			{
			    printf( RED "ERROR: ");
			    printf( WHITE "File not received\n");
			}
		    }
		    else if(operation == GET)//TO SEND A FILE TO CLIENT
		    {
			if(send_file(data_fd,&data_pac) == SUCCESS)
			{
			    printf( WHITE "successfully file send to client\n");
			}
			else
			{
			    printf("ERROR: file doesnot send \n");
			}
		    }
		    else if(operation == DISCONNECT)//DISCONNECT THE CLIENT
		    {
			printf( CYAN "INFO: ");
			printf( WHITE "Client has disconnected\n");
	                close(server_sock_fd);
			exit(0);
			break;
		    }
		}
	    }
	}
    }
}


