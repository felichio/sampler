#include <unistd.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <memory>
#include "TcpClient.hpp"
#include "Debug.hpp"
#include "TcpServer.hpp"

uint32_t R::TcpServer::connectionid = 0;

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

    
    std::cout << "TcpServer created with port_n: " << m_portnumber << std::endl;
    std::cout << "m_socketfd: " << m_socketfd << std::endl;
}

R::TcpServer::~TcpServer()
{
    freeaddrinfo(m_serverinfo);
    close(m_socketfd);
}

void R::TcpServer::set_destination_coordinates(const std::string &ip_address, uint16_t port)
{
    m_destination_ipaddress = ip_address;
    m_dport = port;
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
        save_peer_address(connectionid, peer_address_str, remote_port);
        std::cout << "Accepted connection: " << "\t address: " << peer_address_str << "\t port: " << remote_port << "\t fdescriptor: " << newfd << std::endl;
        

        std::unique_ptr<TcpClient> tcp_client;
        if (!m_destination_ipaddress.empty())
        {
            try {
                tcp_client.reset(new TcpClient(m_destination_ipaddress, m_dport));
            } catch (std::exception &e)
            {

            }
        }

        std::unique_ptr<TcpConnection> tcpConnection(new TcpConnection(*this, newfd, connectionid, std::move(tcp_client)));
        // m_tcpConnections.push_back(std::move(tcpConnection));
        m_tcpConnections[connectionid] = std::move(tcpConnection);
        m_tcpConnections[connectionid]->start_reading_thread();
        connectionid++;
        cleanup();
    }
}


void R::TcpServer::save_peer_address(uint32_t id, std::string &ip_address, uint16_t port)
{
    std::lock_guard<std::mutex> g(m_mutex);
    m_peerAddresses[id] = std::make_pair(ip_address, port);
}

void R::TcpServer::remove_peer_address(uint32_t id)
{
    std::lock_guard<std::mutex> g(m_mutex);
    m_peerAddresses.erase(id);
}

void R::TcpServer::cleanup()
{
    for (std::map<uint32_t, std::unique_ptr<TcpConnection>>::iterator it = m_tcpConnections.begin(); it != m_tcpConnections.end(); ++it)
    {
        if (m_peerAddresses.find(it->first) == m_peerAddresses.end())
        {
            debug_print("Erasing: " << it->first);
            m_tcpConnections.erase(it->first);
        }
    }
}
