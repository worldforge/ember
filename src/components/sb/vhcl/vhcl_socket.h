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

#ifndef VHCL_SOCKET_H
#define VHCL_SOCKET_H


namespace vhcl
{

#if WIN_BUILD
typedef void * socket_t;
#else
typedef int socket_t;
#endif

std::string SocketGetHostname();
bool SocketStartup();
bool SocketShutdown();
socket_t SocketOpenTcp();
socket_t SocketAccept(socket_t socket);
void SocketClose(socket_t socket);
bool SocketSetReuseAddress(socket_t socket, bool reuse);
bool SocketBind(socket_t socket, int port);
bool SocketConnect(socket_t socket, const std::string & server, int port);
bool SocketSetBlocking(socket_t socket, bool blocking);
bool SocketListen(socket_t socket, int numBackLog = 10);
bool SocketIsDataPending(socket_t socket);
bool SocketSend(socket_t socket, const std::string & msg);
int SocketReceive(socket_t socket, char * buffer, int bufferSize);

};


#endif  // VHCL_SOCKET_H
