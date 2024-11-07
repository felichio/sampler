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
