#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdexcept>
#include "TcpClient.hpp"


R::TcpClient::TcpClient(std::string ip_address, uint16_t port_n)
    : m_ipaddress{ip_address}
    , m_port{port_n}
    , m_socketfd{-1}
    , m_numOfWrite{}
    , m_sumOfWrite{}
    , m_globalSumOfWrite{}
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);
    inet_pton(m_addr.sin_family, m_ipaddress.c_str(), &(m_addr.sin_addr));

    if ((m_socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw std::runtime_error("socket failed");
    }
    
    if (connect(m_socketfd, reinterpret_cast<sockaddr *>(&m_addr), INET_ADDRSTRLEN) == -1)
    {
        throw std::runtime_error("connect failed");
    }

}

R::TcpClient::~TcpClient()
{
    close(m_socketfd);
}


void R::TcpClient::send(int valuesToWrite)
{
    std::cout << "sending m_SumOfWrite " << valuesToWrite << std::endl;
    while ((m_numOfWrite = write(m_socketfd, m_buffer + m_sumOfWrite, valuesToWrite - m_sumOfWrite)) > 0)
    {
        m_sumOfWrite += m_numOfWrite;
        m_globalSumOfWrite += m_numOfWrite;
        std::cout << "writing: " << m_numOfWrite << " bytes" << std::endl;
    }

    m_sumOfWrite = 0;
}

uint8_t *R::TcpClient::get_send_buffer()
{
    return m_buffer;
}
