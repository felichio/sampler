#include <iostream>
#include "Handler.hpp"


int main(int argc, char *argv[])
{
    R::Handler h(8888);
    h.init();
    return 0;
}