#include <iostream>
#include "InputManager.hpp"
#include "Handler.hpp"


int main(int argc, char *argv[])
{
    R::InputManager imanager(argv, argc);
    R::Handler h(imanager.m_port);
    h.init();
    return 0;
}