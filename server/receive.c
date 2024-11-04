/*
NAME:G.AJAY REDDY
DATE:26/7/2024
DESCRIPTION:TFTP SERVER
->RECEIVE A FILE FROM THE CLIENT
*/
#include"main.h"

int receive_file(int data_fd, struct packet *data_pac)
{
    char file_name[256];
    int pac_count = 0;
    int file_fd,i;
    int write_size;
    data_pac->mode = NORMAL_MODE;

    //receive ack from the client

    if (recv(data_fd, (void *)data_pac, sizeof(*data_pac), 0) <= 0) {
        perror("recv");
        return FAILURE;
    }
    //copy the file name
    strncpy(file_name, data_pac->buffer, sizeof(file_name) - 1);
    file_name[sizeof(file_name) - 1] = '\0';
    //check for the file
     file_fd = open(file_name,O_WRONLY | O_TRUNC);

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
    //send ack to client the server is ready to get a file
    data_pac->errn = SERVER_READY;
    send(data_fd, data_pac, sizeof(*data_pac), 0);

    do {
        if (recv(data_fd, (void *)data_pac, sizeof(*data_pac), 0) <= 0) {
            perror("recv");
            close(file_fd);
            return FAILURE;
        }

	//Writing to the file
		if( data_pac->mode != NET_ASCII )
		{
			write_size = write(file_fd, data_pac->buffer, data_pac->byte );
		}
		else
		{
			//Convertion for .Net ASCII mode
			write_size = 0;

			for( i = 0 ; i < data_pac->byte ; i++ )
			{
				if( data_pac->buffer[i] != '\n' )
				{
					write_size += write(file_fd, &data_pac->buffer[i], 1 );
				}
				else
				{
					write(file_fd, "\r", 1);
					write_size += write(file_fd, &data_pac->buffer[i], 1 );
				}
			}
		}
        if (write_size != data_pac->byte)//check wite data size and no of packets 
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
            data_pac->errn = SERVER_READY;
        }

        send(data_fd, data_pac, sizeof(*data_pac), 0);
        pac_count++;//count the packtes
    } while (data_pac->errn != END); //send untill the client send end ack

    printf(CYAN "INFO: ");
    printf(WHITE "Server has received data from client\n");

    if (close(file_fd) < 0) //close the file
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

