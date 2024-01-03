#include <iostream>
#include <unistd.h>
#include "TcpConnection.hpp"



R::TcpConnection::TcpConnection(R::TcpServer &server, int socketfd): m_tcpServer{server}, m_socketfd{socketfd}
{
    std::cout << "TcpConnection created with fd: " << m_socketfd << std::endl;
}


R::TcpConnection::~TcpConnection()
{
    std::cout << "TcpConnection destroyed" << " fd: " << m_socketfd << std::endl;
    close(m_socketfd);
}


void R::TcpConnection::start_reading_thread()
{
    std::cout << "Reading thread started" << std::endl;
    std::thread t(&TcpConnection::fill_buffer, this);
    std::cout << "Thread id: " << t.get_id() << std::endl;
    t.detach();
}

void R::TcpConnection::fill_buffer()
{
    int numOfRead = 0;
    int sumOfRead = 0;
    std::cout << "reading " << std::endl;
    while ((numOfRead = read(m_socketfd, m_buffer + sumOfRead, buffer_size - sumOfRead)) > 0)
    {
        sumOfRead += numOfRead;
        std::cout << "numOfRead: " << numOfRead << std::endl; 
        std::cout << "sumOfRead: " << sumOfRead << std::endl; 
        std::cout << "buffer: " << reinterpret_cast<void *>(m_buffer) << std::endl; 
        std::cout << "buffer ofs: " << reinterpret_cast<void *>(m_buffer + sumOfRead)<< std::endl; 
        std::cout << "available sp: " << buffer_size - sumOfRead << std::endl; 
    }
    std::cout << "finsihed reading" << std::endl;
}
