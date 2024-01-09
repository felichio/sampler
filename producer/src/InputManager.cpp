#include <iostream>
#include "TcpClient.hpp"
#include "InputManager.hpp"


R::InputManager::InputManager(char **argv, int argc)
    : m_ipaddress{}
    , m_port{}
    , m_filename{}
    , m_endianess{R::ENDIANESS::LE}
    , m_args{argv}
    , m_argc{argc}
{

}

void R::InputManager::getInput()
{
    char *value;
    if ((value = getSpecificArg("-ip")) != 0)
    {
        m_ipaddress = value;
    }
    if ((value = getSpecificArg("-p")) != 0)
    {
        m_port = std::stoi(value);
    }
    if ((value = getSpecificArg("-e")) != 0)
    {
        std::string endianess = value;
        if (endianess == "be" || endianess == "BE")
        {
            m_endianess = R::ENDIANESS::BE;
        }
        else if (endianess == "le" || endianess == "LE")
        {
            m_endianess = R::ENDIANESS::LE;
        }
    }
    if ((value = getSpecificArg("-f")) != 0)
    {
        m_filename = value;
    }
    std::cout << "ip address: " << m_ipaddress << std::endl;
    std::cout << "port: " << m_port << std::endl;
    std::cout << "endianess: " << (m_endianess == 0x00 ? "Little" :  "Big") << " endian" << std::endl;
    std::cout << "file input: " << m_filename << std::endl;
}

char *R::InputManager::getSpecificArg(std::string option)
{
    char **iter = std::find(m_args, m_args + m_argc, option);

    if (iter != (m_args + m_argc))
    {
        return *++iter;
    }
    return 0;
}
