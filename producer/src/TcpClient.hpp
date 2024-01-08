#ifndef __TCP_CLIENT_HPP
#define __TCP_CLIENT_HPP
#include <string>
#include <cstdint>
#include <arpa/inet.h>


namespace R
{
    static const uint32_t buffer_size = 8192;
    

    enum ENDIANESS: uint8_t
    {
        LE = 0x00,
        BE = 0x01
    };

    enum TYPE: uint8_t
    {
        INT64 = 0x00,
        INT32 = 0x01
    };

    class TcpClient
    {
        public:
            TcpClient(std::string ip_address, uint16_t port_n);

            void send(int valuesToWrite);
            uint8_t *get_send_buffer();
        private:
            std::string m_ipaddress;
            uint16_t m_port;
            int m_socketfd;
            sockaddr_in m_addr;
            uint8_t m_buffer[buffer_size];
            int m_numOfWrite;
            int m_sumOfWrite;
            int m_globalSumOfWrite;
    };

} // R

#endif
