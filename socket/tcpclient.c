#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include "common.h"

#define DEST_PORT 2000
#define SERVER_IP "127.0.0.1"

char data_buffer[1024];

void
setup_tcp_communication() {
    int sock_fd = 0,
        sent_recv_bytes = 0;

    int addr_len = sizeof(struct sockaddr);

    char* data;

    /*to store socket addesses : ip address and port*/
    struct sockaddr_in dest;

    dest.sin_family = AF_INET;

    /*Client wants to send data to server process which is running on server machine, and listening on
     * port on DEST_PORT, server IP address SERVER_IP.
     * Inform client about which server to send data to : All we need is port number, and server ip address. Pls note that
     * there can be many processes running on the server listening on different no of ports,
     * our client is interested in sending data to server process which is lisetning on PORT = DEST_PORT*/
    dest.sin_port = DEST_PORT;
    struct hostent *host = (struct hostent*)gethostbyname(SERVER_IP);
    dest.sin_addr = *((struct in_addr*)host->h_addr);

    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    connect(sock_fd, (struct sockaddr*)&dest, sizeof(struct sockaddr));

    test_request req;
    printf("enter first operand\n");
    scanf("%d", &req.a);
    printf("enter second operand\n");
    scanf("%d", &req.b);

    sent_recv_bytes = sendto(sock_fd, &req, sizeof(test_request), 0, (struct sockaddr*)&dest, sizeof(struct sockaddr));
    printf("sent %d bytes\n", sent_recv_bytes);
    
    test_response resp;
    sent_recv_bytes = recvfrom(sock_fd, (char*)&resp, sizeof(test_response), 0, (struct sockaddr*)&dest, &addr_len);
    printf("received %d bytes\n", sent_recv_bytes);

    printf("sum = %u\n", resp.sum);
}

int main() {
    setup_tcp_communication();
    printf("exiting application\n");
    return 0;
}
