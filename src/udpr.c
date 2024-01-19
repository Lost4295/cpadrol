#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdlib.h>

/***

An ubersimple example of UDP broadcasting with SDL_net.
Opens a UDP port to recieve UDP broadcast from port 9999, then waits
for a packet.
Tyler Montbriand, Feb. 2004
*/
#ifndef INADDR_BROADCAST
#error "You need SDL_net 1.2.5 or newer to send broadcasts!"
#endif

#define UDP_PORT 9999

#define CRYPTIC_MSG "Hold the mayo and pass the cosmic awareness."

int main(int argc, char *argv[])
{
Uint16 udp_port=9999;
IPaddress bc_recv_ip={INADDR_ANY ,udp_port};
UDPsocket bc_recv=NULL;
UDPpacket *packet=NULL;

if(SDL_Init(SDL_INIT_TIMER)<0)
{
fprintf(stderr,"Couldn’t init SDL: %s\n",SDL_GetError());
return(1);
}
else
atexit(SDL_Quit);

if(SDLNet_Init()<0)
{
fprintf(stderr,"Couldn’t initialize SDL_Net\n");
return(1);
}
else
atexit(SDLNet_Quit);

bc_recv=SDLNet_UDP_Open(UDP_PORT);
if(bc_recv==NULL)
{
fprintf(stderr,"Couldn’t open bc_recv\n");
SDLNet_UDP_Close(bc_recv);
return(1);
}

if(SDLNet_UDP_Bind(bc_recv,-1,&bc_recv_ip)<0)
{
fprintf(stderr,"Couldn’t bind broadcast recieving address\n");
SDLNet_UDP_Close(bc_recv);
return(1);
}

fprintf(stderr,"Recieving port opened!\n");

packet=SDLNet_AllocPacket(1024);
if(packet==NULL)
{
fprintf(stderr,"Couldn’t allocate packet!\n");
SDLNet_UDP_Close(bc_recv);
return(1);
}

{
int tick=SDL_GetTicks64();
int recieved=0;

fprintf(stderr,"Waiting for packet.\n");

while(((SDL_GetTicks64()-tick)<10000)&&(!recieved))
{
  if(SDLNet_UDP_Recv(bc_recv,packet)>0)
    recieved=1;
  else
    SDL_Delay(100);
}

if(recieved)
  fprintf(stderr,"UDP speaketh:  %s\n",packet->data);
else
  fprintf(stderr,"Timeout;  no packet.\n");
}

SDLNet_UDP_Close(bc_recv);
SDLNet_FreePacket(packet);

return(0);
}

