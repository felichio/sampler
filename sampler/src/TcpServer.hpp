#ifndef _TCP_SERVER_HPP

#define _TCP_SERVER_HPP

#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <map>
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
            void save_peer_address(uint32_t id, std::string &ip_address, uint16_t port);
            void remove_peer_address(uint32_t id);
            void set_destination_coordinates(const std::string &ip_address, uint16_t port);

        private:
            std::mutex m_mutex;
            uint16_t m_portnumber;
            addrinfo m_hints, *m_serverinfo;
            int m_socketfd;
            std::map<uint32_t, std::pair<std::string, uint16_t>> m_peerAddresses;
            std::map<uint32_t, std::unique_ptr<TcpConnection>> m_tcpConnections;
            static uint32_t connectionid;
            std::string m_destination_ipaddress;
            uint16_t m_dport;

            void cleanup();
            
    };

}

#endif
