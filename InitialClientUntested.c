#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void reportError(char *err);

int main(int argc, char *argv[])
{
   int socketFDT;                 //Socket File Descriptor Table
   int portNum;		 	  //Server Port #
   struct sockaddr_in serverAddr; //Socket Address 
   char msgBuffer[4096];          //Message Buffer Size
   char ipAddr[12];		  //Ip address of server
   int messageLen;		 //Size of message sent between client/server

   /* 
    * Create socket. Using AF_INET, Stream Reading, and let system decide on	       	   *  TCP/UDP by passing 0 
    */
   socketFDT = socket(AF_INET, SOCK_STREAM, 0);

   if(socketFDT < 0) //If value is negative, socket creation failed, exit and print error
    {
        reportError("Socket Initialization Failed!!"); 
    }
    printf("Please enter the IP address of the server you wish to connect to: \n");
    scanf("%s", ipAddr);

    /*Assign the given port number and IP address to the socket*/
    memset(&serverAddr, 0 , sizeof(serverAddr));  //Fill address with zeros
    serverAddr.sin_family = AF_INET;		  //Address family
    serverAddr.sin_port = htons(portNum);         //Set port using Net Byte Order
    serverAddr.sin_addr.s_addr = inet_addr(ipAddr);	  //Ip Address of Server

    /*Connect and check connection*/
    if (connect(socketFDT, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0)
    {
        reportError("Server Connection Failed!!");
    }

    /*Send data to server*/
    printf("Enter a message to send to the server: \n");
    memset(msgBuffer, 0 , 4096);   //Fill buffer with zeros
    fgets(msgBuffer,4095,stdin);
   
    /*Write to socket*/
    messageLen = write(socketFDT,msgBuffer,4095);
    
    if (messageLen < 0)
    {
      reportError("Write Failed!");
    }    
    
    memset(msgBuffer, 0 , 4096);   //Fill buffer with zeros

    /*Read from socket*/
    messageLen = read(socketFDT,msgBuffer,4095);
    
     if (messageLen < 0)
    {
      reportError("Read Failed!");
    }  
    printf("%s", msgBuffer);
    
    return 0; 
}

/*Reports and error and exits the program*/
void reportError(char *err)
{
    perror(err);
    exit(1);
}
