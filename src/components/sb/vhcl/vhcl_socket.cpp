/*
   This file is part of VHMsg written by Edward Fast at 
   University of Southern California's Institute for Creative Technologies.
   http://www.ict.usc.edu
   Copyright 2008 Edward Fast, University of Southern California

   VHMsg is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   VHMsg is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with VHMsg.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "vhcl.h"
#include "vhcl_socket.h"

#include <string>
#include <vector>

#if WIN_BUILD
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
typedef int SOCKET;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s);
#define SOCKADDR struct sockaddr
#endif


using std::string;
using std::vector;


namespace vhcl
{


//http://tangentsoft.net/wskfaq/articles/bsd-compatibility.html

bool g_wsaStartupCalled = false;

string SocketGetHostname()
{
   char * hostname = new char [256];
   /*int ret = */ gethostname(hostname, 256);  // 256 is guaranteed to be long enough  http://msdn.microsoft.com/en-us/library/windows/desktop/ms738527(v=vs.85).aspx
   string hostnameStr = hostname;
   delete [] hostname;
   return hostnameStr;
}

bool SocketStartup()
{
#if WIN_BUILD
   WSADATA wsaData;
   int err = WSAStartup( MAKEWORD(2,2), &wsaData );
   if ( err != 0 )
   {
      printf( "WSAStartup failed. Code: %d\n", err );
      return false;
   }

   g_wsaStartupCalled = true;
   return true;
#else
   return true;
#endif
}

bool SocketShutdown()
{
#if WIN_BUILD
   if ( g_wsaStartupCalled )
   {
      WSACleanup();
      g_wsaStartupCalled = false;
   }
   return true;
#else
   return true;
#endif
}

socket_t SocketOpenTcp()
{
   SOCKET sockTCP = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
   if ( sockTCP == INVALID_SOCKET )
   {
      printf( "Couldn't create socket tcp.\n" );
#if WIN_BUILD
      int errnum = WSAGetLastError();
#else
      int errnum = errno;
#endif
      printf( "socket error: %d\n", errnum );
      return (socket_t)NULL;
   }

   return (socket_t)sockTCP;
}

socket_t SocketAccept(socket_t socket)
{
   SOCKET sockTCP;
   sockaddr_in newToAddr;

#if WIN_BUILD
   int i = sizeof( sockaddr_in );
#else
   socklen_t i = sizeof( sockaddr_in );
#endif

#if !defined(FLASH_BUILD)
   sockTCP = accept( (SOCKET)socket, (sockaddr *)&newToAddr, &i );
#endif

   //printf( "New Connection!\n" );
   //string clientIP = inet_ntoa( newToAddr.sin_addr );

   return (socket_t)sockTCP;  // TODO - check for errors
}

void SocketClose(socket_t socket)
{
   closesocket((SOCKET)socket);
}

bool SocketSetReuseAddress(socket_t socket, bool reuse)
{
   int reuseAddr = 1;
   setsockopt((SOCKET)socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseAddr, sizeof(int));
   return true;  // TODO - check return code
}

bool SocketBind(socket_t socket, int port)
{
   sockaddr_in m_addrTCP;
   m_addrTCP.sin_family      = AF_INET;
   m_addrTCP.sin_addr.s_addr = INADDR_ANY;
   m_addrTCP.sin_port        = htons( (uint16_t)port );
   memset(m_addrTCP.sin_zero, 0, sizeof(m_addrTCP.sin_zero));

   if (bind((SOCKET)socket, (sockaddr *)&m_addrTCP, sizeof(m_addrTCP)) == SOCKET_ERROR)
   {
      printf("bind() failed.\n");
#if WIN_BUILD
      int errnum = WSAGetLastError();
#else
      int errnum = errno;
#endif
      printf("socket error: %d\n", errnum);
      return false;
   }

   return true;
}

bool SocketConnect(socket_t socket, const std::string & server, int port)
{
   sockaddr_in toAddrTCP;

   // see if we're specifying a host by name or by number
   if (isalpha(server[0]))
   {
      hostent * host = gethostbyname(server.c_str());
      if (host == NULL)
      {
         printf( "gethostbyname() failed.\n" );
#if WIN_BUILD
         int errnum = WSAGetLastError();
#else
         int errnum = errno;
#endif
         printf( "socket error: %d\n", errnum );
         return false;
      }

      toAddrTCP.sin_family = AF_INET;
      toAddrTCP.sin_addr = *((in_addr *)host->h_addr);
      toAddrTCP.sin_port = htons((uint16_t)port);
   }
   else
   {
      toAddrTCP.sin_family = AF_INET;
      toAddrTCP.sin_addr.s_addr = inet_addr(server.c_str());
      toAddrTCP.sin_port = htons((uint16_t)port);
   }


   int ret;

#if !defined(FLASH_BUILD)
   ret = connect((SOCKET)socket, (SOCKADDR*)&toAddrTCP, sizeof(toAddrTCP));
#endif

   if (ret < 0)
   {
      printf( "connect() failed.\n" );
#if WIN_BUILD
      int errnum = WSAGetLastError();
#else
      int errnum = errno;
#endif
      printf( "socket error: %d\n", errnum );
      return false;
   }

   printf( "Connect Success\n" );
   return true;
}

bool SocketSetBlocking(socket_t socket, bool blocking)
{
   u_long nonBlocking = blocking ? 0 : 1;
#if WIN_BUILD
   ioctlsocket((SOCKET)socket, FIONBIO, &nonBlocking);
#else
   ioctl((SOCKET)socket, FIONBIO, &nonBlocking);
#endif
   return true;  // TODO - check return code
}

bool SocketListen(socket_t socket, int numBackLog)
{
   listen((SOCKET)socket, numBackLog);
   return true;  // TODO - check return code
}

bool SocketIsDataPending(socket_t socket)
{
   fd_set readfds;
   FD_ZERO(&readfds);
   FD_SET((SOCKET)socket, &readfds);
   timeval timeout = { 0, 0 };  // return immediately
#if WIN_BUILD
   int error = select(0, &readfds, 0, 0, &timeout);   // 1st parameter ignored by winsock
#else
   int error = select((SOCKET)socket + 1, &readfds, 0, 0, &timeout);   // http://www.lowtek.com/sockets/select.html
#endif
   if ( error == SOCKET_ERROR )
   {
      return false;
   }
   else if ( error == 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

bool SocketSend(socket_t socket, const string & msg)
{
   int bytesSent = send((SOCKET)socket, msg.c_str(), msg.length(), 0);
   if (bytesSent < 0)
   {
#if WIN_BUILD
      /*int errnum =*/ WSAGetLastError();
#else
      //int errnum = errno;
#endif
      //printf( "socket error: %d\n", errnum );
      //fprintf( fp, "socket error: %d\n", errnum );

      return false;
   }
   else if ( bytesSent > 0 )
   {
      //printf( "send: %ld\n", bytesSent );
      //fprintf( fp, "send: %ld\n", bytesSent );
   }

   return true;
}

int SocketReceive(socket_t socket, char * buffer, int bufferSize)
{
   int bytesReceived = recv((SOCKET)socket, buffer, bufferSize - 1, 0);
   if (bytesReceived > 0)
   {
      buffer[bytesReceived] = 0;  // null terminate the buffer, since recv() doesn't
   }

   return bytesReceived;
}


};
