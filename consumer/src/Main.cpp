#include <iostream>
#include "TcpServer.hpp"

uint32_t RBUFFER_SIZE = 2u;

int main(int argc, char *argv[])
{
    R::TcpServer server(8080);
    server.init_listen(10);
    return 0;
}
