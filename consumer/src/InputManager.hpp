#ifndef __INPUT_MANAGER_HPP
#define __INPUT_MANAGER_HPP
#include <string>
#include <cstdint>
#include <iostream>
namespace R
{
    class InputManager
    {
        public:
            InputManager(char **argv, int argc)
                : m_args{argv}
                , m_argc{argc}
                , m_port{8080}
            {
                getInput();
            }

            char **m_args;
            int m_argc;
            uint16_t m_port;

        private:
            void getInput();
            char *getSpecificArg(std::string option);
    };
}


#endif
