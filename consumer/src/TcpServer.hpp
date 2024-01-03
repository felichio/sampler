#ifndef _TCP_SERVER_HPP

#define _TCP_SERVER_HPP

#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include "TcpConnection.hpp"


namespace R
{
    class TcpConnection; // forward declaration
    class TcpServer
    {
        public:
           TcpServer(uint16_t port_n);
           ~TcpServer();
           void init_listen(int backlog);
           void save_peer_address(std::string &ip_address, uint16_t port);
           void remove_peer_address(std::string &ip_address, uint16_t port);


        private:
            std::mutex m_mutex;
            uint16_t m_portnumber;
            addrinfo m_hints, *m_serverinfo;
            int m_socketfd;
            std::vector<std::pair<std::string, uint16_t>> m_peerAddresses;
            std::vector<std::unique_ptr<TcpConnection>> m_tcpConnections;
    };

}

#endif
