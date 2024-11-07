#ifndef __TCP_CLIENT_HPP
#define __TCP_CLIENT_HPP
#include <string>
#include <cstdint>
#include <arpa/inet.h>


namespace R
{

    class TcpClient
    {
        public:
            TcpClient(std::string ip_address, uint16_t port_n);
            ~TcpClient();

            void send(int valuesToWrite);
            uint8_t *get_send_buffer();
        private:
            std::string m_ipaddress;
            uint16_t m_port;
            int m_socketfd;
            sockaddr_in m_addr;
            uint8_t m_buffer[8192];
            int m_numOfWrite;
            int m_sumOfWrite;
            int m_globalSumOfWrite;
    };

} // R

#endif
