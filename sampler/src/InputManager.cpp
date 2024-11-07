#include <algorithm>
#include <iostream>
#include "InputManager.hpp"



void R::InputManager::getInput()
{
    char *value;
    if ((value = getSpecificArg("-p")) != 0)
    {
        m_port = std::stoi(value);
    }
    if ((value = getSpecificArg("-r")) != 0)
    {
        m_bsize = std::stoi(value);
    }
    if ((value = getSpecificArg("-dip")) != 0)
    {
        m_ipaddress = value;
    }
    if ((value = getSpecificArg("-dp")) != 0)
    {
        m_dport = std::stoi(value);
    }
    
    std::cout << "dip address: " << m_ipaddress << std::endl;
    std::cout << "dp port: " << m_dport << std::endl;
    std::cout << "port: " << m_port << std::endl;
    std::cout << "reservoir bfer sz: " << m_bsize << std::endl;
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
