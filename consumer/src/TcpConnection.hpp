#ifndef _TCP_CONNECTION_HPP

#define _TCP_CONNECTION_HPP

#include <thread>
#include "TcpServer.hpp"
#include "RbufferManager.hpp"

extern uint32_t RBUFFER_SIZE;
namespace R
{
    static const uint32_t buffer_size = 8192;
    static const uint32_t r_buffer_size = RBUFFER_SIZE;

    enum ENDIANESS: uint8_t
    {
        LE = 0x00,
        BE= 0x01
    };

    enum TYPE: uint8_t
    {
        INT64 = 0x00,
        INT32 = 0x01
    };

    class TcpServer; // forward declaration
    class TcpConnection
    {
        public:
            TcpConnection(TcpServer &server, int socketfd); 
            ~TcpConnection();
            void start_reading_thread();
        private:
            void fill_buffer();
            void choose_buffer();
            void flush_buffer(uint8_t *beg, uint8_t *end);
            TcpServer& m_tcpServer; 
            uint8_t m_buffer[buffer_size];
            int m_socketfd;
            std::unique_ptr<RbufferManagerBase> m_RbufferManager;
            bool m_locked;
            bool m_buffer_pending;
    };
}

#endif
