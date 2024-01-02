#include <iostream>
#include "TcpServer.hpp"



int main(int argc, char *argv[])
{
    R::TcpServer server(8080);
    server.init_listen(10);
    return 0;
}
