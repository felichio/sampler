#include <iostream>
#include <unistd.h>
#include "OutputStream.hpp"
#include "Reader.hpp"



R::Reader::Reader(int socketfd, bool endianess, uint8_t dimension, size_t type_size): 
    m_socketfd{socketfd}, 
    m_endianess{endianess}, 
    m_dimension{dimension}, 
    m_type_size{type_size}, 
    m_bytes_seen{},
    m_buffer_pending{false}
{
    
}

void R::Reader::read_output_stream()
{
    int numOfRead = 0;
    int sumOfRead = 0;
    int globalSumOfRead = 0;
    while ((numOfRead = read(m_socketfd, m_buffer + sumOfRead, buffer_size - sumOfRead)) > 0)
    {
        sumOfRead += numOfRead;
        globalSumOfRead += numOfRead;

        if (m_buffer_pending && globalSumOfRead > 2)
        {
            get_com_attributes();
        }


        if (globalSumOfRead > 2)
        {
            if (sumOfRead == globalSumOfRead)
            {
                flush_buffer(m_buffer + 3, m_buffer + sumOfRead);
            }
            else
            {
                flush_buffer(m_buffer, m_buffer + sumOfRead);
            }
            sumOfRead = 0;
        }
    }
}

void R::Reader::flush_buffer(uint8_t *beg, uint8_t *end)
{
    for (uint8_t *it = beg; it != end; ++it)
    {
        const uint8_t value = *it;
        m_agnostic_stream.push_back(value);
        m_bytes_seen++;
        std::cout << std::hex << "0x" << static_cast<uint32_t>(value) << std::dec;
        std::cout << std::endl;

        if (m_bytes_seen % m_type_size == 0)
        {
            uint8_t *begin;
            uint8_t *end;
            if (m_endianess)
            {
                begin = &*m_agnostic_stream.rbegin();
                end = begin - m_type_size;
            }
            else
            {
                end = &*m_agnostic_stream.end();
                begin = end - m_type_size;
            }
            
            if (m_type_size == TYPE::INT64)
            {
                OutputStream<int64_t> os(m_endianess, m_dimension);
                os.fill_buffer(begin, end);
            }
            else if (m_type_size == TYPE::INT32)
            {
                OutputStream<int32_t> os(m_endianess, m_dimension);
                os.fill_buffer(begin, end);

            }

        }

    }
}

void R::Reader::get_com_attributes()
{
    uint8_t end = m_buffer[0u];
    uint8_t type = m_buffer[1u];
    uint8_t dimension = m_buffer[2u];

    m_endianess = static_cast<bool>(end);
    m_type_size = type == TYPE::INT64 ? static_cast<size_t>(8) : static_cast<size_t>(4);
    m_dimension = dimension;
    m_buffer_pending = false;
}