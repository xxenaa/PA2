#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <iomanip>

int sockfd, portno,n;
//struct sockaddr_in serv_addr;
//struct hostent *server;

pthread_mutex_t stopGo;

void error(char *msg)
{
    perror(msg);
    exit(0);
}
struct info{
    std::string num;
    std::string elias;
    std::string bin;
    char hostname[100];
    int port;
    
};
//
void *send (void *str){
    struct info *ptr= (struct info*)str;
    char buffer[100];
    bzero(buffer,sizeof(buffer));
    char rec1[100];
    bzero(rec1,sizeof(rec1));
    char rec2[100];
    bzero(rec2,sizeof(rec2));
    
    pthread_mutex_lock(&stopGo);
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(ptr->hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
       serv_addr.sin_family = AF_INET;
       bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
       serv_addr.sin_port = htons(portno);
       if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
           error("ERROR connecting");
    
    for(int j=0;j<ptr->num.length();j++){
        buffer[j]=ptr->num[j];
    }
    
    n = write(sockfd,buffer,strlen(buffer));
    // if (n < 0)
      //    error("ERROR writing to socket");
     
    n = read(sockfd,rec1,sizeof(rec1));
     //if (n < 0)
      //    error("ERROR reading from socket");
    n = read(sockfd,rec2,sizeof(rec2));
    //if (n < 0)
     //    error("ERROR reading from socket");
    
    std::string e = rec1;
    std::string b = rec2;
    //std::cout<<"bin  "<<b<< std::endl;
    ptr->elias=e;
    ptr->bin=b;
    close(sockfd);
    pthread_mutex_unlock(&stopGo);
    return NULL;
}

int main(int argc, char *argv[])
{
    

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    //sockfd = socket(AF_INET, SOCK_STREAM, 0);
    

    std::string text;
    std::vector<std::string> vect;
    std::string temp;
    int sum;
       
    getline(std::cin,text);
    sum=stoi(text);
    
    for(int i=0; i<sum;i++){
        getline(std::cin,text);
        vect.push_back(text);
    }
    
    
    struct info in[vect.size()];
    pthread_t tid[vect.size()];
    std::string x;
    
    
    
    for(int i=0; i<vect.size() ;i++){
        x=vect.at(i);
        in[i].num=x;
        strcpy(in[i].hostname,argv[1]);
        in[i].port=portno;
        pthread_create(&tid[i], NULL, send, &in[i]);
        pthread_join(tid[i],NULL);

    }
    for(int i=0; i<vect.size() ;i++){
        std::cout<<"Value: "<< vect.at(i)<<", Binary Code: "<<in[i].bin<<", Elias-Gamma code: "<<in[i].elias<<std::endl;

        
    }
   
    return 0;
}
