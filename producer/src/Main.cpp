#include <iostream>
#include "TcpClient.hpp"
#include "ValueManager.hpp"


int main()
{
    R::TcpClient client("192.168.1.30", 8080);
    R::ValueManager manager(client, "../../input/4.csv");
    manager.start_streaming();
    return 0;
}
