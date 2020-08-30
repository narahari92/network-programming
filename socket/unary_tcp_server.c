#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include "common.h"

#define SERVER_PORT 2000

char data_buffer[1024];

void
setup_tcp_server_communication() {
    /*Socket handle and other variables*/
    /*Master socket file descriptor, used to accept new client connection only, no data exchange*/
    int master_sock_tcp_fd = 0,
        sent_recv_bytes = 0,
        addr_len = 0,
        opt = 1;

    /*client specific communication socket file descriptor, 
     * used for only data exchange/communication between client and server*/
    int comm_socket_fd = 0;

    /* Set of file descriptor on which select() polls. Select() unblocks whever data arrives on
     * any fd present in this set*/
    fd_set readfds;

    struct sockaddr_in server_addr;    // Variable to hold server information
    struct sockaddr_in client_addr;    // Variable to hold client information

    /* TCP master socket creation */
    if((master_sock_tcp_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket creation failed\n");
        exit(1);
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = SERVER_PORT;
    server_addr.sin_family = AF_INET;

    addr_len = sizeof(struct sockaddr);

    /* Bind the server. Binding means, we are telling kernel(OS) that any data 
     * you recieve with dest ip address = machine ip, and tcp port no = 2000, pls send that data to this process
     * bind() is a mechnism to tell OS what kind of data server process is interested in to recieve. Remember, server machine
     * can run multiple server processes to process different data and service different clients. Note that, bind() is 
     * used on server side, not on client side*/
    if(bind(master_sock_tcp_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("socket bind failed\n");
        exit(1);
    }

    /*Tell the Linux OS to maintain the queue of max length to Queue incoming
     * client connections.*/
    if(listen(master_sock_tcp_fd, 5) < 0) {
        perror("listen failed\n");
        exit(1);
    }

    while(1) {
        FD_ZERO(&readfds);     // Initialise file descriptor set
        FD_SET(master_sock_tcp_fd, &readfds);    // Add socket to this set on which our server is running

        printf("blocked on select system call\n");

        /*Call the select system call, server process blocks here. Linux OS keeps this process blocked until 
         * the data arrives on any of the file descriptors in the 'readfds' set*/
        select(master_sock_tcp_fd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(master_sock_tcp_fd, &readfds)) {
            printf("New connection request received. Accept the connection\n");

            /* accept() returns a new temporary file desriptor(fd). Server uses this 'comm_socket_fd' fd for the rest of the
             * life of connection with this client to send and recieve msg. Master socket is used only for accepting
             * new client's connection and not for data exchange with the client*/
            if((comm_socket_fd = accept(master_sock_tcp_fd, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
                printf("accept error: errno = %d\n", errno);
                exit(1);
            }

            //printf("connection accepted from client: %s:%u\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            printf("connection accepted from client\n");
            
            while(1) {
                printf("server ready to service client messages\n");

                /*Drain to store client info (ip and port) when data arrives from client, sometimes, server would want to find the identity of the client sending msgs*/
                memset(data_buffer, 0, sizeof(data_buffer));

                /*Like in client case, this is also a blocking system call, meaning, server process halts here until
                 * data arrives on this comm_socket_fd from client whose connection request has been accepted via accept()*/
                sent_recv_bytes = recvfrom(comm_socket_fd, (char*)data_buffer, sizeof(data_buffer), 0,
                                    (struct sockaddr*)&client_addr, &addr_len);

                //printf("server received %d bytes from client %s:%u\n", sent_recv_bytes, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                printf("server received %d bytes\n", sent_recv_bytes);

                if(sent_recv_bytes == 0) {
                    /*If server recvs empty msg from client, server may close the connection and wait
                     * for fresh new connection from client - same or different*/
                    close(comm_socket_fd);
                    break;
                }

                test_request *req = (test_request*)data_buffer;
                if(req->a == 0 && req->b == 0) {
                    close(comm_socket_fd);
                    printf("server closing connection with client\n");
                    break;
                }

                printf("a: %d, b: %d\n", req->a, req->b);

                test_response *resp;
                resp->sum = req->a + req->b;

                sent_recv_bytes = sendto(comm_socket_fd, (char*)resp, sizeof(test_response), 0, (struct sockaddr*)&client_addr, sizeof(struct sockaddr));

                printf("sent %d bytes\n", sent_recv_bytes);
            }
        }
    }
}

int
main() {
    setup_tcp_server_communication();
    return 0;
}
