#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

void dns(int sockfd);
void query(int sockfd);

int main(int argc, char *argv[]) {
   int sockfd, portno, n, x;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[256];
   
   portno = 1234;

   
   
   server = gethostbyname("127.0.0.1");
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   
   
   /* Now ask for a message from the user, this message
      * will be read by server
   */
   
   /* Now read server response */
   while(1){

      /* Create a socket point */
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
      if (sockfd < 0) {
         perror("ERROR opening socket");
         exit(1);
      }

      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
      serv_addr.sin_port = htons(portno);
   
      /* Now connect to the server */
      if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
         perror("ERROR connecting");
         exit(1);
      }


      //n = write(sockfd, "initial!", 9);
      bzero(buffer,256);
      n = read(sockfd, buffer, 255);
      if (n < 0) {
         perror("ERROR reading from socket");
         exit(1);
      }
      printf("%s",buffer);      

      /* Send message to the server */
      bzero(buffer,256);
      fgets(buffer,255,stdin);
      n = write(sockfd, buffer, strlen(buffer));
      x = atoi(buffer);

      /* Now read server response */
      bzero(buffer,256);
      n = read(sockfd, buffer, 255);
      if (n < 0) {
         perror("ERROR reading from socket");
         exit(1);
      }
      buffer[strcspn(buffer, "\n")] = 0;
      printf("%s",buffer);

      switch(x){
         case 1:
            dns(sockfd);
            break;
         case 2:
            query(sockfd);
            break;
         case 3:
            exit(1);
            break;
         default:
            break;
      }
      close(sockfd);
   }
   return 0;
   
}

void dns(int sockfd){
   int n;
   char addr[256];
   char ip[100];

   /* Send address to the server */
   bzero(addr,256);
   fgets(addr,255,stdin);
   addr[strcspn(addr, "\n")] = 0;
   n = write(sockfd, addr, strlen(addr));
   /* Now read server ip response */
   bzero(ip,100);
   n = read(sockfd, ip, 99);
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   printf("address get from domain name : %s\n", ip);
   
}

void query(int sockfd){
   char email[256];
   char ID[15];
   int n;
    
   /* Send studentID to the server */
   bzero(ID,15);
   fgets(ID,14,stdin);
   ID[strcspn(ID, "\n")] = 0;
   n = write(sockfd, ID, strlen(ID)); 
   if (n < 0){
      perror("ERROR writing to socket");
      exit(1);
   }
   /* Now read server email response */ 
   bzero(email, 256);
   n = read(sockfd, email, 255); 
   if (n < 0){
      perror("ERROR reading from socket");
      exit(1);
   } 
   printf("Email get from the server : %s\n", email);
}