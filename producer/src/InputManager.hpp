#ifndef __INPUT_MANAGER_HPP
#define __INPUT_MANAGER_HPP

#include <string>
#include <unistd.h>
#include <algorithm>


namespace R
{
    class InputManager
    {
        public:
            InputManager(char **argv, int argc);
            void getInput();
            std::string m_ipaddress;
            uint16_t m_port;
            std::string m_filename;
            uint8_t m_endianess;
        private:
            char **m_args;
            int m_argc;

            char *getSpecificArg(std::string option);
    };

} // R

#endif
