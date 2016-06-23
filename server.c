#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>


#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>


void doprocessing (int sock);
int hostname_to_ip(char * hostname , char* ip);
void query(int newsockfd);

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 1234;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   
   /* Now start listening for the clients, here
      * process will go in sleep mode and will wait
      * for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   while (1) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }
      
      /* Create child process */
      pid = fork();
      
      if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the client process */

         close(sockfd);
         doprocessing(newsockfd);
         exit(0);
      }
      else {
         close(newsockfd);
      }
      
   } /* end of while */
}

void doprocessing (int sock) {
   int n, x;
   char buffer[256];

   /*
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n != 0){
      n = write(sock,"What's your requirement? 1.DNS 2.QUERY 3.QUIT : ", 53);
   }*/
   n = write(sock,"What's your requirement? 1.DNS 2.QUERY 3.QUIT : ", 53);
   bzero(buffer,256);
   n = read(sock,buffer,255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   x = atoi(buffer);
   if (x == 1){
      char *hostname;
      char ip[100];
      /* Send message to the client */
      n = write(sock,"Input URL address : ",20);
      /* Now read client response */
      bzero(buffer,256);
      n = read(sock,buffer,255);
      hostname = buffer;
      hostname_to_ip(hostname , ip);
      n = write(sock, ip, strlen(ip)); 
   }
   else if (x == 2){
      /* Send message to the client */
      n = write(sock,"Input student ID : ",19);
      query(sock);
   }
   else 
      exit(1);

   //close(sock);
   
}

int hostname_to_ip(char * hostname , char* ip){
   struct hostent *he;
   struct in_addr **addr_list;
   int i;

   if ( (he = gethostbyname( hostname ) ) == NULL) {
        // get the host info
        herror("gethostbyname");
        return 1;
   }
 
   addr_list = (struct in_addr **) he->h_addr_list;
     
   for(i = 0; addr_list[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
   }
   
   return 1;
}

void query(int newsockfd){
   FILE * fp;
   int isMatch = 0;
   char ID[15], email[256];
   char input[256];
   int n;
    
   bzero(input ,256);
   n = read(newsockfd ,input ,255 );
    
   if (n < 0){
      perror("ERROR reading from socket");
      exit(1);
   }
    
   fp = fopen("query.txt", "r");
   rewind(fp);
   while(!feof(fp)){ //strcmp 1 = false, 0 = true
      fscanf(fp, "%s  %s", ID, email);
      if(strcmp(input, ID) == 0){
         isMatch = 1;
         break;
      }
   }
   fclose(fp);
    
   if(isMatch == 1){
      n = write(newsockfd ,email ,strlen(email));
      if (n < 0){
         perror("ERROR writing to socket");
         exit(1);
      }
   }
   else{
      n = write(newsockfd ,"No such student ID" ,strlen(email));   
      if (n < 0){
         perror("ERROR writing to socket");
         exit(1);
      }
   }
}