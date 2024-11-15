#ifndef __INPUT_MANAGER_HPP
#define __INPUT_MANAGER_HPP
#include <string>
#include <cstdint>
namespace R
{
    class InputManager
    {
        public:
            InputManager(char **argv, int argc)
                : m_args{argv}
                , m_argc{argc}
                , m_port{8080}
                , m_bsize{10}
                , m_ipaddress{}
                , m_dport{9999}
                , m_z{}
            {
                getInput(); 
            }

            char **m_args;
            int m_argc;
            uint16_t m_port;
            uint32_t m_bsize;
            std::string m_ipaddress;
            uint16_t m_dport;
            double m_z;

        private:
            void getInput();
            char *getSpecificArg(std::string option);
    };
}


#endif
