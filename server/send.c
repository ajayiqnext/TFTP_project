/*
NAME:G.AJAY REDDY
DATE:
DESCRIPTION:TFTP SERVER
->SEND A FILE TO THE SERVER 
SAMPLE I/P:
SAMPLE O/P:
*/
#include"main.h"

int send_file(int sock_fd,struct packet *data_pac) 
{
    char file_name[20];
    int rd_byte,count=0,pack_size;
    //receive ack from the client
    recv(sock_fd,(void *)data_pac,sizeof(*data_pac),0);

    if(data_pac->oper != GET)
    {
	return 9;
    }
    
    data_pac->errn = SUCCESS;

    send(sock_fd,data_pac,sizeof(*data_pac),0);


    recv(sock_fd,(void *)data_pac,sizeof(*data_pac),0);
    //copy the file name for the packet

    strcpy(file_name,data_pac->buffer);

    int file_fd;
    //check for the file presnet or not
    file_fd = open(file_name,O_RDONLY);

    if(file_fd < 0 )
    {
	printf("there is no file exist\n");
	data_pac->errn = FAILURE;
    send(sock_fd,data_pac,sizeof(*data_pac),0);//send ack file is not present to client
    }
    data_pac->errn = SUCCESS;

    send(sock_fd,data_pac,sizeof(*data_pac),0);

    recv(sock_fd,(void *)data_pac,sizeof(*data_pac),0);
    //checking for the modes of packet size
   if( data_pac->mode != OCTET_MODE )
	{
		pack_size = 512;
	}
	else if( data_pac->mode == OCTET_MODE )
	{
		pack_size = 1;
	}
     
       if( data_pac->errn == FAILURE )
	{
		printf( RED "ERROR: ");
		printf( WHITE "Client is not able to create file\n");
		return FAILURE;
	}

	//If the client is ready to accept data, start sending data
	do
	{
		//Read from the file
		rd_byte = read( file_fd, data_pac->buffer, pack_size );

		data_pac->byte = rd_byte;
		data_pac->errn = 1;

		//Checking if the complete data from the file has been read
		if( rd_byte == 0 )
		{
			//Setting end of operation flag
			data_pac->errn = END;
		}

		//Sending the packet to the client
		send( sock_fd, data_pac, sizeof( *data_pac), 0 );

		//Receive acknowledgement from client if write was succesful
		recv( sock_fd, data_pac, sizeof( *data_pac), 0 );
		count++;

		if( data_pac->errn == FAILURE )
		{
			//If write operation at client side was not successful, terminate operation
			close( file_fd );
			return FAILURE;
		}

	}while( rd_byte != 0 );

	//Receive client acknowledgement
	recv( sock_fd,data_pac, sizeof( *data_pac), 0 );

	//Close the file 
	close( file_fd );

	if( data_pac->errn != SUCCESS )
	{
		return FAILURE;
	}

	printf( CYAN "INFO: ");
	printf( WHITE "Number of packets sent : %d\n", count );

	return SUCCESS;
}






