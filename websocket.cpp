
/**********************************************************************************/
/* example.cpp                                                                    */
/*                                                                                */
/* This file contains an example on how to create a simple web socket live        */
/* broadcasting service from an mp4 file                                          */
/**********************************************************************************/
/*                  This file is part of the ERT-Tiroir project                   */
/*                      github.com/ert-tiroir/wsserver-video                      */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2023 ert-tiroir                                                  */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

using namespace std;

#include <wsserver/server.h>
#include <wsvideo/broadcaster.h>

void nonblocking (int fd) {
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl( fd, F_SETFL, flags);
}
string create (char* buffer, int size) {
    string str(size, '.');
    for (int i = 0; i < size; i ++) str[i] = buffer[i];
    return str;
}

int main () {
    printf("Creating web socket server...\n");
    WebSocketServer server;
    server.init(5420);

    printf("Creating broadcast\n");
    VideoBroadcaster broadcast (&server, "flux.mp4");
    
    printf("Writing initial packet...\n");

    int fd_stdin = fileno(stdin);

    nonblocking(fd_stdin);

    const int SIZE = 1000000;
    char buffer[SIZE];
    
    while (true) {
        server.listen();

        broadcast.tick();
    
        int size = read(fd_stdin, buffer, SIZE - 1);
        if (size > 0) {
            broadcast.sendPacket( create(buffer, size) );
            usleep(1000);
        }
    }

    server.close();
}
