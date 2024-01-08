#ifndef __VALUE_MANAGER_HPP
#define __VALUE_MANAGER_HPP
#include <iostream>
#include <vector>
#include <fstream>
#include "TcpClient.hpp"

namespace R
{
    static const uint32_t chunk_size = 0x100; // 256
    class ValueManager
    {
        public:
            ValueManager(TcpClient& tcpclient, std::string filename);

            void start_streaming();
            void copy_to_send_buffer(std::vector<int32_t> &values);
            void read_chunk_from_file(std::vector<int32_t> &store);
        private:
            TcpClient m_TcpClient; 
            std::string m_filename;
            std::ifstream m_ifstream;
            std::vector<uint8_t> m_valuebuffer;  
            bool m_endianess;
            uint8_t m_dimension;
            bool m_headcsv;
    };


} // R

#endif
