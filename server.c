#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

char indexPage[]=
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title><Lakshapana Falls</title>\r\n"
"<style>body { background-color: #47eeec }</style></head>\r\n"
"<body><center><h1>Lakshapana Falls</h1><br>\r\n"
"<img src=\"IMG_1490-1024x683.jpg\"></center>"
"<center><h1>Audio Stream</h1></center>"
"<center>"
"<audio controls>"
"<source src=\"seeYouAgain.ogg\" type=\"audio/ogg\">"
"<source src=\"seeYouAgain.mp3\" type=\"audio/mpeg\">"
"Your browser does not support the audio tag."
"</audio>"
"</center>"
"<center><h1>Video Stream</h1></center>"
"<center>"
"<video width=\"320\" height=\"240\" controls>"
"<source src=\"got.mp4\" type=\"video/mp4\">"
" <source src=\"got.ogg\" type=\"video/ogg\">"
" Your browser does not support the video tag."
"</video>"
"</center>"
"</body></html>\r\n";


int main(int argc,char *argv[]){

    struct sockaddr_in server_addr,client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int server , client;

    char buffer[2048];
    int imgDt;
    int on=1;

    server= socket(AF_INET,SOCK_STREAM,0);
    if(server <0){
        perror("socket is not created succesfully");
        exit(1);
    }

    setsockopt(server, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(int) );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port = htons(8080);

    int bindVal = bind(server,(struct sockaddr *) &server_addr,sizeof(server_addr));

    if(bindVal == -1){

        perror("bind is not succesfully");
        close(server);
        exit(1);
    }

    int listenVal = listen(server,10);

    if(listenVal == -1){

        perror("listen is not happening");
        close(server);
        exit(1);

    }

    while(1){
        client= accept(server,(struct sockaddr *) &client_addr,&sin_len);

        if(client == -1){
            perror("connection failed....\n");
            continue;


        }

        printf("Got client connection.....");

        if(!fork()){
            close(server);
            memset(buffer,0,2048);
            read(client,buffer,2047);

            printf("%s\n",buffer);

            if(!strncmp(buffer,"GET /Dapino-Summer-Holiday-Palm-tree.ico",16)){

                imgDt = open("Dapino-Summer-Holiday-Palm-tree.ico", O_RDONLY);
                sendfile(client, imgDt, NULL, 4000);
                close(imgDt);


            }

            else if(!strncmp(buffer,"GET /IMG_1490-1024x683.jpg",16)){


                imgDt = open("IMG_1490-1024x683.jpg", O_RDONLY);
                sendfile(client, imgDt, NULL, 1000000);
                close(imgDt);
            }

            else if(!strncmp(buffer,"GET /seeYouAgain.mp3",16)){


                imgDt = open("seeYouAgain.mp3", O_RDONLY);
                sendfile(client, imgDt, NULL, 100000000);
                close(imgDt);
            }

            else if(!strncmp(buffer,"GET /got.mp4",16)){


                imgDt = open("got.mp4", O_RDONLY);
                sendfile(client, imgDt, NULL, 100000000);
                close(imgDt);
            }

             else if(!strncmp(buffer,"GET /textfile.txt",16)){


                imgDt = open("textfile.txt", O_RDONLY);
                sendfile(client, imgDt, NULL, 100000000);
                close(imgDt);
            }




            else
                write(client,indexPage,sizeof(indexPage)-1);

                close(client);
                printf("closing....\n");
                exit(0);

        }

        close(client);

    }
    return 0;
}