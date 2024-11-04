/*
NAME:
DATE:
DESCRIPTION:
SAMPLE I/P:
SAMPLE O/P:
 */
#include"main.h"

int receive_file(struct packet *data_pac,int data_fd)
{
    char file_name[20];
    int pac_count = 0;
    int file_fd,i;
    int write_size;
    
    data_pac->oper = GET;
    data_pac->errn = 1;
    //send ack to the server for get operation
    send(data_fd,data_pac,sizeof(*data_pac),0);
    
    printf( CYAN "INFO: ");
    printf( WHITE "Indication sent to server. Waiting for server acknowledgement\n");
    
    recv(data_fd,(void *)data_pac,sizeof(*data_pac),0);
    
    
    if(data_pac->errn != SUCCESS)
    {
	printf("server is ready to send data\n");
    }
   //read the file name from client stdin
    
    printf("enter the file name:");
    scanf("%s",file_name);

    strcpy(data_pac->buffer,file_name);
    //send file name to server
    send(data_fd,data_pac,sizeof(*data_pac),0);

    recv(data_fd,(void *)data_pac,sizeof(*data_pac),0);

    if(data_pac->errn == FAILURE)
    {
	printf("file not exist in server network\n");

    }


    file_fd = open(file_name,O_WRONLY | O_TRUNC,0644);

    if( file_fd < 0)
    {
	printf("there is no file exist\n");

	file_fd = open(file_name,O_WRONLY |O_CREAT,0644);

	if(file_fd < 0 )
	{                       printf( RED "ERROR: ");
	    printf( WHITE "Unable to create file\n");

	    //Sending error signal to the client
	    data_pac->errn = FAILURE;
	    send( data_fd, data_pac, sizeof( *data_pac), 0 );

	    return FAILURE;
	}
    }
    else
    {
	printf( CYAN "INFO: ");
	printf( WHITE "File does exists. Opening and clearing the contents of the file\n");
    }

    data_pac->errn = CLIENT_READY;
    send(data_fd, data_pac, sizeof(*data_pac), 0);

    do {
	if (recv(data_fd, (void *)data_pac, sizeof(*data_pac), 0) <= 0)
       	{
	    perror("recv");
	    close(file_fd);
	    return FAILURE;
	}

	//Writing the data to the file
		if( data_pac->mode != NET_ASCII )
		{
			write_size = write( file_fd, &data_pac->buffer,data_pac->byte );
		}
		else
		{
			write_size = 0;
			//netasci mode
			//replace \n with \r
			for( i = 0 ; i < data_pac->byte ; i++ )
			{
				if( data_pac->buffer[i] != '\n' )
				{
					write_size += write( file_fd, &data_pac->buffer[i], 1 );
				}
				else
				{
					write( file_fd, "\r", 1 );
					write_size += write( file_fd, &data_pac->buffer[i], 1 );
				}
			}
		}


	if (write_size != data_pac->byte)
       	{
	    perror("write");
	    printf(RED "ERROR: ");
	    printf(WHITE "Write size mismatch occurred\n");

	    data_pac->errn = FAILURE;
	    send(data_fd, data_pac, sizeof(*data_pac), 0);
	    close(file_fd);
	    return FAILURE;
	}

	if (data_pac->errn != END)
       	{
	    data_pac->errn = CLIENT_READY;
	}

	send(data_fd, data_pac, sizeof(*data_pac), 0);
	pac_count++;
    } while (data_pac->errn != END);//receive data upto server send end ack

    printf(CYAN "INFO: ");
    printf(WHITE "Server has received data from client\n");
 //close the file
    if (close(file_fd) < 0) 
    {
	perror("close");
	data_pac->errn = FAILURE;
	send(data_fd, data_pac, sizeof(*data_pac), 0);
	return FAILURE;
    }

    data_pac->errn = SUCCESS;
    send(data_fd, data_pac, sizeof(*data_pac), 0);

    printf(CYAN "INFO: ");
    printf(WHITE "Number of packets received: %d\n", pac_count);

    return SUCCESS;
}
