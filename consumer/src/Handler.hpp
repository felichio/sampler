#ifndef __HANDLER_HPP
#define __HANDLER_HPP

#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace R
{

    class Handler
    {
        public:
        Handler(uint16_t port);
        void init();

        private:
        uint16_t m_port;
        int m_socketfd;
        addrinfo m_hints, *m_serverinfo;
    };



}

#endif