#ifndef _TCP_CONNECTION_HPP

#define _TCP_CONNECTION_HPP

#include <thread>
#include "TcpServer.hpp"

static const uint32_t buffer_size = 8192;

namespace R
{
    class TcpServer; // forward declaration
    class TcpConnection
    {
        public:
            TcpConnection(TcpServer &server, int socketfd); 
            ~TcpConnection();
            void start_reading_thread();
            void fill_buffer();
        private:
            TcpServer& m_tcpServer; 
            uint8_t m_buffer[buffer_size];
            int m_socketfd;
    };
}

#endif
