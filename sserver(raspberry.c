#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mysql/mysql.h>

#define BUF_SIZE 1024
MYSQL my_connection;
MYSQL_RES *res_ptr;
MYSQL_ROW sqlrow;
void error_handling(char *message);

int main(int argc, char *argv[])
{
   int res, first_row =1;
   int serv_sock, clnt_sock;
   char buf[BUF_SIZE];
   int str_len, i,j;
   int count =1;
   struct sockaddr_in serv_adr, clnt_adr;
   socklen_t clnt_adr_sz;

   if(argc!=2)
   {
      printf("Usage : %s <port>\n", argv[0]);
      exit(1);
   }

   serv_sock = socket(PF_INET, SOCK_STREAM, 0);
   if(serv_sock ==-1)
      error_handling("socket() error");

   memset(&serv_adr, 0, sizeof(serv_adr));
   serv_adr.sin_family = AF_INET;
   serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_adr.sin_port = htons(atoi(argv[1]));

   if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) ==1)
      error_handling("bind() error");

   if(listen(serv_sock, 5) == -1)
      error_handling("listen() error");

   clnt_adr_sz = sizeof(clnt_adr);

   for(i = 0; i < 5; i++)
   {
      clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
      if(clnt_sock ==-1)
         error_handling("accept() error");
      else
         printf("Connected client %d \n", i+1);

      while((str_len = read(clnt_sock, buf, BUF_SIZE))!=0)
      {

         buf[str_len] = '\0';

         printf("%d %s\n",count++,buf);
         char *ptr = strtok(buf, " ");
         int j =0;
         char *sArr[30];
         while(ptr!=NULL)
         {
            sArr[j] = ptr;
            j++;
            ptr = strtok(NULL, " ");
         }
         int k = 0;
         for(k=0; k<3; k++)
         {
            if(sArr[k] !=NULL)
            printf("%s\n",sArr[k]);
         }
         printf("\n");
         char summer[15]="summerclothes";
         char spf[13]="spfclothes";
         char usual[13]="longclothes";
         int m=0;
         int result = atoi(sArr[0]);
         char query[255];
         //printf("%d\n", result);
	 mysql_init(&my_connection);
         if(mysql_real_connect(&my_connection, "localhost","root","root","noise",0,NULL,0))
	 {
         if(result > 30)
         {
               printf("connect success\n");
            sprintf(query, "INSERT INTO data(time,noise) values(now(),'%s')",sArr[0]);
            res = mysql_query(&my_connection,query);
	 
         	if(!res)
         	{
         	   printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));
         	}

         	else
         	{
         	   fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
         	}

         	   mysql_close(&my_connection);
		 } 
		 if(result > 26)
         {
               printf("connect success\n");
            sprintf(query, "INSERT INTO noisy(time,noise) values(now(),'%s')",sArr[0]);
            res = mysql_query(&my_connection,query);
	 
         	if(!res)
         	{
         	   printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));
         	}

         	else
         	{
         	   fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
         	}

         	   mysql_close(&my_connection);
		 }
		 if(result <27)
         {
               printf("connect success\n");
            sprintf(query, "INSERT INTO noisy(time,noise) values(now(),'%s')",sArr[0]);
            res = mysql_query(&my_connection,query);
	 
         	if(!res)
         	{
         	   printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));
         	}

         	else
         	{
         	   fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
         	}

         	   mysql_close(&my_connection);
		 }
         	else
         	{
                    fprintf(stderr,"Connection failed\n");
                    if(mysql_errno(&my_connection))
                    {
                           fprintf(stderr,"Connection error %d: %s \n",mysql_errno(&my_connection),mysql_error(&my_connection));
                    }
         	}
            
         }

}      
      close(clnt_sock);
   }

   close(serv_sock);
}

   


void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}

