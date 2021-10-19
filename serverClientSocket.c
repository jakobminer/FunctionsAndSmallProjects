#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define PORT 49999

int server(){
	struct sockaddr_in address;
	int socketfd, listenfd, host, count = 0, length = sizeof(struct sockaddr_in);
	char hostName[NI_MAXHOST], serverTime[20];
	time_t currentTime;

	//set up socket
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

	//initialize address
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket
	if (bind(listenfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("Error: ");
		exit(1);
	}

	//listen
	if (listen(listenfd, 1) < 0) {
		perror("Error: ");
		exit(1);
	}

	//hold for connection
	while(1){
		if ((socketfd = accept(listenfd, (struct sockaddr *) &address, &length)) < 0) {
			perror("Error: ");
			exit(1);
		}

		//once connection is made, increment count
		count++;

		//fork
		if (fork() == 0){
			//child gets host name, sends server time
			host = getnameinfo((struct sockaddr*)&address, sizeof(address), hostName, sizeof(hostName), NULL, 0, NI_NUMERICSERV);
		    if (host != 0){
		    	fprintf(stderr, "Error: %s\n", gai_strerror(host));
		    	exit(1);
		    }
		    printf("%s %d\n", hostName, count);
		    fflush(stdout);

		    time(&currentTime);
		    strcpy(serverTime, ctime(&currentTime));
		    serverTime[18] = '\n';
		    serverTime[19] = '\0';
		    if (write(socketfd, serverTime, 20) < 0){
		    	perror("Error: ");
		    	exit(1);
		    }
		    exit(0);
		}	
		else {
			//parent closes socket, waits for child to finish before reopening connection
			close(socketfd);
			wait(NULL);
		}
	}
	return 0;
}

int client(const char *address){
	struct sockaddr_in server;
	struct addrinfo hints, *actualdata;
	int socketfd, err;
	char buffer[100];

	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	//client parses address
	err = getaddrinfo(address, "49999", &hints, &actualdata);
	if (err != 0){
		fprintf(stderr, "Error: %s\n", gai_strerror(err));
		exit(1);
	}

	//initializes socket
	socketfd = socket(actualdata -> ai_family, actualdata -> ai_socktype, 0);

	//attempts to connect
    if (connect(socketfd, actualdata -> ai_addr, actualdata -> ai_addrlen) < 0) {
    	perror("Error: ");
    	exit(1);
    }

    //reads server time from socket
    if (read(socketfd, buffer, 20) < 0){
    	perror("Error: ");
    	exit(1);
    }
    printf("%s", buffer);

    return 0;
    
}

int main(int argc, char const *argv[])
{
	//parses input for client and server
	if (argc <= 1) {
		printf("missing args\n");
		return 0;
	}
	if (strcmp(argv[1], "server") == 0) server();
	if (strcmp(argv[1], "client") == 0) {
		if (argc < 3) {
			printf("missing address\n");
			return 0;
		}
		else client(argv[2]);
	}
	return 0;
}