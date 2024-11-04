/* ____________________ TFTP - Client _____________________________
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
int pack_size;

int main()
{
    int opt,connect_flag=0;
    int sock_fd;
    struct packet data_pac;

    data_pac.mode = NORMAL_MODE; //Setting the mode as Normal by default
    pack_size = 512; //Setting 512 bytes in normal mode
    data_pac.errn = 9;
    data_pac.oper = 9;



    do 
    {
	printf("AJAY client acess system\n");
	printf("1.CONNECT\n2.PUT\n3.GET\n4.MODE\n5.EXIT\n");
	printf("enter your choice:");
	scanf("%d",&opt);
	switch(opt)
	{
	    case 1:
		if(connect_flag == 1)
		{
		    printf( CYAN "INFO: ");
		    printf( WHITE "Client already connected with server\n");
		    break;
		}	
		if(connect_the_server(&sock_fd) == SUCCESS)//CONNECT TO THE SERVER
		{
		    printf( CYAN "INFO:");
		    printf( WHITE " Sucessfully connected to server\n");
		    connect_flag = 1;
		}
		else
		{
		    printf( RED "ERROR: ");
		    printf( WHITE "Invalid IP Address\n");
		}

		break;
	    case 2:

		if(connect_flag == 0)
		{
		    printf( CYAN "INFO: ");
		    printf( WHITE "server didnot connect with the client\n");
		    break;
		}
		if(send_file(&data_pac,sock_fd) == SUCCESS)//SEND A FILE TO SERVER
		{
		    printf( CYAN "INFO: ");
		    printf( WHITE "File has been sent to the server\n");
		}
		else
		{
		    printf( RED "ERROR: ");
		    printf( WHITE "Put operation failed\n");
		}

		break;
	    case 3:
		if(connect_flag == 0)
		{
		    printf( CYAN "INFO: ");
		    printf( WHITE "server didnot connect with the client\n");
		    break;
		}
		if(receive_file(&data_pac,sock_fd) == SUCCESS) //RECEIVE FILE FROM A SERVER
		{
		    printf( CYAN "INFO: ");
		    printf( WHITE "File has recieved from the server\n");
		}
		else
		{
		    printf( RED "ERROR: ");
		    printf( WHITE "Put operation failed\n");
		}
		break;
	    case 4:
		int mod_opt;
		printf("SELECT THE MODE\n");
		printf("1.NORMAL MODE\n2.OCTET MODE\n3.NETASCII\n");//CHANGE THE MODE OF SENDING AND RECEIVING DATA
		scanf("%d",&mod_opt);
		switch(mod_opt)
		{
		    case 1:
			pack_size = 512;
			data_pac.mode = NORMAL_MODE;
			break;
		    case 2:
			pack_size = 1;
			data_pac.mode = OCTET_MODE;
			break;
		    case 3:
			pack_size = 512;
			data_pac.mode = NET_ASCII;
			break;
		    default:
			printf("ERROR: ENTER A VALID OPTION\n");
			break;
		}
		break;

	    case 5:
		data_pac.oper = DISCONNECT;
		send( sock_fd, &data_pac, sizeof(data_pac), 0 );

		close( sock_fd ); //Closing the TCP Socket
		break;
	}
    }
    while(opt != 5 );
}



