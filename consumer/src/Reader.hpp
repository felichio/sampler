#ifndef __READER_HPP
#define __READER_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <memory>
#include "OutputStream.hpp"


namespace R
{

    static const uint32_t buffer_size = 8192;

    enum TYPE: uint8_t
    {
        INT64 = 0x00,
        INT32 = 0x01
    };

    class Reader
    {
        public:
        Reader(int socketfd, bool endianess, uint8_t dimension, size_t type_size);
        ~Reader();
        void read_output_stream();
        void flush_buffer(uint8_t *beg, uint8_t *end);
        void get_com_attributes();

        private:
        int m_socketfd;
        uint8_t m_buffer[buffer_size];
        bool m_endianess; 
        uint8_t m_dimension;
        size_t m_type_size;
        uint64_t m_bytes_seen;
        bool m_buffer_pending;
        std::vector<uint8_t> m_agnostic_stream;
        std::unique_ptr<OutputStreamBase> m_os;
        

    };


}

#endif