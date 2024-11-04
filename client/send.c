/*
NAME:
DATE:
DESCRIPTION:
SAMPLE I/P:
SAMPLE O/P:
*/
#include"main.h"
extern int pack_size;

int send_file(struct packet *data_pac, int sock_fd) 
{
    char file_name[20];
    int read_size;

    //read file name from the stdin
    printf("Enter the file name: ");
    scanf("%s", file_name);

    //chekcing file is present or not
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd < 0) {
        perror("Error: There is no file exist");
        return FAILURE;
    }

    data_pac->oper = PUT;
    data_pac->errn = 1;

    //sending operation type ack to the server
    if (send(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) {
        perror("Error sending indication to server");
        close(file_fd);
        return FAILURE;
    }

    printf("INFO: Indication sent to server. Waiting for server acknowledgement\n");
    //receive ack form the server
    if (recv(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) {
        perror("Error receiving acknowledgement from server");
        close(file_fd);
        return FAILURE;
    }

    if (data_pac->errn == SERVER_READY) {
        printf("INFO: Server Acknowledges request\n");
    } else {
        printf("INFO: Server has not acknowledged the request\n");
        close(file_fd);
        return FAILURE;
    }

    //copy the filename
    strcpy(data_pac->buffer, file_name);
    data_pac->errn = 1;

    if (send(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) {
        perror("Error sending file name to server");
        close(file_fd);
        return FAILURE;
    }

    if (recv(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) {
        perror("Error receiving server's acknowledgement for file name");
        close(file_fd);
        return FAILURE;
    }

    if (data_pac->errn == FAILURE) {
        printf("ERROR: Server is unable to receive file\n");
        close(file_fd);
        return FAILURE;
    }

    //read data according to the packet size from the file and send to the sever

    do {
        read_size = read(file_fd, data_pac->buffer, pack_size);

        data_pac->byte = read_size;
        data_pac->errn = 1;

        if (read_size == 0)
       	{
            data_pac->errn = END;//if curser reach end of the file 
        }
       	else if (read_size > 0) 
	{
            if (send(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) 
	    {
                perror("Error sending packet to server");
                close(file_fd);
                return FAILURE;
            }
        }

	send(sock_fd,data_pac,sizeof(*data_pac),0);

        if (recv(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) {
            perror("Error receiving server's acknowledgement for packet");
            close(file_fd);
            return FAILURE;
        }

        if (data_pac->errn == FAILURE) {
            close(file_fd);
            return FAILURE;
        }
    } while (read_size != 0);

    if (recv(sock_fd, data_pac, sizeof(*data_pac), 0) < 0) {
        perror("Error receiving final server acknowledgement");
        close(file_fd);
        return FAILURE;
    }
    //close the file

    close(file_fd);

    //if (data_pac->errn != SUCCESS) {
     //   return FAILURE;
   // }

    return SUCCESS;
}

