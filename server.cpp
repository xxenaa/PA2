#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cmath>
#include <string>
#include <iostream>
#include <netdb.h>
#include <sys/wait.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}
void fireman(int){
    while(waitpid(-1,NULL, WNOHANG) > 0)
        std::cout<<"A child process ended" <<std::endl;
}
std::string encode(int value){
    int x,temp,i;
    x= value;
    temp = floor(log2(x));
    std::string ret;
    for (i=0; x>0;i++){
        int a=x%2;
        ret=std::to_string(a)+ret;
        x=x/2;
    }
    for(i=0;i<temp;i++){
        ret="0"+ret;
    }
    return ret;
    
}
std::string bin(int value){
    int x,i;
    x= value;
    std::string ret;
    for (i=0; x>0;i++){
        int a=x%2;
        ret=std::to_string(a)+ret;
        x=x/2;
    }

    return ret;
    
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, n;
     
    
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
    
     sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket
     if (sockfd < 0)
        error("ERROR opening socket");
    
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);//assignin port
    //start pop serv addy local host points to same comp
    bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr));
             // error("ERROR on binding");
    
    
    pid_t pid;
    listen(sockfd,100);
    clilen = sizeof(cli_addr);
    signal(SIGCHLD, fireman);
    
    
    while(true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
        //if (newsockfd < 0) //use to read
         //   error("ERROR on accept");
            //
            if(fork()==0)
            {
                char buffer[100];
                bzero(buffer,sizeof(buffer));
                char eli[100];
                bzero(eli,sizeof(eli));
                char b[100];
                bzero(b,sizeof(eli));
                std::string elias, binary;
                n = read(newsockfd,buffer,sizeof(buffer));
                //if (n < 0)
                 //   error("ERROR reading from socket");
                
                std::string str = buffer;
                int a= std::stoi(str);
                
                elias=encode(a);
                binary=bin(a);
                
                for(int j=0;j<elias.length();j++){
                    eli[j]=elias[j];
                }
                for(int j=0;j<binary.length();j++){
                    b[j]=binary[j];
                }
               
                n = write(newsockfd,eli,sizeof(eli));
                //if (n < 0)
                 //   error("ERROR writing to socket");
                n = write(newsockfd,b,sizeof(b));
                //if (n < 0)
                 //   error("ERROR writing to socket");
                
                close(newsockfd);
                _exit(0);
            }
        
        
    }
    
     return 0;
}

