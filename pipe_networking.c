#include "pipe_networking.h"
#define WKP "the computer interaction club official discord server"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int*to_client){
	char pied[HANDSHAKE_BUFFER_SIZE];
	if(mkfifo(WKP,0644)){
		printf("server makeification unsuccesful, please try deleting system32\n%s\n",strerror(errno));
		return 0;
	}
	read(*to_client=open(WKP,O_RDONLY),pied,HANDSHAKE_BUFFER_SIZE);
	close(*to_client);
	remove(WKP);
	int uppity=open(pied,O_WRONLY);
	write(uppity,ACK,HANDSHAKE_BUFFER_SIZE);
	printf("connection established with client %s\n",pied);
	return uppity;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int*to_server){
	char pied[HANDSHAKE_BUFFER_SIZE],i=0,ackchk[HANDSHAKE_BUFFER_SIZE];
	for(int p=getpid();i<HANDSHAKE_BUFFER_SIZE&&p;pied[i++]=p%10+'0',p/=10);
	for(;i<HANDSHAKE_BUFFER_SIZE;pied[i++]=0);
	if(mkfifo(pied,0644)){
		printf("server makeification unsuccesful, please try installing more ram\n%s\n",strerror(errno));
		return 0;
	}
	int downitty=open(WKP,O_WRONLY);
	write(downitty,pied,HANDSHAKE_BUFFER_SIZE);
	read(*to_server=open(pied,O_RDONLY),ackchk,HANDSHAKE_BUFFER_SIZE);
	remove(pied);
	if(strcmp(ACK,ackchk)){
		printf("server had the big RUDE and did not properly acknowledge\n");
		return 0;
	}
	printf("client %s (that's me) established connection with server %s, they said %s\n", pied, WKP, ackchk);
	return downitty;
}
