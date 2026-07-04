#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc,char **argv)
{
    int s, t;
	int n,cp;
    struct sockaddr_in local, remote;
    char str[100];
	if(argc!=2)
	{
    printf("Usage : server  <port>");
	exit(0);
	}
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
        perror("socket");
        exit(1);
    }
    bzero((char *)&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port =htons((short)atoi(argv[1]));
    local.sin_addr.s_addr =htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&local,sizeof(local)) == -1) 
	{
        perror("bind");
        exit(1);
    }
	t = sizeof(remote);
	memset(str,0,100);

/* memset 
Syntax:  
 #include <string.h>
void* memset( void* buffer, int ch, size_t count );
The function memset() copies ch into the first count characters of buffer, and returns buffer. memset() is useful for initializing a section of memory to some value. For example, this command:
   memset( the_array, '\0', sizeof(the_array) );                
...is a very efficient way to set all values of the_array to zero.
The table below compares two different methods for initializing an array of characters: a for-loop versus memset(). As the size of the data being initialized increases, memset() clearly gets the job done much more quickly:
*/

	for(;;) 
	{
	   if((n=recvfrom(s, str, 100, 0,(struct sockaddr *)&remote,&t))<0)
	   {
            perror("recv from");
		    break;
	   }
		    
	   if((cp=fork())==0)
	   {
	       while(1)
		   {
            if (sendto(s, str, n, 0,(struct sockaddr *)&remote,sizeof(remote)) < 0) 
		    {
                 perror("send to");
				 break;
            }
			memset(str,0,100);
		    if((n=recvfrom(s, str, 100, 0,(struct sockaddr *)&remote,&t))<0)
	        {
                 perror("recv from");
				 break;
			}
		   }
        }
	    else if(cp<0)
	    {
		   perror("fork error");
		   exit(0);
		}
	   }
    close(s);
	return 0;
}
