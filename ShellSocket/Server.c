#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "headers.h"

#define MAXWAIT 5    /*Maximum number of connection requests*/
char msgBuffer[256];	//Character buffer for reading from socket

void reportError(char *err);


int main(int argc, char *argv[])
{
    const char *SM_name = "Shared Mem";
    int shm_fd;                 //shared memory segment return  
    int serverFDT,clientFDT;    //Socket File Descriptor Tables
    int portNum;		//Server Port #
    socklen_t clientAddrLen; //Size of Client address (used in system call)
    int messageLen;		//Size of message sent between client/server
    struct sockaddr_in serverAddr; //Server Address
    struct sockaddr_in clientAddr; //Client Address 
    shared_struct *ptr;	// base address, from mmap()
    int size;
    int exitProg;

    exitProg = 1;
    size = sizeof(shared_struct);  

     //open shared memory segment
     shm_fd = shm_open(SM_name, O_RDWR, 0666);
     if (shm_fd == -1) {
		printf("Shared memory failed\n");
		exit(-1);
	}

    //map shared memory segment in address space of this process
    ptr = mmap(0,size,PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	} 

    /*
     * Prompt user for port number to listen on and setup socket.
     */
    printf("Listening Port Number is 5555 \n");
    portNum = 5555;
   // scanf("%d", &portNum);   

    /* 
     * Using AF_INET, Stream Reading, and let system decide on TCP/UDP by passing 0 
     */
    serverFDT = socket(AF_INET, SOCK_STREAM, 0);

    if(serverFDT < 0) //If value is negative, socket creation failed, exit and print error
    {
        reportError("Socket Initialization Failed!!"); 
    }

    /*Assign the given port number to the socket*/
    memset(&serverAddr, 0 , sizeof(serverAddr));   //Fill address with zeros
    serverAddr.sin_family = AF_INET;		   //Address family
    serverAddr.sin_port = htons(portNum);       // Set local port using Network Byte Order
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Local IP address (Host IP)

    /*Bind Local Socket and exit with error if failure occurs.*/
    if(bind(serverFDT, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0)
    {
        reportError("Server Socket Bind Failed!!"); 
    }

    /*Setup server socket to listen for incoming connections.*/
    if(listen(serverFDT, MAXWAIT) < 0)
    {
        reportError("Error Listening!!");
    }

    /*Client Handling*/
    clientAddrLen =sizeof(clientAddr);

	while(exitProg){
       	 /*Block program until a client connects*/
        clientFDT = accept(serverFDT, (struct sockaddr * ) &clientAddr, &clientAddrLen);
	    if(clientFDT < 0)
	    {
        	reportError("Error Accepting Client Connection!");
	    }

	    printf("Client Connected!\n");
	    memset(msgBuffer, 0 , 256);   //Fill buffer with zeros

	    /*Get data and process*/
	    messageLen = read(clientFDT,msgBuffer,255);
	    
	    if(messageLen < 0)
	    {
	     reportError("Error Receiving Data!");
	    }
   
		sprintf(ptr->buffer[0].data,"%s",msgBuffer);
	        printf("\n");	        ;
		//Set outbox to 1 to show message has been received
   		ptr->out = 1;
		msgBuffer[strcspn(msgBuffer, "\n")] = 0; 
		if(strcmp(msgBuffer, "exit") == 0)
		{
		 exitProg = 0;
		} 

	    /*Send Message Back To Client*/
	    messageLen = write(clientFDT,"Command Received Successfully!\n",100);

	    if(messageLen < 0)
	    {
	     reportError("Error Sending Data!");
	    }

	}
	return 0;
}

/*Reports and error and exits the program*/
void reportError(char *err)
{
    perror(err);
    exit(1);
}






