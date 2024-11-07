#include <iostream>
#include "InputManager.hpp"
#include "TcpServer.hpp"

uint32_t RBUFFER_SIZE;

int main(int argc, char *argv[])
{
    R::InputManager imanager(argv, argc);
    RBUFFER_SIZE = imanager.m_bsize;
    R::TcpServer server(imanager.m_port);
    server.set_destination_coordinates(imanager.m_ipaddress, imanager.m_dport);
    server.init_listen(10);
    return 0;
}
