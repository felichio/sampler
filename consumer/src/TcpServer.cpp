#include "TcpServer.hpp"
#include <iostream>

R::TcpServer::TcpServer(uint16_t port_n): m_portnumber{port_n}
{
    std::cout << "TcpServer created with port_n: " << m_portnumber << std::endl;
}
