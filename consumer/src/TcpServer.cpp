#include "TcpServer.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <exception>

R::TcpServer::TcpServer(uint16_t port_n): m_portnumber{port_n}
{
    // zero out the structure
    memset(&m_hints, 0, sizeof m_hints);
    // set fields form TCP listening socket
    m_hints.ai_family = AF_INET;
    m_hints.ai_socktype= SOCK_STREAM;
    m_hints.ai_flags = AI_PASSIVE;
    
    int status = 0;
    // search for available addrinfo structures
    if ((status = getaddrinfo(NULL, std::to_string(m_portnumber).c_str(), &m_hints, &m_serverinfo)) != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << gai_strerror(status) << std::endl;
        throw std::runtime_error("getaddrinfo failed");
    }

    addrinfo *p = NULL;
    for (p = m_serverinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in*>(p->ai_addr);
            char buf[INET_ADDRSTRLEN];
            inet_ntop(p->ai_family, &(ipv4->sin_addr), buf, INET_ADDRSTRLEN);
            std::cout << "listening ip address: " << buf << std::endl;

            if ((m_socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            {
                throw std::runtime_error("socket failed");
            }

            // set SO_REUSEADDR option
            int enable = 1;
            if (setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
            {
                throw std::runtime_error("setsockopt failed");
            }

            if (bind(m_socketfd, p->ai_addr, p->ai_addrlen) == -1)
            {
                throw std::runtime_error("bind failed");
            }
        }
    }

    // clean getaddrinfo results
    freeaddrinfo(m_serverinfo);
    
    std::cout << "TcpServer created with port_n: " << m_portnumber << std::endl;
    std::cout << "m_socketfd: " << m_socketfd << std::endl;
}


void R::TcpServer::init_listen(int backlog)
{
    if (listen(m_socketfd, backlog) == -1)
    {
        throw std::runtime_error("listen failed");
    }
    
    // accept connections loop
    while (1)
    {
        sockaddr_storage peer_address;
        socklen_t size_of_peer = sizeof(peer_address);
        int newfd = accept(m_socketfd, reinterpret_cast<sockaddr *>(&peer_address), &size_of_peer);
        sockaddr_in *peer_address_sockaddr = reinterpret_cast<sockaddr_in *>(&peer_address);
        uint16_t remote_port = ntohs(peer_address_sockaddr->sin_port);
        char buf[INET_ADDRSTRLEN];
        inet_ntop(peer_address.ss_family, &peer_address_sockaddr->sin_addr, buf, INET_ADDRSTRLEN);
        std::string peer_address_str(buf);
        
        // store peer coordinates
        save_peer_address(peer_address_str, remote_port);
        std::cout << "Accepted connection: " << "\t address: " << peer_address_str << "\t port: " << remote_port << "\t fdescriptor: " << newfd << std::endl;
    }
}


void R::TcpServer::save_peer_address(std::string &ip_address, uint16_t port)
{
    std::lock_guard<std::mutex> g(m_mutex);
    m_peerAddresses.push_back(std::make_pair(ip_address, port));
}

void R::TcpServer::remove_peer_address(std::string &ip_address, uint16_t port)
{
    for (std::vector<std::pair<std::string, uint16_t>>::iterator it = m_peerAddresses.begin(); it != m_peerAddresses.end(); ++it)
    {
        if (it->second == port && it->first == ip_address)
        {
            std::lock_guard<std::mutex> g(m_mutex);
            m_peerAddresses.erase(it);
            break;
        }
    }
}

