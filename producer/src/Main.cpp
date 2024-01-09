#include <iostream>
#include "TcpClient.hpp"
#include "ValueManager.hpp"
#include "InputManager.hpp"


int main(int argc, char *argv[])
{
    R::InputManager imanager(argv, argc);
    imanager.getInput();
    R::TcpClient client(imanager.m_ipaddress, imanager.m_port);
    R::ValueManager manager(client, imanager.m_filename, imanager.m_endianess);
    manager.start_streaming();
    return 0;
}
