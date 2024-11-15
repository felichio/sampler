#ifndef _TCP_CONNECTION_HPP

#define _TCP_CONNECTION_HPP

#include <thread>
#include <fstream>
#include "TcpClient.hpp"
#include "TcpServer.hpp"
#include "RbufferManager.hpp"
#include "InputManager.hpp"
#include "Enum.hpp"


extern uint32_t RBUFFER_SIZE;
namespace R
{
    static const uint32_t buffer_size = 8192;

    

    class TcpServer; // forward declaration
    class TcpConnection
    {
        public:
            TcpConnection(TcpServer &server, int socketfd, uint32_t connection_id, std::unique_ptr<TcpClient> tcp_client, InputManager &input_manager); 
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
            std::ofstream m_file;
            uint32_t m_connectionid;
            std::unique_ptr<TcpClient> m_tcp_client;
            InputManager &m_Input;
    };
}

#endif
