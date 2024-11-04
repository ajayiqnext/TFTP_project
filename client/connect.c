/*
NAME:
DATE:
DESCRIPTION:
SAMPLE I/P:
SAMPLE O/P:
*/
#include"main.h"

char server_ip[20];
int server_port;

int connect_the_server(int *sock_fd)
{
    printf("Enter the IP address: ");
    scanf("%s", server_ip);

    // Input and validate the port number
    char port_str[10];
    printf("Enter the port number: ");
    scanf("%s", port_str);

    // Convert port string to integer
    server_port = atoi(port_str);

    if (server_port < 1024 || server_port > 49151)
    {
        printf(RED "ERROR: ");
        printf(WHITE "Invalid Port Number\n");
        printf("Valid Port number range: 1024 - 49151\n");
        return FAILURE;
    }

    struct sockaddr_in client_addr;

    // Create a socket
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("socket");
        return FAILURE;
    }

    // Initialize socket address structure
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(server_ip);
    client_addr.sin_port = htons(server_port);

    // Connect to the server
    if (connect(*sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) != 0)
    {
        perror("connect");
        close(*sock_fd);
        return FAILURE;
    }

    printf("Connected successfully to %s:%d\n", server_ip, server_port);
    return SUCCESS;
}


