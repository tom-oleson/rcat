/*
 * Copyright (c) 2019, Tom Oleson <tom dot oleson at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * The names of its contributors may NOT be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "server.h"



void _delay(int interval) {

    // allow other threads to do some work
    long n = 1000000;
    time_t s = 0;
    if(interval > 0) { 
        n = ((long) interval % 1000L) * 1000000L;
        s = (time_t) interval / 1000;
    }
    timespec delay = {s, n};
    nanosleep(&delay, NULL);    // interruptable
}


void cat_stdin(int socket, int interval) {
    char buf[1024] = {'\0'};
    size_t sz = 0;
    while(!std::cin.eof()) {
        std::cin.getline(buf, sizeof(buf));
        if((sz = std::cin.gcount()) > 0) {
            buf[sz-1] = '\n';
            if(-1 == cm_net::write(socket, buf, sz))
                break;
            _delay(interval);
        }
    }
}

void cat_file(int socket, int interval, std::string &file_name) {

    std::fstream fs;
    char buf[1024] = {'\0'};
    size_t sz = 0;

    fs.open(file_name.c_str(), std::fstream::in);
    if(fs.is_open()) {
        
        while(!fs.eof()) {
            fs.getline(buf, sizeof(buf));
            if((sz = fs.gcount()) > 0) {
                buf[sz-1] = '\n';
                if(-1 == cm_net::write(socket, buf, sz))
                    break;
                _delay(interval);
            }
        }
        fs.close();
    }
}


void client_receive(int socket, const char *buf, size_t sz) {
    puts(std::string(buf, sz).c_str());
}

void rcat::run(int keep, int interval, const std::string &host_name, int host_port, const std::vector<std::string> &files) {

    if(cm_net::gethostbyname(host_name, NULL) == CM_NET_ERR) {
        return;
    }

    cm_net::client_thread *client = nullptr;
    client = new cm_net::client_thread(host_name, host_port, client_receive); 
    if(nullptr != client && client->is_connected()) {

        if(files.size() == 0) {
            cat_stdin(client->get_socket(), interval);
        }
        else {
            for(auto file_name : files) {
                cat_file(client->get_socket(), interval, file_name);
            }
        }
    }
    
    long n = 1000000000;
    if(keep > 0)  {
        n *= keep;
        timespec delay = {0, n};
        nanosleep(&delay, NULL);
    }
    

    if(nullptr != client) delete client;
}
