#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdlib.h>

#ifndef INADDR_BROADCAST
#error "You need SDL_net 1.2.5 or newer for broadcast support!"
#endif
#define CRYPTIC_MSG "Hold the mayo and pass the cosmic awareness."

/**

An ubersimple example of UDP broadcasting with SDL_net.
It opens a UDP broadcast socket and sends on port 9999
Tyler Montbriand, Feb 2004
*/
#define UDP_PORT 9999

int main(int argc, char *argv[])
{
Uint16 udp_port=9999;
IPaddress bc_send_ip={INADDR_BROADCAST,SDL_SwapBE16(udp_port)};
UDPsocket bc_send=NULL;
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

bc_send=SDLNet_UDP_Open(UDP_PORT);
if(bc_send==NULL)
{
fprintf(stderr,"Couldn’t open bc_send\n");
return(1);
}

if(SDLNet_UDP_Bind(bc_send,-1,&bc_send_ip)<0)
{
fprintf(stderr,"Couldn’t bind broadcast sending address\n");
SDLNet_UDP_Close(bc_send);
return(1);
}

fprintf(stderr,"Broadcast socket opened!\n");

packet=SDLNet_AllocPacket(1024);
if(packet==NULL)
{
fprintf(stderr,"Couldn’t allocate packet!\n");
SDLNet_UDP_Close(bc_send);
return(1);
}

strcpy(packet->data,CRYPTIC_MSG);
packet->len=strlen(CRYPTIC_MSG)+1;
fprintf(stderr,"Sending packet of %d bytes\n",packet->len);
if(SDLNet_UDP_Send(bc_send,0,packet)<0)
{
fprintf(stderr,"Couldn’t send packet!\n");
SDLNet_UDP_Close(bc_send);
SDLNet_FreePacket(packet);
return(1);
}

SDLNet_UDP_Close(bc_send);
SDLNet_FreePacket(packet);

fprintf(stderr,"Done!\n");

return(0);
}